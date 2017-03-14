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
    ~FlowController();
    void begin();
    void loop();
    Number eval(const __FlashStringHelper *code, FlowError *error = 0);
    void addFunction(const __FlashStringHelper *funcName, ApplyFunction func, int numStates = 0, void *callbackArg = 0);

    inline Stream *getStream() { return stream; }

    int saveToEEPROM();
    int loadFromEEPROM();
    void clear();

private:
    Stream *stream;
    yypstate *streamParseState;
    FlowLexer streamLexer;
    Node *document;
    Node *editingSub;

    Number evalDeclaration(Node *node);
    void link(Node *parentNode, Node *node);
    Number eval(Node *node);
    void readStreamCode();
    void printPrompt();
};

extern FlowController Flow;

