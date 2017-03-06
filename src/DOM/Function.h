#pragma once

class FlowController;
class Node;

typedef Number (*ApplyFunc)(FlowController *flow, int numInputs, Number *inputs, Number *states, void *callbackArg);

class Function
{
public:
    ~Function() {}
    virtual int numStates() { return 0; };
    virtual Number apply(FlowController *flow, int numInputs, Number *inputs, Number *states) = 0;
};

class NativeFunction : public Function
{
    ApplyFunc apply_;
    byte numStates_;
    void *callbackArg;
public:
    NativeFunction(ApplyFunc func, int numStates = 0, void *callbackArg = 0)
        : apply_(func), numStates_(numStates)
    {}
    int numStates() { return numStates_; }
    Number apply(FlowController *flow, int numInputs, Number *inputs, Number *states) {
        return apply_(flow, numInputs, inputs, states, callbackArg);
    }
};

class UserFunction : public Function
{
    Node *params;
    Node *body;
};
