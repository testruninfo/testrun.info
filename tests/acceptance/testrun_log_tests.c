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

        @file           testrun_log_tests.c
        @author         Markus Toepfer
        @date           2017-11-13

        @ingroup        testrun_lib

        @brief          Macro testing for testrun_log
                        This test function will call all defined macros
                        and inline functions of the header, and dump the
                        output to stdout.

                        Output check need to be done manually.
                        This is not an automated unit test.

        ------------------------------------------------------------------------
*/

#include "../tools/testrun.h"
#include "../../include/testrun_log.h"

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */

int run_inline_testrun_log_create_timestamp(){

        char *result = testrun_log_create_timestamp(false);

        //01234567890123456789
        //2017-11-20T14:57:01Z

        // structure check only
        testrun(result != NULL);
        testrun(strlen(result) == 20);
        testrun(result[19] == 'Z');
        testrun(result[16] == ':');
        testrun(result[13] == ':');
        testrun(result[10] == 'T');
        testrun(result[7]  == '-');
        testrun(result[4]  == '-');

        //01234567890123456789012345678
        //2017-11-20T14:57:01.123456Z

        result = testrun_log_create_timestamp(true);
        testrun(result != NULL);
        testrun(strlen(result) == 27);
        testrun(result[26] == 'Z');
        testrun(result[19] == '.');
        testrun(result[16] == ':');
        testrun(result[13] == ':');
        testrun(result[10] == 'T');
        testrun(result[7]  == '-');
        testrun(result[4]  == '-');

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_inline_testrun_log_create_timestamp_save(){


        char result[30];

        testrun(!testrun_log_create_timestamp_save(true, NULL,   0));
        testrun(!testrun_log_create_timestamp_save(true, result, 0));
        testrun(!testrun_log_create_timestamp_save(true, result, 27));
        testrun(!testrun_log_create_timestamp_save(false,result, 20));


        testrun(testrun_log_create_timestamp_save(false, result, 21));

        //01234567890123456789
        //2017-11-20T14:57:01Z

        // structure check only
        testrun(strlen(result) == 20);
        testrun(result[19] == 'Z');
        testrun(result[16] == ':');
        testrun(result[13] == ':');
        testrun(result[10] == 'T');
        testrun(result[7]  == '-');
        testrun(result[4]  == '-');
        bzero(result, 30);
        testrun(strlen(result) == 0);

        testrun(!testrun_log_create_timestamp_save(true,result, 27));
        testrun(testrun_log_create_timestamp_save(false,result, 21));
        // structure check only
        testrun(strlen(result) == 20);
        testrun(result[19] == 'Z');
        testrun(result[16] == ':');
        testrun(result[13] == ':');
        testrun(result[10] == 'T');
        testrun(result[7]  == '-');
        testrun(result[4]  == '-');
        bzero(result, 30);
        testrun(strlen(result) == 0);

        testrun(testrun_log_create_timestamp_save(true,result, 28));

        //01234567890123456789012345678
        //2017-11-20T14:57:01.123456Z
        testrun(result != NULL);
        testrun(strlen(result) == 27);
        testrun(result[26] == 'Z');
        testrun(result[19] == '.');
        testrun(result[16] == ':');
        testrun(result[13] == ':');
        testrun(result[10] == 'T');
        testrun(result[7]  == '-');
        testrun(result[4]  == '-');

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_testrun_log_file_print_plain(){

        testrun(testrun_log_file_print_plain(
                stdout, "testrun_log_file_print_plain %s %d", "message", 1));

        /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_testrun_log_file_print_level_plain(){

        testrun(testrun_log_file_print_level_plain(
                stdout, 1, "testrun_log_file_print_level_plain %s %d", \
                "message", 1));

        /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_testrun_log_file_print_time_plain(){

        testrun(testrun_log_file_print_time_plain(
                stdout, 1,
                testrun_log_create_timestamp(false),
                "testrun_log_file_print_time_plain %s %d",
                "message", 1));

        /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_testrun_log_file_print_all_plain(){

        testrun(testrun_log_file_print_all_plain(
                stdout, 1,
                testrun_log_create_timestamp(false),
                "testrun_log_file_print_all_plain %s %d",
                "message", 1));

        /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_testrun_log_file_print_time_json(){

        testrun(testrun_log_file_print_time_json(
                stdout, 1,
                testrun_log_create_timestamp(false),
                "testrun_log_file_print_time_json %s %d",
                "message", 1));

        /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_testrun_log_file_print_all_json_format1(){

        testrun(testrun_log_file_print_all_json_format1(
                stdout, 1,
                testrun_log_create_timestamp(false),
                "testrun_log_file_print_all_json_format1 %s %d",
                "message", 1));

        /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_testrun_log_file_print_all_json_format2(){

        testrun(testrun_log_file_print_all_json_format2(
                stdout, 1,
                testrun_log_create_timestamp(false),
                "testrun_log_file_print_all_json_format2 %s %d",
                "message", 1));

        /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_testrun_log_file_print_all_json_no_format(){

        testrun(testrun_log_file_print_all_json_no_format(
                stdout, 1,
                testrun_log_create_timestamp(false),
                "testrun_log_file_print_all_json_no_format %s %d",
                "message", 1));

        /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_testrun_log_file(){

        testrun_log_file(10, stdout, "testrun_log_file %s %d", "message", 1);

        /*
         *      NOTE This is not a unit test.
         *      TBD  currently manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         *
         *      Message MUST be: "testrun_log_file message 1"
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_testrun_log_file_timestamp_shared(){

        testrun_log_file_timestamp_shared(10, stdout,
                        "testrun_log_file_timestamp_shared %s %d","message", 1);

        /*
         *      NOTE This is not a unit test.
         *      TBD  currently manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         *
         *      Message MUST be: "testrun_log_file message 1"
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_testrun_log_forward(){

        testrun_log_forward(10, "testrun_log_forward %s %d", "message", 1);

        /*
         *      NOTE This is not a unit test.
         *      TBD  currently manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         *
         *      Message MUST be: "testrun_log_forward message 1"
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_log_dev(){

        log_dev("log_dev %s %d", "message", 1);

         /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_log_oom(){

        log_oom();

         /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_log_debug(){

        log_debug("log_debug %s %d", "message", 1);

         /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_log_info(){

        log_debug("log_info %s %d", "message", 1);

         /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int run_macro_log_notice(){

        log_debug("log_notice %s %d", "message", 1);

         /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int run_macro_log_warning(){

        log_debug("log_warning %s %d", "message", 1);

         /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int run_macro_log_error(){

        log_debug("log_error %s %d", "message", 1);

         /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_log_critical(){

        log_debug("log_critical %s %d", "message", 1);

         /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int run_macro_log_alert(){

        log_debug("log_alert %s %d", "message", 1);

         /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
         */

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int run_macro_log_emergency(){

        log_debug("log_emergency %s %d", "message", 1);

         /*
         *      NOTE TBD  manual inspection of log output is required
         *
         *      Check console for:
         *
         *              (1) actual log message seen
         *              (2) correct log level used
         *              (3) format specifiers replaced
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

int all_tests() {

        testrun_init();

        testrun_test(run_inline_testrun_log_create_timestamp);
        testrun_test(run_inline_testrun_log_create_timestamp_save);

        testrun_test(run_macro_testrun_log_file_print_plain);
        testrun_test(run_macro_testrun_log_file_print_level_plain);
        testrun_test(run_macro_testrun_log_file_print_time_plain);
        testrun_test(run_macro_testrun_log_file_print_all_plain);
        testrun_test(run_macro_testrun_log_file_print_time_json);
        testrun_test(run_macro_testrun_log_file_print_all_json_format1);
        testrun_test(run_macro_testrun_log_file_print_all_json_format2);
        testrun_test(run_macro_testrun_log_file_print_all_json_no_format);

        testrun_test(run_macro_testrun_log_file);
        testrun_test(run_macro_testrun_log_file_timestamp_shared);
        testrun_test(run_macro_testrun_log_forward);

        testrun_test(run_macro_log_dev);
        testrun_test(run_macro_log_oom);
        testrun_test(run_macro_log_debug);
        testrun_test(run_macro_log_info);
        testrun_test(run_macro_log_notice);
        testrun_test(run_macro_log_warning);
        testrun_test(run_macro_log_error);
        testrun_test(run_macro_log_critical);
        testrun_test(run_macro_log_alert);
        testrun_test(run_macro_log_emergency);
        return 1;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST EXECUTION                                                  #EXEC
 *
 *      ------------------------------------------------------------------------
 */

testrun_run(all_tests);
