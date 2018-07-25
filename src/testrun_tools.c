/***
        ------------------------------------------------------------------------

        Copyright 2018 Markus Toepfer

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.

        ------------------------------------------------------------------------
*//**
        @file           testrun_tools.c
        @author         Markus Toepfer
        @date           2018-07-10

        @ingroup        testrun_lib

        @brief          Standard implementation of all required testrun tools.

        ------------------------------------------------------------------------
*/
#include "../include/testrun_tools.h"

/*
 *      ------------------------------------------------------------------------
 *
 *      SHELL header 
 *
 *      this constant string will be used to generate the default SHELL header
 *      of the testrun scripts. 
 *
 *      ------------------------------------------------------------------------
 */

static const char *bash_header = 
"#!/usr/bin/env bash\n"
"#\n"
"#       Copyright 2017 Markus Toepfer\n"
"#\n"
"#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"#       you may not use this file except in compliance with the License.\n"
"#       You may obtain a copy of the License at\n"
"#\n"
"#               http://www.apache.org/licenses/LICENSE-2.0\n"
"#\n"
"#       Unless required by applicable law or agreed to in writing, software\n"
"#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"#       See the License for the specific language governing permissions and\n"
"#       limitations under the License.\n"
"#\n"
"#       ------------------------------------------------------------------------\n";

/*
 *      ------------------------------------------------------------------------
 *
 *      C header file                                             #TESRUN_HEADER
 *
 *      this constant string will be used to generate 
 *      testrun.h 
 *
 *      ------------------------------------------------------------------------
 */

static const char *testrun_header = 
"/***\n"
"        ------------------------------------------------------------------------\n"
"\n"
"        Copyright 2017 Markus Toepfer\n"
"\n"
"        Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"        you may not use this file except in compliance with the License.\n"
"        You may obtain a copy of the License at\n"
"\n"
"                http://www.apache.org/licenses/LICENSE-2.0\n"
"\n"
"        Unless required by applicable law or agreed to in writing, software\n"
"        distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"        See the License for the specific language governing permissions and\n"
"        limitations under the License.\n"
"\n"
"        This file is part of the testrun project. http://testrun.info\n"
"\n"
"        ------------------------------------------------------------------------\n"
"*//**\n"
"\n"
"        @file           testrun.h\n"
"        @author         Markus Toepfer\n"
"        @date           2017-11-24\n"
"\n"
"        @brief          Simple serial test execution framework.\n"
"\n"
"        NOTE            This framework uses an alternative to assert based\n"
"                        testing, which is compatible with parallel\n"
"                        test execution of @see testrun2.h. So this header is\n"
"                        replacable with testrun2.h for parallel test setup,\n"
"                        without replacing any written testcase.\n"
"\n"
"                        (Assert.h is not included, to force to write testrun2.h\n"
"                        compatible tests by default)\n"
"\n"
"        ------------------------------------------------------------------------\n"
"*/\n"
"\n"
"#ifndef testrun_h\n"
"#define testrun_h\n"
"\n"
"#include <unistd.h>     /* C89/C90 */\n"
"#include <stdlib.h>     /* C89/C90 */\n"
"#include <stdio.h>      /* C89/C90 */\n"
"#include <string.h>     /* C89/C90 */\n"
"#include <errno.h>      /* C89/C90 */\n"
"#include <time.h>       /* C89/C90 */\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        String error initialization of no error.\n"
"*/\n"
"#define testrun_errno() \\\n"
"        (errno == 0 ? \"NONE\" :  strerror(errno))\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Log a failure. Failure: Inability to perform a function as expected.\n"
"*/\n"
"#define testrun_log_failure(msg, ...) \\\n"
"        fprintf(stderr, \"\\t[FAIL]\\t%s line:%d errno:%s message: \" msg \"\\n\",\\\n"
"        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Log an error. Error: Difference between expected and actual result.\n"
"*/\n"
"#define testrun_log_error(msg, ...) \\\n"
"        fprintf(stderr, \"\\t[ERROR]\\t%s line:%d errno:%s message: \" msg \"\\n\",\\\n"
"        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log_success(msg, ...) \\\n"
"        fprintf(stdout, \"\\t[OK] \\t%s \" msg \"\\n\", __FUNCTION__, ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log(msg, ...) \\\n"
"        fprintf(stdout, \"\\t\" msg \"\\n\", ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log_function_info(msg, ...) \\\n"
"        fprintf(stdout, \"\\t[INFO] \\t%s line:%d message: \" msg \"\\n\", \\\n"
"                __FUNCTION__, __LINE__, ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log_clock(start, end) \\\n"
"        fprintf(stdout, \"\\tClock ticks function: ( %s ) | %f s | %.0f ms \\n\", \\\n"
"        __func__, \\\n"
"        ((double)(end - start)) / CLOCKS_PER_SEC, \\\n"
"        (((double)(end - start)) / CLOCKS_PER_SEC ) * 1000)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_init()  \\\n"
"        int result = 0; \\\n"
"        int testrun_counter = 0;\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Run a single atomar test. Return the surrounding block on error.\n"
"        This function will leave the context block running on error. The\n"
"        Mindset is a defused assert. LEAVE THE FUNCTION NOT THE PROGRAM.\n"
"\n"
"        @param test     boolean decision input.\n"
"        @returns        the calling function on error with -1\n"
"*/\n"
"#define testrun_check(test, ... )\\\n"
"        if (!(test)) { testrun_log_error(__VA_ARGS__);  return -1;}\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Alias to @see testrun_check.\n"
"*/\n"
"#define testrun(test, ...)\\\n"
"        testrun_check(test, __VA_ARGS__ )\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/*--------------- For EXAMPLE code check http://testrun.info -----------------*/\n"
"/**\n"
"        Run a single test (execute a function pointer. Runs a test function.\n"
"        On non negative return value of the function run, a testrun_counter\n"
"        is increased, on negative result, the negative result will be returned.\n"
"\n"
"        @param test     function pointer of the test to run\n"
"        @NOTE           The surrounding block is left on negative result of the\n"
"                        function pointer execution.\n"
"*/\n"
"#define testrun_test(test)\\\n"
"        result = test(); testrun_counter++; if (result < 0) return result;\n"
"\n"
"/**\n"
"        Runs a function pointer, which SHALL contain the test function pointers\n"
"        to run. The function pointer is wrapped in a main procedure, which and\n"
"        allows indepentent testruns of the input testcluster over external\n"
"        execution.\n"
"\n"
"        A clock will be started, as soon as the main is executed and the the\n"
"        time is stopped again, at the end of the execution. The difference\n"
"        will be printed and is the runtime of the whole input testcluster.\n"
"\n"
"        A run will fail, as soon as one of the tests in the testcluster fails.\n"
"        (Fail on first) or will run all functions dependent on the testsetup.\n"
"\n"
"        @param testcluster      function pointer to be executed.\n"
"*/\n"
"#define testrun_run(testcluster) int main(int argc, char *argv[]) {\\\n"
"        argc = argc;\\\n"
"        clock_t start1_t, end1_t; \\\n"
"        start1_t = clock(); \\\n"
"        testrun_log(\"\\ntestrun\\t%s\", argv[0]);\\\n"
"        int64_t result = testcluster();\\\n"
"        if (result > 0) \\\n"
"                testrun_log(\"ALL TESTS RUN (%jd tests)\", result);\\\n"
"        end1_t = clock(); \\\n"
"        testrun_log_clock(start1_t, end1_t); \\\n"
"        testrun_log(\"\");\\\n"
"        result >=0 ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE); \\\n"
"}\n"
"\n"
"/**     -----------------------------------------------------------------------\n"
"\n"
"        @example        testrun_base_example.c\n"
"        @author         Markus Toepfer\n"
"        @date           2017-11-24\n"
"\n"
"        @brief          Example test file using testrun.h\n"
"\n"
"        This example shows the test style for testing with testrun.h and is\n"
"        build around the testrun_test() macro, which increases a counter which\n"
"        MUST be initialized in a testcluster function.\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        @code\n"
"        #include \"../tools/testrun.h\"\n"
"\n"
"        int example_function() {\n"
"                return 1;\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        int test_function1() {\n"
"\n"
"                // use of testrun_check() for evaluation\n"
"                testrun_check(1 == 1);\n"
"                testrun_check(1 == 1, \"some additional information\");\n"
"\n"
"                // use of testrun() for evaluation\n"
"                testrun(1 == 1);\n"
"                testrun(1 == 1, \"some additional information\");\n"
"\n"
"                // use of manual evaluation and logging\n"
"                if (1 != example_function()){\n"
"                        testrun_log_error(\"some additional information.\");\n"
"                        return -1;\n"
"                }\n"
"\n"
"                // will not be reached in case of error\n"
"                return testrun_log_success();\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        int test_function2() {\n"
"\n"
"                testrun_check(1 == 1);\n"
"                return testrun_log_success();\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        int test_function3() {\n"
"\n"
"                testrun_check(1 == 1);\n"
"                return testrun_log_success();\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        int testcase_cluster() {\n"
"\n"
"                testrun_init();\n"
"\n"
"                testrun_test(test_function1);\n"
"                testrun_test(test_function2);\n"
"                testrun_test(test_function3);\n"
"\n"
"                return testrun_counter;\n"
"\n"
"        }\n"
"\n"
"        testrun_run(testcase_cluster);\n"
"        @endcode\n"
"\n"
"*/\n"
"\n"
"#endif /* testrun_h */\n";

/*
 *      ------------------------------------------------------------------------
 *
 *      C header file                                             #OPENMP_HEADER
 *
 *      this constant string will be used to generate 
 *      the testrun_openmp.h 
 *
 *      ------------------------------------------------------------------------
 */

