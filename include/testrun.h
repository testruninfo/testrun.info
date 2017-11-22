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
 *      @file               testrun.h
 *      @author             Markus Toepfer
 *      @date               2017-11-17
 *
 *      @brief              Serial and parallel test executing framework
 *                          with or without assert based testing.
 *
 *      For parallel test runs, this framework makes use of OpenMP and the
 *      code MUST be compiled with -fopenmp (for GCC), otherwise the code
 *      will stay unparallel and execute sequential.
 *
 *      @NOTE to use all provided functionality of the header, tests SHOULD be
 *      compiled using -fopenmp and -rdynamic
 *
 *      ------------------------------------------------------------------------
 */

#ifndef testrun_h
#define testrun_h

#include <omp.h>                /* OpenMP parallel (part of GCC, Clang/LLVM) */

#include <stdbool.h>            /* C99 */
#include <stdint.h>             /* C99 */

#include <unistd.h>             /* C89/C90 */
#include <stdlib.h>             /* C89/C90 */
#include <stdio.h>              /* C89/C90 */
#include <string.h>             /* C89/C90 */
#include <errno.h>              /* C89/C90 */
#include <time.h>               /* C89/C90 */
#include <assert.h>             /* C89/C90 */

#if defined(__GLIBC__)
#include <execinfo.h>           /* Gnulib backtrace functions pointer names */
#endif

#define TESTRUN_DEFAULT_CLUSTER_MAX 1000

/*----------------------------------------------------------------------------
 *
 *      Block for log function macros used within the framework.
 *
 *      All logging is based in simple STDOUT STDERR logging to
 *      simplify the framework, and to be able to provide all
 *      functionality using a small header only style.
 *
 *      Fprintf statements are used within testrun_log macros for
 *      readability and structural reasons of tests files only.
 *
 *----------------------------------------------------------------------------*/

/**
        @brief          Error initialization.
*/
#define testrun_errno() \
        (errno == 0 ? "NONE" :  strerror(errno))

/*----------------------------------------------------------------------------*/

/**
        @brief          Log a failure.
        @param msg      message to be logged
        @param ...      VA_ARGS to be logged
**/
#define testrun_log_NOK(msg, ...) \
        fprintf(stderr, "\t[ERROR]\t%s line:%d errno:%s message: " msg "\n",\
        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)

/*----------------------------------------------------------------------------*/

