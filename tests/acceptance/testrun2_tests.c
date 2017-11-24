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
 *      @file           testrun2_tests.c
 *      @author         Markus Toepfer
 *      @date           2017-11-21
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Testing of the testrun2.h header
 *
 *      ------------------------------------------------------------------------
 **/

/*******************************************************************************
 *
 *      TEST SETUP
 *
 ******************************************************************************/

#include "../../include/testrun2.h"
#include "../../include/testrun_log.h"

static int testing = 1;

/*******************************************************************************
 *
 *      TEST CASES
 *
 ******************************************************************************/

int test_test1() {

        testrun_check(true, "test %s %d", "msg", 1);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_test2() {

        testrun_check(true, "test %s %d", "msg", 2);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_test3() {

        testrun_check(true, "test %s %d", "msg", 3);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_test4() {

        testrun_check(true, "test %s %d", "msg", 4);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_test5() {

        testrun_check(true, "test %s %d", "msg", 5);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

/*******************************************************************************
 *
 *      TEST CLUSTER SETUP
 *
 ******************************************************************************/

int64_t module_add_tests(int(*tests[])(), size_t slot, size_t max) {

        int64_t testrun_counter = 0;

        testrun_add(test_test1);
        testrun_add(test_test2);
        testrun_add(test_test3);
        testrun_add(test_test4);
        testrun_add(test_test5);

        log_dev("test counter %jd", testrun_counter);

        for (size_t i = 0; i < max; i++){
                printf("%jd %p \n", i, tests[i]);
        }

        return testrun_counter;
}

/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/

int64_t run_tests() {

        size_t  max     = 10;
        size_t  counter = 0;
        int64_t r = 0;

        int (*testcases[max])();

        // initialize all function pointer slots of the array to NULL
        testrun_init_testcases(testcases, 0, max);

        r = testrun_add_testcases(testcases, &counter, &max, module_add_tests);
        if (r < 0)
                return -1;

        // example to dump all enabled testcases (usefull for testdevelopment)
        testrun_dump_testcases(testcases, max, true);

        // run all tests again, but this time in parallel
        r = testrun_parallel(testcases, counter);
        if (r < 0)
                log_debug("Failure parallel run");

        // run all tests serial, one after the other and dont break on failure
        r = testrun_sequential(testcases, counter, false);
        if (r < 0)
                log_debug("Failure serial run");

        return counter;
}

testrun_run(run_tests);