static const char *testrun_header_openmp = 
"/***\n"
"        ------------------------------------------------------------------------\n"
"\n"
"        Copyright 2017 Markus Toepfer\n"
"\n"
"        Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"        you may not use this file except in compliance with the License.\n"
"        You may obtain a copy of the License at\n"
"\n"
"                http://www.apache.org/licenses/LICENSE-2.0\n"
"\n"
"        Unless required by applicable law or agreed to in writing, software\n"
"        distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"        See the License for the specific language governing permissions and\n"
"        limitations under the License.\n"
"\n"
"        This file is part of the testrun project. http://testrun.info\n"
"\n"
"        ------------------------------------------------------------------------\n"
" *//**\n"
"\n"
"        @file           testrun_openmp.h\n"
"        @author         Markus Toepfer\n"
"        @date           2017-11-17\n"
"\n"
"        @brief          Serial and parallel test executing framework with or\n"
"                        without assertion based testing.\n"
"\n"
"        This is an enhanced and compatible version of the initial idea of an\n"
"        small and simple C89 compatible C unittest header (@see testrun.h)\n"
"\n"
"        For parallel test runs, this framework makes use of OpenMP. Therefore\n"
"        the code MUST be compiled with -fopenmp, otherwise the code will stay\n"
"        unparallel and execution sequential.\n"
"\n"
"        @NOTE to use all provided functionality of the header, tests SHOULD be\n"
"        compiled using:\n"
"\n"
"                -fopenmp    (parallel execution) and\n"
"                -rdynamic   (function name backtracing)\n"
"\n"
"        @NOTE Valgrind based file execution in libomp based OpenMP scenarios\n"
"        may not work, @see docs/valgrind/openMP/README.MD for additional\n"
"        information.\n"
"\n"
"        ------------------------------------------------------------------------\n"
" */\n"
"\n"
"#ifndef testrun_openmp_h\n"
"#define testrun_openmp_h\n"
"\n"
"#include <omp.h>                /* OpenMP parallel (part of GCC, Clang/LLVM) */\n"
"\n"
"#include <stdbool.h>            /* C99 */\n"
"#include <stdint.h>             /* C99 */\n"
"\n"
"#include <unistd.h>             /* C89/C90 */\n"
"#include <stdlib.h>             /* C89/C90 */\n"
"#include <stdio.h>              /* C89/C90 */\n"
"#include <string.h>             /* C89/C90 */\n"
"#include <errno.h>              /* C89/C90 */\n"
"#include <time.h>               /* C89/C90 */\n"
"#include <assert.h>             /* C89/C90 */\n"
"\n"
"#if defined(__GLIBC__)\n"
"#include <execinfo.h>           /* Gnulib backtrace of function pointer names */\n"
"#endif\n"
"\n"
"#define TESTRUN_DEFAULT_CLUSTER_MAX 1000\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Error initialization of none error.\n"
"*/\n"
"#define testrun_errno() \\\n"
"        (errno == 0 ? \"NONE\" :  strerror(errno))\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Log a failure. Failure: Inability to perform a function as expected.\n"
"*/\n"
"#define testrun_log_failure(msg, ...) \\\n"
"        fprintf(stderr, \"\\t[FAIL]\\t%s line:%d errno:%s message: \" msg \"\\n\",\\\n"
"        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Log an error. Error: Difference between expected and actual result.\n"
"*/\n"
"#define testrun_log_error(msg, ...) \\\n"
"        fprintf(stderr, \"\\t[ERROR]\\t%s line:%d errno:%s message: \" msg \"\\n\",\\\n"
"        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log_success(msg, ...) \\\n"
"        fprintf(stdout, \"\\t[OK] \\t%s \" msg \"\\n\", __FUNCTION__, ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log(msg, ...) \\\n"
"        fprintf(stdout, \"\\t\" msg \"\\n\", ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log_function_info(msg, ...) \\\n"
"        fprintf(stdout, \"\\t[INFO] \\t%s line:%d message: \" msg \"\\n\", \\\n"
"                __FUNCTION__, __LINE__, ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log_clock(start, end) \\\n"
"        fprintf(stdout, \"\\tClock ticks function: ( %s ) | %f | %.0f ms \\n\",\\\n"
"        __func__, \\\n"
"        ((double)(end - start)) / CLOCKS_PER_SEC, \\\n"
"        (((double)(end - start)) / CLOCKS_PER_SEC ) * 1000)\n"
"\n"
"/*----------------------------------------------------------------------------\n"
" *\n"
" *      Block of supporting MACROS for assert based testing.\n"
" *\n"
" *      Assert based testing is build around the principle to bundle and\n"
" *      define some testcases, which will be run in series.\n"
" *      Within the testcases testrun_assert(), or assert() may be used to\n"
" *      stop testing.\n"
" *\n"
" *      -----------------------------------------------------------------\n"
" *\n"
" *      Example usage:\n"
" *\n"
" *      int testcase1_function(){\n"
" *              assert(true);\n"
" *              return testrun_log_success();\n"
" *      }\n"
" *\n"
" *      int testcase1_function(){\n"
" *              testrun_assert(true, \"additional info an failure.\");\n"
" *              return testrun_log_success();\n"
" *      }\n"
" *\n"
" *      int testseries() {\n"
" *\n"
" *              testrun_init();\n"
" *\n"
" *              testrun_test(testcase1_function);\n"
" *              testrun_test(testcase2_function);\n"
" *\n"
" *              return testrun_counter;\n"
" *      }\n"
" *\n"
" *      testrun_run(testseries);\n"
" *\n"
" *----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_init()  \\\n"
"        int result = 0; \\\n"
"        int testrun_counter = 0;\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Wrapper around assert, which adds a message level to assert, to provide\n"
"        additional and related information e.g. a failure description.\n"
"\n"
"        @param test     an actual test case e.g. (1 == 0)\n"
"        @param message  additional message to log e.g. \"Failure: 1 is not one\"\n"
"*/\n"
"#define testrun_assert(test, ... )\\\n"
"        if (!(test)) { testrun_log_error(__VA_ARGS__); assert(test); }\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Run a single test (execute a function pointer. Runs a test function.\n"
"        On non negative return value of the function run, a testrun_counter\n"
"        is increased, on negative result, the negative result will be returned.\n"
"\n"
"        @param test     function pointer of the test to run\n"
"        @NOTE           The surrounding block is left on negative result of the\n"
"                        function pointer execution.\n"
"*/\n"
"#define testrun_test(test)\\\n"
"        result = test(); testrun_counter++; if (result < 0) return result;\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Runs a function pointer, which SHALL contain the test function pointers\n"
"        to run. The function pointer is wrapped in a main procedure, which and\n"
"        allows indepentent testruns of the input testcluster over external\n"
"        execution.\n"
"\n"
"        A clock will be started, as soon as the main is executed and the the\n"
"        time is stopped again, at the end of the execution. The difference\n"
"        will be printed and is the runtime of the whole input testcluster.\n"
"\n"
"        A run will fail, as soon as one of the tests in the testcluster fails.\n"
"        (Fail on first) or will run all functions dependent on the testsetup.\n"
"\n"
"        @param testcluster      function pointer to be executed.\n"
"*/\n"
"#define testrun_run(testcluster) int main(int argc, char *argv[]) {\\\n"
"        argc = argc;\\\n"
"        clock_t start1_t, end1_t; \\\n"
"        start1_t = clock(); \\\n"
"        testrun_log(\"\\ntestrun\\t%s\", argv[0]);\\\n"
"        int64_t result = testcluster();\\\n"
"        if (result > 0) \\\n"
"                testrun_log(\"ALL TESTS RUN (%jd tests)\", result);\\\n"
"        end1_t = clock(); \\\n"
"        testrun_log_clock(start1_t, end1_t); \\\n"
"        testrun_log(\"\");\\\n"
"        result >= 0 ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE); \\\n"
"}\n"
"\n"
"/*----------------------------------------------------------------------------\n"
" *\n"
" *      Block of supporting MACROS an inline functions for sequntial and\n"
" *      parallel testing. Most of the functionality is realted to configure\n"
" *      testseries for parallel and/or sequential runs. Which functions may\n"
" *      be run as parallel tests or sequential tests, is up to the test\n"
" *      developer.\n"
" *\n"
" *      This type of testing is highly customizable and may be adapted\n"
" *      and customized by each test module implementation.\n"
" *\n"
" *      -----------------------------------------------------------------\n"
" *\n"
" *      An implementation MUST to support the testrun_fun_tests() function\n"
" *      is the implementation of the configure functions. These functions\n"
" *      define, which testseries may be run in parallel and which sequential.\n"
" *\n"
" *              bool testrun_configure_parallel(\n"
" *                      int (*testcases[])(),\n"
" *                      size_t * const start,\n"
" *                      size_t const * const max);\n"
" *\n"
" *      as well as\n"
" *\n"
" *              bool testrun_configure_sequential(\n"
" *                      int (*testcases[])(),\n"
" *                      size_t * const start,\n"
" *                      size_t const * const max);\n"
" *\n"
" *      -----------------------------------------------------------------\n"
" *\n"
" *      Example usage:\n"
" *\n"
" *      int testcase1_function(){\n"
" *              testrun(true);\n"
" *              return testrun_log_success();\n"
" *      }\n"
" *\n"
" *      int testcase1_function(){\n"
" *              testrun(true, \"additional info an failure.\");\n"
" *              return testrun_log_success();\n"
" *      }\n"
" *\n"
" *      int64_t testseries(int(*tests[])(), size_t slot, size_t max) {\n"
" *\n"
" *              testrun_init();\n"
" *\n"
" *              testrun_add(testcase1_function);\n"
" *              testrun_add(testcase2_function);\n"
" *\n"
" *              return testrun_counter;\n"
" *      }\n"
" *\n"
" *      -----------------------------------------------------------------\n"
" *\n"
" *      NOTE: Here we configure a testseries to be run sequential and parallel\n"
" *\n"
" *      bool testrun_configure_parallel(\n"
" *                      int (*testcases[])(),\n"
" *                      size_t * const start,\n"
" *                      size_t const * const max){\n"
" *\n"
" *              if (testrun_add_testcases(testcases,start,end,testseries) < 0)\n"
" *                      return false;\n"
" *\n"
" *              return true;\n"
" *\n"
" *      bool testrun_configure_sequential(\n"
" *                      int (*testcases[])(),\n"
" *                      size_t * const start,\n"
" *                      size_t const * const max){\n"
" *\n"
" *              if (testrun_add_testcases(testcases,start,end,testseries) < 0)\n"
" *                      return false;\n"
" *\n"
" *              return true;\n"
" *\n"
" *      -----------------------------------------------------------------\n"
" *\n"
" *      NOTE: This last function definition is needed to configure the\n"
" *      maximum amount of parallel and sequential tests as parameters\n"
" *      instead of a predefinition.\n"
" *\n"
" *      int64_t run_tests(){\n"
" *              return testrun_run_tests(1000,1000,false);\n"
" *      }\n"
" *\n"
" *      testrun_run(run_tests);\n"
" *\n"
" *----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        MUST be implemented to configure parallel tests.\n"
"\n"
"        @param testcases        array of function pointers\n"
"        @param start            first slot the be used in testcases\n"
"        @param max              maximum slots of testcases (last slot to be set)\n"
"        @returns                true on success, false on errror\n"
"*/\n"
"bool testrun_configure_parallel(\n"
"        int (*testcases[])(),\n"
"        size_t * const start,\n"
"        size_t const * const max);\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        MUST be implemented to configure sequential tests.\n"
"\n"
"        @param testcases        array of function pointers\n"
"        @param start            first slot the be used in testcases\n"
"        @param max              maximum slots of testcases (last slot to be set)\n"
"        @returns                true on success, false on errror\n"
"*/\n"
"bool testrun_configure_sequential(\n"
"        int (*testcases[])(),\n"
"        size_t * const start,\n"
"        size_t const * const max);\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Run a single atomar test. Return the surrounding block on error.\n"
"        This function will leave the context block running on error. The\n"
"        Mindset is a defused assert. LEAVE THE FUNCTION NOT THE PROGRAM.\n"
"\n"
"        @param test     Boolean decision input.\n"
"*/\n"
"#define testrun_check(test, ... )\\\n"
"        if (!(test)) { testrun_log_error(__VA_ARGS__);  return -1;}\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Alias to @see testrun_check.\n"
"*/\n"
"#define testrun(test, ...)\\\n"
"        testrun_check(test, __VA_ARGS__ )\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Add a new test to the tests array. This is a convinience function\n"
"        to add a function pointer to the array tests[]. This MACRO uses\n"
"        block variables **slot**, **testrun_counter**, **max** and **tests[]**.\n"
"\n"
"        @param test     function pointer to a new test to be added.\n"
"*/\n"
"#define testrun_add(test)  \\\n"
"        if (slot + testrun_counter == max) { \\\n"
"                testrun_log_failure(\"All test slots filled, \" \\\n"
"                        \"check config TESTS[MAX].\"); \\\n"
"                if (testrun_counter == 0) \\\n"
"                        return -1; \\\n"
"                return -testrun_counter; \\\n"
"        } else { \\\n"
"                tests[slot + testrun_counter] = test; \\\n"
"                testrun_counter++; \\\n"
"        }\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Array initialization to point to NULL.\n"
"\n"
"        @param array    array to be initialized\n"
"        @param start    first item to set to NULL\n"
"        @param end      last item to set to NULL\n"
"*/\n"
"#define testrun_init_testcases(array, start, end, ...) \\\n"
"        for (size_t i = start; i < end; i++ ) { array[i] = NULL; }\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Add some test cases to a testcase function pointer array, using\n"
"        a user provided function to add the testcases.\n"
"\n"
"        Function will log the result of testcases added.\n"
"\n"
"        @param tests            pointer to function pointer array\n"
"        @param last             pointer to counter of last set item\n"
"        @param max              pointer to value of max items\n"
"        @param function         function to add the tests to the array\n"
"\n"
"        @returns                negative count of testcases to add\n"
"                                positive count of added testcases\n"
" */\n"
"static inline int64_t testrun_add_testcases(\n"
"        int (*tests[])(),\n"
"        size_t * const last,\n"
"        size_t const * const max,\n"
"        int64_t (*function)(int (*tests[])(), size_t, size_t)){\n"
"\n"
"        if (!tests || !function || !last || !max)\n"
"                return -1;\n"
"\n"
"        if (*last > *max)\n"
"                return -1;\n"
"\n"
"        int64_t r = 0;\n"
"\n"
"        r = function(tests, *last, *max);\n"
"\n"
"        if (r < 0) {\n"
"\n"
"                // reinit all from last to end to NULL\n"
"                testrun_init_testcases(tests, *last, *max);\n"
"\n"
"                testrun_log_failure(\n"
"                        \"Failed to add tests to TESTS[] \"\n"
"                        \"(usage %jd/%jd)\",\n"
"                        *last, *max);\n"
"\n"
"                return -1;\n"
"\n"
"        } else {\n"
"\n"
"                *last += r;\n"
"                testrun_log_function_info(\n"
"                        \"added %jd tests to TESTS[]\"\n"
"                        \"(usage %jd/%jd)\",\n"
"                        r, *last, *max);\n"
"        }\n"
"\n"
"        return r;\n"
"\n"
"}\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Dumb the test cases to stdout.\n"
"\n"
"        To enable a backtrace with names, the file MUST be compiled with\n"
"        MODCFLAGS += -rdynamic\n"
"\n"
"        @param function         pointer to function pointer array\n"
"        @param items            amount of items in functions\n"
"        @param names            bool to try to backtrace names\n"
"        @returns                negative count of failed tests\n"
"                                positive count of run tests otherwise\n"
" */\n"
"static inline bool testrun_dump_testcases(\n"
"        int (*functions[])(),\n"
"        size_t max,\n"
"        bool names) {\n"
"\n"
"        if (!functions || max < 1)\n"
"                return false;\n"
"\n"
"        void *pointer = NULL;\n"
"\n"
"        // dump is formated to fit to standard header log and to dump 20 digits\n"
"        fprintf(stdout, \"\\t[DUMP]\\ttestcases tests[%jd]\\n\", max);\n"
"        if (names){\n"
"                #if defined(__GLIBC__)\n"
"                        fprintf(stdout, \"\\t[DUMP]\\t ... try to backtrace\\n\");\n"
"                #else\n"
"                        fprintf(stdout, \"\\t[DUMP]\\t ... names not implemented\\n\");\n"
"                #endif\n"
"        }\n"
"\n"
"        for (size_t i = 0; i < max; i++) {\n"
"\n"
"                pointer = (void*) functions[i];\n"
"\n"
"                if (names) {\n"
"                        #if defined(__GLIBC__)\n"
"                                backtrace_symbols_fd(&pointer, 1, STDOUT_FILENO);\n"
"                        #else\n"
"                                // fallback to printf\n"
"                                fprintf(stdout, \"%20jd %p \\n\", i, pointer);\n"
"                        #endif\n"
"                } else {\n"
"                        fprintf(stdout, \" %20jd %p \\n\", i, pointer);\n"
"                }\n"
"\n"
"        }\n"
"\n"
"        return true;\n"
"}\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Run a bunch of tests in parallel. This will run all configured\n"
"        tests independently and return the result of the test batch,\n"
"        once all tests are done.\n"
"\n"
"        A clock of the batch runtime will be logged in addition to the\n"
"        result of the testrun.\n"
"\n"
"        @param function         pointer to function pointer array\n"
"        @param items            amount of items in functions\n"
"        @returns                negative count of failed tests\n"
"                                positive count of run tests otherwise\n"
" */\n"
"static inline int64_t testrun_parallel(\n"
"        int (*functions[])(),\n"
"        size_t items) {\n"
"\n"
"        if (!functions || items < 1)\n"
"                return 0;\n"
"\n"
"        if (items > INT64_MAX )\n"
"                return 0;\n"
"\n"
"        int64_t c_OK  = 0;\n"
"        int64_t c_NOK = 0;\n"
"\n"
"        clock_t start, end;\n"
"        start = clock();\n"
"\n"
"        int nthreads = 0, tid = 0;\n"
"\n"
"\n"
"        /*\n"
"         *      Use this if you want to reduce or set the number of threads\n"
"         *\n"
"         *      omp_set_dynamic(0);\n"
"         *      omp_set_num_threads(1);\n"
"         */\n"
"\n"
"        #pragma omp parallel for\n"
"        for (size_t i = 0; i < items; i++){\n"
"\n"
"                if (nthreads == 0){\n"
"                        tid = omp_get_thread_num();\n"
"                        if (tid == 0)\n"
"                                nthreads = omp_get_num_threads();\n"
"                }\n"
"\n"
"                if (functions[i] != 0) {\n"
"\n"
"                        if (functions[i]() < 0){\n"
"                                #pragma omp atomic\n"
"                                c_NOK++;\n"
"                        } else {\n"
"                                #pragma omp atomic\n"
"                                c_OK++;\n"
"                        }\n"
"                }\n"
"        }\n"
"\n"
"        testrun_log(\"---------------------------------------------------------\");\n"
"        testrun_log(\"NOTE PARALLEL TESTING\");\n"
"        testrun_log(\"\");\n"
"        testrun_log(\"This version is using OpenMP. Using GCC for compilation \");\n"
"        testrun_log(\"may produce false valgrind output due to use of libomp.\");\n"
"        testrun_log(\"More information is included in docs/valgrind/openMP.\");\n"
"        testrun_log(\"---------------------------------------------------------\");\n"
"\n"
"\n"
"        testrun_log(\"Parallel RUN (%jd) TESTS in %d threads: \"\n"
"                \"success %jd error %jd)\",\n"
"                c_OK + c_NOK, nthreads,\n"
"                c_OK, c_NOK);\n"
"\n"
"        end = clock();\n"
"        testrun_log_clock(start, end);\n"
"        testrun_log(\"\");\n"
"\n"
"        if (c_NOK > 0)\n"
"                return -c_NOK;\n"
"\n"
"        return c_OK;\n"
"}\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Run a bunch of tests serial. This will run all configured\n"
"        tests independently and return the result of the test batch,\n"
"        once all tests are done or the first tests fails, if break_on_error\n"
"        is set.\n"
"\n"
"        A clock of the batch runtime will be logged in addition to the\n"
"        result of the testrun.\n"
"\n"
"        @param function         pointer to function pointer array\n"
"        @param items            amount of items in function\n"
"        @param break_on_error   (true) fail test batch on first error\n"
"                                (false) run all tests before error return\n"
"        @returns                negative count of failed tests\n"
"                                positive count of run tests otherwise\n"
" */\n"
"static inline int64_t testrun_sequential(\n"
"        int (*functions[])(),\n"
"        size_t items,\n"
"        bool break_on_error) {\n"
"\n"
"        if (!functions || items < 1)\n"
"                return 0;\n"
"\n"
"        if (items > INT64_MAX )\n"
"                return 0;\n"
"\n"
"        int64_t c_OK  = 0;\n"
"        int64_t c_NOK = 0;\n"
"\n"
"        clock_t start, end;\n"
"        start = clock();\n"
"\n"
"        for (size_t i = 0; i < items; i++){\n"
"\n"
"                if (functions[i] != 0) {\n"
"\n"
"                        if (functions[i]() < 0) {\n"
"\n"
"                                c_NOK++;\n"
"                                if (break_on_error)\n"
"                                        break;\n"
"\n"
"                        } else {\n"
"\n"
"                                c_OK++;\n"
"\n"
"                        }\n"
"                }\n"
"        }\n"
"\n"
"        testrun_log(\"Serial RUN (%jd) TESTS: success %jd error %jd)\",\n"
"                c_OK + c_NOK,\n"
"                c_OK, c_NOK);\n"
"\n"
"        end = clock();\n"
"        testrun_log_clock(start, end);\n"
"        testrun_log(\"\");\n"
"\n"
"        if (c_NOK > 0)\n"
"                return -c_NOK;\n"
"\n"
"        return c_OK;\n"
"}\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Run a bunch of configurable parallel and sequential tests serial.\n"
"\n"
"        @param max_parallel     maximum test cases parallel\n"
"        @param max_sequential   maximum test cases sequential\n"
"        @param break_on_error   (true) fail sequential test batch on first error\n"
"                                (false) run all sequential tests\n"
"        @returns                negative count of run tests cased on error\n"
"                                positive count of run tests\n"
" */\n"
"static inline int64_t testrun_run_tests(\n"
"        size_t max_parallel,\n"
"        size_t max_sequential,\n"
"        bool break_on_error) {\n"
"\n"
"        int64_t result_parallel   = 0;\n"
"        int64_t result_sequential = 0;\n"
"        size_t counter_parallel   = 0;\n"
"        size_t counter_sequential = 0;\n"
"\n"
"        if ( (max_parallel == 0) && (max_sequential == 0))\n"
"                return -1;\n"
"\n"
"        // LOAD & RUN test cases\n"
"\n"
"        if (max_parallel > 0) {\n"
"\n"
"                int (*testcases[max_parallel])();\n"
"                testrun_init_testcases(testcases, 0, max_parallel);\n"
"\n"
"                if (!testrun_configure_parallel(\n"
"                        testcases, &counter_parallel, &max_parallel)){\n"
"                        testrun_log_failure(\"Failure configure parallel.\");\n"
"                        return -1;\n"
"                }\n"
"\n"
"                result_parallel = testrun_parallel(testcases, counter_parallel);\n"
"\n"
"                if (result_parallel < 0)\n"
"                        testrun_log(\"Failure testrun parallel run\");\n"
"\n"
"        }\n"
"\n"
"        if (max_sequential > 0) {\n"
"\n"
"                int (*testcases[max_sequential])();\n"
"                testrun_init_testcases(testcases, 0, max_sequential);\n"
"\n"
"                if (!testrun_configure_sequential(\n"
"                        testcases, &counter_sequential, &max_sequential)){\n"
"                        testrun_log_failure(\"Failure configure sequential.\");\n"
"                        return -1;\n"
"                }\n"
"\n"
"                result_sequential = testrun_sequential(\n"
"                        testcases, counter_sequential, break_on_error);\n"
"\n"
"                if (result_sequential < 0)\n"
"                        testrun_log(\"Failure testrun sequential run\");\n"
"\n"
"        }\n"
"\n"
"        if ( (result_parallel < 0) || (result_sequential < 0)) {\n"
"                if ( (counter_parallel + counter_sequential) == 0)\n"
"                        return -1;\n"
"                return ( -1 * (counter_parallel + counter_sequential));\n"
"        }\n"
"\n"
"        return (counter_parallel + counter_sequential);\n"
"}\n"
"\n"
"/**     -----------------------------------------------------------------------\n"
"\n"
"        @example        testrun_assert_example.c\n"
"        @author         Markus Toepfer\n"
"        @date           2017-10-31\n"
"\n"
"        @brief          Example test file using testrun.h\n"
"\n"
"        This example shows assert() style based testing with testrun.h and is\n"
"        build around the testrun_test() macro, which increases a counter which\n"
"        MUST be initialized in a testcluster function.\n"
"\n"
"        -----------------------------------------------------------------------\n"
"\n"
"        @code\n"
"        #include \"../tools/testrun_parallel.h\"\n"
"\n"
"        bool example_function() {\n"
"                return true;\n"
"        }\n"
"        -----------------------------------------------------------------------\n"
"\n"
"        int test_with_assert_function() {\n"
"\n"
"                //     Fail on first testing\n"
"                //\n"
"                //     Fail on first can be implemented using assert,\n"
"                //     or by returning a negative result of the testrun_test\n"
"                //     The following examples do all the same, the will stop\n"
"                //     the whole testrun and report a failure.\n"
"\n"
"                testrun_assert(\n"
"                        example_function() == true, \\\n"
"                        \"Failure: NOK result is true.\"\n"
"                );\n"
"\n"
"                assert(true == example_function());\n"
"                assert(example_function());\n"
"\n"
"                if (!example_function())\n"
"                        return -1;\n"
"\n"
"                // will not be reached in case of error\n"
"                return testrun_log_success();\n"
"        }\n"
"\n"
"        -----------------------------------------------------------------------\n"
"\n"
"        int test_whatever_OK() {\n"
"\n"
"                bool failure = false;\n"
"\n"
"                // Positive result logging\n"
"\n"
"                if (!failure)\n"
"                        return testrun_log_success();\n"
"\n"
"                // will be reached in case of error\n"
"                return testrun_log_error();\n"
"        }\n"
"\n"
"        -----------------------------------------------------------------------\n"
"\n"
"        int test_whatever_NOK() {\n"
"\n"
"                // Failure logging (Don't fail the testrun, just log a failure)\n"
"\n"
"                if (failure)\n"
"                        return testrun_log_error();\n"
"\n"
"                // will not be reached in case of error\n"
"                return testrun_log_success();\n"
"\n"
"        }\n"
"\n"
"        -----------------------------------------------------------------------\n"
"\n"
"        int assert_based_testing() {\n"
"\n"
"                testrun_init();\n"
"\n"
"                testrun_test(test_with_assert_function);\n"
"                testrun_test(test_whatever_OK);\n"
"                testrun_test(test_whatever_NOK);\n"
"\n"
"                return testrun_counter;\n"
"\n"
"        }\n"
"\n"
"        testrun_run(assert_based_testing);\n"
"        @endcode\n"
"\n"
"**/\n"
"/**     -----------------------------------------------------------------------\n"
"\n"
"        @example        testrun_example.c\n"
"        @author         Markus Toepfer\n"
"        @date           2017-11-22\n"
"\n"
"        @brief          Example test file using testrun.h\n"
"\n"
"        This example shows parallel and sequential style based testing\n"
"        with testrun.h and is build around a MACRO set to execute tests in\n"
"        parallel or seqentuial run.\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        @code\n"
"        #include \"../tools/testrun_parallel.h\"\n"
"\n"
"        bool example_function() {\n"
"                return true;\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        int testcase_block1(){\n"
"\n"
"                testrun(example_function());\n"
"                testrun(true);\n"
"                testrun(example_function(), \"second run of function.\");\n"
"\n"
"                return testrun_log_success();\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        int testcase_block2(){\n"
"\n"
"                return testrun_log_success();\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        int testcase_block3(){\n"
"\n"
"                return testrun_log_success();\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        Int testcase_block4(){\n"
"\n"
"               return testrun_log_success();\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        int64_t cluster_tests1(int(*tests[])(), size_t slot, size_t max) {\n"
"\n"
"                testrun_init();                 // create local variables\n"
"                testrun_add(testcase_block1);   // adds block1 to tests[]\n"
"                testrun_add(testcase_block2);   // adds block2 to tests[]\n"
"\n"
"                return testrun_counter;\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        int64_t cluster_tests2(int(*tests[])(), size_t slot, size_t max) {\n"
"\n"
"                testrun_init();                 // create local variables\n"
"                testrun_add(testcase_block3);   // adds block3 to tests[]\n"
"                testrun_add(testcase_block4);   // adds block4 to tests[]\n"
"\n"
"                return testrun_counter;\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        bool testrun_configure_parallel(\n"
"                int (*testcases[])(),\n"
"                size_t * const start,\n"
"                size_t const * const max){\n"
"\n"
"                if (!testcases || !start || !max)\n"
"                        return false;\n"
"\n"
"                if(testrun_add_testcases(\n"
"                        testcases,start, max, cluster_tests1) < 0)\n"
"                        return false;\n"
"\n"
"                return true;\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"\n"
"        bool testrun_configure_sequential(\n"
"                int (*testcases[])(),\n"
"                size_t *const start,\n"
"                size_t const * const max){\n"
"\n"
"                if (!testcases || !start || !max)\n"
"                        return false;\n"
"\n"
"                if(testrun_add_testcases(\n"
"                        testcases,start, max, cluster_tests1) < 0)\n"
"                        return false;\n"
"\n"
"                if(testrun_add_testcases(\n"
"                        testcases,start, max, cluster_tests2) < 0)\n"
"                        return false;\n"
"\n"
"                return true;\n"
"\n"
"        }\n"
"\n"
"        //---------------------------------------------------------------------\n"
"\n"
"        int64_t run_tests() {\n"
"\n"
"                return testrun_run_tests(1000,1000,false);\n"
"        }\n"
"\n"
"        testrun_run(run_tests);\n"
"        @endcode\n"
"\n"
"**/\n"
"\n"
"#endif /* testrun_openmp_h */\n";

