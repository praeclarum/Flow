
#include <Flow.h>

#include <CppUnitTest.h>

TEST_CLASS(FunctionTests)
{
    TEST_METHOD(t)
    {
        auto t0 = Flow.eval("t");
        delay(10);
        auto t1 = Flow.eval("t");
        Assert::IsGreater (t1, t0);
        Assert::AreAboutEqual (0.01, t1 - t0, 1e-2);
    }

    TEST_METHOD(pi)
    {
        Assert::AreAboutEqual(3.14159f, Flow.eval("pi"), 1e-5f);
    }

    TEST_METHOD(sin)
    {
        Assert::AreEqual(0.0f, Flow.eval("sin"));
        Assert::AreAboutEqual(0.0f, Flow.eval("sin(pi)"), 1e-7f);
        Assert::AreAboutEqual(1.0f, Flow.eval("sin(pi/2)"), 1e-7f);
    }

    TEST_METHOD(cos)
    {
        Assert::AreEqual(1.0f, Flow.eval("cos"));
        Assert::AreAboutEqual(-1.0f, Flow.eval("cos(pi)"), 1e-7f);
        Assert::AreAboutEqual(0.0f, Flow.eval("cos(pi/2)"), 1e-7f);
    }

    TEST_METHOD(min)
    {
        Assert::AreEqual(0.0f, Flow.eval("min"));
        Assert::AreEqual(1.0f, Flow.eval("min(1)"));
        Assert::AreEqual(-1.0f, Flow.eval("min(-1)"));
        // Assert::AreEqual(1.0f, Flow.eval("min(1, 2)"));
        // Assert::AreEqual(-1.0f, Flow.eval("min(-1, 2)"));
        // Assert::AreEqual(-2.0f, Flow.eval("min(1, -2)"));
        // Assert::AreEqual(-2.0f, Flow.eval("min(-1, -2)"));
    }
};

static FunctionTests t;





