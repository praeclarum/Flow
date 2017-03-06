#pragma once

#include <Arduino.h>

#include "Parsers/FlowLexer.h"

enum FlowError
{
    FE_None = 0,
    FE_Unknown = 1,
    FE_OutOfMemory = 2,
    FE_SyntaxError = 3,
    FE_SyntaxIncomplete = 4,
};

class FlowController
{
public:
    FlowController();
    void begin();
    void loop();
    Number eval(const char *code, FlowError *error = 0);

    inline Stream *getStream() { return stream; }

private:
    Stream *stream;
    yypstate *streamParseState;
    FlowLexer streamLexer;

    Number eval(Node *node);
    void readStreamCode();
    void printPrompt();
};

extern FlowController Flow;

