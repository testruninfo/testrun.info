/***
        ------------------------------------------------------------------------

        Copyright 2017 Markus Toepfer

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.

        This file is part of the testrun project. http://testrun.info

        ------------------------------------------------------------------------
 *//**

        @file           testrun_parallel.h
        @author         Markus Toepfer
        @date           2017-11-17

        @brief          Serial and parallel test executing framework with or
                        without assertion based testing.

        This is an enhanced and compatible version of the initial idea of an
        small and simple C89 compatible C unittest header (@see testrun.h)

        For parallel test runs, this framework makes use of OpenMP. Therefore
        the code MUST be compiled with -fopenmp, otherwise the code will stay
        unparallel and execution sequential.

        @NOTE to use all provided functionality of the header, tests SHOULD be
        compiled using:

                -fopenmp    (parallel execution) and
                -rdynamic   (function name backtracing)

        @NOTE Valgrind based file execution in libomp based OpenMP scenarios
        may not work, @see docs/valgrind/openMP/README.MD for additional
        information.

        ------------------------------------------------------------------------
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
#include <execinfo.h>           /* Gnulib backtrace of function pointer names */
#endif

#define TESTRUN_DEFAULT_CLUSTER_MAX 1000

/*----------------------------------------------------------------------------*/

/**
        Error initialization of none error.
*/
#define testrun_errno() \
        (errno == 0 ? "NONE" :  strerror(errno))

/*----------------------------------------------------------------------------*/

/**
        Log a failure. Failure: Inability to perform a function as expected.
*/
#define testrun_log_failure(msg, ...) \
        fprintf(stderr, "\t[FAIL]\t%s line:%d errno:%s message: " msg "\n",\
        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)

/*----------------------------------------------------------------------------*/

/**
        Log an error. Error: Difference between expected and actual result.
*/
#define testrun_log_error(msg, ...) \
        fprintf(stderr, "\t[ERROR]\t%s line:%d errno:%s message: " msg "\n",\
        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)

/*----------------------------------------------------------------------------*/

