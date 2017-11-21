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
 *      @file           testrun_parallel_tests.c
 *      @author         Markus Toepfer
 *      @date           2017-11-17
 *
 *      @ingroup
 *
 *      @brief
 *
 *
 *      ------------------------------------------------------------------------
 **/

/*******************************************************************************
 *
 *      TEST SETUP
 *
 ******************************************************************************/

#include "../../include/testrun.h"
#include "../../src/testrun_parallel.c"

static int testing = 1;

/*----------------------------------------------------------------------------*/

void dummy_test1(){

	size_t testrun_counter_all     = 0;
	size_t testrun_counter_success = 0;
	size_t testrun_counter_failure = 0;

        testrun(true);
}

/*----------------------------------------------------------------------------*/

void dummy_test2(){

	size_t testrun_counter_all     = 0;
	size_t testrun_counter_success = 0;
	size_t testrun_counter_failure = 0;

        testrun(true);
}

/*----------------------------------------------------------------------------*/

void dummy_test3(){

	size_t testrun_counter_all     = 0;
	size_t testrun_counter_success = 0;
	size_t testrun_counter_failure = 0;

        testrun(true);
}

/*----------------------------------------------------------------------------*/


/*******************************************************************************
 *
 *      TEST CASES
 *
 ******************************************************************************/

/*----------------------------------------------------------------------------*/
/*
void test_testrun_parallel(){

	testrun_init();

        for (int i = 0; i < 25; i++){
                testrun_add(dummy_test1);
                testrun_add(dummy_test2);
                testrun_add(dummy_test3);
        }

        testrun(true == testrun_parallel(tests,
        	testrun_counter_all,
        	&testrun_counter_success,
        	&testrun_counter_failure));


}
*/

/*******************************************************************************
 *
 *      TEST CLUSTER
 *
 ******************************************************************************/


int all_tests() {

	//test_testrun_parallel();
/*
	size_t testsrun_tests_max       = 1000;
        size_t testrun_counter_all      = 0;
        size_t testrun_counter_success  = 0;
        size_t testrun_counter_failure  = 0;
        void (*tests[testsrun_tests_max])();
        bzero(tests, sizeof(tests));


	testrun_add(test_testrun_parallel);

	testrun_parallel(tests, testrun_counter_all, &testrun_counter_success, &testrun_counter_failure);
  */
       	return 1;
}

/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/




testrun_run(all_tests);
