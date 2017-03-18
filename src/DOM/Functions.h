#pragma once

#include <Arduino.h>
#include "Primitives.h"

class Function
{
    ApplyFunction apply_;
    byte stateSize_;
public:
    Function(ApplyFunction func, byte stateSize)
        : apply_(func), stateSize_(stateSize)
    {}
    inline int stateSize() { return stateSize_; }
    inline Number apply(FlowController *flow, void *state, int numInputs, Number *inputs) {
        return apply_(flow, state, numInputs, inputs);
    }
};

struct FunctionReference
{
    const int numInputs;
    Function *function;
    byte *values;
    FunctionReference(Function *function, int numInputs)
        : function(function), numInputs(numInputs)
    {
        int nvalues = numInputs*sizeof(Number) + function->stateSize();
        values = (byte*)calloc(nvalues, 1);
    }
    ~FunctionReference()
    {
        if (values)
            free(values);
    }
    inline Number *inputs() { return numInputs > 0 ? (Number*)values : 0; }
    inline Number apply(FlowController *flow) {
        byte *state = values + numInputs*sizeof(Number);
        return function->apply(flow, state, numInputs, (Number*)values);
    }
};

#define FUNCTION(functionName) \
    Number functionName##Function(FlowController *flow, void *state, int numInputs, Number *inputs)

FUNCTION(t);
FUNCTION(pi);
FUNCTION(sin);
FUNCTION(cos);
FUNCTION(min);
FUNCTION(max);
FUNCTION(clamp);
FUNCTION(save);
FUNCTION(load);

#undef FUNCTION
