#pragma once

class FlowController
{
public:
    void begin();
    void loop();
    void eval(const char *code);
};

extern FlowController Flow;

