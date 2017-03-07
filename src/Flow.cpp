#include <Flow.h>

#include <Parsers/FlowParser.h>

static const char kSyntaxError[] PROGMEM = "Syntax error.";
static const char kPrompt[] PROGMEM = "> ";
static const char kMorePrompt[] PROGMEM = ". ";

#ifdef FLOW_CLICOLORS
#define esc(k) stream->print(k);
static const char kEscPrompt[] PROGMEM = "\x1B[36m";
static const char kEscResult[] PROGMEM = "\x1B[33;1m";
static const char kEscColRef[] PROGMEM = "\x1B[31;1m";
static const char kEscError[] PROGMEM = "\x1B[37m";
static const char kEscReset[] PROGMEM = "\x1B[0m";
#else
#define esc(k)
#endif

FlowController::FlowController()
    : stream(0)
    , streamParseState(0)
{
}

void FlowController::begin()
{
    stream = &Serial;
    printPrompt();
}

void FlowController::loop()
{
    readStreamCode();
}

void FlowController::printPrompt()
{
    if (stream) {
        esc(kEscPrompt);
        stream->print(kPrompt);
        esc(kEscReset);
    }
}

void FlowController::readStreamCode()
{
    while (stream && stream->available()) {
        if (!streamParseState)
            streamParseState = yypstate_new();
        if (!streamParseState)
            break;

        bool gotnewline = false;
        int parseres = -1;
        bool hasResult = false;
        Node *result = 0;
        while (stream->available() && !gotnewline) {
            int r = stream->read();
            if (r == '\n') gotnewline = true;
            if (parseres == 1) continue;
            bool consumed = false;
            do {
                consumed = streamLexer.push(r);
                if (streamLexer.state == FLS_Complete && parseres != 0 && parseres != 1) {
                    parseres = yypush_parse(streamParseState, streamLexer.tok, &streamLexer.val, this, &hasResult, &result);
                }
            } while (!consumed);
        }
        if (gotnewline) {
            if (parseres < 0) {
                // Nothing parsed
                printPrompt();
            }
            else if (parseres == 0) {
                // Successful Parse!
                if (hasResult && result) {
                    Number v = eval(result);
                    esc(kEscResult);
                    stream->println(v);
                    esc(kEscReset);
                    delete result;
                    result = 0;
                }
                streamLexer.reset();
                printPrompt();
                yypstate_delete(streamParseState);
                streamParseState = 0;
            }
            else if (parseres == 1) {
                // Failed Parse. :-(
                for (int i = 1; i<streamLexer.tcol + 2; i++) {
                    stream->print(' ');
                }
                esc(kEscColRef);
                stream->println('^');
                esc(kEscError);
                stream->println(kSyntaxError);
                esc(kEscReset);
                streamLexer.reset();
                printPrompt();
                yypstate_delete(streamParseState);
                streamParseState = 0;
            }
            else if (parseres == YYPUSH_MORE) {                
                // Needs more
                esc(kEscPrompt);
                stream->print(kMorePrompt);
                esc(kEscReset);
            }            
        }
    }
}

static const char newline[] = {'\n', '\0'};

Number FlowController::eval(const char *code, FlowError *error)
{
    yypstate *parseState = yypstate_new();
    FlowLexer lexer;
    const char *p = code;
    bool onnewline = false;
    while (*p) {
        bool gotnewline = false;
        int parseres = -1;
        bool hasResult = false;
        Node *result = 0;
        while (!gotnewline) {
            if (*p == 0 && !onnewline) {
                onnewline = true;
                p = newline;
            }
            int r = *p;
            if (r == '\n') gotnewline = true;
            if (parseres == 1) { gotnewline = true; break; }
            bool consumed = false;
            do {
                consumed = lexer.push(r);
                if (lexer.state == FLS_Complete && parseres != 0 && parseres != 1) {
                    parseres = yypush_parse(parseState, lexer.tok, &lexer.val, this, &hasResult, &result);
                }
            } while (!consumed);
            if (*p == 0) break;
            else p++;
        }
        if (gotnewline) {
            if (parseres == 0) {
                // Successful Parse!
                Number v = 0;
                if (hasResult && result) {
                    v = eval(result);
                    delete result;
                }
                yypstate_delete(parseState);
                if (error) *error = FE_None;
                return v;
            }
            else if (parseres == 1) {
                yypstate_delete(parseState);
                if (error) *error = FE_SyntaxError;
                return 0;
            }
            else if (parseres == YYPUSH_MORE) {
                if (*p == 0) {
                    yypstate_delete(parseState);
                    if (error) *error = FE_SyntaxIncomplete;
                    return 0;
                }
            }
            else {
                yypstate_delete(parseState);
                if (error) *error = FE_OutOfMemory;
                return 0;
            }
        }
    }
    yypstate_delete(parseState);
    if (error) *error = FE_SyntaxIncomplete;
    return 0;
}

Number FlowController::eval(Node *node)
{
    Node *c = 0;
    Number r = 0;
    switch (node->nodeType) {
    case NT_Document:
        c = node->firstChild;
        while (c) {
            r = eval(c);
            c = c->nextSibling;
        }
        return r;
    case NT_UnaryOperator: {
        Number v = node->firstChild ? eval(node->firstChild) : 0;
        switch (node->value.unop) {
        case UO_Negate:
            return -v;
        default:
            return 0;
        }
    }
    case NT_BinaryOperator: {
        if (!node->firstChild || !node->firstChild->nextSibling) return 0;
        Number left = eval(node->firstChild);
        Number right = eval(node->firstChild->nextSibling);
        switch (node->value.binop) {
        case BO_Add:
            return left + right;
        case BO_Sub:
            return left - right;
        case BO_Mul:
            return left * right;
        case BO_Div:
            return left / right;
        case BO_Pow:
            return pow(left, right);
        default:
            return 0;
        }
    }
    case NT_NumberLiteral:
        return node->value.number;
    case NT_FunctionDefinition:
        return 0;
    case NT_Assignment:
        return 0;
    case NT_Name:
        return 0;
    case NT_Call:
        return 0;
    case NT_SwitchToSub:
        return 0;
    case NT_End:
        return 0;
    }
}

void yyerror(FlowController *flow, bool *hasResult, Node **result, const char *m)
{
}

FlowController Flow;
