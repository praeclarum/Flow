
#include "Functions.h"

#define FUNCTION(functionName) \
    Number functionName##Function(FlowController *flow, int numInputs, Number *inputs, Number *states, void *callbackArg)

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

#undef FUNCTION
