#include <stdio.h>

#ifndef _RETEST_H
#define _RETEST_H

struct retest_test {
        unsigned long test_passed;
        unsigned long test_failed;
        unsigned long test_total;
};

#ifndef RETEST_TESTING_STREAM_OUT
#define RETEST_TESTING_STREAM_OUT stderr
#endif

#define RETEST_TESTING_INIT \
struct retest_test __retest_test_record = {0};

#define __RETEST_TESTING_UPDATE_PASS            \
        __retest_test_record.test_passed += 1;  \
        __retest_test_record.test_total += 1;

#define __RETEST_TESTING_UPDATE_FAIL            \
        __retest_test_record.test_failed += 1;  \
        __retest_test_record.test_total += 1;

#define __RETEST_TESTING_PRINT_FAIL                             \
        fprintf(RETEST_TESTING_STREAM_OUT,                      \
                "[log] FAILED test at file: %s, line: %d\n",    \
                __FILE__,                                       \
                __LINE__                                        \
                );

#define __RETEST_TESTING_PRINT_PASS                             \
        fprintf(RETEST_TESTING_STREAM_OUT,                      \
                "[log] PASSED test at file: %s, line: %d\n",    \
                __FILE__,                                       \
                __LINE__                                        \
                );

#define RETEST_ASSERT(TEST_EXPRESSION)                  \
        if (TEST_EXPRESSION) {                          \
                __RETEST_TESTING_UPDATE_PASS            \
                __RETEST_TESTING_PRINT_PASS             \
        } else {                                        \
                __RETEST_TESTING_UPDATE_FAIL            \
                __RETEST_TESTING_PRINT_FAIL             \
        }

#define RETEST_ASSERT_EQ(TEST_EXPL, TEST_EXPR)          \
        if (TEST_EXPL == TEST_EXPR) {                   \
                __RETEST_TESTING_UPDATE_PASS            \
                __RETEST_TESTING_PRINT_PASS             \
        } else {                                        \
                __RETEST_TESTING_UPDATE_FAIL            \
                __RETEST_TESTING_PRINT_FAIL             \
        }

#define RETEST_ASSERT_NE(TEST_EXPL, TEST_EXPR)          \
        if (TEST_EXPL != TEST_EXPR) {                   \
                __RETEST_TESTING_UPDATE_PASS            \
                __RETEST_TESTING_PRINT_PASS             \
        } else {                                        \
                __RETEST_TESTING_UPDATE_FAIL            \
                __RETEST_TESTING_PRINT_FAIL             \
        }

#define RETEST_TESTING_ADD(TESTUNIT)    \
TESTUNIT();

#define RETEST_TESTING_START int main(int argc, char** argv) {

#define RETEST_TESTING_END                                                    \
        if (__retest_test_record.test_failed != 0) {                          \
                fprintf(RETEST_TESTING_STREAM_OUT,                            \
                        "[log] test not fully passed: %ld/%ld units failed",  \
                        __retest_test_record.test_failed,                     \
                        __retest_test_record.test_total                       \
                );                                                            \
        } else {                                                              \
                fprintf(RETEST_TESTING_STREAM_OUT,                            \
                        "[log] %ld test(s) passed successfully\n",            \
                        __retest_test_record.test_passed                      \
                        );                                                    \
        }                                                                     \
}

#endif
