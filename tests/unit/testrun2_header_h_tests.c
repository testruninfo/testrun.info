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

        @file           testrun2_header_h.c
        @author         Markus Toepfer
        @date           2017-11-23

        @ingroup        testrun_lib

        @brief          Check the header MACROs and inline functions


        ------------------------------------------------------------------------
*/

#include "../../include/testrun2.h"

static int testing = 1;

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */


int dummy_test1() {

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int dummy_test2() {

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int dummy_test3() {

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int dummy_test_fail() {

        testrun_log_failure("TESTCASE with expected failure");
        return -1;
}

/*----------------------------------------------------------------------------*/

int64_t dummy_tests(int(*tests[])(), size_t slot, size_t max) {

        int64_t testrun_counter = 0;

        testrun_add(dummy_test1);
        testrun_add(dummy_test2);
        testrun_add(dummy_test3);

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

bool testrun_configure_parallel(
        int (*testcases[])(),
        size_t * const start,
        size_t const * const max){

        // THIS FUNCTION MUST BE IMPLEMENTED BY TESTMODULE

        if (!testcases || !start || !max)
                return false;

        if (testrun_add_testcases(testcases, start, max, dummy_tests) > 0)
                return true;

        return false;
}

/*----------------------------------------------------------------------------*/

bool testrun_configure_sequential(
        int (*testcases[])(),
        size_t * const start,
        size_t const * const max){

        // THIS FUNCTION MUST BE IMPLEMENTED BY TESTMODULE

        if (!testcases || !start || !max)
                return false;

        if (testrun_add_testcases(testcases, start, max, dummy_tests) > 0)
                return true;

        return false;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES LOGGING                                                 #LOG
 *
 *      ------------------------------------------------------------------------
 */


int check_macro_testrun_errno(){

        char *expect = NULL;
        char *result = NULL;

        errno  = 0;
        result = testrun_errno();
        testrun(strncmp(result, "NONE", 4) == 0);

        errno  = 10;
        expect = strerror(errno);
        result = testrun_errno();
        testrun(strncmp(result, expect, strlen(expect)) == 0);

        for (int i = 1; i < 30 ; i++){
                // check for the first 30 errno messaged
                errno  = i;
                expect = strerror(errno);
                result = testrun_errno();
                testrun(strncmp(result, expect, strlen(expect)) == 0);
        }

        // RESET ERRNO before leave
        errno  = 0;

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_macro_testrun_log_failure(){

        testrun(testrun_log_failure("msg"));
        testrun(testrun_log_failure("msg %s %d", "text", 1));

        // check console output for verification
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_macro_testrun_log_error(){

        testrun(testrun_log_error("msg"));
        testrun(testrun_log_error("msg %s %d", "text", 1));

        // check console output for verification
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_macro_testrun_log_success(){

        testrun(testrun_log_success("msg"));
        testrun(testrun_log_success("msg %s %d", "text", 1));

        // check console output for verification
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_macro_testrun_log(){

        testrun(testrun_log("msg"));
        testrun(testrun_log("msg %s %d", "text", 1));

        // check console output for verification
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_macro_testrun_log_clock(){

        clock_t end = 0, start = clock();

        testrun(testrun_log_clock(start, start));
        testrun(testrun_log_clock(start, end));
        testrun(testrun_log_clock(start, 10));
        end = clock();
        testrun(testrun_log_clock(start, end));

        // check console output for verification
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_macro_testrun_log_function_info(){

        testrun(testrun_log_function_info("msg"));
        testrun(testrun_log_function_info("msg %s %d", "text", 1));

        // check console output for verification
        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES ASSERT                                              #ASSERT
 *
 *      ------------------------------------------------------------------------
 */

int check_macro_testrun_init(){

        testrun_init();

        testrun(result          == 0,   "expected initialization of result");
        testrun(testrun_counter == 0,   "expected initialization of counter");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_macro_testrun_test(){

        testrun_log_function_info("When you are able to read this it works.");
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/


int check_macro_testrun_assert(){

        testrun_assert(true);
        testrun_assert(true, "should log OK and this message");

        // THAT is why this test should be at the end or uncommented
        testrun_assert(false, "EXPECTED ERROR\n"
                "\t[INFO!] This is an expected [ERROR] based on testing "
                "the assert functionality. \n"
                "\t[INFO!] This test will break the whole TEST SERIES and "
                "should be the last test \n"
                "\t[INFO!] of the test series. \n\n");

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES TESTRUN                                              #TESTRUN
 *
 *      ------------------------------------------------------------------------
 */


int check_macro_testrun_check(){

        testrun_log_function_info("When you are able to read this it works.");
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_macro_testrun(){

        testrun_log_function_info("Calling: check_macro_testrun_check()");
        testrun(check_macro_testrun_check());
        testrun_log_function_info("Done calling: check_macro_testrun_check()");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_macro_testrun_add(){

        size_t slot             = 0;
        size_t max              = 100;
        size_t testrun_counter  = 0;
        size_t counter          = 0;

        int (*tests[max])();
        void *pointer = NULL;

        testrun_init_testcases(tests, slot, max);

        testrun(slot == 0);
        testrun_add(check_macro_testrun_add);
        testrun(slot == 0);
        testrun(testrun_counter == 1);

        // check slot + counter in sync
        for (int i = 1; i < 20; i++){
                testrun(slot == 0);
                testrun(testrun_counter == i);
                testrun_add(check_macro_testrun_add);
                testrun(slot == i + 1);
                testrun(testrun_counter == i + 1);
        }

        // check slot + counter offset
        slot = 50;
        testrun(testrun_counter == 20);
        for (int i = slot; i < 70; i++){
                counter++;
                testrun_add(check_macro_testrun_add);
                testrun(testrun_counter == 20 + counter);
                testrun(slot == 50);
        }

        for (int i = 0; i < max; i++) {

                pointer = (void*) tests[i];

                if (i < 20) {
                        testrun( pointer);
                } else if (i < 70) {
                        testrun( !pointer);
                } else if (i < 90) {
                        testrun( pointer);
                } else {
                        testrun( !pointer);
                }

        }

        // reset all
        max  = 100;
        slot = 0;
        testrun_counter = 0;
        testrun_init_testcases(tests, slot, max);

        // check add up to max
        for (int i = 0; i < max; i++){
                testrun_add(check_macro_testrun_add);
        }

        testrun(slot            == 0);
        testrun(testrun_counter == 100);
        testrun(max             == 100);

        // function will be left by calling testrun_add with filled TESTS[]
        // at MAX, therefore we log the success early, as leaving the parent
        // function on error is the desired behaviour.
        testrun_log_success();
        testrun_log_function_info("adding beyond TESTS[MAX] will log "
                                  "an failure next.");
        // add beyound max
        testrun_add(check_macro_testrun_add);

        /*
         *      NOTE this function is only able to return once,
         *      so some negative tests e.g. testrun_counter = 0
         *      and slot == max are shifted to be done implicit in
         *      check_inline_function_testrun_add_testcases
         */

        // in case the function was not left on MAX the test failed!
        return testrun_log_error();
}


/*----------------------------------------------------------------------------*/

int check_macro_testrun_init_testcases(){

        size_t slot             = 0;
        size_t max              = 100;
        size_t testrun_counter  = 0;

        int (*tests[max])();
        void *pointer = NULL;

        testrun_init_testcases(tests, slot, max);

        // check all slots are initialized to NULL
        for (int i = 1; i < max; i++){

                testrun(!(void*) tests[i]);

        }

        // add some function pointers
        testrun_add(check_macro_testrun_init_testcases);
        testrun_add(check_macro_testrun_init_testcases);
        testrun_add(check_macro_testrun_init_testcases);

        slot = 47;
        testrun_add(check_macro_testrun_init_testcases);
        testrun_add(check_macro_testrun_init_testcases);
        testrun_add(check_macro_testrun_init_testcases);

        slot = 74;
        testrun_add(check_macro_testrun_init_testcases);
        testrun_add(check_macro_testrun_init_testcases);
        testrun_add(check_macro_testrun_init_testcases);

        // check defined tests[]

        for (int i = 1; i < max; i++){

                if (i < 3) {
                        testrun((void*) tests[i]);
                } else if (i < 50) {
                        testrun(!(void*) tests[i]);
                } else if (i < 53) {
                        testrun((void*) tests[i]);
                } else if (i < 80) {
                        testrun(!(void*) tests[i]);
                } else if (i < 83) {
                        testrun((void*) tests[i]);
                } else {
                        testrun(!(void*) tests[i]);
                }

        }

        // check remove from
        testrun_init_testcases(tests, 60, max);

         // check defined tests[]

        for (int i = 1; i < max; i++){

                if (i < 3) {
                        testrun((void*) tests[i]);
                } else if (i < 50) {
                        testrun(!(void*) tests[i]);
                } else if (i < 53) {
                        testrun((void*) tests[i]);
                } else {
                        testrun(!(void*) tests[i]);
                }

        }

        // check remove to
        testrun_init_testcases(tests, 0, 40);

        for (int i = 1; i < max; i++){

                if (i < 50) {
                        testrun(!(void*) tests[i]);
                } else if (i < 53) {
                        testrun((void*) tests[i]);
                } else {
                        testrun(!(void*) tests[i]);
                }

        }

        // clear all
        testrun_init_testcases(tests, 0, max);

        // check all slots are reinitialized to NULL
        for (int i = 1; i < max; i++){

                testrun(!(void*) tests[i]);

        }

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_inline_function_testrun_add_testcases(){

        size_t slot             = 0;
        size_t max              = 100;
        size_t testrun_counter  = 0;

        int (*tests[max])();
        void *pointer = NULL;

        testrun_init_testcases(tests, 0, max);

        testrun(-1 == testrun_add_testcases(NULL,  NULL, NULL, NULL));
        testrun(-1 == testrun_add_testcases(NULL,  &slot, &max, dummy_tests));
        testrun(-1 == testrun_add_testcases(tests, NULL, &max, dummy_tests));
        testrun(-1 == testrun_add_testcases(tests, &slot, NULL, dummy_tests));
        testrun(-1 == testrun_add_testcases(tests, &slot, &max, NULL));

        testrun( 0 == slot );
        testrun( 3 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        testrun( 3 == slot );

        for (int i = 0; i < max; i++){
                if (i < 3) {
                        testrun((void*)tests[i],  "check nothing was deleted.");
                } else {
                        testrun(!(void*)tests[i], "check nothing was added.");
                }
        }

        // last > max
        slot = 101;
        testrun( -1 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        for (int i = 0; i < max; i++){
                if (i < 3) {
                        testrun((void*)tests[i],  "check nothing was deleted.");
                } else {
                        testrun(!(void*)tests[i], "check nothing was added.");
                }
        }

        // not enough space
        slot = 100;
        testrun( -1 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        testrun(100 == slot );
        for (int i = 0; i < max; i++){
                if (i < 3) {
                        testrun((void*)tests[i],  "check nothing was deleted.");
                } else {
                        testrun(!(void*)tests[i], "check nothing was added.");
                }
        }

        // not enough space
        slot = 99;
        testrun( -1 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        testrun(99  == slot );
        for (int i = 0; i < max; i++){
                if (i < 3) {
                        testrun((void*)tests[i],  "check nothing was deleted.");
                } else {
                        testrun(!(void*)tests[i], "check nothing was added.");
                }
        }

        // not enough space
        slot = 98;
        testrun( -1 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        testrun(98  == slot );
        for (int i = 0; i < max; i++){
                if (i < 3) {
                        testrun((void*)tests[i],  "check nothing was deleted.");
                } else {
                        testrun(!(void*)tests[i], "check nothing was added.");
                }
        }

        // add to max
        slot = 97;
        testrun(   3 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        testrun( 100 == slot);
        for (int i = 0; i < max; i++){
                if (i < 3 || i > 96) {
                        testrun((void*)tests[i],  "check function enabled");
                } else {
                        testrun(!(void*)tests[i], "check nothing enabled");
                }
        }

        // check adding inbetween
        slot = 50;

        testrun(   3 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        testrun(  53 == slot);

        for (int i = 0; i < max; i++){
                if (i < 3 || i > 96) {
                        testrun((void*)tests[i],  "check function enabled");
                } else if ( (i < 53) && (i >= 50)){
                        testrun((void*)tests[i],  "check function enabled");
                } else {
                        testrun(!(void*)tests[i], "check nothing enabled");
                }
        }

        // check reinit on error
        slot = 51;
        max  = 53;
        // NOTE this will remove / override the tests set in the step earlier.
        testrun(  -1 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        testrun(  51 == slot);

        for (int i = 0; i < max; i++){
                if (i < 3 || i > 96) {
                        testrun((void*)tests[i],  "check function enabled");
                } else if (i == 50){
                        testrun((void*)tests[i],  "check function enabled");
                } else {
                        testrun(!(void*)tests[i], "check nothing enabled");
                }
        }

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_inline_function_testrun_dump_testcases(){

        size_t slot             = 0;
        size_t max              = 25;
        size_t testrun_counter  = 0;

        int (*tests[max])();
        void *pointer = NULL;

        testrun_init_testcases(tests, 0, max);

        testrun(false == testrun_dump_testcases(NULL,   0, false));
        testrun(false == testrun_dump_testcases(tests,  0, false));
        testrun(false == testrun_dump_testcases(NULL,   1, false));

        testrun(true  == testrun_dump_testcases(tests,  1, false));
        testrun(true  == testrun_dump_testcases(tests,  1, true));

        // prepare some data
        testrun( 3 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        slot = 10;
        testrun( 3 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        slot = 20;
        testrun( 3 == testrun_add_testcases(tests, &slot, &max, dummy_tests));

        testrun(true  == testrun_dump_testcases(tests,  max, true));
        testrun(true  == testrun_dump_testcases(tests,  max, false));

        // manually check logging output
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_inline_function_testrun_parallel(){

        size_t slot = 0;
        size_t max  = 25;
        size_t testrun_counter = 0;

        int (*tests[max])();
        void *pointer = NULL;

        testrun_init_testcases(tests, 0, max);

        testrun(0 == testrun_parallel(NULL,  0));
        testrun(0 == testrun_parallel(tests, 0));
        testrun(0 == testrun_parallel(NULL,  1));

        // no tests to execute
        testrun(0 == testrun_parallel(tests,  1));
        // no tests to execute
        testrun(0 == testrun_parallel(tests,  max));

        testrun( 3 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        testrun( 3 == slot);

        testrun( 3 == testrun_parallel(tests,  max));
        testrun_counter = 3;

        // add a failing testcase
        testrun_add(dummy_test_fail);
        testrun( -1 == testrun_parallel(tests,  max));

        // add additional failing and succeding tests
        testrun_add(dummy_test_fail);
        testrun_add(dummy_test1);
        testrun_add(dummy_test1);
        testrun_add(dummy_test1);
        testrun_add(dummy_test1);
        testrun_add(dummy_test1);
        testrun_add(dummy_test_fail);
        testrun_add(dummy_test_fail);
        testrun_add(dummy_test2);
        testrun_add(dummy_test2);
        testrun_add(dummy_test3);
        testrun_add(dummy_test_fail);

        // check the run reports the 5 failed tests
        testrun( -5 == testrun_parallel(tests,  max));
        testrun_add(dummy_test1);
        testrun_add(dummy_test2);
        testrun_add(dummy_test3);
        testrun_add(dummy_test_fail);
        testrun( -6 == testrun_parallel(tests,  max));

        // check log for test logging output last output should contain:
        // Parallel RUN (20) TESTS in x threads: success 14 error 6)
        // NOTE this test will most propably report memory leaks, when run with
        // valgrind based on libomp if compiled with futex on.
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_inline_function_testrun_sequential(){

        size_t slot = 0;
        size_t max  = 25;
        size_t testrun_counter = 0;

        int (*tests[max])();
        void *pointer = NULL;

        testrun_init_testcases(tests, 0, max);

        testrun(0 == testrun_sequential(NULL,  0, false));
        testrun(0 == testrun_sequential(tests, 0, false));
        testrun(0 == testrun_sequential(NULL,  1, false));

        // no tests to execute
        testrun(0 == testrun_sequential(tests,  1, false));
        // no tests to execute
        testrun(0 == testrun_sequential(tests,  max, false));

        testrun( 3 == testrun_add_testcases(tests, &slot, &max, dummy_tests));
        testrun( 3 == slot);

        testrun( 3 == testrun_parallel(tests,  max));

        // add a failing testcase
        testrun_add(dummy_test_fail);
        testrun( -1 == testrun_sequential(tests,  max, false));

        // add additional failing and succeding tests
        testrun_add(dummy_test_fail);
        testrun_add(dummy_test1);
        testrun_add(dummy_test1);
        testrun_add(dummy_test1);
        testrun_add(dummy_test1);
        testrun_add(dummy_test1);
        testrun_add(dummy_test_fail);
        testrun_add(dummy_test_fail);
        testrun_add(dummy_test2);
        testrun_add(dummy_test2);
        testrun_add(dummy_test3);
        testrun_add(dummy_test_fail);

        // check the run reports the 5 failed tests
        testrun( -5 == testrun_sequential(tests,  max, false));
        testrun_add(dummy_test1);
        testrun_add(dummy_test2);
        testrun_add(dummy_test3);
        testrun_add(dummy_test_fail);
        testrun( -6 == testrun_sequential(tests,  max, false));
        // check log for test logging output third last output should contain:
        // Serial RUN (20) TESTS: success 14 error 6)

        // break on first error
        testrun( -1 == testrun_sequential(tests,  max, true));
        // check log for test logging output second last output should contain:
        // Serial RUN (4) TESTS: success 3 error 1)

        // remove all errors up to next non error
        tests[3] = NULL;
        tests[4] = NULL;
        // break on next first error (after some more successful tests)
        testrun( -1 == testrun_sequential(tests,  max, true));
        // check log for test logging output second last output should contain:
        // Serial RUN (9) TESTS: success 8 error 1)

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int check_inline_function_testrun_run_tests(){

        size_t slot = 0;
        size_t max  = 25;
        size_t testrun_counter = 0;

        int (*tests[max])();
        void *pointer = NULL;

        testrun_init_testcases(tests, 0, max);


        testrun(-1 == testrun_run_tests(0,0,false));

        // not enough slots for parallel tests
        testrun(-1 == testrun_run_tests(1,0,false));
        testrun(-1 == testrun_run_tests(2,0,false));
        // RUN ONLY PARALLEL
        testrun(3  == testrun_run_tests(3,0,false));

printf("AFTER PARALLEL ONLY TESTS ---------------------------------------\n\n");
        // not enough slots for sequential tests
        testrun(-1 == testrun_run_tests(0,1,false));
        testrun(-1 == testrun_run_tests(0,2,false));
        // RUN ONLY SEQUNETIAL
        testrun(3  == testrun_run_tests(0,3,false));

printf("AFTER SEQUENTIAL ONLY TESTS -------------------------------------\n\n");

        testrun(-1 == testrun_run_tests(2,3,false));
        testrun(-1 == testrun_run_tests(1,3,false));
        testrun(-1 == testrun_run_tests(3,2,false));
        testrun(-1 == testrun_run_tests(3,1,false));

printf("AFTER SLOT MISSING TESTS ----------------------------------------\n\n");

        testrun(6  == testrun_run_tests(3,3,false));

        testrun_log_function_info("UNTESTED: break_on_error.");
        /*
         *      NOTE break_on_error cannot be tested, without changing the
         *      configuration to a different testsetup of the whole seqeuntial
         *      test configuration. This is possible, by making the function
         *      adaptable.
         *      BUT! It is desired to have testrun_configure_parallel as well as
         *      testrun_configure_sequential an implementation requirement for
         *      the test module, which is kind of static.
         *
         *      It is a framework defined place where a user defined
         *      config MUST be done. This config SHOULD be static for the tool.
         */
        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                   #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int check_macros_logging() {

        testrun_init();

        testrun_test(check_macro_testrun_errno);
        testrun_test(check_macro_testrun_log_failure);
        testrun_test(check_macro_testrun_log_error);
        testrun_test(check_macro_testrun_log_success);
        testrun_test(check_macro_testrun_log);
        testrun_test(check_macro_testrun_log_clock);
        testrun_test(check_macro_testrun_log_function_info);

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int check_macros_assert() {

        testrun_init();

        testrun_test(check_macro_testrun_init);
        testrun_test(check_macro_testrun_test);
        // ASSERT TEST SHOULD BE THE LAST OR UNCOMMENTED!!!
        testrun_test(check_macro_testrun_assert);

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int check_macros_testrun() {

        testrun_init();

        testrun_test(check_macro_testrun_check);
        testrun_test(check_macro_testrun);

        // run testrun_add isolated, as it is expected to fail
        testrun(-1 == check_macro_testrun_add());
        testrun_counter++;

        testrun_test(check_macro_testrun_init_testcases);


        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int check_inline_functions_testrun() {

        testrun_init();

        testrun_test(check_inline_function_testrun_add_testcases);
        testrun_test(check_inline_function_testrun_dump_testcases);
        testrun_test(check_inline_function_testrun_parallel);
        testrun_test(check_inline_function_testrun_sequential);
        testrun_test(check_inline_function_testrun_run_tests);

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int all_tests() {

       testrun_init();

       testrun_counter += check_macros_logging();
       testrun_counter += check_macros_testrun();
       testrun_counter += check_inline_functions_testrun();

       /* TEST BLOCK ASSERT MUST BE LAST (as long as assert is really tested)*/
       //testrun_counter += check_macros_assert();

       return testrun_counter;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST EXECUTION                                                  #EXEC
 *
 *      ------------------------------------------------------------------------
 */
testrun_run(all_tests);
