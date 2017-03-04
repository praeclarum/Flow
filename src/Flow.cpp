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
        float result = 0;
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
                if (hasResult) {
                    esc(kEscResult);
                    stream->println(result);
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

void FlowController::eval(const char *code)
{
    yypstate *p = yypstate_new();
    bool hr;
    float r;
    yypush_parse(p, 0, 0, this, &hr, &r);
    yypstate_delete(p);
}

void yyerror(FlowController *flow, bool *hasResult, float *result, const char *m)
{
}

FlowController Flow;
