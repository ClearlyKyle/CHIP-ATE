#ifndef __TEST_SUITE_H__
#define __TEST_SUITE_H__

#include <stdio.h>

/*
RUNNING----] test_Add_Function
-----FAILED] (test_Add.c:123) expected 10 but got 13
HAS-FAILS--]
PASSED-----] test_Add_Function
--SUMMARY--] Passed : 2, Failed 0

Example test file:

void test_startup(void)  {}
void test_teardown(void) {}

TEST_CASE(test_Add)
{
    TEST_ASSERT(3 + 4 == 6)
}

int main(void)
{
    TEST_RUN(test_Add);

    TEST_SUMMARY();

    return 0;
}
*/

void test_startup(void);  // called before each test
void test_teardown(void); // called after each test

#define TEST_CASE(name) \
    void name##_test(void)

#define _FAILED_MESSAGE(FILE, LINE, MESSAGE)

#define TEST_ASSERT(condition) \
    TEST_ASSERT_MSG(condition, "")

#define TEST_ASSERT_MSG(condition, message)                                   \
    do                                                                        \
    {                                                                         \
        if (!(condition))                                                     \
        {                                                                     \
            printf("-----FAILED] (%s:%d) %s\n", __FILE__, __LINE__, message); \
            _test_failed_count++;                                             \
        }                                                                     \
        else                                                                  \
        {                                                                     \
            _test_passed_count++;                                             \
        }                                                                     \
    } while (0)

#define TEST_EXPECT_INT(value, expected)                                                                      \
    do                                                                                                        \
    {                                                                                                         \
        if ((value) != (expected))                                                                            \
        {                                                                                                     \
            printf("-----FAILED] (%s:%d) expected %d but got %d\n", __FILE__, __LINE__, (expected), (value)); \
            _test_failed_count++;                                                                             \
        }                                                                                                     \
        else                                                                                                  \
        {                                                                                                     \
            _test_passed_count++;                                                                             \
        }                                                                                                     \
    } while (0)

#define TEST_EXPECT_CHAR(value, expected)                                                                     \
    do                                                                                                        \
    {                                                                                                         \
        if ((value) != (expected))                                                                            \
        {                                                                                                     \
            printf("-----FAILED] (%s:%d) expected %s but got %s\n", __FILE__, __LINE__, (expected), (value)); \
            _test_failed_count++;                                                                             \
        }                                                                                                     \
        else                                                                                                  \
        {                                                                                                     \
            _test_passed_count++;                                                                             \
        }                                                                                                     \
    } while (0)

#define TEST_RUN(name)                                            \
    do                                                            \
    {                                                             \
        test_startup();                                           \
        const int current_test_failed_count = _test_failed_count; \
        name##_test();                                            \
        if (current_test_failed_count == _test_failed_count)      \
            printf("PASSED-----] %s\n", #name);                   \
        else                                                      \
            printf("HAS-FAILS--] %s\n", #name);                   \
        test_teardown();                                          \
                                                                  \
    } while (0)

#define TEST_SUMMARY()                                                                         \
    do                                                                                         \
    {                                                                                          \
        printf("--SUMMARY--] Passed %d, Failed %d\n", _test_passed_count, _test_failed_count); \
    } while (0)

static int _test_passed_count = 0;
static int _test_failed_count = 0;

#endif // __TEST_SUITE_H__