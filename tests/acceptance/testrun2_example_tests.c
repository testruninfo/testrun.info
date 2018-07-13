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

        @file           testrun2_example_tests.c
        @author         Markus Toepfer
        @date           2017-11-22

        @ingroup        testrun_lib

        @brief          Example test file using testrun2.h

        This example shows parallel and sequential style based testing
        with testrun.h and is build around a MACRO set to execute tests in
        parallel or seqentuial run.

        @NOTE           This is the testrun_example.c of the header.

        ------------------------------------------------------------------------
*/

#include "../tools/testrun2.h"

bool example_function() {
        return true;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */

int testcase_block1(){

        // maybe a unit test of a function

        testrun(example_function());
        testrun(true);
        testrun(example_function(), "second run of function.");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int testcase_block2(){

        // maybe a unit test of another function

        testrun(example_function());
        testrun(true);
        testrun(example_function(), "second run of function.");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int testcase_block3(){

        // maybe a unit test of a third function

        testrun(example_function());
        testrun(true);
        testrun(example_function(), "second run of function.");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int testcase_block4(){

        // maybe a unit test of a fourth function

        testrun(example_function());
        testrun(true);
        testrun(example_function(), "second run of function.");

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                   #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */


int64_t cluster_tests1(int(*tests[])(), size_t slot, size_t max) {

        /* e.g. TESTCASES which may be run in parallel */

        int testrun_counter = 0;        // create local variables
        testrun_add(testcase_block1);   // adds block1 to tests[]
        testrun_add(testcase_block2);   // adds block2 to tests[]

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int64_t cluster_tests2(int(*tests[])(), size_t slot, size_t max) {

        /* e.g. TESTCASES which MUST be run sequential */

        int testrun_counter = 0;        // create local variables
        testrun_add(testcase_block3);   // adds block3 to tests[]
        testrun_add(testcase_block4);   // adds block4 to tests[]

        return testrun_counter;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CONFIGURATION                                              #CONFIG
 *
 *      ------------------------------------------------------------------------
 */

bool testrun_configure_parallel(
        int (*testcases[])(),
        size_t * const start,
        size_t const * const max){

        if (!testcases || !start || !max)
                return false;

        int r = 0;

        r = testrun_add_testcases(testcases,
                        start, max, cluster_tests1);

        if (r < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_configure_sequential(
        int (*testcases[])(),
        size_t *const start,
        size_t const * const max){

        if (!testcases || !start || !max)
                return false;

        int r = 0;

        r = testrun_add_testcases(testcases,
                        start, max, cluster_tests1);

        if (r < 0)
                return false;

        r = testrun_add_testcases(testcases,
                        start, max, cluster_tests2);

        if (r < 0)
                return false;

        return true;

}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST EXECUTION                                                  #EXEC
 *
 *      ------------------------------------------------------------------------
 */

int64_t run_tests() {

        // use the header provided execution sequence

        return testrun_run_tests(1000,1000,false);
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST ACTIVATED                                                  #ACTIV
 *
 *      ------------------------------------------------------------------------
 */

testrun_run(run_tests);

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST RESULTS AND NOTES                                         #RESULTS
 *
 *      ------------------------------------------------------------------------
 *
 *      EXAMPLE RUN using combined test creation and valgrind execution
 *
 *      make test testname=tests/acceptance/testrun_example_tests.c  &&
 *      valgrind ./build/test/acceptance/testrun_example_tests.test
 *
 *      (CC)    tests/acceptance/testrun_example_tests.c
 *      ==22343== Memcheck, a memory error detector
 *      ==22343== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
 *      ==22343== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
 *      ==22343== Command: ./build/test/acceptance/testrun_example_tests.test
 *      ==22343==
 *
 *      testrun ./build/test/acceptance/testrun_example_tests.test
 *              [INFO]  testrun_add_testcases line:486 message: added 2 tests to TESTS[](usage 2/1000)
 *              [OK]    testcase_block2
 *              [OK]    testcase_block1
 *              ---------------------------------------------------------
 *              NOTE PARALLEL TESTING
 *
 *              This version is using OpenMP. Using GCC for compilation
 *              may produce false valgrind output due to use of libomp.
 *              More information is included in docs/valgrind/openMP.
 *              ---------------------------------------------------------
 *              Parallel RUN (2) TESTS in 4 threads: success 2 error 0)
 *              Clock ticks function: ( testrun_parallel ) | 0.035083 | 0.000035 ms
 *
 *              [INFO]  testrun_add_testcases line:486 message: added 2 tests to TESTS[](usage 2/1000)
 *              [INFO]  testrun_add_testcases line:486 message: added 2 tests to TESTS[](usage 4/1000)
 *              [OK]    testcase_block1
 *              [OK]    testcase_block2
 *              [OK]    testcase_block3
 *              [OK]    testcase_block4
 *              Serial RUN (4) TESTS: success 4 error 0)
 *              Clock ticks function: ( testrun_sequential ) | 0.005768 | 0.000006 ms
 *
 *              ALL TESTS RUN (6 tests)
 *              Clock ticks function: ( main ) | 0.073920 | 74 ms
 *
 *      ==22343==
 *      ==22343== HEAP SUMMARY:
 *      ==22343==     in use at exit: 3,184 bytes in 7 blocks
 *      ==22343==   total heap usage: 9 allocs, 2 frees, 37,024 bytes allocated
 *      ==22343==
 *      ==22343== LEAK SUMMARY:
 *      ==22343==    definitely lost: 0 bytes in 0 blocks
 *      ==22343==    indirectly lost: 0 bytes in 0 blocks
 *      ==22343==      possibly lost: 864 bytes in 3 blocks
 *      ==22343==    still reachable: 2,320 bytes in 4 blocks
 *      ==22343==         suppressed: 0 bytes in 0 blocks
 *      ==22343== Rerun with --leak-check=full to see details of leaked memory
 *      ==22343==
 *      ==22343== For counts of detected and suppressed errors, rerun with: -v
 *      ==22343== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 */
