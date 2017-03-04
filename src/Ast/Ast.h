#pragma once

#include <Arduino.h>

typedef float Num;
typedef uint32_t Ident;

class Expr
{
public:
    virtual ~Expr() {}
    virtual Num eval() = 0;
};

class Func
{
public:
    virtual ~Func() {}
    virtual int numStates() const { return 0; }
    virtual Num apply(Num *states, int argc, Expr **argv) const = 0;
};

class NumberExpr : public Expr
{
    Num number;
public:
    NumberExpr(Num number) : number(number) {}
    Num eval() { return number; }
};

class FuncallExpr : public Expr
{
    const Func *func;
    Num *state;
    int argc;
    Expr **argv;
    static Num *allocState(const Func *func) {
        if (!func) return 0;
        int n = func->numStates();
        if (n <= 0) return 0;
        return new Num[n];
    }
public:
    FuncallExpr(const Func *func, int argc, Expr **argv)
        : func(func), state(allocState(func)), argc(argc), argv(argv)
    {}
    FuncallExpr(const Func *func, Expr *left, Expr *right)
        : func(func), state(allocState(func)), argc(2)
    {
        argv = new Expr*[2];
        argv[0] = left;
        argv[1] = right;
    }
    FuncallExpr(const Func *func, Expr *arg)
        : func(func), state(allocState(func)), argc(1)
    {
        argv = new Expr*[1];
        argv[0] = arg;
    }
    ~FuncallExpr() { delete[] state; delete[] argv; state = 0; argv = 0; }
    Num eval() { return func ? func->apply(state, argc, argv) : 0; }
};


