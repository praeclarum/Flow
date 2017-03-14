#pragma once

#include <Arduino.h>
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
    inline int numStates() { return numStates_; }
    inline Number apply(FlowController *flow, int numInputs, Number *inputs, Number *states) {
        return apply_(flow, numInputs, inputs, states, callbackArg);
    }
};

struct FunctionReference
{
    const int numInputs;
    Function *function;
    Number *values;
    FunctionReference(Function *function, int numInputs)
        : function(function), numInputs(numInputs)
    {
        int nvalues = numInputs + function->numStates();
        values = nvalues > 0 ? (Number*)calloc(nvalues, sizeof(Number)) : 0;
    }
    ~FunctionReference()
    {
        if (values)
            free(values);
    }
    inline Number *inputs() { return numInputs > 0 ? values : 0; }
    inline Number apply(FlowController *flow) {
        return function->apply(flow, numInputs, values, values + numInputs);
    }
};

#define FUNCTION(functionName) \
    Number functionName##Function(FlowController *flow, int numInputs, Number *inputs, Number *states, void *callbackArg)

FUNCTION(t);
FUNCTION(pi);
FUNCTION(sin);
FUNCTION(cos);
FUNCTION(save);
FUNCTION(load);

#undef FUNCTION
