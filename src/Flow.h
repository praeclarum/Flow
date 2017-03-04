#pragma once

#include <Arduino.h>

class FlowController
{
public:
    FlowController();
    void begin();
    void loop();
    void eval(const char *code);

private:
    Stream *stream;

    void readStreamCode();
    void printPrompt();
};

extern FlowController Flow;

