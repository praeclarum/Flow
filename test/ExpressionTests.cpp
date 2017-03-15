
#include <Flow.h>

#include <CppUnitTest.h>


TEST_CLASS(ExpressionTests)
{
    TEST_METHOD(singleDigitNumber)
    {
        FlowError e = FE_None;
        auto x = Flow.eval("2", &e);
        Assert::AreEqual(FE_None, e);
        Assert::AreEqual(2.0f, x);
    }
    TEST_METHOD(onlyDecimalNumber)
    {
        FlowError e = FE_None;
        auto x = Flow.eval(".14159", &e);
        Assert::AreEqual(FE_None, e);
        Assert::AreEqual(.14159f, x);
    }
    TEST_METHOD(numberEndingInDecimal)
    {
        FlowError e = FE_None;
        auto x = Flow.eval("14159.", &e);
        Assert::AreEqual(FE_None, e);
        Assert::AreEqual(14159.0f, x);
    }
    TEST_METHOD(decimalNumber)
    {
        FlowError e = FE_None;
        auto x = Flow.eval("3.14159", &e);
        Assert::AreEqual(FE_None, e);
        Assert::AreEqual(3.14159f, x);
    }
    TEST_METHOD(decimal)
    {
        FlowError e = FE_None;
        auto x = Flow.eval(".", &e);
        Assert::AreEqual(FE_SyntaxError, e);
    }
    TEST_METHOD(callNoArgs)
    {
        FlowError e = FE_None;
        auto x = Flow.eval("cos", &e);
        Assert::AreEqual(FE_None, e);
        Assert::AreEqual(1.0f, x);
    }
    TEST_METHOD(callEmptyArgs)
    {
        FlowError e = FE_None;
        auto x = Flow.eval("sin()", &e);
        Assert::AreEqual(FE_None, e);
        Assert::AreEqual(0.0f, x);
    }
    TEST_METHOD(callArg)
    {
        FlowError e = FE_None;
        auto x = Flow.eval("sin(0)", &e);
        Assert::AreEqual(FE_None, e);
        Assert::AreEqual(0.0f, x);
    }
    TEST_METHOD(callNewlineArg)
    {
        FlowError e = FE_None;
        auto x = Flow.eval("sin(\n0)", &e);
        Assert::AreEqual(FE_None, e);
        Assert::AreEqual(0.0f, x);
    }
    TEST_METHOD(moreFails)
    {
        FlowError e = FE_None;
        auto x = Flow.eval("2+", &e);
        Assert::AreEqual(FE_SyntaxIncomplete, e);
        Assert::AreEqual(0.0f, x);
    }
};

static ExpressionTests t;

