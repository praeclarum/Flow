#include <Flow.h>
#include <EEPROM.h>

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
    , editingSub(0)
    , webServer(this, 8080)
    , streamLexer(&names)
{
    clear();
#define FUNCTION(functionName) addFunction(F(#functionName), functionName##Function, 0, 0)
    FUNCTION(t);
    FUNCTION(pi);
    FUNCTION(sin);
    FUNCTION(cos);
    FUNCTION(min);
    FUNCTION(max);
    FUNCTION(clamp);
    FUNCTION(save);
    FUNCTION(load);
#undef FUNCTION
}

void FlowController::clear()
{
    if (!document) return;

    //
    // Remove all subs
    //
    bool changed = true;
    while (changed) {
        changed = false;
        Node *c = document->firstChild;
        while (c) {
            if (c->nodeType == NT_Sub) {
                document->removeChild(c);
                changed = true;
                break;
            }
            c = c->nextSibling;
        }
    }

    //
    // Add new blank one
    //
    editingSub = new Node(NT_Sub);
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
    while (!Serial);
    webServer.setup();
    stream = &Serial;
    printPrompt();
}

void FlowController::loop()
{
    //
    // Read serial input
    //
    readStreamCode();

    //
    // Read web input
    //
    webServer.loop();

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
    FlowLexer lexer(&names);
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
    char nameBuffer[32];
    int nameBufferLength = 0;
    PGM_P p = reinterpret_cast<PGM_P>(funcName);
    uint32_t crc = ~0L;
    byte b = pgm_read_byte(p);
    while (b && nameBufferLength + 2 <= 32) {
        nameBuffer[nameBufferLength++] = b;
        p++;
        b = pgm_read_byte(p);
    }
    nameBuffer[nameBufferLength] = 0;
    names.put(nn->value.name, nameBuffer);

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
    link(editingSub, node);
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
                link(document, editingSub); // relink the sub
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
    case NT_Null:
        return 0;
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
        return node->value.function ? node->value.function->apply(this, 0, 0, 0) : 0;
    case NT_Assignment:
        return node->value.number;
    case NT_Name:
        return node->firstChild ? eval(node->firstChild) : 0;
    case NT_FunctionReference:    
        return node->value.functionReference ? node->value.functionReference->apply(this) : 0;
    case NT_AssignmentReference:
        return node->value.assignmentReference ? eval(node->value.assignmentReference) : 0;
    case NT_Call:
        if (node->firstChild && node->firstChild->firstChild && node->firstChild->firstChild->nodeType == NT_FunctionReference) {
            FunctionReference *f = node->firstChild->firstChild->value.functionReference;
            if (f) {
                Number *inputs = f->inputs();
                if (inputs) {
                    
                    Node *a = node->firstChild->nextSibling;
                    int i = 0;
                    while (a && i < f->numInputs) {
                        inputs[i] = eval(a);
                        a = a->nextSibling;
                        i++;
                    }
                }
                return f->apply(this);
            }
        }
        return 0;
    case NT_SwitchToSub:
        return 0;
    case NT_End:
        return 0;
    }
}

void FlowController::link(Node *parentNode, Node *node)
{
    Node *c = 0;
    Number r = 0;
    if (!node) return; // No need to link these
    if (node->nodeType == NT_Assignment) {
        // Only link the RHS of assignment
        if (node->firstChild) link(node, node->firstChild->nextSibling);
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
                if (rhs && rhs->nodeType == NT_Function) {
                    Node *rn = new Node(NT_FunctionReference);
                    if (rn) {
                        int numInputs = 0;
                        if (parentNode && parentNode->nodeType == NT_Call && parentNode->firstChild == node) {
                            Node *a = node->nextSibling;
                            while (a) {
                                numInputs++;
                                a = a->nextSibling;
                            }
                        }
                        Function *f = rhs->value.function;
                        rn->value.functionReference = new FunctionReference(f, numInputs);
                        node->firstChild = rn;
                    }
                    else {
                        // No memory :-(
                    }
                }
                else if (rhs) {
                    Node *rn = new Node(NT_AssignmentReference);
                    if (rn) {
                        rn->value.assignmentReference = c;
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
            link(node, c);
            c = c->nextSibling;
        }
    }
 }

int FlowController::saveToEEPROM()
{
    if (!document) return false;
    EEPROM.update(0, 42);
    EEPROM.update(1, sizeof(Node::value));
    return document->saveToEEPROM(2);
}

int FlowController::loadFromEEPROM()
{
    if (!document) return false;
    clear();
    if (EEPROM.read(0) != 42) return 0;
    if (EEPROM.read(1) != sizeof(Node::value)) return 0;
    Node *newDocument = new Node(NT_Document);
    int end = 0;
    int r = newDocument->loadFromEEPROM(2);
    document->removeChild(editingSub);
    editingSub = 0;
    Node *c = newDocument->firstChild;
    while (c) {
        if (c->nodeType == NT_Sub && editingSub == 0)
            editingSub = c;
        Node *ns = c->nextSibling;
        c->nextSibling = 0;
        document->appendChild(c);
        c = ns;
    }
    newDocument->firstChild = 0;
    delete newDocument;
    if (!editingSub) {
        editingSub = new Node(NT_Sub);
        document->appendChild(editingSub);
    }
    link(0, document);
    return r;
}

void yyerror(FlowController *flow, bool *hasResult, Node **result, const char *m)
{
}

FlowController Flow;