/*
 *      ------------------------------------------------------------------------
 *
 *      Gitignore file                                            #GITRIGNORE
 *
 *      this constant string will be used to generate 
 *      the default gitignore content.
 *
 *      ------------------------------------------------------------------------
 */

static const char *testrun_gitignore = 
"# Prerequisites\n"
"*.d\n"
"\n"
"# Object files\n"
"*.o\n"
"*.so\n"
"*.ko\n"
"*.obj\n"
"*.elf\n"
"\n"
"# Linker output\n"
"*.ilk\n"
"*.map\n"
"*.exp\n"
"\n"
"# Precompiled Headers\n"
"*.gch\n"
"*.pch\n"
"\n"
"# Libraries\n"
"*.lib\n"
"*.a\n"
"*.la\n"
"*.lo\n"
"\n"
"# Shared objects (inc. Windows DLLs)\n"
"*.dll\n"
"*.so\n"
"*.so.*\n"
"*.dylib\n"
"\n"
"# Executables\n"
"*.exe\n"
"*.out\n"
"*.app\n"
"*.i*86\n"
"*.x86_64\n"
"*.hex\n"
"\n"
"# Debug files\n"
"*.dSYM/\n"
"*.su\n"
"*.idb\n"
"*.pdb\n"
"\n"
"# Kernel Module Compile Results\n"
"*.mod*\n"
"*.cmd\n"
".tmp_versions/\n"
"modules.order\n"
"Module.symvers\n"
"Mkfile.old\n"
"dkms.conf\n"
"\n"
"# Local files\n"
"**/local\n"
"**/bin/\n"
"**/gen/\n"
"**/build/\n"
"**/docs/doxygen/\n"
"**/doxygen/documentation/\n"
"\n"
"# vagrant (if used)\n"
".vagrant\n"
"\n"
"# subprojects (if used)\n"
"*.git\n"
"\n"
"# generated package config\n"
"*.pc\n"
"\n"
"# ctags\n"
".tags\n"
"tags\n"
"functions\n"
"\n"
"# IDE\n"
"\n"
"## IntelliJ\n"
".idea\n"
"\n"
"## Sublime\n"
"*.sublime-workspace\n"
"*.sublime-project\n"
"\n"
"## VIM\n"
"[._]*.s[a-w][a-z]\n"
"[._]s[a-w][a-z]\n"
"*.un~\n"
"Session.vim\n"
".netrwhist\n"
"*~\n";

