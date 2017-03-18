#pragma once


typedef float Number;
typedef uint32_t Name;

class FlowController;

typedef Number (*ApplyFunction)(FlowController *flow, int numInputs, Number *inputs, Number *states, void *callbackArg);

class NNode;

class NameTable
{
    NNode *head;
public:
    NameTable();
    ~NameTable();
    void put(Name name, const char *str);
    const char *get(Name name);
};
