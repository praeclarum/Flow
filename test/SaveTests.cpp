
#include <Flow.h>
#include <EEPROM.h>

#include <CppUnitTest.h>

#define NODE_VALUE_SIZE sizeof(Node::value)
#define NODE_SIZE (2 + NODE_VALUE_SIZE)
#define DOC_HEADER_SIZE 2

TEST_CLASS(SaveTests)
{
    TEST_METHOD(clear)
    {
        Flow.clear();
        auto end = Flow.saveToEEPROM();
        Assert::AreEqual(42, EEPROM.read(0));
        Assert::AreEqual(NODE_VALUE_SIZE, EEPROM.read(1));
        Assert::AreEqual(DOC_HEADER_SIZE + 2*NODE_SIZE, end);
    }
    TEST_METHOD(assignment)
    {
        Flow.clear();
        Flow.eval("x = 6");
        auto end = Flow.saveToEEPROM();
        Assert::AreEqual(DOC_HEADER_SIZE + (2 + 3)*NODE_SIZE, end);
    }
    TEST_METHOD(resolvedAssignments)
    {
        Flow.clear();
        Flow.eval("x = 6");
        auto y = Flow.eval("y = x");
        auto end = Flow.saveToEEPROM();
        Assert::AreEqual(6, y);
        Assert::AreEqual(DOC_HEADER_SIZE + (2 + 2*3)*NODE_SIZE, end);
    }
    TEST_METHOD(loadResolvedAssignments)
    {
        Flow.clear();
        Flow.eval("x = 6");
        auto y = Flow.eval("y = x");
        auto end = Flow.saveToEEPROM();

        Flow.loadFromEEPROM();
        auto yy = Flow.eval("y");
        Assert::AreEqual(6, yy);
    }
};

static SaveTests t;





