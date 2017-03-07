#include <Arduino.h>

#include "Node.h"


Node::~Node()
{
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