#define testrun_log_success(msg, ...) \
        fprintf(stdout, "\t[OK] \t%s " msg "\n", __FUNCTION__, ##__VA_ARGS__)

/*----------------------------------------------------------------------------*/

#define testrun_log(msg, ...) \
        fprintf(stdout, "\t" msg "\n", ##__VA_ARGS__)

/*----------------------------------------------------------------------------*/

#define testrun_log_function_info(msg, ...) \
        fprintf(stdout, "\t[INFO] \t%s line:%d message: " msg "\n", \
                __FUNCTION__, __LINE__, ##__VA_ARGS__)

/*----------------------------------------------------------------------------*/

#define testrun_log_clock(start, end) \
        fprintf(stdout, "\tClock ticks function: ( %s ) | %f | %.0f ms \n", \
        __func__, \
        ((double)(end - start)) / CLOCKS_PER_SEC, \
        (((double)(end - start)) / CLOCKS_PER_SEC ) * 1000)

/*----------------------------------------------------------------------------
 *
 *      Block of supporting MACROS for assert based testing.
 *
 *      Assert based testing is build around the principle to bundle and
 *      define some testcases, which will be run in series.
 *      Within the testcases testrun_assert(), or assert() may be used to
 *      stop testing.
 *
 *      -----------------------------------------------------------------
 *
 *      Example usage:
 *
 *      int testcase1_function(){
 *              assert(true);
 *              return testrun_log_success();
 *      }
 *
 *      int testcase1_function(){
 *              testrun_assert(true, "additional info an failure.");
 *              return testrun_log_success();
 *      }
 *
 *      int testseries() {
 *
 *              testrun_init();
 *
 *              testrun_test(testcase1_function);
 *              testrun_test(testcase2_function);
 *
 *              return testrun_counter;
 *      }
 *
 *      testrun_run(testseries);
 *
 *----------------------------------------------------------------------------*/

#define testrun_init()  \
        int result = 0; \
        int testrun_counter = 0;

/*----------------------------------------------------------------------------*/

/**
        Wrapper around assert, which adds a message level to assert, to provide
        additional and related information e.g. a failure description.

        @param test     an actual test case e.g. (1 == 0)
        @param message  additional message to log e.g. "Failure: 1 is not one"
*/
#define testrun_assert(test, ... )\
        if (!(test)) { testrun_log_error(__VA_ARGS__); assert(test); }

/*----------------------------------------------------------------------------*/

/**
        Run a single test (execute a function pointer. Runs a test function.
        On non negative return value of the function run, a testrun_counter
        is increased, on negative result, the negative result will be returned.

        @param test     function pointer of the test to run
        @NOTE           The surrounding block is left on negative result of the
                        function pointer execution.
*/
#define testrun_test(test)\
        result = test(); testrun_counter++; if (result < 0) return result;

/*----------------------------------------------------------------------------*/

/**
        Runs a function pointer, which SHALL contain the test function pointers
        to run. The function pointer is wrapped in a main procedure, which and
        allows indepentent testruns of the input testcluster over external
        execution.

        A clock will be started, as soon as the main is executed and the the
        time is stopped again, at the end of the execution. The difference
        will be printed and is the runtime of the whole input testcluster.

        A run will fail, as soon as one of the tests in the testcluster fails.
        (Fail on first) or will run all functions dependent on the testsetup.

        @param testcluster      function pointer to be executed.
*/
#define testrun_run(testcluster) int main(int argc, char *argv[]) {\
        argc = argc;\
        clock_t start1_t, end1_t; \
        start1_t = clock(); \
        testrun_log("\ntestrun\t%s", argv[0]);\
        int64_t result = testcluster();\
        if (result > 0) \
                testrun_log("ALL TESTS RUN (%jd tests)", result);\
        end1_t = clock(); \
        testrun_log_clock(start1_t, end1_t); \
        testrun_log("");\
        result >= 0 ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE); \
}

/*----------------------------------------------------------------------------
 *
 *      Block of supporting MACROS an inline functions for sequntial and
 *      parallel testing. Most of the functionality is realted to configure
 *      testseries for parallel and/or sequential runs. Which functions may
 *      be run as parallel tests or sequential tests, is up to the test
 *      developer.
 *
 *      This type of testing is highly customizable and may be adapted
 *      and customized by each test module implementation.
 *
 *      -----------------------------------------------------------------
 *
 *      An implementation MUST to support the testrun_fun_tests() function
 *      is the implementation of the configure functions. These functions
 *      define, which testseries may be run in parallel and which sequential.
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
 *      -----------------------------------------------------------------
 *
 *      Example usage:
 *
 *      int testcase1_function(){
 *              testrun(true);
 *              return testrun_log_success();
 *      }
 *
 *      int testcase1_function(){
 *              testrun(true, "additional info an failure.");
 *              return testrun_log_success();
 *      }
 *
 *      int64_t testseries(int(*tests[])(), size_t slot, size_t max) {
 *
 *              testrun_init();
 *
 *              testrun_add(testcase1_function);
 *              testrun_add(testcase2_function);
 *
 *              return testrun_counter;
 *      }
 *
 *      -----------------------------------------------------------------
 *
 *      NOTE: Here we configure a testseries to be run sequential and parallel
 *
 *      bool testrun_configure_parallel(
 *                      int (*testcases[])(),
 *                      size_t * const start,
 *                      size_t const * const max){
 *
 *              if (testrun_add_testcases(testcases,start,end,testseries) < 0)
 *                      return false;
 *
 *              return true;
 *
 *      bool testrun_configure_sequential(
 *                      int (*testcases[])(),
 *                      size_t * const start,
 *                      size_t const * const max){
 *
 *              if (testrun_add_testcases(testcases,start,end,testseries) < 0)
 *                      return false;
 *
 *              return true;
 *
 *      -----------------------------------------------------------------
 *
 *      NOTE: This last function definition is needed to configure the
 *      maximum amount of parallel and sequential tests as parameters
 *      instead of a predefinition.
 *
 *      int64_t run_tests(){
 *              return testrun_run_tests(1000,1000,false);
 *      }
 *
 *      testrun_run(run_tests);
 *
 *----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

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
        Run a single atomar test. Return the surrounding block on error.
        This function will leave the context block running on error. The
        Mindset is a defused assert. LEAVE THE FUNCTION NOT THE PROGRAM.

        @param test     Boolean decision input.
*/
#define testrun_check(test, ... )\
        if (!(test)) { testrun_log_error(__VA_ARGS__);  return -1;}

/*----------------------------------------------------------------------------*/

/**
        Alias to @see testrun_check.
*/
#define testrun(test, ...)\
        testrun_check(test, __VA_ARGS__ )

/*----------------------------------------------------------------------------*/

/**
        Add a new test to the tests array. This is a convinience function
        to add a function pointer to the array tests[]. This MACRO uses
        block variables **slot**, **testrun_counter**, **max** and **tests[]**.

        @param test     function pointer to a new test to be added.
*/
#define testrun_add(test)  \
        if (slot + testrun_counter == max) { \
                testrun_log_failure("All test slots filled, " \
                        "check config TESTS[MAX]."); \
                if (testrun_counter == 0) \
                        return -1; \
                return -testrun_counter; \
        } else { \
                tests[slot + testrun_counter] = test; \
                testrun_counter++; \
        }

/*----------------------------------------------------------------------------*/

/**
        Array initialization to point to NULL.

        @param array    array to be initialized
        @param start    first item to set to NULL
        @param end      last item to set to NULL
*/
#define testrun_init_testcases(array, start, end, ...) \
        for (size_t i = start; i < end; i++ ) { array[i] = NULL; }

/*----------------------------------------------------------------------------*/

/**
        Add some test cases to a testcase function pointer array, using
        a user provided function to add the testcases.

        Function will log the result of testcases added.

        @param tests            pointer to function pointer array
        @param last             pointer to counter of last set item
        @param max              pointer to value of max items
        @param function         function to add the tests to the array

        @returns                negative count of testcases to add
                                positive count of added testcases
 */
static inline int64_t testrun_add_testcases(
        int (*tests[])(),
        size_t * const last,
        size_t const * const max,
        int64_t (*function)(int (*tests[])(), size_t, size_t)){

        if (!tests || !function || !last || !max)
                return -1;

        if (*last > *max)
                return -1;

        int64_t r = 0;

        r = function(tests, *last, *max);

        if (r < 0) {

                // reinit all from last to end to NULL
                testrun_init_testcases(tests, *last, *max);

                testrun_log_failure(
                        "Failed to add tests to TESTS[] "
                        "(usage %jd/%jd)",
                        *last, *max);

                return -1;

        } else {

                *last += r;
                testrun_log_function_info(
                        "added %jd tests to TESTS[]"
                        "(usage %jd/%jd)",
                        r, *last, *max);
        }

        return r;

}

/*----------------------------------------------------------------------------*/

/**
        Dumb the test cases to stdout.

        To enable a backtrace with names, the file MUST be compiled with
        MODCFLAGS += -rdynamic

        @param function         pointer to function pointer array
        @param items            amount of items in functions
        @param names            bool to try to backtrace names
        @returns                negative count of failed tests
                                positive count of run tests otherwise
 */
static inline bool testrun_dump_testcases(
        int (*functions[])(),
        size_t max,
        bool names) {

        if (!functions || max < 1)
                return false;

        void *pointer = NULL;

        // dump is formated to fit to standard header log and to dump 20 digits
        fprintf(stdout, "\t[DUMP]\ttestcases tests[%jd]\n", max);
        if (names){
                #if defined(__GLIBC__)
                        fprintf(stdout, "\t[DUMP]\t ... try to backtrace\n");
                #else
                        fprintf(stdout, "\t[DUMP]\t ... names not implemented\n");
                #endif
        }

        for (size_t i = 0; i < max; i++) {

                pointer = (void*) functions[i];

                if (names) {
                        #if defined(__GLIBC__)
                                backtrace_symbols_fd(&pointer, 1, STDOUT_FILENO);
                        #else
                                // fallback to printf
                                fprintf(stdout, "%20jd %p \n", i, pointer);
                        #endif
                } else {
                        fprintf(stdout, " %20jd %p \n", i, pointer);
                }

        }

        return true;
}

/*----------------------------------------------------------------------------*/

/**
        Run a bunch of tests in parallel. This will run all configured
        tests independently and return the result of the test batch,
        once all tests are done.

        A clock of the batch runtime will be logged in addition to the
        result of the testrun.

        @param function         pointer to function pointer array
        @param items            amount of items in functions
        @returns                negative count of failed tests
                                positive count of run tests otherwise
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
        testrun_log("This version is using OpenMP. Using GCC for compilation ");
        testrun_log("may produce false valgrind output due to use of libomp.");
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
        Run a bunch of tests serial. This will run all configured
        tests independently and return the result of the test batch,
        once all tests are done or the first tests fails, if break_on_error
        is set.

        A clock of the batch runtime will be logged in addition to the
        result of the testrun.

        @param function         pointer to function pointer array
        @param items            amount of items in function
        @param break_on_error   (true) fail test batch on first error
                                (false) run all tests before error return
        @returns                negative count of failed tests
                                positive count of run tests otherwise
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
        Run a bunch of configurable parallel and sequential tests serial.

        @param max_parallel     maximum test cases parallel
        @param max_sequential   maximum test cases sequential
        @param break_on_error   (true) fail sequential test batch on first error
                                (false) run all sequential tests
        @returns                negative count of run tests cased on error
                                positive count of run tests
 */
static inline int64_t testrun_run_tests(
        size_t max_parallel,
        size_t max_sequential,
        bool break_on_error) {

        int64_t result_parallel   = 0;
        int64_t result_sequential = 0;
        size_t counter_parallel   = 0;
        size_t counter_sequential = 0;

        if ( (max_parallel == 0) && (max_sequential == 0))
                return -1;

        // LOAD & RUN test cases

        if (max_parallel > 0) {

                int (*testcases[max_parallel])();
                testrun_init_testcases(testcases, 0, max_parallel);

                if (!testrun_configure_parallel(
                        testcases, &counter_parallel, &max_parallel)){
                        testrun_log_failure("Failure configure parallel.");
                        return -1;
                }

                result_parallel = testrun_parallel(testcases, counter_parallel);

                if (result_parallel < 0)
                        testrun_log("Failure testrun parallel run");

        }

        if (max_sequential > 0) {

                int (*testcases[max_sequential])();
                testrun_init_testcases(testcases, 0, max_sequential);

                if (!testrun_configure_sequential(
                        testcases, &counter_sequential, &max_sequential)){
                        testrun_log_failure("Failure configure sequential.");
                        return -1;
                }

                result_sequential = testrun_sequential(
                        testcases, counter_sequential, break_on_error);

                if (result_sequential < 0)
                        testrun_log("Failure testrun sequential run");

        }

        if ( (result_parallel < 0) || (result_sequential < 0)) {
                if ( (counter_parallel + counter_sequential) == 0)
                        return -1;
                return ( -1 * (counter_parallel + counter_sequential));
        }

        return (counter_parallel + counter_sequential);
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
        #include "../tools/testrun_parallel.h"

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
                return testrun_log_success();
        }

        -----------------------------------------------------------------------

        int test_whatever_OK() {

                bool failure = false;

                // Positive result logging

                if (!failure)
                        return testrun_log_success();

                // will be reached in case of error
                return testrun_log_error();
        }

        -----------------------------------------------------------------------

        int test_whatever_NOK() {

                // Failure logging (Don't fail the testrun, just log a failure)

                if (failure)
                        return testrun_log_error();

                // will not be reached in case of error
                return testrun_log_success();

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

        //---------------------------------------------------------------------

        @code
        #include "../tools/testrun_parallel.h"

        bool example_function() {
                return true;
        }

        //---------------------------------------------------------------------

        int testcase_block1(){

                testrun(example_function());
                testrun(true);
                testrun(example_function(), "second run of function.");

                return testrun_log_success();
        }

        //---------------------------------------------------------------------

        int testcase_block2(){

                return testrun_log_success();
        }

        //---------------------------------------------------------------------

        int testcase_block3(){

                return testrun_log_success();
        }

        //---------------------------------------------------------------------

        Int testcase_block4(){

               return testrun_log_success();
        }

        //---------------------------------------------------------------------

        int64_t cluster_tests1(int(*tests[])(), size_t slot, size_t max) {

                testrun_init();                 // create local variables
                testrun_add(testcase_block1);   // adds block1 to tests[]
                testrun_add(testcase_block2);   // adds block2 to tests[]

                return testrun_counter;
        }

        //---------------------------------------------------------------------

        int64_t cluster_tests2(int(*tests[])(), size_t slot, size_t max) {

                testrun_init();                 // create local variables
                testrun_add(testcase_block3);   // adds block3 to tests[]
                testrun_add(testcase_block4);   // adds block4 to tests[]

                return testrun_counter;
        }

        //---------------------------------------------------------------------

        bool testrun_configure_parallel(
                int (*testcases[])(),
                size_t * const start,
                size_t const * const max){

                if (!testcases || !start || !max)
                        return false;

                if(testrun_add_testcases(
                        testcases,start, max, cluster_tests1) < 0)
                        return false;

                return true;
        }

        //---------------------------------------------------------------------


        bool testrun_configure_sequential(
                int (*testcases[])(),
                size_t *const start,
                size_t const * const max){

                if (!testcases || !start || !max)
                        return false;

                if(testrun_add_testcases(
                        testcases,start, max, cluster_tests1) < 0)
                        return false;

                if(testrun_add_testcases(
                        testcases,start, max, cluster_tests2) < 0)
                        return false;

                return true;

        }

        //---------------------------------------------------------------------

        int64_t run_tests() {

                return testrun_run_tests(1000,1000,false);
        }

        testrun_run(run_tests);
        @endcode

**/

#endif /* testrun_openmp_h */
