
#include "Functions.h"
#include "../Flow.h"

#define FUNCTION(functionName) \
    Number functionName##Function(FlowController *flow, void *state, int numInputs, Number *inputs)

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

FUNCTION(clamp)
{
    if (numInputs <= 0) return 0;
    Number v = inputs[0];
    if (numInputs > 1 && v < inputs[1])
        v = inputs[1];
    if (numInputs > 2 && v > inputs[2])
        v = inputs[2];
    return v;
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
