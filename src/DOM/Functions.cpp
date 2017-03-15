
#include "Functions.h"
#include "../Flow.h"

#define FUNCTION(functionName) \
    Number functionName##Function(FlowController *flow, int numInputs, Number *inputs, Number *states, void *callbackArg)

FUNCTION(t)
{
    return micros() / 1000000.0;
}

FUNCTION(pi)
{
    return M_PI;
}

FUNCTION(sin)
{
    return (numInputs > 0) ? sin(inputs[0]) : 0;
}

FUNCTION(cos)
{
    return (numInputs > 0) ? cos(inputs[0]) : 1;
}

FUNCTION(min)
{
    if (numInputs <= 0) return 0;
    Number m = inputs[0];
    for (int i = 1; i < numInputs; i++)
        if (inputs[i] < m) m = inputs[i];
    return m;
}

FUNCTION(max)
{
    if (numInputs <= 0) return 0;
    Number m = inputs[0];
    for (int i = 1; i < numInputs; i++)
        if (inputs[i] > m) m = inputs[i];
    return m;
}

FUNCTION(save)
{
    return flow->saveToEEPROM();
}

FUNCTION(load)
{
    return flow->loadFromEEPROM();
}

#undef FUNCTION