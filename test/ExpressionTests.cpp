
#include <Flow.h>

#include <CppUnitTest.h>


TEST_CLASS(ExpressionTests)
{
    TEST_METHOD(singleDigitNumber)
    {
        auto x = Flow.eval("2");
        Assert::AreEqual(2.0f, x);
    }
    TEST_METHOD(moreFails)
    {
        auto x = Flow.eval("2+");
        Assert::AreEqual(0.0f, x);
    }
};

static ExpressionTests t;





