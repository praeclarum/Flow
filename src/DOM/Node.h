#pragma once

typedef float Number;
typedef uint32_t Name;

#include "Function.h"

enum NodeType
{
    NT_Document,
    NT_NumberLiteral,
    NT_BinaryOperator,
    NT_UnaryOperator,
    NT_FunctionDefinition,
    NT_Assignment,
    NT_Name,
    NT_Call,
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
        BinaryOperator binop;
        UnaryOperator unop;
        Function *function;
        Name name;
    } value;

    Node(NodeType nodeType)
        : nextSibling(0), firstChild(0)
        , nodeType(nodeType)
    {}
    ~Node();

    void appendChild(Node *newChild);
    inline bool hasChildNodes(Node *newChild) const { return firstChild != 0; }
    int childNodesLength() const;

    static Node *createName(Name name) {
        Node *n = new Node(NT_Name);
        if (n) {
            n->value.name = name;
        }
        return n;
    }
    static Node *createNumberLiteral(Number value) {
        Node *n = new Node(NT_NumberLiteral);
        if (n) {
            n->value.number = value;
        }
        return n;
    }
    static Node *createBinaryOperator(BinaryOperator binop, Node *left, Node *right) {
        Node *n = new Node(NT_BinaryOperator);
        if (n) {
            n->value.binop = binop;
            n->firstChild = left;
            if (left) left->nextSibling = right;
        }
        return n;
    }
    static Node *createUnaryOperator(UnaryOperator unop, Node *child) {
        Node *n = new Node(NT_UnaryOperator);
        if (n) {
            n->value.unop = unop;
            n->firstChild = child;
        }
        return n;
    }
    static Node *createAssignment(Node *left, Node *right) {
        Node *n = new Node(NT_Assignment);
        if (n) {
            n->firstChild = left;
            if (left) left->nextSibling = right;
        }
        return n;
    }
    static Node *createCall(Node *f, Node *args) {
        Node *n = new Node(NT_Assignment);
        if (n) {
            n->firstChild = f;
            if (f) f->nextSibling = args;
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


