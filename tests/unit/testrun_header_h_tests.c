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
 *      @file           testrun_header_h.c
 *      @author         Markus Toepfer
 *      @date           2017-11-24
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Check the header MACROs and inline functions
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

/*******************************************************************************
 *
 *      TEST CASES LOGGING
 *
 ******************************************************************************/

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

/*******************************************************************************
 *
 *      TEST CASES ASSERT BASED TESTING
 *
 ******************************************************************************/

int check_macro_testrun_init(){

        testrun_init();

        testrun(result          == 0,   "expected initialization of result");
        testrun(testrun_counter == 0,   "expected initialization of counter");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

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

int check_macro_testrun_test(){

        testrun_log_function_info("When you are able to read this it works.");
        return testrun_log_success();
}

/*******************************************************************************
 *
 *      TEST CLUSTER
 *
 ******************************************************************************/

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

int check_macros_execution() {

        testrun_init();

        testrun_test(check_macro_testrun_init);
        testrun_test(check_macro_testrun_check);
        testrun_test(check_macro_testrun);
        testrun_test(check_macro_testrun_test);

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int all_tests() {

       testrun_init();

       testrun_counter += check_macros_logging();
       testrun_counter += check_macros_execution();

       return testrun_counter;
}

/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/

testrun_run(all_tests);
