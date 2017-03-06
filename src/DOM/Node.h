#pragma once

typedef float Number;

#include "Function.h"

enum NodeType
{
    NT_Document,
    NT_NumberLiteral,
    NT_BinaryOperator,
    NT_UnaryOperator,
    NT_FunctionDefinition,
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
    } value;

    Node(NodeType nodeType)
        : nextSibling(0), firstChild(0)
        , nodeType(nodeType)
    {}
    ~Node();

    void appendChild(Node *newChild);
    inline bool hasChildNodes(Node *newChild) const { return firstChild != 0; }
    int childNodesLength() const;

    static Node *createNumberLiteral(Number value) {
        Node *n = new Node(NT_NumberLiteral);
        n->value.number = value;
        return n;
    }
    static Node *createBinaryOperator(BinaryOperator binop, Node *left, Node *right) {
        Node *n = new Node(NT_BinaryOperator);
        n->value.binop = binop;
        n->firstChild = left;
        left->nextSibling = right;
        return n;
    }
    static Node *createUnaryOperator(UnaryOperator unop, Node *child) {
        Node *n = new Node(NT_UnaryOperator);
        n->value.unop = unop;
        n->firstChild = child;
        return n;
    }
};


