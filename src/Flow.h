#pragma once

#include <Arduino.h>

#include "Parsers/FlowLexer.h"
#include "WebServer/WebServer.h"

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
    void addFunction(const __FlashStringHelper *funcName, ApplyFunction func, int stateSize = 0);

    inline Stream *getStream() { return stream; }
    inline Node *getDocument() { return document; }
    inline const char *getNameText(Name name) { return names.get(name); }

    int saveToEEPROM();
    int loadFromEEPROM();
    void clear();

private:
    Stream *stream;
    yypstate *streamParseState;
    FlowLexer streamLexer;
    Node *document;
    Node *editingSub;
    WebServer webServer;
    NameTable names;

    Number evalDeclaration(Node *node);
    void link(Node *parentNode, Node *node);
    Number eval(Node *node);
    void readStreamCode();
    void printPrompt();
};

extern FlowController Flow;

