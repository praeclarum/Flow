#pragma once

#include <Arduino.h>

#include "Parsers/FlowLexer.h"

class FlowController
{
public:
    FlowController();
    void begin();
    void loop();
    void eval(const char *code);

private:
    Stream *stream;
    yypstate *streamParseState;
    FlowLexer streamLexer;

    void readStreamCode();
    void printPrompt();
};

extern FlowController Flow;

