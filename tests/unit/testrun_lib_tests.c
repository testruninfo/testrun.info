/***
 *      -------------------------------------------------------------------------
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
 *      -------------------------------------------------------------------------
 ***//**
 *
 *      @file           testrun_lib_tests.c
 *      @author         Markus Toepfer
 *      @date           2017-11-13
 *
 *      @ingroup
 *
 *      @brief
 *
 *
 *      -------------------------------------------------------------------------
 **/


/*******************************************************************************
 *
 *      TEST SETUP
 *
 ******************************************************************************/

#include "../../include/testrun.h"
#include "../../src/testrun_lib.c"

static int testing = 1;

/*******************************************************************************
 *
 *      TEST CASES
 *
 ******************************************************************************/
/*
int test_test1() {

        testrun_assert(1 == 1, "Failure: 1 == 1");
        return testrun_log_success();
}
*/
/*----------------------------------------------------------------------------*/

/*******************************************************************************
 *
 *      TEST CLUSTER
 *
 ******************************************************************************/

int all_tests() {

       testrun_init();


       return testrun_counter;
}

/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/


testrun_run(all_tests);
