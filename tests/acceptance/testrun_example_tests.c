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
 ***//**
 *
 *      @file           testrun_example_tests.c
 *      @author         Markus Toepfer
 *      @date           2017-11-22
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Example test file using testrun.h
 *
 *      This example shows parallel and sequential style based testing
 *      with testrun.h and is build around a MACRO set to execute tests in
 *      parallel or seqentuial run.
 *
 *      @NOTE           This is the testrun_example.c of the header.
 *
 *      ------------------------------------------------------------------------
 **/

/*******************************************************************************
 *
 *      TEST SETUP
 *
 ******************************************************************************/

#include "../../include/testrun.h"

bool example_function() {
        return true;
}

/*******************************************************************************
 *
 *      TEST CASES
 *
 ******************************************************************************/

int testcase_block1(){

        // maybe a unit test of a function

        testrun(example_function());
        testrun(true);
        testrun(example_function(), "second run of function.");

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int testcase_block2(){

        // maybe a unit test of another function

        testrun(example_function());
        testrun(true);
        testrun(example_function(), "second run of function.");

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int testcase_block3(){

        // maybe a unit test of a third function

        testrun(example_function());
        testrun(true);
        testrun(example_function(), "second run of function.");

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int testcase_block4(){

        // maybe a unit test of a fourth function

        testrun(example_function());
        testrun(true);
        testrun(example_function(), "second run of function.");

        return testrun_log_OK();
}

/*******************************************************************************
 *
 *      TEST CLUSTER SETUP
 *
 ******************************************************************************/

int64_t cluster_tests1(int(*tests[])(), size_t slot, size_t max) {

        /* e.g. TESTCASES which may be run in parallel */

        testrun_init();                 // create local variables
        testrun_add(testcase_block1);   // adds block1 to tests[]
        testrun_add(testcase_block2);   // adds block2 to tests[]

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int64_t cluster_tests2(int(*tests[])(), size_t slot, size_t max) {

        /* e.g. TESTCASES which MUST be run sequential */

        testrun_init();                 // create local variables
        testrun_add(testcase_block3);   // adds block3 to tests[]
        testrun_add(testcase_block4);   // adds block4 to tests[]

        return testrun_counter;
}

/*******************************************************************************
 *
 *      TEST CONFIGURATION
 *
 ******************************************************************************/

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

/*******************************************************************************
 *
 *      EXECUTION SEQUENCE
 *
 ******************************************************************************/

int64_t run_tests() {

        // use the header provided execution sequence

        return testrun_run_tests(1000,1000,false);
}

/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/

testrun_run(run_tests);

/*
 *      EXAMPLE RUN using combined test creation and valgrind execution
 *
 *      make test testname=tests/acceptance/testrun_assert_example_tests.c  &&
 *      valgrind ./build/test/acceptance/testrun_assert_example_tests.test
 *
 *  (CC)    tests/acceptance/testrun_assert_example_tests.c
 * ==14634== Memcheck, a memory error detector
 * ==14634== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
 * ==14634== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
 * ==14634== Command: ./build/test/acceptance/testrun_assert_example_tests.test
 * ==14634==
 *
 * testrun ./build/test/acceptance/testrun_assert_example_tests.test
 *         [OK]    test_with_assert_function
 *         [OK]    test_whatever_OK
 *         [OK]    test_whatever_NOK
 *         ALL TESTS RUN (3 tests)
 *         Clock ticks function: ( main ) | 0.013501 | 0.000014 ms
 *
 * ==14634==
 * ==14634== HEAP SUMMARY:
 * ==14634==     in use at exit: 0 bytes in 0 blocks
 * ==14634==   total heap usage: 1 allocs, 1 frees, 1,024 bytes allocated
 * ==14634==
 * ==14634== All heap blocks were freed -- no leaks are possible
 * ==14634==
 * ==14634== For counts of detected and suppressed errors, rerun with: -v
 * ==14634== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 */
