#include <Arduino.h>
#include <EEPROM.h>

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

int Node::saveToEEPROM(int idx)
{
    // Is there enough space?
    if (idx + 2 + sizeof(value) > EEPROM.length()) {
        if (idx < EEPROM.length())
            EEPROM.update(idx, 0);
        return EEPROM.length();
    }
    // Don't write functions
    if (nodeType == NT_Assignment && firstChild && firstChild->nextSibling && firstChild->nextSibling->nodeType == NT_Function)
        return idx;
    // Write the type and value
    int i = idx;
    EEPROM.update(i++, nodeType);
    EEPROM.put(i, value);
    i += sizeof(value);
    // Write the children (but not resolved names)
    if (nodeType != NT_Name) {
        Node *c = firstChild;
        while (c) {
            i = c->saveToEEPROM(i);
            c = c->nextSibling;
        }
    }
    EEPROM.update(i++, 0);
    return i;
}
