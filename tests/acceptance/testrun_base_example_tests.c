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

        @file           testrun_base_example_tests.c
        @author         Markus Toepfer
        @date           2017-11-24

        @ingroup        testrun_lib

        @brief          Check the base exampe of the header

        NOTE            This is a copy & paste of the example included.

        ------------------------------------------------------------------------
*/

#include "../../include/testrun.h"

int example_function() {
        return 1;
}

//---------------------------------------------------------------------

int test_function1() {

        // use of testrun_check() for evaluation
        testrun_check(1 == 1);
        testrun_check(1 == 1, "some additional information");

        // use of testrun() for evaluation
        testrun(1 == 1);
        testrun(1 == 1, "some additional information");

        // use of manual evaluation and logging
        if (1 != example_function()){
                testrun_log_error("some additional information.");
                return -1;
        }

        // will not be reached in case of error
        return testrun_log_success();
}

//---------------------------------------------------------------------

int test_function2() {

        testrun_check(1 == 1);
        return testrun_log_success();
}

//---------------------------------------------------------------------

int test_function3() {

        testrun_check(1 == 1);
        return testrun_log_success();
}

//---------------------------------------------------------------------

int testcase_cluster() {

        testrun_init();

        testrun_test(test_function1);
        testrun_test(test_function2);
        testrun_test(test_function3);

        return testrun_counter;

}

testrun_run(testcase_cluster);
