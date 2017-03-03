#include <Flow.h>

#include <Parsers/FlowParser.h>


void FlowController::begin()
{
    yypstate *p = yypstate_new();
    yypush_parse(p, 0, 0);
    yypstate_delete(p);
}

void yyerror(const char *m)
{

}

FlowController Flow;
