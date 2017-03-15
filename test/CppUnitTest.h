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
    const auto &_e = expected; \
    const auto &_a = actual; \
    String _s = #expected " == " #actual; \
    std::ostringstream _oss; \
    _oss << " (" << _e << " == " << _a << ")"; \
    _s += _oss.str(); \
    Assert::failOnCondition(_e == _a, _s.c_str(), __FILE__, __LINE__, __FUNCTION__); \
} while(false);

#define AreAboutEqual(expected, actual, tolerance) nop(); do { \
    const auto &_e = expected; \
    const auto &_a = actual; \
    String _s = #expected " == " #actual; \
    std::ostringstream _oss; \
    _oss << " (" << _e << " == " << _a << ")"; \
    _s += _oss.str(); \
    auto d = _e - _a; \
    if (d < 0) d = -d; \
    Assert::failOnCondition(d < tolerance, _s.c_str(), __FILE__, __LINE__, __FUNCTION__); \
} while(false);

#define IsGreater(expected, actual) nop(); do { \
    const auto &_e = expected; \
    const auto &_a = actual; \
    String _s = #expected " > " #actual; \
    std::ostringstream _oss; \
    _oss << " (" << _e << " > " << _a << ")"; \
    _s += _oss.str(); \
    Assert::failOnCondition(_e > _a, _s.c_str(), __FILE__, __LINE__, __FUNCTION__); \
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