/**
        @brief          Log a success.
        @param msg      message to be logged
        @param ...      VA_ARGS to be logged
**/
#define testrun_log_OK(msg, ...) \
        fprintf(stdout, "\t[OK] \t%s " msg "\n", __FUNCTION__ ##__VA_ARGS__)

/*----------------------------------------------------------------------------*/

/**
        @brief          Log any message.
        @param msg      message to be logged
        @param ...      VA_ARGS to be logged
**/
#define testrun_log(msg, ...) \
        fprintf(stdout, "\t" msg "\n", ##__VA_ARGS__)

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------
 *
 *      Block of supporting MACROS for assert based testing.
 *
 *      (A) Define testblocks (e.g. unit tests, which make use of assert)
 *
 *              int test_block1(){      // e.g. block to unit test function1
 *
 *                      assert(test1);
 *                      assert(test2);
 *                      assert(test3);
 *
 *                      return testrun_log_OK();
 *              }
 *
 *              int test_block2(){      // e.g. block to unit test function2
 *
 *                      assert(test1);
 *                      assert(test2);
 *                      assert(test3);
 *
 *                      return testrun_log_OK();
 *              }
 *
 *      (B) Cluster tests to be run as series (create an executable test series)
 *
 *              int assert_based_test_series() {
 *
 *                      testrun_init();            // create local variables
 *
 *                      testrun_test(test_block1); // execution of block1
 *                      testrun_test(test_block2); // execution of block2
 *
 *                      return testrun_counter;    // created in testrun_init()
 *              }
 *
 *      (C) Execute a testseries over a main() function
 *
 *              testrun_run(assert_based_test_series);
 *
 *----------------------------------------------------------------------------*/

/**
        @brief          Default initialization.
*/
#define testrun_init()  \
        int result = 0; \
        int testrun_counter = 0;

/**
        @brief          Wrapper around assert, which adds a message level
                        to assert, to provide additional, related information
                        E.g. an expected value, or a failure description.
        @param test     an actual test case e.g. (1 == 0)
        @param message  additional message to log e.g. "Failure: 1 is not one"
*/
#define testrun_assert(test, ... )\
        if (!(test)) { testrun_log_NOK(__VA_ARGS__); assert(test); }

/**
        @brief          Run a single test (execute function pointer)
                        Runs a test function.
                        On success      increase a test counter.
                        On error        return the parent function
                                        with the result of the function pointer
        @param test     function pointer of the test to run
*/
#define testrun_test(test)\
        result = test(); testrun_counter++; if (result < 0) return result;

/*----------------------------------------------------------------------------
 *
 *      Block of supporting MACROS for series and parallel testing. This type
 *      of testing is highly customizable and may be adapted by each test module
 *      implementation.
 *
 *      A given implementation MUST is the implementation of the configure
 *      functions:
 *
 *              bool testrun_configure_parallel(
 *                      int (*testcases[])(),
 *                      size_t * const start,
 *                      size_t const * const max);
 *
 *      as well as
 *
 *              bool testrun_configure_sequential(
 *                      int (*testcases[])(),
 *                      size_t * const start,
 *                      size_t const * const max);
 *
 *      NOTE TBD It is foreseen to reuse these configuration functions
 *      to embed the whole testcase setup of the module within a library based
 *      import of the tests to a continuous integration server.
 *
 *      (A)     Define testblocks (e.g. unit tests,
 *              which make use of testrun() MACRO)
 *
 *              int test_block1(){      // e.g. block to unit test function1
 *
 *                      testrun(test1);
 *                      testrun(test2);
 *                      testrun(test3);
 *              }
 *
 *              int test_block2(){      // e.g. block to unit test function2
 *
 *                      testrun(test1);
 *                      testrun(test2);
 *                      testrun(test3);
 *              }
 *
 *      (B) Cluster tests to be run (create a test array)
 *
 *              // parameter names MUST be used as stated,
 *              // otherwise testrun_add will fail.
 *
 *              int64_t cluster_tests(
 *                      int(*tests[])(), size_t slot, size_t max) {
 *
 *                      testrun_init();            // create local variables
 *                      testrun_add(test_block1);  // adds block1 to tests[]
 *                      testrun_add(test_block2);  // adds block2 to tests[]
 *
 *                      return testrun_counter;
 *              }
 *
 *      (C) Configure testruns (functions MUST be implemented in testfile)
 *
 *              bool testrun_configure_parallel(
 *                      int (*testcases[])(),
 *                      size_t * const start,
 *                      size_t const * const max){
 *
 *                      int r = 0;
 *
 *                      r = testrun_add_testcases(testcases,
 *                                      start, end, cluster_tests);
 *
 *                      if (r < 0)
 *                              return false;
 *
 *                      return true;
 *              }
 *
 *              bool testrun_configure_sequential(
 *                      int (*testcases[])(),
 *                      size_t * const start,
 *                      size_t const * const max){
 *
 *                      int r = 0;
 *
 *                      r = testrun_add_testcases(testcases,
 *                                      start, end, cluster_tests);
 *
 *                      if (r < 0)
 *                              return false;
 *
 *                      return true;
 *
 *              }
 *
 *      (D) Create an execution sequence
 *
 *              int64_t run_tests() {
 *
 *                      // MAY be done local or using the header function
 *                      return testrun_run_tests(1000,1000,false);
 *              }
 *
 *
 *      (E) Execute an execution sequence of testcases over a main() function
 *
 *              testrun_run(run_tests);
 *
 *----------------------------------------------------------------------------*/

/**
        @brief          Run a single atomar test. This function is leaving the
                        test block function, where it is executed, on error.
                        (e.g. Mindset note ... think of it like assert defused
                         ... to leave a function, instead of the program ...)
                        On error the block which failed will be logged.
                        (e.g. the unit test block using this function)
        @param test     Boolean decision input.
*/
#define testrun_check(test, ... )\
        if (!test) { testrun_log_NOK(__VA_ARGS__);  return -1;}

/*----------------------------------------------------------------------------*/

/**
        @brief          alias to @see testrun_check
*/
#define testrun(boolean, ...)\
        testrun_check(boolean, __VA_ARGS__)

/*----------------------------------------------------------------------------*/
/**
        @brief          Add a new test to the tests array.
                        This is just a convinience function to add a function
                        pointer to the array tests[]. It used the variables of
                        the function slot, testrun_counter, max and tests[].
        @param test     function pointer to a new test to be added.
*/
#define testrun_add(test)  \
        if (slot + testrun_counter == max) { \
                return -testrun_counter; \
        } else { \
                tests[slot + testrun_counter] = test; \
                testrun_counter++; \
        }

/*----------------------------------------------------------------------------*/

/**
        @brief          Array initialization to point to NULL
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
        int (*tests[])(),
        size_t * const last,
        size_t const * const max,
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

                testrun_log(    "... \tfailed to add %jd tests to array"
                                "(allready in use %jd of %jd slots)",
                                -r, *last, *max);

                return -1;

        } else {

                *last += r;
                testrun_log(    "... \tadded %jd tests to array"
                                "(current used %jd of %jd slots)",
                                r, *last, *max);
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
static inline bool testrun_dump_testcases(
        int (*functions[])(),
        size_t max) {

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

        return true;
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

static inline int64_t testrun_parallel(
        int (*functions[])(),
        size_t items) {

        if (!functions || items < 1)
                return 0;

        if (items > INT64_MAX )
                return 0;

        int64_t c_OK  = 0;
        int64_t c_NOK = 0;

        clock_t start, end;
        start = clock();

        int nthreads = 0, tid = 0;

        /*
         *      Use this if you want to reduce or set the number of threads
         *
         *      omp_set_dynamic(0);
         *      omp_set_num_threads(1);
         */

        #pragma omp parallel for
        for (size_t i = 0; i < items; i++){

                if (nthreads == 0){
                        tid = omp_get_thread_num();
                        if (tid == 0)
                                nthreads = omp_get_num_threads();
                }

                if (functions[i] != 0) {

                        if (functions[i]() < 0){
                                #pragma omp atomic
                                c_NOK++;
                        } else {
                                #pragma omp atomic
                                c_OK++;
                        }
                }
        }

        testrun_log("---------------------------------------------------------");
        testrun_log("NOTE PARALLEL TESTING");
        testrun_log("");
        testrun_log("This version is using OpenMP.");
        testrun_log("Using GCC for compilation may produce unreliable valgrind");
        testrun_log("output due to custom synchronization primitives(non POSIX).");
        testrun_log("More information is included in docs/valgrind/openMP.");
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
static inline int64_t testrun_sequential(
        int (*functions[])(),
        size_t items,
        bool break_on_error) {

        if (!functions || items < 1)
                return 0;

        if (items > INT64_MAX )
                return 0;

        int64_t c_OK  = 0;
        int64_t c_NOK = 0;

        clock_t start, end;
        start = clock();

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

/*----------------------------------------------------------------------------*/

/**
        MUST be implemented to configure parallel tests.
        @param testcases        array of function pointers
        @param start            first slot the be used in testcases
        @param max              maximum slots of testcases (last slot to be set)
        @returns                true on success, false on errror
*/
bool testrun_configure_parallel(
        int (*testcases[])(),
        size_t * const start,
        size_t const * const max);

/**
        MUST be implemented to configure sequential tests.
        @param testcases        array of function pointers
        @param start            first slot the be used in testcases
        @param max              maximum slots of testcases (last slot to be set)
        @returns                true on success, false on errror
*/
bool testrun_configure_sequential(
        int (*testcases[])(),
        size_t * const start,
        size_t const * const max);

/*----------------------------------------------------------------------------*/

/**
 *      Run a bunch of configurable parallel and sequential tests serial.
 *
 *      @param max_parallel     maximum test cases parallel
 *      @param max_sequential   maximum test cases sequential
 *      @param break_on_error   (true) fail sequential test batch on first error
 *                              (false) run all sequential tests
 *      @returns                negative count of run tests cased on error
 *                              positive count of run tests
 */
static inline int64_t testrun_run_tests(
        size_t max_parallel,
        size_t max_sequential,
        bool break_on_error) {

        int64_t result_parallel   = 0;
        int64_t result_sequential = 0;
        size_t counter_parallel   = 0;
        size_t counter_sequential = 0;

        // LOAD & RUN test cases

        if (max_parallel > 0) {

                int (*testcases[max_parallel])();
                testrun_init_testcases(testcases, 0, max_parallel);

                if (!testrun_configure_parallel(
                        testcases, &counter_parallel, &max_parallel))
                        return -1;

                result_parallel = testrun_parallel(testcases, counter_parallel);

                if (result_parallel < 0)
                        testrun_log("Failure testrun parallel run");

        }

        if (max_sequential > 0) {

                int (*testcases[max_sequential])();
                testrun_init_testcases(testcases, 0, max_sequential);

                if (!testrun_configure_sequential(
                        testcases, &counter_sequential, &max_sequential))
                        return -1;

                result_sequential = testrun_sequential(
                        testcases, counter_sequential, break_on_error);

                if (result_sequential < 0)
                        testrun_log("Failure testrun sequential run");

        }

        if ( (result_parallel < 0) || (result_sequential < 0))
                return ( -1 * (counter_parallel + counter_sequential));

        return (counter_parallel + counter_sequential);
}

/*----------------------------------------------------------------------------*/

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

/**     -----------------------------------------------------------------------

        @example        testrun_assert_example.c
        @author         Markus Toepfer
        @date           2017-10-31

        @brief          Example test file using testrun.h

        This example shows assert() style based testing with testrun.h and is
        build around the testrun_test() macro, which increases a counter which
        MUST be initialized in a testcluster function.

        -----------------------------------------------------------------------

        @code
        #include "../tools/testrun.h"
        #include "../../src/example.c"

        bool example_function() {
                return true;
        }
        -----------------------------------------------------------------------

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
                return testrun_log_OK();
        }

        -----------------------------------------------------------------------

        int test_whatever_OK() {

                bool failure = false;

                // Positive result logging

                if (!failure)
                        return testrun_log_OK();

                // will be reached in case of error
                return testrun_log_NOK();
        }

        -----------------------------------------------------------------------

        int test_whatever_NOK() {

                // Failure logging (Don't fail the testrun, just log a failure)

                if (failure)
                        return testrun_log_NOK();

                // will not be reached in case of error
                return testrun_log_OK();

        }

        -----------------------------------------------------------------------

        int assert_based_testing() {

                testrun_init();

                testrun_test(test_with_assert_function);
                testrun_test(test_whatever_OK);
                testrun_test(test_whatever_NOK);

                return testrun_counter;

        }

        testrun_run(assert_based_testing);
        @endcode

**/
/**     -----------------------------------------------------------------------

        @example        testrun_example.c
        @author         Markus Toepfer
        @date           2017-11-22

        @brief          Example test file using testrun.h

        This example shows parallel and sequential style based testing
        with testrun.h and is build around a MACRO set to execute tests in
        parallel or seqentuial run.

        -----------------------------------------------------------------------

        @code
        #include "../tools/testrun.h"
        #include "../../src/example.c"

        bool example_function() {
                return true;
        }
        -----------------------------------------------------------------------

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
                return testrun_log_OK();
        }

        -----------------------------------------------------------------------

        int test_whatever_OK() {

                bool failure = false;

                // Positive result logging

                if (!failure)
                        return testrun_log_OK();

                // will be reached in case of error
                return testrun_log_NOK();
        }

        -----------------------------------------------------------------------

        int test_whatever_NOK() {

                // Failure logging (Don't fail the testrun, just log a failure)

                if (failure)
                        return testrun_log_NOK();

                // will not be reached in case of error
                return testrun_log_OK();

        }

        -----------------------------------------------------------------------

        int assert_based_testing() {

                testrun_init();

                testrun_test(test_with_assert_function);
                testrun_test(test_whatever_OK);
                testrun_test(test_whatever_NOK);

                return testrun_counter;

        }

        testrun_run(assert_based_testing);
        @endcode

**/

#endif /* testrun_h */
