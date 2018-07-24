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

        @file           testrun_openmp_assert_example_tests.c
        @author         Markus Toepfer
        @date           2017-11-21

        @ingroup        testrun_lib

        @brief          Example test file using testrun_openmp.h

        This example shows assert() style based testing with testrun.h and is
        build around the testrun_test() macro, which increases a counter which
        MUST be initialized in a testcluster function.

        @NOTE           This is the testrun_assert_example of the header.

       ------------------------------------------------------------------------
*/

#include "../tools/testrun_openmp.h"

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

/*----------------------------------------------------------------------------*/

int test_whatever_OK() {

        bool failure = false;

        // Positive result logging
        if (!failure)
                return testrun_log_success();

        // will be reached in case of error
        return testrun_log_failure();
}

/*----------------------------------------------------------------------------*/

int test_whatever_NOK() {

        bool failure = false;

        // Failure logging (Don't fail the testrun, just log a failure)

        if (failure)
                return testrun_log_failure();

        // will not be reached in case of error
        return testrun_log_success();

}

/*----------------------------------------------------------------------------*/

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                   #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int assert_based_testing() {

        testrun_init();

        testrun_test(test_with_assert_function);
        testrun_test(test_whatever_OK);
        testrun_test(test_whatever_NOK);

        return testrun_counter;

}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST EXECUTION                                                  #EXEC
 *
 *      ------------------------------------------------------------------------
 */

testrun_run(assert_based_testing);

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
 *         Clock ticks function: ( main ) | 0.013501 | 14 ms
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
