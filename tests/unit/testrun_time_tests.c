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

        @file           testrun_time_tests.c
        @author         Markus Toepfer
        @date           2017-11-28

        @ingroup        testrun_lib

        @brief          Testing of testrun time.

        ------------------------------------------------------------------------
*/

#include "../../include/testrun.h"
#include "../../src/testrun_time.c"
#include <ctype.h>


/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */

int test_testrun_timestamp(){

        char *result = testrun_timestamp(false);

        //01234567890123456789
        //2017-11-20T14:57:01Z

        // structure check only
        testrun(result != NULL);
        testrun(strlen(result) == 20);
        for (int i = 0; i < 20; i++){

                if (i == 19) {
                        testrun(result[19] == 'Z');
                } else if (i == 16) {
                        testrun(result[16] == ':');
                } else if (i == 13) {
                        testrun(result[13] == ':');
                } else if (i == 10) {
                        testrun(result[10] == 'T');
                } else if (i == 7) {
                        testrun(result[7]  == '-');
                } else if (i == 4){
                        testrun(result[4]  == '-');
                } else {
                        testrun(isdigit(result[i]));
                }
        }
        free(result);

        //01234567890123456789012345678
        //2017-11-20T14:57:01.123456Z

        result = testrun_timestamp(true);
        testrun(result != NULL);
        testrun(strlen(result) == 27);
        for (int i = 0; i < 20; i++){

                if (i == 26) {
                        testrun(result[19] == 'Z');
                } else if (i == 19) {
                        testrun(result[16] == ':');
                } else if (i == 16) {
                        testrun(result[16] == ':');
                } else if (i == 13) {
                        testrun(result[13] == ':');
                } else if (i == 10) {
                        testrun(result[10] == 'T');
                } else if (i == 7) {
                        testrun(result[7]  == '-');
                } else if (i == 4){
                        testrun(result[4]  == '-');
                } else {
                        testrun(isdigit(result[i]));
                }
        }
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_timestamp_write_to(){


        char result[30];

        testrun(!testrun_timestamp_write_to(true, NULL,   0));
        testrun(!testrun_timestamp_write_to(true, result, 0));
        testrun(!testrun_timestamp_write_to(true, result, 27));
        testrun(!testrun_timestamp_write_to(false,result, 20));


        testrun(testrun_timestamp_write_to(false, result, 21));

        //01234567890123456789
        //2017-11-20T14:57:01Z

        // structure check only
        testrun(strlen(result) == 20);
        for (int i = 0; i < 20; i++){

                if (i == 19) {
                        testrun(result[19] == 'Z');
                } else if (i == 16) {
                        testrun(result[16] == ':');
                } else if (i == 13) {
                        testrun(result[13] == ':');
                } else if (i == 10) {
                        testrun(result[10] == 'T');
                } else if (i == 7) {
                        testrun(result[7]  == '-');
                } else if (i == 4){
                        testrun(result[4]  == '-');
                } else {
                        testrun(isdigit(result[i]));
                }
        }

        testrun(!testrun_timestamp_write_to(true,result, 27));
        testrun(testrun_timestamp_write_to(false,result, 21));
        // structure check only
        testrun(strlen(result) == 20);


        testrun(testrun_timestamp_write_to(true,result, 28));

        //01234567890123456789012345678
        //2017-11-20T14:57:01.123456Z
        testrun(result != NULL);
        testrun(strlen(result) == 27);
        for (int i = 0; i < 20; i++){

                if (i == 26) {
                        testrun(result[19] == 'Z');
                } else if (i == 19) {
                        testrun(result[16] == ':');
                } else if (i == 16) {
                        testrun(result[16] == ':');
                } else if (i == 13) {
                        testrun(result[13] == ':');
                } else if (i == 10) {
                        testrun(result[10] == 'T');
                } else if (i == 7) {
                        testrun(result[7]  == '-');
                } else if (i == 4){
                        testrun(result[4]  == '-');
                } else {
                        testrun(isdigit(result[i]));
                }
        }


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_time_string(){


        time_t t;
        time(&t);

        struct timeval tv;

        size_t size = 30;
        char time_utc[size];

        char *result = testrun_time_string(TESTRUN_SCOPE_SECOND);

        testrun(0 == gettimeofday(&tv, NULL));

        testrun(strftime(time_utc, size, "%F %T",gmtime(&tv.tv_sec)));
        testrun(strlen(time_utc) == strlen(result));
        testrun(strncmp(result, time_utc, strlen(time_utc)) == 0);
        free(result);

        result = testrun_time_string(TESTRUN_SCOPE_MINUTE);
        testrun(strftime(time_utc, size, "%F %R",gmtime(&tv.tv_sec)));
        testrun(strlen(time_utc) == strlen(result));
        testrun(strncmp(result, time_utc, strlen(time_utc)) == 0);
        free(result);

        result = testrun_time_string(TESTRUN_SCOPE_HOUR);
        testrun(strftime(time_utc, size, "%F %H",gmtime(&tv.tv_sec)));
        testrun(strlen(time_utc) == strlen(result));
        testrun(strncmp(result, time_utc, strlen(time_utc)) == 0);
        free(result);

        result = testrun_time_string(TESTRUN_SCOPE_DAY);
        testrun(strftime(time_utc, size, "%F",gmtime(&tv.tv_sec)));
        testrun(strlen(time_utc) == strlen(result));
        testrun(strncmp(result, time_utc, strlen(time_utc)) == 0);
        free(result);

        result = testrun_time_string(TESTRUN_SCOPE_MONTH);
        testrun(strftime(time_utc, size, "%Y-%m",gmtime(&tv.tv_sec)));
        testrun(strlen(time_utc) == strlen(result));
        testrun(strncmp(result, time_utc, strlen(time_utc)) == 0);
        free(result);

        result = testrun_time_string(TESTRUN_SCOPE_YEAR);
        testrun(strftime(time_utc, size, "%Y",gmtime(&tv.tv_sec)));
        testrun(strlen(time_utc) == strlen(result));
        testrun(strncmp(result, time_utc, strlen(time_utc)) == 0);
        free(result);

        // TESTRUN_SCOPE_MILLISECOND

        result = testrun_time_string(TESTRUN_SCOPE_MILLISECOND);

        //01234567890123456789012
        //2017-11-20 14:57:01.123

        // structure check only
        testrun(result != NULL);
        testrun(strlen(result) == 23);
        for (int i = 0; i < 23; i++){

                if (i == 19) {
                        testrun(result[19] == '.');
                } else if (i == 16) {
                        testrun(result[16] == ':');
                } else if (i == 13) {
                        testrun(result[13] == ':');
                } else if (i == 10) {
                        testrun(result[10] == ' ');
                } else if (i == 7) {
                        testrun(result[7]  == '-');
                } else if (i == 4){
                        testrun(result[4]  == '-');
                } else {
                        testrun(isdigit(result[i]));
                }
        }
        free(result);


        result = testrun_time_string(TESTRUN_SCOPE_MICROSECOND);
        //01234567890123456789012345678
        //2017-11-20 14:57:01.123456
        testrun(result != NULL);
        testrun(strlen(result) == 26);
        for (int i = 0; i < 26; i++){

                if (i == 19) {
                        testrun(result[19] == '.');
                } else if (i == 16) {
                        testrun(result[16] == ':');
                } else if (i == 13) {
                        testrun(result[13] == ':');
                } else if (i == 10) {
                        testrun(result[10] == ' ');
                } else if (i == 7) {
                        testrun(result[7]  == '-');
                } else if (i == 4){
                        testrun(result[4]  == '-');
                } else {
                        testrun(isdigit(result[i]));
                }
        }
        free(result);

        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int test_testrun_time_write_to(){

        size_t i    = 0;
        size_t size = 50;
        char result[size];
        bzero(result, size);

        testrun(!testrun_time_write_to(TESTRUN_SCOPE_YEAR, NULL,   0));
        testrun(!testrun_time_write_to(TESTRUN_SCOPE_YEAR, result, 0));


        testrun(!testrun_time_write_to(TESTRUN_SCOPE_YEAR, result, 4));
        testrun(result[0] == 0);
        testrun(testrun_time_write_to(TESTRUN_SCOPE_YEAR,  result, 5));
        testrun(strlen(result) == 4);
        for( i = 0; i < strlen(result); i++){
                testrun(isdigit(result[i]));
        }
        bzero(result, size);

        testrun(!testrun_time_write_to(TESTRUN_SCOPE_MONTH, result, 7));
        testrun(result[0] == 0);
        testrun(testrun_time_write_to(TESTRUN_SCOPE_MONTH,  result, 8));
        testrun(strlen(result) == 7);
        for( i = 0; i < strlen(result); i++){
                if (i == 4) {
                        testrun(result[i] == '-');
                } else {
                        testrun(isdigit(result[i]));
                }
        }
        bzero(result, size);

        testrun(!testrun_time_write_to(TESTRUN_SCOPE_DAY, result, 10));
        testrun(result[0] == 0);
        testrun(testrun_time_write_to(TESTRUN_SCOPE_DAY,  result, 11));
        testrun(strlen(result) == 10);
        for( i = 0; i < strlen(result); i++){

                if (i == 4) {
                        testrun(result[i] == '-');
                } else if (i == 7) {
                        testrun(result[i] == '-');
                } else {
                        testrun(isdigit(result[i]));
                }
        }
        bzero(result, size);

        testrun(!testrun_time_write_to(TESTRUN_SCOPE_HOUR, result, 13));
        testrun(result[0] == 0);
        testrun(testrun_time_write_to(TESTRUN_SCOPE_HOUR,  result, 14));
        testrun(strlen(result) == 13);
        for( i = 0; i < strlen(result); i++){

                if (i == 4) {
                        testrun(result[i] == '-');
                } else if (i == 7) {
                        testrun(result[i] == '-');
                } else if (i == 10) {
                        testrun(result[i] == ' ');
                } else {
                        testrun(isdigit(result[i]));
                }
        }
        bzero(result, size);

        testrun(result[0] == 0);
        testrun(!testrun_time_write_to(TESTRUN_SCOPE_MINUTE, result, 16));
        testrun(result[0] == 0);
        testrun(testrun_time_write_to(TESTRUN_SCOPE_MINUTE,  result, 17));
        testrun(strlen(result) == 16);
        for( i = 0; i < strlen(result); i++){

                if (i == 4) {
                        testrun(result[i] == '-');
                } else if (i == 7) {
                        testrun(result[i] == '-');
                } else if (i == 10) {
                        testrun(result[i] == ' ');
                } else if (i == 13) {
                        testrun(result[i] == ':');
                } else {
                        testrun(isdigit(result[i]));
                }
        }
        bzero(result, size);


        testrun(!testrun_time_write_to(TESTRUN_SCOPE_SECOND, result, 19));
        testrun(result[0] == 0);
        testrun(testrun_time_write_to(TESTRUN_SCOPE_SECOND,  result, 20));
        testrun(strlen(result) == 19);
        for( i = 0; i < strlen(result); i++){

                if (i == 4) {
                        testrun(result[i] == '-');
                } else if (i == 7) {
                        testrun(result[i] == '-');
                } else if (i == 10) {
                        testrun(result[i] == ' ');
                } else if (i == 13) {
                        testrun(result[i] == ':');
                } else if (i == 16) {
                        testrun(result[i] == ':');
                } else {
                        testrun(isdigit(result[i]));
                }
        }
        bzero(result, size);

        testrun(!testrun_time_write_to(TESTRUN_SCOPE_MILLISECOND, result, 23));
        testrun(result[0] == 0);
        testrun(testrun_time_write_to(TESTRUN_SCOPE_MILLISECOND,  result, 24));
        testrun(strlen(result) == 23);
        for( i = 0; i < strlen(result); i++){

                if (i == 4) {
                        testrun(result[i] == '-');
                } else if (i == 7) {
                        testrun(result[i] == '-');
                } else if (i == 10) {
                        testrun(result[i] == ' ');
                } else if (i == 13) {
                        testrun(result[i] == ':');
                } else if (i == 16) {
                        testrun(result[i] == ':');
                } else if (i == 19) {
                        testrun(result[i] == '.');
                } else {
                        testrun(isdigit(result[i]));
                }
        }
        bzero(result, size);


        testrun(!testrun_time_write_to(TESTRUN_SCOPE_MICROSECOND, result, 26));
        testrun(result[0] == 0);
        testrun(testrun_time_write_to(TESTRUN_SCOPE_MICROSECOND,  result, 27));
        testrun(strlen(result) == 26);
        for( i = 0; i < strlen(result); i++){

                if (i == 4) {
                        testrun(result[i] == '-');
                } else if (i == 7) {
                        testrun(result[i] == '-');
                } else if (i == 10) {
                        testrun(result[i] == ' ');
                } else if (i == 13) {
                        testrun(result[i] == ':');
                } else if (i == 16) {
                        testrun(result[i] == ':');
                } else if (i == 19) {
                        testrun(result[i] == '.');
                } else {
                        testrun(isdigit(result[i]));
                }
        }
        bzero(result, size);



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

        testrun_test(test_testrun_timestamp);
        testrun_test(test_testrun_timestamp_write_to);
        testrun_test(test_testrun_time_string);
        testrun_test(test_testrun_time_write_to);

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
