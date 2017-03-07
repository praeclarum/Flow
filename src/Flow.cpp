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

//
// FLOW CONTROLLER
//

FlowController::FlowController()
    : stream(0)
    , streamParseState(0)
    , document(new Node(NT_Document))
    , editingSub(new Node(NT_Sub))
{
#define FUNCTION(functionName) addFunction(F(#functionName), functionName##Function, 0, 0)
    FUNCTION(sin);
    FUNCTION(cos);
#undef FUNCTION

    document->appendChild(editingSub);
}

FlowController::~FlowController()
{
    delete document;
    if (streamParseState) {
        yypstate_delete(streamParseState);
    }
}

void FlowController::begin()
{
    stream = &Serial;
    printPrompt();
}

void FlowController::loop()
{
    readStreamCode();

    //
    // Loop subs
    //
    Node *sub = document->firstChild;
    while (sub) {
        if (sub->nodeType == NT_Sub) {
            Node *a = sub->firstChild;
            while (a) {
                if (a->nodeType == NT_Assignment && a->firstChild) {
                    a->value.number = eval(a->firstChild->nextSibling);
                }
                a = a->nextSibling;
            }
        }
        sub = sub->nextSibling;
    }
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
                    Number v = evalDeclaration(result);
                    esc(kEscResult);
                    stream->println(v);
                    esc(kEscReset);
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

Number FlowController::eval(const __FlashStringHelper *code, FlowError *error)
{
    PGM_P p = reinterpret_cast<PGM_P>(code);
    yypstate *parseState = yypstate_new();
    FlowLexer lexer;
    bool onnewline = false;
    while (pgm_read_byte(p)) {
        bool gotnewline = false;
        int parseres = -1;
        bool hasResult = false;
        Node *result = 0;
        while (!gotnewline) {
            if (pgm_read_byte(p) == 0 && !onnewline) {
                onnewline = true;
                p = newline;
            }
            int r = pgm_read_byte(p);
            if (r == '\n') gotnewline = true;
            if (parseres == 1) { gotnewline = true; break; }
            bool consumed = false;
            do {
                consumed = lexer.push(r);
                if (lexer.state == FLS_Complete && parseres != 0 && parseres != 1) {
                    parseres = yypush_parse(parseState, lexer.tok, &lexer.val, this, &hasResult, &result);
                }
            } while (!consumed);
            if (pgm_read_byte(p) == 0) break;
            else p++;
        }
        if (gotnewline) {
            if (parseres == 0) {
                // Successful Parse!
                Number v = 0;
                if (hasResult && result) {
                    v = evalDeclaration(result);
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
                if (pgm_read_byte(p) == 0) {
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

uint32_t crc_string(const __FlashStringHelper *s);

void FlowController::addFunction(const __FlashStringHelper *funcName, ApplyFunction func, int numStates, void *callbackArg)
{
    Function *f = new Function(func, numStates, callbackArg);
    if (!f) return;
    Node *fdn = new Node(NT_Function);
    if (!fdn) {
        delete f;
        return;
    }
    fdn->value.function = f;
    Node *nn = new Node(NT_Name);
    if (!nn) {
        delete fdn;
        return;
    }
    nn->value.name = crc_string(funcName);
    Node *an = new Node(NT_Assignment);
    if (!an) {
        delete fdn;
        delete nn;
        return;
    }
    an->firstChild = nn;
    nn->nextSibling = fdn;
    document->appendChild(an);
}

Number FlowController::evalDeclaration(Node *node)
{
    Number r = 0;
    link(node);
    switch (node->nodeType) {
    case NT_Assignment:
        if (node->firstChild && node->firstChild->nextSibling && node->firstChild->nodeType == NT_Name) {
            Name name = node->firstChild->value.name;
            Node *existing = editingSub->firstChild;
            while (existing && !(existing->nodeType == NT_Assignment && existing->firstChild && existing->firstChild->value.name == name)) {
                existing = existing->nextSibling;
            }
            if (existing) {
                // Replace
                delete existing->firstChild->nextSibling;
                existing->firstChild->nextSibling = node->firstChild->nextSibling;
                node->firstChild->nextSibling = 0;
                delete node;
            }
            else {
                existing = node;
                editingSub->appendChild(node);
                link(editingSub); // relink the sub
            }            
            r = eval(existing->firstChild->nextSibling);
            existing->value.number = r;
        }
        else {
            delete node;
        }
        break;
    case NT_SwitchToSub: {
        }
        delete node;
        break;
    default:
        r = eval(node);
        delete node;
        break;
    }
    return r;
}

Number FlowController::eval(Node *node)
{
    switch (node->nodeType) {
    case NT_Document:
        return 0;
    case NT_UnaryOperator: {
        Number v = node->firstChild ? eval(node->firstChild) : 0;
        switch (node->value.unaryOperator) {
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
        switch (node->value.binaryOperator) {
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
    case NT_Number:
        return node->value.number;
    case NT_Sub:
        return 0;
    case NT_Function:
        return node->value.function->apply(this, 0, 0, 0);
    case NT_Assignment:
        return node->value.number;
    case NT_Name:
        return node->firstChild ? eval(node->firstChild) : 0;
    case NT_Reference:
        return node->value.reference ? eval(node->value.reference) : 0;
    case NT_Call:
        return 0;
    case NT_SwitchToSub:
        return 0;
    case NT_End:
        return 0;
    }
}

void FlowController::link(Node *node)
{
    Node *c = 0;
    Number r = 0;
    if (!node) return; // No need to link these
    if (node->nodeType == NT_Assignment) {
        // Only link the RHS of assignment
        if (node->firstChild) link(node->firstChild->nextSibling);
    }
    else if (node->nodeType == NT_Name) {
        if (!node->firstChild) {
            Name name = node->value.name;

            if (node->firstChild) {
                delete node->firstChild;
                node->firstChild = 0;
            }

            //
            // Is it in the doc?
            //
            c = document->firstChild;
            while (c) {
                if (c->nodeType == NT_Assignment && c->firstChild && c->firstChild->nodeType == NT_Name && c->firstChild->value.name == name) {
                    break;
                }
                c = c->nextSibling;
            }
            //
            // How about the sub?
            //
            if (!c && editingSub) {                
                c = editingSub->firstChild;
                while (c) {
                    if (c->nodeType == NT_Assignment && c->firstChild && c->firstChild->nodeType == NT_Name && c->firstChild->value.name == name) {
                        break;
                    }
                    c = c->nextSibling;
                }
            }
            //
            // Did we link?
            //
            if (c) {
                Node *rhs = c->firstChild->nextSibling;
                if (rhs) {
                    Node *rn = new Node(NT_Reference);
                    if (rn) {
                        rn->value.reference = c;
                        node->firstChild = rn;
                    }
                    else {
                        // No memory :-(
                    }
                }
                else {
                    // Assignment is invalid
                }
            }
            else {
                // Link failed
            }
        }
        else {
            // OK, already linked
        }
    }
    else {
        //
        // Link the children
        //
        c = node->firstChild;
        while (c) {
            link(c);
            c = c->nextSibling;
        }
    }
 }

void yyerror(FlowController *flow, bool *hasResult, Node **result, const char *m)
{
}

FlowController Flow;
