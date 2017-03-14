#pragma once

#include "Primitives.h"
#include "Functions.h"

enum NodeType
{
    NT_Null,
    NT_Document,
    NT_Number,
    // Expressions
    NT_BinaryOperator,
    NT_UnaryOperator,
    NT_Call,
    // Declaring and Referencing Things
    NT_Function,
    NT_Sub,
    NT_Assignment,
    NT_Name,
    NT_AssignmentReference,
    NT_FunctionReference,
    // Control Statements
    NT_SwitchToSub,
    NT_End,
};

enum BinaryOperator
{
    BO_Add = '+',
    BO_Sub = '-',
    BO_Mul = '*',
    BO_Div = '/',
    BO_Pow = '^',
};

enum UnaryOperator
{
    UO_Negate = '-',
};

struct Node
{
    Node *nextSibling;
    Node *firstChild;
    NodeType nodeType;
    union {
        Number number;
        BinaryOperator binaryOperator;
        UnaryOperator unaryOperator;
        Function *function;
        Name name;
        Node *assignmentReference;
        FunctionReference *functionReference;
    } value;

    Node(NodeType nodeType)
        : nextSibling(0), firstChild(0)
        , nodeType(nodeType)
    {
        value.number = 0;
    }
    ~Node();

    void appendChild(Node *newChild);
    void removeChild(Node *child);
    inline bool hasChildNodes(Node *newChild) const { return firstChild != 0; }
    int childNodesLength() const;
    int saveToEEPROM(int idx);
    int loadFromEEPROM(int idx);

    static Node *createName(Name name) {
        Node *n = new Node(NT_Name);
        if (n) {
            n->value.name = name;
        }
        return n;
    }
    static Node *createNumberLiteral(Number value) {
        Node *n = new Node(NT_Number);
        if (n) {
            n->value.number = value;
        }
        return n;
    }
    static Node *createBinaryOperator(BinaryOperator binaryOperator, Node *left, Node *right) {
        Node *n = new Node(NT_BinaryOperator);
        if (n) {
            n->value.binaryOperator = binaryOperator;
            n->firstChild = left;
            if (left) left->nextSibling = right;
        }
        else {
            delete left;
            delete right;
        }
        return n;
    }
    static Node *createUnaryOperator(UnaryOperator unaryOperator, Node *child) {
        Node *n = new Node(NT_UnaryOperator);
        if (n) {
            n->value.unaryOperator = unaryOperator;
            n->firstChild = child;
        }
        else {
            delete child;
        }
        return n;
    }
    static Node *createAssignment(Node *left, Node *right) {
        Node *n = new Node(NT_Assignment);
        if (n) {
            n->firstChild = left;
            if (left) left->nextSibling = right;
        }
        else {
            delete left;
            delete right;
        }
        return n;
    }
    static Node *createCall(Node *f, Node *args) {
        Node *n = new Node(NT_Call);
        if (n) {
            n->firstChild = f;
            if (f) f->nextSibling = args;
        }
        else {
            delete f;
            delete args;
        }
        return n;
    }
    static Node *createSwitchToSub(Name subName) {
        Node *n = new Node(NT_SwitchToSub);
        if (n) {
            n->value.name = subName;
        }
        return n;
    }
    static Node *createEnd() {
        Node *n = new Node(NT_End);
        return n;
    }
};


