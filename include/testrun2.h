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
 *      @file               testrun2.h
 *      @author             Markus Toepfer
 *      @date               2017-11-17
 *
 *      @brief              Serial and parallel test executing framework
 *                          without assert.
 *
 *      For parallel test runs, this framework makes use of OpenMP.
 *
 *      ------------------------------------------------------------------------
 */

#ifndef testrun2_h
#define testrun2_h

#include <omp.h>                /* OpenMP parallel (part of GCC, Clang/LLVM) */
#include <stdbool.h>            /* C99 */
#include <stdint.h>             /* C99 */
#include <unistd.h>
#include <stdlib.h>             /* C89/C90 */
#include <stdio.h>              /* C89/C90 */
#include <string.h>             /* C89/C90 */
#include <errno.h>              /* C89/C90 */
#include <time.h>               /* C89/C90 */

#if defined(__GLIBC__)
#include <execinfo.h>           /* Gnulib backtrace functions pointer names */
#endif


#include "testrun_log.h"

#define TESTRUN_DEFAULT_CLUSTER_MAX 1000


/**
        @brief          Error initialization.
*/
#define testrun_errno() \
        (errno == 0 ? "NONE" :  strerror(errno))

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
        fprintf(stdout, "\t[OK] \t%s " message "\n", __FUNCTION__ ##__VA_ARGS__)

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
        @brief          Run a single atomar test, log the result and
                        Increse the success or failure counter.
                        Leave the current function on error.
        @param input    Boolean input to decide the logging output.
*/
#define testrun_check(test, ... )\
        if (!test) { testrun_log_NOK(__VA_ARGS__);  return -1;}


/**
        @see testrun_check
*/
#define testrun(boolean, ...)\
        testrun_check(boolean, __VA_ARGS__)

/**
        @brief          Add a new test to the test sequence.
        @param test     function pointer to a new test to be added.
*/
#define testrun_add(test)  \
        if (slot + module_test_counter == max) { \
                return -module_test_counter; \
        } else { \
                tests[slot + module_test_counter] = test; \
                module_test_counter++; \
        }


/*----------------------------------------------------------------------------*/

/**
        @brief          MACRO for array pointer initialization to NULL
        @param array    array to be initialized
        @param start    first item to set to NULL
        @param end      last item to set to NULL
*/
#define testrun_init_testcases(array, start, end, ...) \
        for (size_t i = start; i < end; i++ ) { array[i] = NULL; }

/*----------------------------------------------------------------------------*/

/**
 *      Add some test cases to a testcase function pointer array, using
 *      a user provided function to add the testcases.
 *
 *      Function will log the result of testcases added.
 *
 *      @param tests            pointer to function pointer array
 *      @param last             pointer to counter of last set item
 *      @param max              pointer to value of max items
 *      @param function         function to add the tests to the array
 *
 *      @returns                negative count of testcases to add
 *                              positive count of added testcases
 */
static inline int64_t testrun_add_testcases(
        int (*tests[])(), size_t * const last, size_t const * const max,
        int64_t (*function)(int (*tests[])(), size_t, size_t)){

        if (!tests || !function || !last || !max)
                return -1;

        if (*last >= *max)
                return -1;

        int64_t r = 0;

        r =  function(tests, *last, *max);
        if (r < 0) {

                // reinit all from last to end to NULL
                testrun_init_testcases(tests, *last, *max);

                log_error("Failed to add %jd tests to array \
                        (allready in use %jd of %jd slots)", -r, *last, *max);

                return -1;

        } else {

                *last += r;
                log_debug("Added %jd tests to array \
                        (current used %jd of %jd slots)", r, *last, *max);
        }

        return r;

}

/*----------------------------------------------------------------------------*/
/**
 *      Dumb the test cases to stdout.
 *
 *      To enable a backtrace with names, the file MUST be compiled with
 *      MODCFLAGS += -rdynamic
 *
 *      @param function         pointer to function pointer array
 *      @param items            amount of items in functions
 *      @returns                negative count of failed tests
 *                              positive count of run tests otherwise
 */
static inline bool testrun_dump_testcases(int (*functions[])(), size_t max) {

        if (!functions)
                return false;

        void *pointer = NULL;

        for (size_t i = 0; i < max; i++) {

                pointer = (void*) functions[i];

                #if defined(__GLIBC__)
                        backtrace_symbols_fd(&pointer, 1, STDOUT_FILENO);
                #else
                        fprintf(stdout, "slot %jd | %p \n", i, pointer);
                #endif
        }
}

/*----------------------------------------------------------------------------*/

/**
 *      Run a bunch of tests in parallel. This will run all configured
 *      tests independently and return the result of the test batch,
 *      once all tests are done.
 *
 *      A clock of the batch runtime will be logged in addition to the
 *      result of the testrun.
 *
 *      @param function         pointer to function pointer array
 *      @param items            amount of items in functions
 *      @returns                negative count of failed tests
 *                              positive count of run tests otherwise
 */

static inline int64_t testrun_parallel(int (*functions[])(), size_t items) {

        if (!functions || items < 1)
                return 0;

        if (items > INT64_MAX )
                return 0;

        int64_t c_OK  = 0;
        int64_t c_NOK = 0;

        clock_t start, end;
        start = clock();

        int64_t i_OK[items];
        int64_t i_NOK[items];

        int nthreads = 0, tid = 0;

        #pragma omp parallel for reduction(+: c_OK) reduction(+: c_NOK)
        for (size_t i = 0; i < items; i++){

                if (nthreads == 0){
                        tid = omp_get_thread_num();
                        if (tid == 0)
                                nthreads = omp_get_num_threads();
                }

                if (functions[i] != 0) {

                        if (functions[i]() < 0)
                                c_NOK++;
                        else
                                c_OK++;

                }
        }

        testrun_log("---------------------------------------------------------");
        testrun_log("NOTE Parallel testing");
        testrun_log("");
        testrun_log("This version is using OpenMP, which implementation in ");
        testrun_log("libgomp MAY show open still reachable as well as ");
        testrun_log("possibly lost data, when running with valgrind.");
        testrun_log("---------------------------------------------------------");


        testrun_log("Parallel RUN (%jd) TESTS in %d threads: "
                "success %jd error %jd)",
                c_OK + c_NOK, nthreads,
                c_OK, c_NOK);

        end = clock();
        testrun_log_clock(start, end);
        testrun_log("");

        if (c_NOK > 0)
                return -c_NOK;

        return c_OK;
}

/*----------------------------------------------------------------------------*/

/**
 *      Run a bunch of tests serial. This will run all configured
 *      tests independently and return the result of the test batch,
 *      once all tests are done or the first tests fails, if break_on_error
 *      is set.
 *
 *      A clock of the batch runtime will be logged in addition to the
 *      result of the testrun.
 *
 *      @param function         pointer to function pointer array
 *      @param items            amount of items in function
 *      @param break_on_error   (true) fail test batch on first error
 *                              (false) run all tests before error return
 *      @returns                negative count of failed tests
 *                              positive count of run tests otherwise
 */
static inline int64_t testrun_serial(
        int (*functions[])(), size_t items, bool break_on_error) {

        if (!functions || items < 1)
                return 0;

        if (items > INT64_MAX )
                return 0;

        int64_t c_OK  = 0;
        int64_t c_NOK = 0;

        clock_t start, end;
        start = clock();

        int nthreads = 0, tid = 0;

        for (size_t i = 0; i < items; i++){

                if (functions[i] != 0) {

                        if (functions[i]() < 0) {

                                c_NOK++;
                                if (break_on_error)
                                        break;

                        } else {

                                c_OK++;

                        }
                }
        }

        testrun_log("Serial RUN (%jd) TESTS: success %jd error %jd)",
                c_OK + c_NOK,
                c_OK, c_NOK);

        end = clock();
        testrun_log_clock(start, end);
        testrun_log("");

        if (c_NOK > 0)
                return -c_NOK;

        return c_OK;
}




static inline int64_t testrun_module_test(
        int64_t (*function_add_tests)(int*(*tests[])(), size_t last, size_t max),
        size_t max){

/*
        size_t  m = max;
        size_t  c = 0;
        int64_t r = 0;

        int (*tests[max])();

        for (size_t i = 0; i < max; i++){
                tests[i] = NULL;
        }

        r = function_add_tests(tests, c, m);
        if (r < 0) {

                for (size_t i = c; i < m; i++){
                        tests[i] = NULL;
                }

                log_debug("Failed to add module tests");
                return -1;
        } else {
                log_debug("Added %d")
                test_counter += r;
        }
*/
        return -1;
}




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
#define testrun_exec(cluster) int main(int argc, char *argv[]) {\
        argc = argc;\
        clock_t start1_t, end1_t; \
        start1_t = clock(); \
        testrun_log("\ntestrun\t%s", argv[0]);\
        int64_t result = cluster();\
        if (result > 0) \
                testrun_log("ALL TESTS RUN (%jd tests)", result);\
        end1_t = clock(); \
        testrun_log_clock(start1_t, end1_t); \
        testrun_log("");\
        result > 0 ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE); \
}





#endif /* testrun_h */
