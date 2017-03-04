#include <Flow.h>

#include <Parsers/FlowParser.h>

static const char kSyntaxError[] PROGMEM = "Syntax error.";
static const char kPrompt[] PROGMEM = "> ";
static const char kMorePrompt[] PROGMEM = ". ";

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
        stream->print("> ");
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
                if (hasResult)
                    stream->println(result);
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
                stream->println('^');
                stream->println(kSyntaxError);
                streamLexer.reset();
                printPrompt();
                yypstate_delete(streamParseState);
                streamParseState = 0;
            }
            else if (parseres == YYPUSH_MORE) {                
                // Needs more
                stream->print(". ");
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
