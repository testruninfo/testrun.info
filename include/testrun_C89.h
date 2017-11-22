/***
 *      ------------------------------------------------------------------------
 *
 *      Copyright 2017 Markus Toepfer
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 *
 *      This file is part of the testrun project. http://testrun.info
 *
 *      ------------------------------------------------------------------------
 *//**
 *
 *      @file               testrun_C89.h
 *      @author             Markus Toepfer
 *      @date               2017-10-31
 *
 *
 *      @brief              Simple and minimalistic test runner framework.
 *                          Intitial version of the testrun idea
 *
 *      This header only test framework runs a cluster of test cases one by one.
 *      Once one of the tests fail, the whole test cluster will fail.
 *      The whole funtionality is based on top of assert as well as
 *      function pointers to call individual test cases.
 *
 *      ------------------------------------------------------------------------
 *
 *      @see This testrun header is inspired by different sources mainly by:
 *      @see http://www.jera.com/techinfo/jtns/jtn002.html
 *      @see https://github.com/zedshaw/liblcthw/blob/master/tests/minunit.h
 *
 *      ------------------------------------------------------------------------
 */

#ifndef testrun_h
#define testrun_h

#include <stdlib.h>             /* C89/C90 */
#include <stdio.h>              /* C89/C90 */
#include <string.h>             /* C89/C90 */
#include <errno.h>              /* C89/C90 */
#include <time.h>               /* C89/C90 */
#include <assert.h>             /* C89/C90 */


#define testrun_init()  \
        int result = 0;


#define testrun_errno() \
(       errno == 0 ? "NONE" :  strerror(errno))

/**
        @brief          Log a failure
                        and include function and line information.

        @param message  message to be logged
        @param ...      VA_ARGS to be logged
**/
#define testrun_log_NOK(message, ...) \
        fprintf(stderr, "\t[ERROR]\t%s line:%d errno:%s message: " message "\n",\
        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)

/**
        @brief          Log a success

        @param message  message to be logged
        @param ...      VA_ARGS to be logged
**/
#define testrun_log_OK(message, ...) \
        fprintf(stdout, "\t[OK] \t%s" message "\n", __FUNCTION__ ##__VA_ARGS__)

/**
        @brief          Log a message

        @param intro    line intro like indentation
        @param message  message to be logged
        @param ...      VA_ARGS to be logged
**/
#define testrun_log(message, ...) \
        fprintf(stdout, "\t" message "\n", ##__VA_ARGS__)

/**
        @brief          log the start and end times of a test run.

        @param start    clock_t with at the start of a test run.
        @param end      clock_t at the end of a test run.
        @param ...      VA_ARGS to log
**/
#define testrun_log_clock(start, end, ...) \
        fprintf(stdout, "\tClock ticks function: ( %s %s) | %f | %f ms \n", \
        __func__, \
        #__VA_ARGS__, \
        ((double)(end - start)) / CLOCKS_PER_SEC, \
        (((double)(end - start)) / CLOCKS_PER_SEC )/1000)

/**
        @brief          wrapper around assert.

        add a message level to assert, to provide additional information
        related to the test case.
        E.g. a value expected or a failure description or some other
        more meaningfull than the assertion statement only (if applicable).

        @param test     an actual test case e.g. (1 == 0)
        @param message  additional message to log e.g. "Failure: 1 is not one"
**/
#define testrun_assert(test, ... )\
        if (!(test)) { testrun_log_NOK(__VA_ARGS__); assert(test); }

/**
        @brief          run a single test (function pointer)
        @param test     function pointer of the test to run

        Runs a test function and increase the test counter on success.
        Input to the test run is a function pointer.
**/
#define testrun_test(test)\
        result = test(); testrun_counter++; if (result < 0) return result;


/**
        @brief          run a cluster of tests (function pointers)
        @param cluster  function pointer to function containing the test
                        function pointers.

        Runs a function pointer, which shall contain the test function pointers
        to run. The function pointer is wrapped in a main procedure,
        which allows indepentent test runs of the test cluster.

        A clock will be started, when the command is invoked and stopped before
        it returns.

        A run will fail, as soon as one of the tests in the testcluster fails.
        (Fail early)

        Note: Based on assert based test aborts, the test run will either abort
        on failure or succed.
**/
#define testrun_run(cluster) int main(int argc, char *argv[]) {\
        argc = argc;\
        clock_t start_t, end_t; \
        start_t = clock(); \
        testrun_log("\ntestrun\t%s", argv[0]);\
        int result = cluster();\
        if (result > 0) \
                testrun_log("ALL TESTS RUN (%d tests)", testrun_counter);\
        end_t = clock(); \
        testrun_log_clock(start_t, end_t); \
        testrun_log("");\
        result > 0 ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE); \
}

int testrun_counter; /** counter for successful tests **/

/**
        @example        testrun_example.c
        @author         Markus Toepfer
        @date           2017-10-31

        @brief          Example test file using testrun.h

        This example shows the usage of the test header defined in testrun.h
        Test functions **SHALL** be prefixed with **test_** followed
        by the function name. This way external tools may relate functions
        and test_functions. e.g.  **add(int a, int b)** with **test_add()**.

        @code
        #include "../tools/testrun.h"
        #include "../../src/example.c"

        static int testing = 1;

        int test_with_assert_function() {

                //     Fail on first testing
                //
                //     Fail on first can be implemented using assert,
                //     or by returning a negative result of the testrun_test
                //     The following examples do all the same, the will stop
                //     the whole testrun and report a failure.

                testrun_assert(
                        example_function() == true, \
                        "Failure: NOK result is true."
                );

                assert(true == example_function());
                assert(example_function());

                if (!example_function())
                        return -1;

                // will not be reached in case of error
                return testrun_log_test_OK();
        }

        int test_whatever_OK() {

                // Positive result logging

                if (!failure)
                        return testrun_log_test_OK();

                // will be reached in case of error
                return testrun_log_test_NOK();
        }

        int test_whatever_NOK() {

                // Failure logging (Don't fail the testrun, just log a failure)

                if (failure)
                        return testrun_log_test_NOK();

                // will not be reached in case of error
                return testrun_log_test_OK();

        }

        int all_tests() {

                testrun_init();

                if (!testing < 1) {
                    testrun_log_test("TESTING SWITCHED OFF");
                    return 1;
                }

                testrun_test(test_with_assert_function);
                testrun_test(test_whatever_OK);
                testrun_test(test_whatever_NOK);

                return 1;

        }

        testrun_run(all_tests);
        @endcode

**/
#endif /* testrun_h */
