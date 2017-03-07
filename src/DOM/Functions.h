#pragma once

#include "Primitives.h"

class Function
{
    ApplyFunction apply_;
    byte numStates_;
    void *callbackArg;
public:
    Function(ApplyFunction func, int numStates, void *callbackArg)
        : apply_(func), numStates_(numStates), callbackArg(callbackArg)
    {}
    int numStates() { return numStates_; }
    Number apply(FlowController *flow, int numInputs, Number *inputs, Number *states) {
        return apply_(flow, numInputs, inputs, states, callbackArg);
    }
};
