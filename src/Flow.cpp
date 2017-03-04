#include <Flow.h>

#include <Parsers/FlowParser.h>

FlowController::FlowController()
    : stream(0)
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
    if (stream && stream->available()) {
        bool gotnewline = false;
        while (stream->available() && !gotnewline) {
            int r = stream->read();
            if (r < 0) break;
            if (r == '\n') gotnewline = true;
        }
        if (gotnewline) {
            printPrompt();
        }
    }
}

void FlowController::eval(const char *code)
{
    yypstate *p = yypstate_new();
    yypush_parse(p, 0, 0);
    yypstate_delete(p);
}

void yyerror(const char *m)
{

}

FlowController Flow;