/*----------------------------------------------------------------------------*/

char *testrun_generate_header(){

        return strdup(testrun_header);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_header_openmp(){

        return strdup(testrun_header_openmp);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_gitignore(){

        return strdup(testrun_gitignore);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_readme(
        const char *projectname,
        const char *description,
        const char *copyright_string){

        size_t size = 5000;
        char buffer[size];
        memset(buffer, 0, size);

        snprintf(buffer, size,
        "# Project %s\n"
        "\n"
        "This module is self supported and may be build, tested, installed and\n"
        "run independently.\n"
        "\n"
        "## Overview\n"
        "\n"
        "* [Description](#description)\n"
        "* [Usage](#usage)\n"
        "* [Installation](#installation)\n"
        "* [Requirements](#requirements)\n"
        "* [Structure](#structure)\n"
        "* [Tests](#tests)\n"
        "* [Tips](#tips)\n"
        "* [Copyright](#copyright)\n"
        "\n"
        "## Description\n"
        "\n"
        "%s\n"
        "\n"
        "## Usage\n"
        "\n"
        "...\n"
        "\n"
        "## Installation\n"
        "\n"
        "...\n"
        """\n"
        "### build sources\n"
        "\n"
        "\\`\\`\\`bash\n"
        "make\n"
        "\\`\\`\\`\n"
        "\n"
        "### build documentation\n"
        "\n"
        "\\`\\`\\`bash\n"
        "make documentation\n"
        "\\`\\`\\`\n"
        "\n"
        "### test sources\n"
        "\n"
        "\\`\\`\\`bash\n"
        "make tested\n"
        "\\`\\`\\`\n"
        "\n"
        "### install binaries\n"
        "\n"
        "\\`\\`\\`bash\n"
        "sudo make install\n"
        "\\`\\`\\`\n"
        "\n"
        "### uninstall binaries\n"
        "\n"
        "\\`\\`\\`bash\n"
        "sudo make uninstall\n"
        "\\`\\`\\`\n"
        "\n"
        "## Requirements\n"
        "\n"
        "## Structure\n"
        "\n"
        "### Default structure of the folder:\n"
        "\n"
        "\\`\\`\\`\n"
        "<pre>\n"
        ".\n"
        "├── README.MD\n"
        "├── .gitignore\n"
        "├── makefile\n"
        "├── makefile_common.mk\n"
        "│\n"
        "├── copyright\n"
        "│   └── ... \n"
        "│\n"
        "├── doxygen\n"
        "│   ├── documentation\n"
        "│   └── doxygen.config\n"
        "│\n"
        "├── docs\n"
        "│   ├── CHANGELOG.MD\n"
        "│   └── ...\n"
        "│\n"
        "├── include\n"
        "│   ├── %s.h\n"
        "│   └── ...\n"
        "│\n"
        "├── src\n"
        "│   ├── %s.c\n"
        "│   └── ...\n"
        "│\n"
        "└── tests\n"
        "    ├── resources\n"
        "    ├── tools\n"
        "    │   ├── testrun.h\n"
        "    │   ├── testrun_runner.sh\n"
        "    │   ├── testrun_gcov.sh\n"
        "    │   ├── testrun_gprof.sh\n"
        "    │   ├── testrun_simple_coverage_tests.sh\n"
        "    │   ├── testrun_simple_unit_tests.sh\n"
        "    │   ├── testrun_simple_acceptance_tests.sh\n"
        "    │   └── testrun_simple_loc.sh\n"
        "    │\n"
        "    ├── acceptance\n"
        "    │   ├── ...\n"
        "    │   └── ...\n"
        "    │\n"
        "    └── unit\n"
        "        ├── %s_test.c\n"
        "        └── ...\n"
        "\n"
        "</pre>\n"
        "\\`\\`\\`\n"
        "\n"
        "## Tests\n"
        "\n"
        "All test sources will be recompiled on each make run. That means,\n"
        "all module tests will be created new on any change in any source file.\n"
        "\n"
        "### Test a project (all files contained in tests/unit)\n"
        "\n"
        "Test compile and run\n"
        "~~~\n"
        "make tested\n"
        "~~~\n"
        "\n"
        "## Tips\n"
        "\n"
        "## Copyright\n"
        "\n"
        "%s\n",
        projectname,
        description,
        projectname,
        projectname,
        projectname,
        copyright_string);

        return strdup(buffer);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_doxygen(
        const char *project_name,
        const char *path_doxygen,
        const char *path_mainfile,
        const char *input){

        size_t size = 5000;
        char buffer[size];
        memset(buffer, 0, size);

        if (snprintf(buffer, size,
                "DOXYFILE_ENCODING       = UTF-8\n"
                "PROJECT_NAME            = %s\n"
                "PROJECT_NUMBER          = 0.0.1\n"
                "PROJECT_LOGO            = %s/logo.png\n"
                "PROJECT_BRIEF           = %s\n"
                "OUTPUT_DIRECTORY        = %s/documentation\n"
                "CREATE_SUBDIRS          = NO\n"
                "ALLOW_UNICODE_NAMES     = NO\n"
                "OUTPUT_LANGUAGE         = English\n"
                "MARKDOWN_SUPPORT        = YES\n"
                "AUTOLINK_SUPPORT        = YES\n"
                "USE_MDFILE_AS_MAINPAGE  = %s\n"
                "INPUT                   = %s\n"
                "INPUT_ENCODING          = UTF-8\n"
                "FILE_PATTERNS           = *.h *.c *.js *.py *.sh\n"
                "RECURSIVE               = YES\n"
                "EXCLUDE_SYMLINKS        = YES\n",
                project_name, path_doxygen, project_name, path_doxygen, path_mainfile, input)< 0)
                return NULL;

        return strdup(buffer);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_service_file(
        const char *project_name,
        const char *install_path){

        size_t size = 5000;
        char buffer[size];
        memset(buffer, 0, size);

        if (snprintf(buffer, size,
                "[Unit]\n"
                "Description= %s service\n"
                "\n"
                "[Service]\n"
                "ExecStart=%s\n"
                "NonBlocking=True\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target\n"
                , project_name, install_path)< 0)
                return NULL;

        return strdup(buffer);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_socket_file(
        const char *project_name){

        size_t size = 5000;
        char buffer[size];
        memset(buffer, 0, size);

         if (snprintf(buffer, size,
                "[Unit]\n"
                "Description= %s socket\n"
                "\n"
                "[Socket]\n"
                "\n"
                "# example interface bound\n"
                "# ListenStream=10.0.0.100:12345\n"
                "\n"
                "# example localhost\n"
                "# ListenStream=127.0.0.1:12345\n"
                "\n"
                "# example listen on all\n"
                "# ListenStream=0.0.0.0:12345\n"
                "\n"
                "# example listen on UDP\n"
                "# ListenDatagram=0.0.0.0:12345\n"
                "\n"
                "# Maximum parallel connections for the socket\n"
                "Backlog=2048\n"
                "\n"
                "# TCP Keepalive (1h)\n"
                "KeepAlive=false\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target\n"
                , project_name)< 0)
                return NULL;

        return strdup(buffer);
}


/*----------------------------------------------------------------------------*/

char *testrun_generate_script_simple_tests(
        const char *type,
        const char *project,
        const char *file_name,
        const char *runner_script,
        const char *path_logfile,
        const char *path_tests,
        const char *path_tools
        ){

        if (    !type           ||
                !project        || 
                !file_name      || 
                !runner_script  || 
                !path_logfile   || 
                !path_tests     ||
                !path_tools)
                return NULL;

        size_t size = 5000;
        char buffer[size];
        memset(buffer, 0, size);

        if (0 > snprintf(buffer, size, 
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2017-11-30\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Run all test executables [PATH_TESTS]/*.test\n"
        "#                       Run the whole folder, until an error occurs.\n"
        "#\n"
        "#                       MODE         FAIL ON ERROR (Fail on first test error)\n"
        "#\n"
        "#                       LOGFILE      [PATH_LOGFILE]/%s.<time>.log\n"
        "#\n"
        "#\n"
        "#       Usage           ./%s /path/to/project\n"
        "#\n"
        "#       Dependencies    bash, touch, chmod, ls, wc, date\n"
        "#\n"
        "#       Last changed    2018-07-11\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "TEST_TYPE=\"%s\"\n"
        "FOLDER_LOGFILE=\"%s\"\n"
        "FOLDER_TESTS=\"%s\"\n"
        "RUNNER_SCRIPT=\"./%s/%s\"\n"
        "\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE $TEST_TYPE TESTING\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"$FOLDER_LOGFILE/$TEST_TYPE.$start_time.log\"\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               REPORT $TEST_TYPE TESTING\"                >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "# RUN THE RUNNER\n"
        "sh $RUNNER_SCRIPT $LOGFILE $FOLDER_TESTS FAIL_ON_ERROR\n"
        "RESULT=$?\n"
        "\n"
        "end_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# FINISH THE REPORT\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"DONE \\t $TEST_TYPE TEST RUN\" >> $LOGFILE\n"
        "if [ $RESULT -eq 0 ]; then\n"
        "        echo \"RESULT\\t SUCCESS\" >> $LOGFILE\n"
        "else\n"
        "        echo \"RESULT\\t FAILURE\" >> $LOGFILE\n"
        "fi\n"
        "echo \"START \\t $start_time\" >> $LOGFILE\n"
        "echo \"END   \\t $end_time\" >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "# DUMP THE REPORT ON SUCCESS\n"
        "if [ $RESULT -eq 0 ]; then\n"
        "        cat $LOGFILE\n"
        "        echo \"\"\n"
        "else\n"
        "        echo \"\"\n"
        "        echo \"$TEST_TYPE TEST FAILED\"\n"
        "        echo \"Logfile dump stopped to point to last error.\"\n"
        "fi\n"
        "exit $RESULT\n",
                bash_header,
                file_name,
                project,
                type,
                file_name,
                type,
                path_logfile,
                path_tests,
                path_tools,
                runner_script
                ))
                return NULL;

        return strdup(buffer);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_script_runner(
        const char *project,
        const char *file_name){

        if (    !project        || 
                !file_name)
                return NULL;

        size_t size = 5000;
        char buffer[size];
        memset(buffer, 0, size);

        if (0 > snprintf(buffer, size, 
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2017-11-30\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Run each TEST.test of a folder and log Ok or NOK\n"
        "#                       for each executed testfile of the folder.\n"
        "#\n"
        "#                       EXAMPLE OUTPUT\n"
        "#\n"
        "#                       [OK]  1/5 filename1.test\n"
        "#                       [NOK] 2/5 filename2.test\n"
        "#\n"
        "#                       MODES\n"
        "#\n"
        "#                       (1) RUN ALL TESTS (log but ignore errors)\n"
        "#                           use script with 2 parameters\n"
        "#                           e.g. ./testrun_runner.sh logfile /path\n"
        "#\n"
        "#                           This mode will not return a test failure and\n"
        "#                           may be used to run all tests and return success\n"
        "#                           if all tests was run. (test results are logged)\n"
        "#\n"
        "#                       (2) FAIL ON ERROR (Fail on first error)\n"
        "#                           use script with 3 parameters\n"
        "#                           e.g. ./testrun_runner.sh logfile /path 1\n"
        "#\n"
        "#                           This mode returns -1 on the first test failure.\n"
        "#\n"
        "#                       PARAMETER\n"
        "#\n"
        "#                       (1) path to logfile destination\n"
        "#                       (2) path to folder with test cases\n"
        "#\n"
        "#\n"
        "#       Usage           ./testrun_runner.sh /path/to/logfile /path/to/test/dir\n"
        "#\n"
        "#       Dependencies    bash, tail, ls, grep, wc\n"
        "#\n"
        "#       Last changed    2017-11-30\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "if [ -z $1 ]; then\n"
        "        echo \"ERROR ... NO LOGFILE INPUT TO SRCIPT\"\n"
        "        exit 1\n"
        "fi\n"
        "LOGFILE=$1\n"
        "\n"
        "if [ -z $2 ]; then\n"
        "        echo \"ERROR ... FOLDER INPUT TO SRCIPT\"\n"
        "        exit 1\n"
        "fi\n"
        "FOLDER=$2\n"
        "\n"
        "FAIL_ON_ERROR=0\n"
        "if [ ! -z $3 ]; then\n"
        "        FAIL_ON_ERROR=1\n"
        "fi\n"
        "\n"
        "if [ ! -w $LOGFILE ]; then\n"
        "        echo \"ERROR ... LOGFILE NOT WRITABLE\"\n"
        "        exit 1\n"
        "fi\n"
        "\n"
        "#       ------------------------------------------------------------------------\n"
        "#       PERFORM TESTRUN\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "FILES=`ls  $FOLDER/ | grep \"\\.test\" | wc -l`\n"
        "if [ $? -ne 0 ]; then\n"
        "        echo \"ERROR ... could not count files of $FOLDER\"\n"
        "        exit 1\n"
        "fi\n"
        "c=0\n"
        "\n"
        "if [ $FILES -eq 0 ]; then\n"
        "        exit 0\n"
        "fi\n"
        "\n"
        "for i in $FOLDER/*.test\n"
        "do\n"
        "        c=$((c+1))\n"
        "\n"
        "        # RUN EXECUTABLE\n"
        "        $i 2>&1 >> $LOGFILE\n"
        "\n"
        "        # CHECK RETURN OF EXECUTABLE\n"
        "        if [ $? -ne 0 ]; then\n"
        "\n"
        "                echo \"NOK\\t(\"$c\"/\"$FILES\")\\t\"$i\n"
        "\n"
        "                if [ $FAIL_ON_ERROR -eq 1 ]; then\n"
        "                        exit 1\n"
        "                fi\n"
        "        else\n"
        "                echo \"OK\\t(\"$c\"/\"$FILES\")\\t\"$i\n"
        "        fi\n"
        "done\n"
        "exit 0\n",
                bash_header,
                file_name,
                project))
                return NULL;

        return strdup(buffer);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_script_loc(
        const char *project,
        const char *file_name,
        const char *path_header,
        const char *path_source,
        const char *path_tests){

        if (    !project        || 
                !file_name      ||
                !path_header    ||
                !path_source    ||
                !path_tests)
                return NULL;

        size_t size = 5000;
        char buffer[size];
        memset(buffer, 0, size);

        if (0 > snprintf(buffer, size, 
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2017-11-30\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Count the lines of header, src and tests.\n"
        "#                       This file uses no error checking.\n"
        "#\n"
        "#       Usage           ./%s /path/to/project\n"
        "#\n"
        "#       Dependencies    bash, find, xargs, wc\n"
        "#\n"
        "#       Last changed    2018-07-11\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "FOLDER_INC=\"%s\"\n" 
        "FOLDER_SRC=\"%s\"\n" 
        "FOLDER_TST=\"%s\"\n" 
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE LOC COUNTER\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"\"\n"
        "echo \"(LOC) HEADER\"\n"
        "find $1/$FOLDER_INC -name '*.h' | xargs wc -l\n"
        "echo \"\"\n"
        "echo \"(LOC) SOURCE\"\n"
        "find $1/$FOLDER_SRC -name '*.c' | xargs wc -l\n"
        "echo \"\"\n"
        "echo \"(LOC) TESTS\"\n"
        "find $1/$FOLDER_TST -name '*.c' | xargs wc -l\n"
        "echo \"\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"\"\n",
                bash_header,
                file_name,
                project,
                file_name,
                path_header,
                path_source,
                path_tests
                ))
                return NULL;

        return strdup(buffer);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_script_coverage(
        const char *project,
        const char *file_name,
        const char *prefix,
        const char *path_logfile,
        const char *path_source,
        const char *path_tests){

        if (    !project        || 
                !file_name      ||
                !prefix         ||
                !path_logfile   ||
                !path_source    ||
                !path_tests)
                return NULL;

        size_t size = 10000;
        char buffer[size];
        memset(buffer, 0, size);

        if (0 > snprintf(buffer, size, 
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2017-11-30\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Count functions of folder src vs unit test functions.\n"
        "#\n"
        "#                       CONVENTION\n"
        "#\n"
        "#                       Each function in any file of the source folder located\n"
        "#                       \"%s\"\n"
        "#                       will have a corresponding test function,\n"
        "#                       using the same name in a file of the unit tests located at\n"
        "#                       \"%s\",\n"
        "#                       with a function name prefix of\n"
        "#                       \"%s\".\n"
        "#\n"
        "#                       EXAMPLE      function | test_function\n"
        "#\n"
        "#                       NOTE         This simple coverage test just covers the\n"
        "#                                    observance of the given coding convention.\n"
        "#\n"
        "#\n"
        "#       Usage           ./%s /path/to/project\n"
        "#\n"
        "#       Dependencies    bash, ctags, awk, sed, grep\n"
        "#\n"
        "#       Last changed    2018-07-11\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "PREFIX=\"%s\"\n"
        "\n"
        "SRCDIR=\"$1/%s\"\n"
        "TESTDIR=\"$1/%s\"\n"
        "FOLDER_LOGFILE=\"$1/%s\"\n"
        ""
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"$FOLDER_LOGFILE/coverage.$start_time.log\"\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               REPORT COVERAGE TESTING\"                  >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"   TIME \t $start_time\" >> $LOGFILE\n"
        "echo \"\" >> $LOGFILE\n"
        "\n"
        "# GENERATE CTAGS SOURCE\n"
        "cd $SRCDIR\n"
        "if [ $? -ne 0 ]; then\n"
        "        exit 1\n"
        "fi\n"
        "ctags --c-types=f -R\n"
        "# remove the ctags stuff, to leave just the function lines\n"
        "sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions\n"
        "# remove anything but the function names\n"
        "awk '{print $1 }' $SRCDIR/functions > $SRCDIR/functionNames\n"
        "# count the lines\n"
        "sourceFkt=\"$(cat functions | wc -l)\"\n"
        "echo \"   count source\\t\" $sourceFkt >> $LOGFILE\n"
        "\n"
        "# GENERATE CTAGS TESTS\n"
        "cd $TESTDIR\n"
        "if [ $? -ne 0 ]; then\n"
        "        exit 1\n"
        "fi\n"
        "ctags --c-types=f -R\n"
        "# remove the ctags stuff, to leave just the function lines\n"
        "sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions\n"
        "# remove anything but the function names\n"
        "awk '{print $1 }' $TESTDIR/functions > $TESTDIR/functionNames\n"
        "# count the lines\n"
        "testFkt=\"$(cat functions | grep -i ^$PREFIX | wc -l)\"\n"
        "echo \"   count tests\\t\" $testFkt >> $LOGFILE\n"
        "\n"
        "echo \"\nUNTESTED: \" >> $LOGFILE\n"
        "# Found functions:\n"
        "while read line;\n"
        "do\n"
        "        grep -n '^'$PREFIX$line'$' $TESTDIR/functionNames > \\\n"
        "        /dev/null || echo $line >> $LOGFILE\n"
        "done < $SRCDIR/functionNames\n"
        "\n"
        "if [ $sourceFkt != 0 ]; then\n"
        "        echo \"............................................\"  >> $LOGFILE\n"
        "        echo \"COVERAGE: $sourceFkt $testFkt\" | \\\n"
        "        awk '{ printf $1 \" %%.2f %%%% \\n\", $3/$2*100}' >> $LOGFILE\n"
        "fi\n"
        "\n"
        "cat $LOGFILE\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"\"\n"
        "\n"
        "# cleanup remove the files we created\n"
        "rm $SRCDIR/tags\n"
        "rm $SRCDIR/functions\n"
        "rm $SRCDIR/functionNames\n"
        "rm $TESTDIR/tags\n"
        "rm $TESTDIR/functions\n"
        "rm $TESTDIR/functionNames\n",
                bash_header,
                file_name,
                project,
                path_source,
                path_tests,
                prefix,
                file_name,
                prefix,
                path_source,
                path_tests,
                path_logfile
                ))
                return NULL;

        return strdup(buffer);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_script_gcov(
        const char *project,
        const char *file_name,
        const char *path_logfile,
        const char *path_tests_exec,
        const char *path_tests_source,
        const char *exec_suffix,
        const char *src_suffix
        ){

        if (    !project           || 
                !file_name         ||
                !path_logfile      ||
                !path_tests_exec   ||
                !path_tests_source)
                return NULL;

        size_t size = 5000;
        char buffer[size];
        memset(buffer, 0, size);

        if (0 > snprintf(buffer, size, 
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2018-02-09\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Run gcov based coverage tests on all test cases.\n"
        "#\n"
        "#       Usage           ./%s /path/to/project\n"
        "#\n"
        "#       Dependencies    bash, gcov\n"
        "#\n"
        "#       Last changed    2018-07-11\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "FOLDER_TEST_EXEC=\"%s\"\n"
        "FOLDER_TEST_SRC=\"%s\"\n"
        "FOLDER_LOGFILE=\"%s\"\n"
        "TEST_EXEC_SUFFIX=\"%s\"\n"
        "TEST_SRC_SUFFIX=\"%s\"\n"
        ""
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"$FOLDER_LOGFILE/gcov.$start_time.log\"\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               GCOV RUNNER\" >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "for test in $FOLDER_TEST_EXEC/*$TEST_EXEC_SUFFIX; do\n"
        "    $test\n"
        "done\n"
        "\n"
        "FILES=`ls  $FOLDER_TEST_EXEC/ | grep $TEST_EXEC_SUFFIX | wc -l`\n"
        "if [ $? -ne 0 ]; then\n"
        "        echo \"ERROR ... could not count files of $FOLDER_TEST_EXEC\"\n"
        "        exit 1\n"
        "fi\n"
        "c=0\n"
        "\n"
        "if [ $FILES -eq 0 ]; then\n"
        "        exit 0\n"
        "fi\n"
        "\n"
        "for i in $FOLDER_TEST_SRC/*$TEST_SRC_SUFFIX.c\n"
        "do\n"
        "        # RUN GCOV\n"
        "        echo $i\n"
        "        gcov $i\n"
        "done\n"
        "\n"
        "# move coverage output to log folder\n"
        "mv *.gcov $FOLDER_LOGFILE\n"
        "exit 0\n",
                bash_header,
                file_name,
                project,
                file_name,
                path_tests_exec,
                path_tests_source,
                path_logfile,
                exec_suffix,
                src_suffix
                ))
                return NULL;

        return strdup(buffer);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_script_gprof(
        const char *project,
        const char *file_name,
        const char *path_logfile,
        const char *path_tests_exec,
        const char *exec_suffix
        ){

        if (    !project           || 
                !file_name         ||
                !path_logfile      ||
                !path_tests_exec)
                return NULL;

        size_t size = 5000;
        char buffer[size];
        memset(buffer, 0, size);

        if (0 > snprintf(buffer, size, 
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2018-02-09\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Run gprof based analysis tests on all test cases.\n"
        "#\n"
        "#       Usage           ./%s /path/to/project\n"
        "#\n"
        "#       Dependencies    bash, gprof\n"
        "#\n"
        "#       Last changed    2018-07-11\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "FOLDER_TEST_EXEC=\"%s\"\n"
        "FOLDER_LOGFILE=\"%s\"\n"
        "TEST_EXEC_SUFFIX=\"%s\"\n"
        ""
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"$FOLDER_LOGFILE/gprof.$start_time.log\"\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               GPROF RUNNER\" >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "# Execute the test once and profile the execution\n"
        "for test in $FOLDER_TEST_EXEC/*$TEST_EXEC_SUFFIX; do\n"
        "        name=${test##*/}"
        "        echo \"Profiling\" $name\n"
        "    $test\n"
        "    gprof $test gmon.out > $name.profile\n"
        "done\n"
        "\n"
        "# move profile to build/tests/logs\n"
        "mv *.profile $FOLDER_LOGFILE\n"
        "exit 0\n",
                bash_header,
                file_name,
                project,
                file_name,
                path_tests_exec,
                path_logfile,
                exec_suffix
                ))
                return NULL;

        return strdup(buffer);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_makefile_common(
        const char *project,
        const char *file_name,
        const char *path_bin,
        const char *path_build,
        const char *path_include,
        const char *path_source,
        const char *path_tests,
        const char *path_tools,
        const char *path_doxygen,
        const char *suffix_test_source,
        const char *suffix_test_exec,
        const char *script_unit_tests,
        const char *script_acceptance_tests,
        const char *script_coverage_tests,
        const char *script_loc,
        const char *script_gcov,
        const char *script_gprof,
        testrun_makefile_target target
        ){

        if (    !project                        ||      
                !file_name                      ||      
                !path_bin                       ||      
                !path_build                     ||      
                !path_include                   ||      
                !path_source                    ||      
                !path_tests                     ||      
                !path_doxygen                   ||      
                !suffix_test_source             ||      
                !suffix_test_exec               ||      
                !script_unit_tests              ||      
                !script_acceptance_tests        ||      
                !script_coverage_tests          ||      
                !script_loc                     ||      
                !script_gcov                    ||      
                !script_gprof)
                return NULL;

        char *target_all        = NULL;
        char *target_install    = NULL;
        char *target_uninstall  = NULL;

        switch (target) {

                case LIB:
                        target_all       = "all_lib";
                        target_install   = "install_lib";
                        target_uninstall = "uninstall_lib";
                        break;
                
                case EXEC:
                        target_all       = "all_exec";
                        target_install   = "install_exec";
                        target_uninstall = "uninstall_exec";
                        break;

                case SERVICE:
                        target_all       = "all_service";
                        target_install   = "install_service";
                        target_uninstall = "uninstall_service";
                        break;

                default:
                        return NULL;
        }

        size_t size = 20000;
        char buffer[size];
        memset(buffer, 0, size);

        if (0 > snprintf(buffer, size, 
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2018-02-18\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Generic makefile for testrun based projects.\n"
        "#\n"
        "#                       Target of this makefile is an independent library\n"
        "#                       or executable to be installed at either PREFIX/lib\n"
        "#                       or PREFIX/bin.\n"
        "#\n"
        "#                       The TESTING part contains all required functionality\n"
        "#                       to use the testrun tools via a makefile. It may be\n"
        "#                       seen as a makefile integrated testrunner framework.\n"
        "#\n"
        "#                       in particular:\n"
        "#\n"
        "#                           \"make clean && make tested\"\n"
        "#\n"
        "#                       may be used to build all sources as well as tests from\n"
        "#                       scratch and perform an integrated testrun over all after\n"
        "#                       compilation.\n"
        "#\n"
        "#                           \"make gcov\"\n"
        "#\n"
        "#                       may be used to rebuild the whole project with gcov\n"
        "#                       coverage testing flag enabled.\n"
        "#\n"
        "#                           \"make gprof\"\n"
        "#\n"
        "#                       may be used to rebuild the whole project with gprof\n"
        "#                       profiling flags enabled.\n"
        "#\n"
        "#                       Following folder structure is required\n"
        "#\n"
        "#                           bin     MUST be located at %s\n"
        "#                           build   MUST be located at %s\n"
        "#                           inludes MUST be located at %s\n"
        "#                           sources MUST be located at %s\n"
        "#                           tests   MUST be located at %s\n"
        "#\n"
        "#                       ALL TEST SCRIPTS MAY BE EXCHANGED WITH CUSTOM RUNNERS\n"
        "#\n"
        "#       Usage           SHOULD be used included by parent makefile\n"
        "#\n"
        "#       NOTE            aligned with tab width 4\n"
        "#\n"
        "#       Dependencies    testrun (makefile & service scripts), doxygen (if used)\n"
        "#\n"
        "#       Last changed    2018-07-12\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "# Switch on colors\n"
        "GCC_COLORS ?= 'gcc colors available, use them!'\n"
        "export GCC_COLORS\n"
        "\n"
        "# ----- Compiler flags -----------------------------------------------------\n"
        "\n"
        "CFLAGS\t\t\t= -Wall -Wextra -fPIC -Iinclude\n"
        "\n"
        "CFLAGS\t\t\t+= $(MODCFLAGS)\n"
        "LFLAGS\t\t\t+= $(MODLFLAGS)\n"
        "\n"
        "# ----- Project path calculation (if used included) ------------------------\n"
        "\n"
        "PROJECTPATH\t\t:= $(abspath $(dir $(PROJECTMK)))\n"
        "DIRNAME\t\t\t:= $(notdir $(patsubst %%/,%%,$(dir $(PROJECTMK))))\n"
        "\n"
        "# ----- Package config setup -----------------------------------------------\n"
        "\n"
        "LIBNAME\t\t\t:= lib$(DIRNAME)\n"
        "LIBNAMEPC\t\t:= $(LIBNAME).pc\n"
        "\n"
        "INCDIR\t\t\t:= $(PREFIX)/usr/local/include/$(DIRNAME)\n"
        "LIBDIR\t\t\t:= $(PREFIX)/usr/local/lib\n"
        "EXECDIR\t\t\t:= $(PREFIX)/usr/local/bin\n"
        "CONFDIR\t\t\t:= $(PREFIX)/etc/$(DIRNAME)\n"
        "SOCKDIR\t\t\t:= $(PREFIX)/etc/systemd/system\n"
        "\n"
        "# ----- TARGETS ------------------------------------------------------------\n"
        "\n"
        "INSTALL\t\t\t:= install\n"
        "\n"
        "EXECUTABLE\t\t= %s/$(DIRNAME)\n"
        "\n"
        "STATIC\t\t\t= %s/lib$(DIRNAME).a\n"
        "SHARED\t\t\t= $(patsubst %%.a,%%.so,$(STATIC))\n"
        "\n"
        "# Source and object files to compile\n"
        "HEADERS\t\t\t= $(wildcard %s/*.h)\n"
        "SOURCES\t\t\t= $(wildcard %s/**/*.c %s/*.c)\n"
        "OBJECTS\t\t\t= $(patsubst %%.c,%%.o,$(SOURCES))\n"
        "\n"
        "# Test sources and targets\n"
        "TESTS_SOURCES   = $(wildcard %s/**/*%s.c %s/*%s.c)\n"
        "TESTS_TARGET    = $(patsubst %s/%%.c, %s/tests/%%%s, $(TESTS_SOURCES))\n"
        "\n"
        "# GCOV support\n"
        "GCOV_FILES\t\t=  $(patsubst %%.c,%%.gcno,$(SOURCES))\n"
        "GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcov,$(SOURCES))\n"
        "GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcda,$(SOURCES))\n"
        "GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcno,$(TESTS_SOURCES))\n"
        "GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcda,$(TESTS_SOURCES))\n"
        "\n"
        "ifdef USE_GCOV\n"
        "CFLAGS += -fprofile-arcs -ftest-coverage\n"
        "LFLAGS += -lgcov --coverage\n"
        "endif\n"
        "\n"
        "ifdef USE_GPROF\n"
        "CFLAGS += -pg\n"
        "endif\n"
        "\n"
        "# ----- TEST_SCRIPTS -------------------------------------------------------\n"
        "\n"
        "TEST_TOOLS_FOLDER\t\t=%s\n"
        "TEST_SCRIPT_UNIT\t\t= $(TEST_TOOLS_FOLDER)/%s\n"
        "TEST_SCRIPT_ACCEPTANCE\t= $(TEST_TOOLS_FOLDER)/%s\n"
        "TEST_SCRIPT_COVERAGE\t=$(TEST_TOOLS_FOLDER)/%s\n"
        "TEST_SCRIPT_LOC\t\t\t= $(TEST_TOOLS_FOLDER)/%s\n"
        "TEST_SCRIPT_GCOV\t\t= $(TEST_TOOLS_FOLDER)/%s\n"
        "TEST_SCRIPT_GPROF\t\t= $(TEST_TOOLS_FOLDER)/%s\n"
        "\n"
        "# ----- DEFAULT MAKE RULES -------------------------------------------------\n"
        "\n"
        "%%.o : %%.c $(HEADERS)\n"
        "\t@echo \" (CC)    $@\"\n"
        "\t@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)\n"
        "\n"
        "%%%s.o : %%%s.c\n"
        "\t@echo \" (CC)    $@\"\n"
        "\t@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)\n"
        "\n"
        "all:\t\t\t%s\n"
        "install:\t\t%s\n"
        "uninstall:\t\t%s\n"
        "\n"
        "all_lib:\t\tstart lib tests pkgconfig done\n"
        "all_exec:\t\tstart lib tests $(EXECUTABLE) done\n"
        "all_service:\tall_exec\n"
        "\n"
        "lib:\t\t\tbuild sources\n"
        "sources:\t\tbuild $(STATIC) $(SHARED)\n"
        "tests:\t\t\ttests-resources $(TESTS_TARGET)\n"
        "\n"
        "$(STATIC):  $(OBJECTS)\n"
        "\t@echo \" (AR)    $@ $(OBJECTS)\"\n"
        "\t@ar rcs $@ $(OBJECTS)\n"
        "\t@ranlib $@\n"
        "\n"
        "$(SHARED): $(STATIC) $(OBJECTS)\n"
        "\t@echo \" (CC)    $@ $(OBJECTS)\"\n"
        "\t@$(CC) -shared -o $@ $(OBJECTS) $(LIBS) $(LFLAGS)\n"
        "\n"
        "$(EXECUTABLE): $(OBJECTS)\n"
        "\t@echo \" (CC)    $@ $(OBJECTS)\"\n"
        "\t$(CC) -o $@ $(STATIC) $(LIBS) $(LFLAGS)\n"
        "\n"
        "# ----- BUILD & CLEANUP ----------------------------------------------------\n"
        "\n"
        "build:\n"
        "\t@mkdir -p %s\n"
        "\t@mkdir -p %s\n"
        "\t@mkdir -p %s/tests\n"
        "\t@mkdir -p %s/tests/unit\n"
        "\t@mkdir -p %s/tests/acceptance\n"
        "\t@mkdir -p %s/tests/log\n"
        "\t@echo \" (MK)    directories for build\"\n"
        "\n"
        ".PHONY: clean\n"
        "clean:\n"
        "\t@echo \" (CLEAN) $(LIBNAME)\"\n"
        "\t@rm -rf %s %s %s/documentation $(OBJECTS) $(TESTS_OBJECTS) \\\n"
        "\t$(LIBNAMEPC) $(TESTS_TMP_FILES) $(GCOV_FILES) *.gcov *.profile *.pc *.out\n"
        "\n"
        "\n"
        "# ----- DOCUMENATION -------------------------------------------------------\n"
        "\n"
        "#NOTE requires doxygen.PHONY: documentation\n"
        "documentation:\n"
        "\tdoxygen %s/doxygen.config\n"
        "\n"
        "# ----- INFORMATION PRINTING -----------------------------------------------\n"
        "\n"
        "# print out a variable of the make file (e.g. \"make print-PROJECTPATH\")\n"
        ".PHONY: print\n"
        "print-%%  : ; @echo $* = $($*)\n"
        "\n"
        ".PHONY: start\n"
        "start:\n"
        "\t@echo \"\\n (HINT)    $(PROJECT)           ==> running make\\n\"\n"
        "\n"
        ".PHONY: done\n"
        "done:\n"
        "\t@echo\n"
        "\t@echo \" (DONE)  make $(PROJECT)\"\n"
        "\t@echo \" (HINT)  with unit testing      ==> 'make tested'\"\n"
        "\t@echo \" (HINT)  perform installation   ==> 'sudo make install\"\n"
        "\t@echo \" (HINT)  generate documentation ==> 'make documentation\"\n"
        "\n"
        "# ----- TESTING ------------------------------------------------------------\n"
        "\n"
        "# ALL IN ONE CALL (compile source, test and run test)\n"
        "tested: all testrun done\n"
        "\n"
        "# copy test resources to build\n"
        "tests-resources:\n"
        "\t@echo \" (CP)    tests/resources\"\n"
        "\t@cp -r %s/resources %s/tests\n"
        "\n"
        "%s/tests/acceptance/%%%s%s: %s/acceptance/%%%s.o\n"
        "\t@echo \" (CC)    $(@)\"\n"
        "\t@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)\n"
        "\n"
        "%s/tests/unit/%%%s%s: %s/unit/%%%s.o\n"
        "\t@echo \" (CC)    $(@)\"\n"
        "\t@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)\n"
        "\n"
        "# TESTRUN runners ----------------------------------------------------------\n"
        "\n"
        "# ACCEPTANCE TEST script invocation\n"
        ".PHONY: testrun-acceptance\n"
        "testrun-acceptance:\n"
        "\tsh $(TEST_SCRIPT_ACCEPTANCE)\n"
        "\n"
        "# UNIT TEST script invocation\n"
        ".PHONY: testrun-unit\n"
        "testrun-unit:\n"
        "\tsh $(TEST_SCRIPT_UNIT)\n"
        "\n"
        "# COVERAGE TEST script invocation\n"
        ".PHONY: testrun-coverage\n"
        "testrun-coverage:\n"
        "\tsh $(TEST_SCRIPT_COVERAGE) $(PROJECTPATH)\n"
        "\n"
        "# LOC TEST script invocation\n"
        ".PHONY: testrun-loc\n"
        "testrun-loc:\n"
        "\tsh $(TEST_SCRIPT_LOC) $(PROJECTPATH)\n"
        "\n"
        "# TESTRUN all scripts\n"
        ".PHONY: testrun\n"
        "testrun:\n"
        "\t@echo \" (HINT)  $(PROJECT) \\t\\t\\t==> running tests\\n\"\n"
        "\tsh $(TEST_SCRIPT_UNIT)\n"
        "\tsh $(TEST_SCRIPT_ACCEPTANCE)\n"
        "\tsh $(TEST_SCRIPT_COVERAGE) $(PROJECTPATH)\n"
        "\tsh $(TEST_SCRIPT_LOC) $(PROJECTPATH)\n"
        "\n"
        "# TESTRUN gcov -------------------------------------------------------------\n"
        "\n"
        ".PHONY: testrun-gcov\n"
        "testrun-gcov: clean\n"
        "\tmake USE_GCOV=1 all\n"
        "\tsh $(TEST_SCRIPT_GCOV) $(PROJECTPATH)\n"
        "\n"
        "# TESTRUN gprof ------------------------------------------------------------\n"
        "\n"
        ".PHONY: testrun-gprof\n"
        "testrun-gprof: clean\n"
        "\tmake USE_GPROF=1 all\n"
        "\tsh $(TEST_SCRIPT_PROF) $(PROJECTPATH)\n"
        "\n"
        "# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------\n"
        "\n"
        ".PHONY: pkgconfig\n"
        "pkgconfig:\n"
        "\t@echo 'prefix='$(PREFIX)'/usr/local/' >  $(LIBNAMEPC)\n"
        "\t@echo 'exec_prefix=$${prefix}' >> $(LIBNAMEPC)\n"
        "\t@echo 'libdir=$${prefix}/lib' >> $(LIBNAMEPC)\n"
        "\t@echo 'includedir=$${prefix}/include' >> $(LIBNAMEPC)\n"
        "\t@echo '' >> $(LIBNAMEPC)\n"
        "\t@echo 'Name: '$(LIBNAME) >> $(LIBNAMEPC)\n"
        "\t@echo 'Description: '$(PROJECT_DESC) >> $(LIBNAMEPC)\n"
        "\t@echo 'Version: '$(VERSION) >> $(LIBNAMEPC)\n"
        "\t@echo 'URL: ' $(PROJECT_URL) >> $(LIBNAMEPC)\n"
        "\t@echo 'Libs: -L$${libdir} -l'$(DIRNAME) >> $(LIBNAMEPC)\n"
        "\t@echo 'Cflags: -I$${includedir}' >> $(LIBNAMEPC)\n"
         "\n"
        "# ----- INSTALLATION -------------------------------------------------------\n"
        "\n"
        "# Installation as a library ------------------------------------------------\n"
        "\n"
        ".PHONY: install_lib\n"
        "install_lib: $(SHARED) $(STATIC)\n"
        "\t@echo \" (OK)    installed $(LIBNAME) to $(LIBDIR)\"\n"
        "\t@mkdir -p $(LIBDIR)/pkgconfig\n"
        "\t@mkdir -p $(INCDIR)\n"
        "\t@$(INSTALL) -m 0644 -t $(INCDIR) $(shell find include -name \"*.h\")\n"
        "\t@$(INSTALL) -m 0755 $(SHARED) $(LIBDIR)\n"
        "\t@$(INSTALL) -m 0755 $(STATIC) $(LIBDIR)\n"
        "\t@$(INSTALL) -m 0644 $(LIBNAMEPC) $(LIBDIR)/pkgconfig\n"
        "\t@ldconfig\n"
        "\n"
        ".PHONY: uninstall_lib\n"
        "uninstall_lib:\n"
        "\t@echo \" (OK)    uninstalled $(LIBNAME) from $(LIBDIR)\"\n"
        "\t@rm -rf $(INCDIR)\n"
        "\t@rm -rf $(LIBDIR)/$(LIBNAME).a\n"
        "\t@rm -rf $(LIBDIR)/$(LIBNAME).so\n"
        "\t@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)\n"
        "\n"
        "# Installation as an executable --------------------------------------------\n"
        "\n"
        ".PHONY: install_exec\n"
        "install_exec: $(SHARED) $(STATIC)\n"
        "\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\"\n"
        "\t@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)\n"
        "\n"
        ".PHONY: uninstall_exec\n"
        "uninstall_exec:\n"
        "\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\"\n"
        "\t@rm -rf $(EXECDIR)/$(DIRNAME)\n"
        "\n"
        "# Installation as a service ------------------------------------------------\n"
        ".PHONY: install_service\n"
        "install_service: copy_service_files enable_service\n"
        "\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\"\n"
        "\n"
        ".PHONY: copy_service_files\n"
        "copy_service_files: $(EXECUTABLE) \n"
        "\t@echo \" (OK)    copied service files\"\n"
        "\t@mkdir -p $(SOCKDIR)\n"
        "\t@$(INSTALL) -m 0755 bin/$(DIRNAME)  $(EXECDIR)\n"
        "\t@$(INSTALL) -m 0755 -d $(SERVICE_DATA)/etc   $(CONFDIR)\n"
        "\t@$(INSTALL) -m 0644 $(SERVICE_DATA)/*.service $(SOCKDIR)\n"
        "\t@$(INSTALL) -m 0644 $(SERVICE_DATA)/*.socket $(SOCKDIR)\n"
        "\n"
        ".PHONY: enable_service\n"
        "enable_service:\n"
        "\t@# IF INSTALLATION IS DONE UNPREFIXED TO /etc, the service will be enabled \n"
        "\t@ifndef ($(PREFIX)) \\\n"
        "\t\t@echo \" (OK)    enable service\" \\\n"
        "\t\t$(shell systemctl enable $(DIRNAME).socket) \\\n"
        "\t\t$(shell systemctl start $(DIRNAME).socket) \\\n"
        "\t\t$(shell systemctl enable $(DIRNAME).service) \\\n"
        "\t\t$(shell systemctl start $(DIRNAME).service) \\\n"
        "\t\t$(shell systemctl daemon-reload) \\\n"
        "\t@endif\n"
        "\n"
        ".PHONY: delete_service_files\n"
        "delete_service_files: \n"
        "\t@echo \" (OK)    delete service files\"\n"
        "\t@rm -rf $(EXECDIR)/$(DIRNAME)\n"
        "\t@rm -rf $(CONFDIR)\n"
        "\t@rm -rf $(SOCKDIR)/$(DIRNAME)*\n"
        "\n"
        ".PHONY: disable_service\n"
        "disable_service:\n"
        "\t@# IF INSTALLATION WAS DONE UNPREFIXED TO /etc, the service will be disabled \n"
        "\t@ifndef ($(PREFIX)) \\\n"
        "\t\t@echo \" (OK)    disable service\" \\\n"
        "\t\t$(shell systemctl stop  $(DIRNAME).service) \\\n"
        "\t\t$(shell systemctl disable $(DIRNAME).service) \\\n"
        "\t\t$(shell systemctl stop $(DIRNAME).socket) \\\n"
        "\t\t$(shell systemctl disable $(DIRNAME).socket) \\\n"
        "\t\t$(shell systemctl daemon-reload) \\\n"
        "\t@endif\n"
        "\n"
        ".PHONY: uninstall_service\n"
        "uninstall_service: disable_service delete_service_files\n"
        "\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\"\n"
        ,
                bash_header,
                file_name,
                project,
                path_bin,
                path_build,
                path_include,
                path_source,
                path_tests,
                path_bin,
                path_build,
                path_include,
                path_source,
                path_source,
                path_tests,
                suffix_test_source,
                path_tests,
                suffix_test_source,
                path_tests,
                path_build,
                suffix_test_exec,
                path_tools,
                script_unit_tests,
                script_acceptance_tests,
                script_coverage_tests,
                script_loc,
                script_gcov,
                script_gprof,
                suffix_test_source,
                suffix_test_source,
                target_all,
                target_install,
                target_uninstall,
                path_bin,
                path_build,
                path_build,
                path_build,
                path_build,
                path_build,
                path_bin,
                path_build,
                path_doxygen,
                path_doxygen,
                path_tests,
                path_build,
                path_build,
                suffix_test_source,
                suffix_test_exec,
                path_tests,
                suffix_test_source,
                path_build,
                suffix_test_source,
                suffix_test_exec,
                path_tests,
                suffix_test_source))
                return NULL;

        return strdup(buffer);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_makefile(
        const char *project,
        const char *file_name,
        const char *version,
        const char *cflags,
        const char *project_url,
        const char *project_desc,
        const char *path_service,
        const char *makefile_common
        ){

        if (    !project        ||
                !file_name      ||
                !version        ||
                !path_service   ||
                !makefile_common)
                return NULL;

        size_t size = 20000;
        char buffer[size];
        memset(buffer, 0, size);

        if (0 > snprintf(buffer, size, 
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Authors         ...\n"
        "#       Date            2018-02-18\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     This makefile defines project specific parameter.\n"
        "#\n"
        "#                       These parameter are:\n"
        "#                       (1) used compiler and special flags\n"
        "#                       (2) name and version\n"
        "#                       (3) installation prefix\n"
        "#                       (4) used libraries\n"
        "#                       (5) general makefiles used\n"
        "#\n"
        "#       Usage           make\n"
        "#\n"
        "#       Dependencies    make & compiler\n"
        "#\n"
        "#       Last changed    2018-07-12\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "CC = gcc\n"
        "\n"
        "PROJECT\t\t\t:= %s\n"
        "VERSION\t\t\t:= %s\n"
        "\n"
        "# project path recalculation (if used included from a parent make)\n"
        "PROJECTMK\t\t:= $(abspath $(lastword $(MAKEFILE_LIST)))\n"
        "\n"
        "# prefix for base directory for installation (default is /)\n"
        "#PREFIX\t\t\t:= some_path\n"
        "\n"
        "# include all pkgconfig files available at PREFIX\n"
        "export PKG_CONFIG_PATH = $(PREFIX)/usr/local/lib/pkgconfig\n"
        "\n"
        "# LIBS USED (uncommented example includes)\n"
        "# ... will allow to include libs installed under PREFIX\n"
        "#LIBS\t\t\t+= `pkg-config --cflags --libs libtestrun.info`\n"
        "#LIBS\t\t\t+= `pkg-config --libs libsystemd`\n"
        "#LIBS\t\t\t+= `pkg-config --libs uuid`\n"
        "#LIBS\t\t\t+= `pkg-config --libs openssl`\n"
        "\n"
        "# MODULE BASED CFLAGS (example)\n"
        "MODCFLAGS\t\t+= %s\n"
        "\n"
        "# EXTRA CFLAGS (example parallel or other GCC custom flags)\n"
        "#MODCFLAGS\t\t+= -fopenmp\n"
        "#MODCFLAGS\t\t+= -rdynamic\n"
        "\n"
        "# EXTRA LFLAGS (example)\n"
        "#MODLFLAGS\t\t+= -pthread\n"
        "\n"
        "# PKG_CONFIG_DATA (used during LIBRARY install)\n"
        "PROJECT_URL\t\t= \"%s\"\n"
        "PROJECT_DESC\t= \"%s\"\n"
        "\n"
        "# SERVICE_CONFIG_DATA (used during SERVICE install)\n"
        "SERVICE_DATA\t= \"%s\"\n"
        "\n"
        "# TMP FILE DEFINITION\n"
        "TESTS_TMP_FILES\t= $(wildcard /tmp/test_*)\n"
        "\n"
        "# INCLUDE BASE MAKEFILE\n"
        "include %s\n"
        ,
                bash_header,
                file_name,
                project,
                project,
                version,
                cflags,
                project_url,
                project_desc,
                path_service,
                makefile_common
        ))
                return NULL;

        return strdup(buffer);
}

/*----------------------------------------------------------------------------*/

testrun_tools testrun_tools_default(){

        struct testrun_tools tools = {

                .testrun_header                 = testrun_generate_header,
                .testrun_header_openmp          = testrun_generate_header_openmp,

                .testrun_simple_tests           = testrun_generate_script_simple_tests,
                .testrun_runner                 = testrun_generate_script_runner,
                .testrun_loc                    = testrun_generate_script_loc,
                .testrun_simple_coverage        = testrun_generate_script_coverage,
                .testrun_gcov                   = testrun_generate_script_gcov,
                .testrun_gprof                  = testrun_generate_script_gprof,

                .makefile_configurable          = testrun_generate_makefile,
                .makefile_common                = testrun_generate_makefile_common,

                .gitignore                      = testrun_generate_gitignore,
                .readme                         = testrun_generate_readme,
                .doxygen                        = testrun_generate_doxygen,
                .service_file                   = testrun_generate_service_file,
                .socket_file                    = testrun_generate_socket_file
        };

        return tools;
}

/*----------------------------------------------------------------------------*/

bool testrun_tools_validate(const testrun_tools *self){

        if (    !self || 
                !self->testrun_header ||
                !self->testrun_header_openmp ||

                !self->testrun_simple_tests ||
                !self->testrun_runner ||
                !self->testrun_loc ||
                !self->testrun_simple_coverage ||
                !self->testrun_gcov ||
                !self->testrun_gprof ||

                !self->makefile_configurable ||
                !self->makefile_common ||

                !self->gitignore ||
                !self->readme ||
                !self->doxygen ||
                !self->service_file || 
                !self->socket_file)
                return false;

        return true;
}