#pragma once

#include <iostream>
#include <sstream>

class AssertFailed {};

class Assert
{
public:
    static int count;
    static int failCount;
    static void failOnCondition(bool c, const char *message, const char *file, int line, const char *function)
    {
        count++;
        if (c) {

        }
        else {
            failCount++;
            printf("%s:%d: %s: %s FAILED\n", file, line, function, message);
            throw AssertFailed();
        }
    }
    static void nop() {}
};

#define AreEqual(expected, actual) nop(); do { \
    const auto &e = expected; \
    const auto &a = actual; \
    String s = #expected " == " #actual; \
    std::ostringstream oss; \
    oss << " (" << e << " == " << a << ")"; \
    s += oss.str(); \
    Assert::failOnCondition(e == a, s.c_str(), __FILE__, __LINE__, __FUNCTION__); \
} while(false);

#define TEST_CLASS(className) \
class className

#define TEST_METHOD(methodName) \
    struct methodName##Info { \
        methodName##Info() { \
try { methodName(); } catch (const AssertFailed &a) {}\
        } \
    }; \
    methodName##Info methodName##Info_; \
    static void methodName()

