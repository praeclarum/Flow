#include <Arduino.h>

#include "Node.h"

Node::~Node()
{
    switch (nodeType)
    {
    case NT_Function:
        delete value.function;
        break;
    case NT_FunctionReference:
        delete value.functionReference;
        break;
    default:
        break;
    }
    delete firstChild;
    delete nextSibling;
}

void Node::appendChild(Node *child)
{
    if (!firstChild) {
        firstChild = child;
        return;
    }
    Node *ps = firstChild;
    while (ps->nextSibling)
        ps = ps->nextSibling;
    ps->nextSibling = child;
}
