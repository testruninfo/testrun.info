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
 *      @file           testrun_log_tests.c
 *      @author         Markus Toepfer
 *      @date           2017-11-13
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Unit testing for testrun_log
 *
 *
 *      ------------------------------------------------------------------------
 **/

/*******************************************************************************
 *
 *      TEST SETUP
 *
 ******************************************************************************/

#include "../tools/testrun.h"
#include "../../include/testrun_log.h"

static int testing = 1;

/*******************************************************************************
 *
 *      TEST CASES
 *
 ******************************************************************************/


int check_log_dev(){

        assert(log_dev("log_dev"));
        assert(log_debug("log_debug"));
        assert(log_info("log_info"));
        assert(log_notice("log_notice"));
        assert(log_warning("log_warning"));
        assert(log_error("log_error"));
        assert(log_critical("log_critical"));
        assert(log_alert("log_alert"));
        assert(log_emergency("log_emergency"));

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int check_testrun_log_forward(){

        assert(testrun_log_forward(10,"testrun_log_forward"));

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int check_testrun_log_file(){

        assert(testrun_log_file(10, stdout, "testrun_log_file"));

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int check_testrun_log_timestamp(){

        char *result = testrun_log_timestamp(false);

        //01234567890123456789
        //2017-11-20T14:57:01Z

        // structure check only
        testrun_assert(result != NULL);
        testrun_assert(strlen(result) == 20);
        testrun_assert(result[19] == 'Z');
        testrun_assert(result[16] == ':');
        testrun_assert(result[13] == ':');
        testrun_assert(result[10] == 'T');
        testrun_assert(result[7]  == '-');
        testrun_assert(result[4]  == '-');

        //01234567890123456789012345678
        //2017-11-20T14:57:01.123456Z

        result = testrun_log_timestamp(true);
        testrun_assert(result != NULL);
        testrun_assert(strlen(result) == 27);
        testrun_assert(result[26] == 'Z');
        testrun_assert(result[19] == '.');
        testrun_assert(result[16] == ':');
        testrun_assert(result[13] == ':');
        testrun_assert(result[10] == 'T');
        testrun_assert(result[7]  == '-');
        testrun_assert(result[4]  == '-');

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/


/*******************************************************************************
 *
 *      TEST CLUSTER
 *
 ******************************************************************************/

int all_tests() {

       testrun_init();

       testrun_test(check_log_dev);
       testrun_test(check_testrun_log_forward);
       testrun_test(check_testrun_log_file);

       testrun_test(check_testrun_log_timestamp);


       return 1;
}

/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/


testrun_run(all_tests);
