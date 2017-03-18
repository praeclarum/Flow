#include <Arduino.h>
#include <EEPROM.h>

#include "Node.h"

Node::~Node()
{
    switch (nodeType)
    {
    case NT_FunctionReference:
        delete value.functionReference;
        break;
    default:
        break;
    }
    delete firstChild;
    delete nextSibling;
}

void Node::append(Node *lastSibling)
{
    Node *s = this;
    while (s->nextSibling)
        s = s->nextSibling;
    s->nextSibling = lastSibling;
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

void Node::removeChild(Node *child)
{
    if (!firstChild)
        return;
    Node *c = firstChild;
    Node *pc = 0;
    while (c) {
        if (c == child) {
            if (pc) {
                pc->nextSibling = c->nextSibling;
                c->nextSibling = 0;
                delete c;
                return;
            }
            else {
                firstChild = c->nextSibling;
                c->nextSibling = 0;
                delete c;
                return;
            }
        }
        pc = c;
        c = c->nextSibling;
    }
}

int Node::saveToEEPROM(int idx)
{
    // Is there enough space?
    if (idx + 2 + sizeof(value) > EEPROM.length()) {
        if (idx < EEPROM.length())
            EEPROM.update(idx, 0);
        return EEPROM.length();
    }
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

int Node::loadFromEEPROM(int idx)
{
    // Is there enough space?
    if (idx + 2 + sizeof(value) > EEPROM.length()) {
        return EEPROM.length();
    }
    int i = idx;
    nodeType = (NodeType)EEPROM.read(i++);
    EEPROM.get(i, value);
    i += sizeof(value);
    delete firstChild;
    firstChild = 0;
    byte nt = EEPROM.read(i);
    while (nt != 0) {
        Node *c = new Node((NodeType)nt);
        if (c) {
            i = c->loadFromEEPROM(i);
            appendChild(c);
            nt = EEPROM.read(i);
        }
        else {
            return EEPROM.length();
        }
    }
    i++;
    return i;
}

class NNode
{
public:
    Name name;
    char *text;
    NNode *less;
    NNode *greater;
    ~NNode()
    {
        free(text);
        delete less;
        delete greater;
    }
};

NameTable::NameTable()
    : head(0)
{
}

NameTable::~NameTable()
{
    delete head;
}

void NameTable::put(Name name, const char *str)
{
    NNode *pn = 0;
    NNode *n = head;
    while (n) {
        if (n->name == name)
            return;
        pn = n;
        n = (name > n->name) ? n->greater : n->less;
    }
    n = new NNode();
    n->name = name;
    n->text = strdup(str);
    n->less = 0;
    n->greater = 0;
    if (pn) {
        if (name > pn->name) pn->greater = n;
        else pn->less = n;
    }
    else {
        head = n;
    }
}

const char *NameTable::get(Name name)
{
    NNode *n = head;
    while (n) {
        if (n->name == name)
            return n->text;
        n = (name > n->name) ? n->greater : n->less;
    }
    return "?";
}
