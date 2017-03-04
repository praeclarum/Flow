#include "Funcs.h"

class BinopFunc : public Func
{
public:
    BinopFunc() {}
    virtual Num apply(Num left, Num right) const = 0;
    Num apply(Num *states, int argc, Expr **argv) const
    {
        Num left = argc > 0 ? argv[0]->eval() : 0;
        Num right = argc > 1 ? argv[1]->eval() : 0;
        return apply(left, right);
    }
};

class UnopFunc : public Func
{
public:
    UnopFunc() {}
    virtual Num apply(Num arg) const = 0;
    Num apply(Num *states, int argc, Expr **argv) const
    {
        Num arg = argc > 0 ? argv[0]->eval() : 0;
        return apply(arg);
    }
};

class AddFunc : public BinopFunc
{
public:
    AddFunc() {}
    Num apply(Num left, Num right) const { return left + right; }
};

class SubFunc : public BinopFunc
{
public:
    SubFunc() {}
    Num apply(Num left, Num right) const { return left - right; }
};

class MulFunc : public BinopFunc
{
public:
    MulFunc() {}
    Num apply(Num left, Num right) const { return left * right; }
};

class DivFunc : public BinopFunc
{
public:
    DivFunc() {}
    Num apply(Num left, Num right) const { return left / right; }
};

class PowFunc : public BinopFunc
{
public:
    PowFunc() {}
    Num apply(Num left, Num right) const { return pow(left, right); }
};

class NegFunc : public UnopFunc
{
public:
    NegFunc() {}
    Num apply(Num arg) const { return -arg; }
};


const Func *addFunc = new AddFunc();
const Func *subFunc = new SubFunc();
const Func *mulFunc = new MulFunc();
const Func *divFunc = new DivFunc();
const Func *negFunc = new NegFunc();
const Func *powFunc = new PowFunc();

