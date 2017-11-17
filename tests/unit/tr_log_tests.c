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
 *      @file           tr_string_tests.c
 *      @author         Markus Toepfer
 *      @date           2017-11-13
 *
 *      @ingroup                testrun_lib
 *
 *      @brief                  Unit testing for tr_strings
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
#include "../../include/tr_log.h"

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

int check_tr_log_forward(){

        assert(tr_log_forward(10,"tr_log_forward"));

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int check_tr_log_file(){

        assert(tr_log_file(10, stdout, "tr_log_file"));

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
       testrun_test(check_tr_log_forward);
       testrun_test(check_tr_log_file);


       return 1;
}

/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/


testrun_run(all_tests);
