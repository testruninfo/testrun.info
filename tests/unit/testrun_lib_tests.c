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

        @file           testrun_lib_tests.c
        @author         Markus Toepfer
        @date           2017-11-29

        @ingroup        testrun_lib

        @brief          Test of generic lib functions


       ------------------------------------------------------------------------
*/



#include "../../include/testrun.h"
#include "../../src/testrun_lib.c"


/*
 *      ------------------------------------------------------------------------
 *
 *      TEST HELPER                                                    #HELPER
 *
 *      ------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------*/

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------*/

int test_testrun_lib_testrun_header_content(){

        size_t size = 10000;
        char expect[size];

        char *result = testrun_lib_testrun_header_content();
        char *date =  testrun_time_string(TESTRUN_SCOPE_DAY);

snprintf(expect, size,
"/***" TESTRUN_LINEEND
"        ------------------------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"        Copyright 2017 Markus Toepfer"TESTRUN_LINEEND
TESTRUN_LINEEND
"        Licensed under the Apache License, Version 2.0 (the \"License\");"TESTRUN_LINEEND
"        you may not use this file except in compliance with the License."TESTRUN_LINEEND
"        You may obtain a copy of the License at"TESTRUN_LINEEND
TESTRUN_LINEEND
"                http://www.apache.org/licenses/LICENSE-2.0"TESTRUN_LINEEND
TESTRUN_LINEEND
"        Unless required by applicable law or agreed to in writing, software"TESTRUN_LINEEND
"        distributed under the License is distributed on an \"AS IS\" BASIS,"TESTRUN_LINEEND
"        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."TESTRUN_LINEEND
"        See the License for the specific language governing permissions and"TESTRUN_LINEEND
"        limitations under the License."TESTRUN_LINEEND
TESTRUN_LINEEND
"        This file is part of the testrun project. http://testrun.info"TESTRUN_LINEEND
TESTRUN_LINEEND
"        ------------------------------------------------------------------------"TESTRUN_LINEEND
"*//**"TESTRUN_LINEEND
"        @file           testrun.h"TESTRUN_LINEEND
"        @author         Markus Toepfer"TESTRUN_LINEEND
"        @date           %s"TESTRUN_LINEEND
TESTRUN_LINEEND
"        @ingroup        testrun"TESTRUN_LINEEND
TESTRUN_LINEEND
"        @brief          Simple serial test execution framework."TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"        ------------------------------------------------------------------------"TESTRUN_LINEEND
"*/"TESTRUN_LINEEND
TESTRUN_LINEEND
"#ifndef testrun_h"TESTRUN_LINEEND
"#define testrun_h"TESTRUN_LINEEND
TESTRUN_LINEEND
"#include <unistd.h>     /* C89/C90 */"TESTRUN_LINEEND
"#include <stdlib.h>     /* C89/C90 */"TESTRUN_LINEEND
"#include <stdio.h>      /* C89/C90 */"TESTRUN_LINEEND
"#include <string.h>     /* C89/C90 */"TESTRUN_LINEEND
"#include <errno.h>      /* C89/C90 */"TESTRUN_LINEEND
"#include <time.h>       /* C89/C90 */"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
TESTRUN_LINEEND
"#define testrun_errno() \\"TESTRUN_LINEEND
"        (errno == 0 ? \"NONE\" :  strerror(errno))"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
TESTRUN_LINEEND
"/**"TESTRUN_LINEEND
"        Log a failure. Failure: Inability to perform a function as expected."TESTRUN_LINEEND
"*/"TESTRUN_LINEEND
"#define testrun_log_failure(msg, ...) \\"TESTRUN_LINEEND
"        fprintf(stderr, \"\\t[FAIL]\\t%%s line:%%d errno:%%s message: \" msg \"\\n\",\\"TESTRUN_LINEEND
"               __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
TESTRUN_LINEEND
"/**"TESTRUN_LINEEND
"        Log an error. Error: Difference between expected and actual result."TESTRUN_LINEEND
"*/"TESTRUN_LINEEND
"#define testrun_log_error(msg, ...) \\"TESTRUN_LINEEND
"        fprintf(stderr, \"\\t[ERROR]\\t%%s line:%%d errno:%%s message: \" msg \"\\n\",\\"TESTRUN_LINEEND
"        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
TESTRUN_LINEEND
"#define testrun_log_success(msg, ...) \\"TESTRUN_LINEEND
"        fprintf(stdout, \"\\t[OK] \\t%%s \" msg \"\\n\", __FUNCTION__, ##__VA_ARGS__)"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
TESTRUN_LINEEND
"#define testrun_log(msg, ...) \\"TESTRUN_LINEEND
"        fprintf(stdout, \"\\t\" msg \"\\n\", ##__VA_ARGS__)"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
TESTRUN_LINEEND
"#define testrun_log_function_info(msg, ...) \\"TESTRUN_LINEEND
"        fprintf(stdout, \"\\t[INFO] \\t%%s line:%%d message: \" msg \"\\n\", \\"TESTRUN_LINEEND
"                __FUNCTION__, __LINE__, ##__VA_ARGS__)"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
TESTRUN_LINEEND
"#define testrun_log_clock(start, end) \\"TESTRUN_LINEEND
"        fprintf(stdout, \"\\tClock ticks function: ( %%s ) | %%f s | %%.0f ms \\n\", \\"TESTRUN_LINEEND
"        __func__, \\"TESTRUN_LINEEND
"        ((double)(end - start)) / CLOCKS_PER_SEC, \\"TESTRUN_LINEEND
"        (((double)(end - start)) / CLOCKS_PER_SEC ) * 1000)"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
""TESTRUN_LINEEND
"#define testrun_init()  \\"TESTRUN_LINEEND
"        int result = 0; \\"TESTRUN_LINEEND
"        int testrun_counter = 0;"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
""TESTRUN_LINEEND
"/**"TESTRUN_LINEEND
"        Run a single atomar test. Return the surrounding block on error."TESTRUN_LINEEND
"        This function will leave the context block running on error. The"TESTRUN_LINEEND
"        Mindset is a defused assert. LEAVE THE FUNCTION NOT THE PROGRAM."TESTRUN_LINEEND
TESTRUN_LINEEND
"        @param test     boolean decision input."TESTRUN_LINEEND
"        @returns        the calling function on error with -1"TESTRUN_LINEEND
"*/"TESTRUN_LINEEND
"#define testrun_check(test, ... )\\"TESTRUN_LINEEND
"        if (!(test)) { testrun_log_error(__VA_ARGS__);  return -1;}"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
TESTRUN_LINEEND
"/**"TESTRUN_LINEEND
"        Alias to @see testrun_check."TESTRUN_LINEEND
"*/"TESTRUN_LINEEND
"#define testrun(test, ...)\\"TESTRUN_LINEEND
"        testrun_check(test, __VA_ARGS__ )"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
""TESTRUN_LINEEND
"/**"TESTRUN_LINEEND
"        Run a single test (execute a function pointer. Runs a test function."TESTRUN_LINEEND
"        On non negative return value of the function run, a testrun_counter"TESTRUN_LINEEND
"        is increased, on negative result, the negative result will be returned."TESTRUN_LINEEND
TESTRUN_LINEEND
"        @param test     function pointer of the test to run"TESTRUN_LINEEND
"        @NOTE           The surrounding block is left on negative result of the"TESTRUN_LINEEND
"                        function pointer execution."TESTRUN_LINEEND
"*/"TESTRUN_LINEEND
"#define testrun_test(test)\\"TESTRUN_LINEEND
"        result = test(); testrun_counter++; if (result < 0) return result;"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*----------------------------------------------------------------------------*/"TESTRUN_LINEEND
""TESTRUN_LINEEND
"/**"TESTRUN_LINEEND
"        Runs a function pointer, which SHALL contain the test function pointers"TESTRUN_LINEEND
"        to run. The function pointer is wrapped in a main procedure, which and"TESTRUN_LINEEND
"        allows indepentent testruns of the input testcluster over external"TESTRUN_LINEEND
"        execution."TESTRUN_LINEEND
TESTRUN_LINEEND
"        A clock will be started, as soon as the main is executed and the the"TESTRUN_LINEEND
"        time is stopped again, at the end of the execution. The difference"TESTRUN_LINEEND
"        will be printed and is the runtime of the whole input testcluster."TESTRUN_LINEEND
TESTRUN_LINEEND
"        A run will fail, as soon as one of the tests in the testcluster fails."TESTRUN_LINEEND
"        (Fail on first) or will run all functions dependent on the testsetup."TESTRUN_LINEEND
TESTRUN_LINEEND
"        @param testcluster      function pointer to be executed."TESTRUN_LINEEND
"*/"TESTRUN_LINEEND
"#define testrun_run(testcluster) int main(int argc, char *argv[]) {\\"TESTRUN_LINEEND
"        argc = argc;\\"TESTRUN_LINEEND
"        clock_t start1_t, end1_t; \\"TESTRUN_LINEEND
"        start1_t = clock(); \\"TESTRUN_LINEEND
"        testrun_log(\"\\ntestrun\\t%%s\", argv[0]);\\"TESTRUN_LINEEND
"        int64_t result = testcluster();\\"TESTRUN_LINEEND
"        if (result > 0) \\"TESTRUN_LINEEND
"                testrun_log(\"ALL TESTS RUN (%%jd tests)\", result);\\"TESTRUN_LINEEND
"        end1_t = clock(); \\"TESTRUN_LINEEND
"        testrun_log_clock(start1_t, end1_t); \\"TESTRUN_LINEEND
"        testrun_log();\\"TESTRUN_LINEEND
"        result >=0 ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE); \\"TESTRUN_LINEEND
"}"TESTRUN_LINEEND
TESTRUN_LINEEND
"/*--------------- For EXAMPLE code check http://testrun.info -----------------*/"TESTRUN_LINEEND
TESTRUN_LINEEND
"#endif /* testrun_h */"TESTRUN_LINEEND,
date);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_script_folder_runner_content(){

        size_t size = 10000;
        char expect[size];

        char *result = testrun_lib_script_folder_runner_content(
                testrun_config_default());

        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);

        snprintf(expect, size,
"#!/usr/bin/env bash"                                                           TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Copyright 2017 Markus Toepfer"                                         TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Licensed under the Apache License, Version 2.0 (the \"License\");"     TESTRUN_LINEEND
"#       you may not use this file except in compliance with the License."      TESTRUN_LINEEND
"#       You may obtain a copy of the License at"                               TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#               http://www.apache.org/licenses/LICENSE-2.0"                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Unless required by applicable law or agreed to in writing, software"   TESTRUN_LINEEND
"#       distributed under the License is distributed on an \"AS IS\" BASIS,"   TESTRUN_LINEEND
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."TESTRUN_LINEEND
"#       See the License for the specific language governing permissions and"   TESTRUN_LINEEND
"#       limitations under the License."                                        TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       File            testrun_runner.sh"                                     TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         testrun_runner.sh"                                     TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     Run each test.test of a folder and log Ok or NOK"      TESTRUN_LINEEND
"#                       for each executed testfile of the folder."             TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       EXAMPLE OUTPUT"                                        TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       [OK]  1/5 filename1.test"                              TESTRUN_LINEEND
"#                       [NOK] 2/5 filename2.test"                              TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       MODES"                                                 TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       (1) RUN ALL TESTS (log but ignore errors)"             TESTRUN_LINEEND
"#                           use script with 2 parameters"                      TESTRUN_LINEEND
"#                           e.g. ./testrun_runner.sh logfile /path"            TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                           This mode will not return a test failure and"      TESTRUN_LINEEND
"#                           may be used to run all tests and return success"   TESTRUN_LINEEND
"#                           if all tests was run. (test results are logged)"   TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       (2) FAIL ON ERROR (Fail on first error)"               TESTRUN_LINEEND
"#                           use script with 3 parameters"                      TESTRUN_LINEEND
"#                           e.g. ./testrun_runner.sh logfile /path 1"          TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                           This mode returns -1 on the first test failure."   TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       PARAMETER"                                             TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       (1) path to logfile destination"                       TESTRUN_LINEEND
"#                       (2) path to folder with test cases"                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           ./testrun_runner.sh /path/to/logfile /path/to/test/dir"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    bash, tail, ls, grep, wc"                              TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
"#       CONFIGURE SCRIPT BASED ON PRESET VARIBALES OR ON INPUT"                TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"if [ -z $1 ]; then"                                                            TESTRUN_LINEEND
"        echo \"ERROR ... NO LOGFILE INPUT TO SRCIPT\""                         TESTRUN_LINEEND
"        exit 1"                                                                TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
"LOGFILE=$1"                                                                    TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"if [ -z $2 ]; then"                                                            TESTRUN_LINEEND
"        echo \"ERROR ... FOLDER INPUT TO SRCIPT\""                             TESTRUN_LINEEND
"        exit 1"                                                                TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
"FOLDER=$2"                                                                     TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"FAIL_ON_ERROR=0"                                                               TESTRUN_LINEEND
"if [ ! -z $3 ]; then"                                                          TESTRUN_LINEEND
"        FAIL_ON_ERROR=1"                                                       TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"if [ ! -w $LOGFILE ]; then"                                                    TESTRUN_LINEEND
"        echo \"ERROR ... LOGFILE NOT WRITABLE\""                               TESTRUN_LINEEND
"        exit 1"                                                                TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
"#       PERFORM TESTRUN"                                                       TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"FILES=`ls  $FOLDER/ | grep \"\\.test\" | wc -l`"                               TESTRUN_LINEEND
"if [ $? -ne 0 ]; then"                                                         TESTRUN_LINEEND
"        echo \"ERROR ... could not count files of $FOLDER\""                   TESTRUN_LINEEND
"        exit 1"                                                                TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
"c=0"                                                                           TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"if [ $FILES -eq 0 ]; then"                                                     TESTRUN_LINEEND
"        exit 0"                                                                TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"for i in $FOLDER/*.test"                                                       TESTRUN_LINEEND
"do"                                                                            TESTRUN_LINEEND
"        c=$((c+1))"                                                            TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"        # RUN EXECUTABLE"                                                      TESTRUN_LINEEND
"        $i 2>&1 >> $LOGFILE"                                                   TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"        # CHECK RETURN OF EXECUTABLE"                                          TESTRUN_LINEEND
"        if [ $? -ne 0 ]; then"                                                 TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"                echo \"NOK\\t(\"$c\"/\"$FILES\")\\t\"$i"                       TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"                if [ $FAIL_ON_ERROR -eq 1 ]; then"                             TESTRUN_LINEEND
"                        exit 1"                                                TESTRUN_LINEEND
"                fi"                                                            TESTRUN_LINEEND
"        else"                                                                  TESTRUN_LINEEND
"                echo \"OK\\t(\"$c\"/\"$FILES\")\\t\"$i"                        TESTRUN_LINEEND
"        fi"                                                                    TESTRUN_LINEEND
"done"                                                                          TESTRUN_LINEEND
"exit 0"                                                                        TESTRUN_LINEEND
, date, date);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_script_acceptance_tests_content(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        testrun_config config = testrun_config_default();

        char *result = testrun_lib_script_acceptance_tests_content(config);

        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);

        snprintf(expect, size,
"#!/usr/bin/env bash"                                                           TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Copyright 2017 Markus Toepfer"                                         TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Licensed under the Apache License, Version 2.0 (the \"License\");"     TESTRUN_LINEEND
"#       you may not use this file except in compliance with the License."      TESTRUN_LINEEND
"#       You may obtain a copy of the License at"                               TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#               http://www.apache.org/licenses/LICENSE-2.0"                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Unless required by applicable law or agreed to in writing, software"   TESTRUN_LINEEND
"#       distributed under the License is distributed on an \"AS IS\" BASIS,"   TESTRUN_LINEEND
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."TESTRUN_LINEEND
"#       See the License for the specific language governing permissions and"   TESTRUN_LINEEND
"#       limitations under the License."                                        TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       File            %s"                                                    TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     Run all test executables build/test/acceptance/*.test" TESTRUN_LINEEND
"#                       Run the whole folder, until an error occurs."          TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       MODE         FAIL ON ERROR (Fail on first test error)" TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       LOGFILE      build/test/log/acceptancetest.<time>.log" TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           ./%s /path/to/project"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    bash, touch, chmod, ls, wc, date"                      TESTRUN_LINEEND
"#"TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"echo \"-------------------------------------------------------\""              TESTRUN_LINEEND
"echo \"               SIMPLE ACCEPTANCE TESTING\""                             TESTRUN_LINEEND
"echo \"-------------------------------------------------------\""              TESTRUN_LINEEND
TESTRUN_LINEEND
"start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")"                           TESTRUN_LINEEND
TESTRUN_LINEEND
"# SET A LOGFILE"                                                               TESTRUN_LINEEND
"LOGFILE=\"build/test/log/acceptance_\".$start_time.\"log\""                    TESTRUN_LINEEND
"echo \" (log)   $start_time\" > $LOGFILE"                                      TESTRUN_LINEEND
"touch $LOGFILE"                                                                TESTRUN_LINEEND
"chmod a+w $LOGFILE"                                                            TESTRUN_LINEEND
TESTRUN_LINEEND
"# SET THE FOLDER"                                                              TESTRUN_LINEEND
"FOLDER=\"build/test/acceptance\""                                              TESTRUN_LINEEND
TESTRUN_LINEEND
"echo \"-------------------------------------------------------\" >> $LOGFILE"  TESTRUN_LINEEND
"echo \"               REPORT ACCEPTANCE TESTING\"                >> $LOGFILE"  TESTRUN_LINEEND
"echo \"-------------------------------------------------------\" >> $LOGFILE"  TESTRUN_LINEEND
TESTRUN_LINEEND
"# RUN THE RUNNER"                                                              TESTRUN_LINEEND
"sh ./tests/./tools/testrun_runner.sh  $LOGFILE $FOLDER FAIL_ON_ERROR"          TESTRUN_LINEEND
"RESULT=$?"                                                                     TESTRUN_LINEEND
TESTRUN_LINEEND
"end_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")"                             TESTRUN_LINEEND
""TESTRUN_LINEEND
"# FINISH THE REPORT"                                                           TESTRUN_LINEEND
"echo \"-------------------------------------------------------\">> $LOGFILE"   TESTRUN_LINEEND
"echo \"DONE \\t ACCEPTANCE TEST RUN\"  >> $LOGFILE"                            TESTRUN_LINEEND
"if [ $RESULT -eq 0 ]; then"                                                    TESTRUN_LINEEND
"        echo \"RESULT\\t SUCCESS\"  >> $LOGFILE"                               TESTRUN_LINEEND
"else"                                                                          TESTRUN_LINEEND
"        echo \"RESULT\\t FAILURE\"  >> $LOGFILE"                               TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
"echo \"START \\t $start_time\" >> $LOGFILE"                                    TESTRUN_LINEEND
"echo \"END   \\t $end_time\" >> $LOGFILE"                                      TESTRUN_LINEEND
"echo \"-------------------------------------------------------\">> $LOGFILE"   TESTRUN_LINEEND
TESTRUN_LINEEND
"# DUMP THE REPORT"                                                             TESTRUN_LINEEND
"cat $LOGFILE"                                                                  TESTRUN_LINEEND
"echo \"\""                                                                     TESTRUN_LINEEND
"exit $RESULT"                                                                  TESTRUN_LINEEND
,config.project.path.tests.tools.acceptance_script,
date,
config.project.path.tests.tools.acceptance_script,
config.project.path.tests.tools.acceptance_script,
date);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));

        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_script_unit_tests_content(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        testrun_config config = testrun_config_default();

        char *result = testrun_lib_script_unit_tests_content(config);

        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);

        snprintf(expect, size,
"#!/usr/bin/env bash"                                                           TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Copyright 2017 Markus Toepfer"                                         TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Licensed under the Apache License, Version 2.0 (the \"License\");"     TESTRUN_LINEEND
"#       you may not use this file except in compliance with the License."      TESTRUN_LINEEND
"#       You may obtain a copy of the License at"                               TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#               http://www.apache.org/licenses/LICENSE-2.0"                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Unless required by applicable law or agreed to in writing, software"   TESTRUN_LINEEND
"#       distributed under the License is distributed on an \"AS IS\" BASIS,"   TESTRUN_LINEEND
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."TESTRUN_LINEEND
"#       See the License for the specific language governing permissions and"   TESTRUN_LINEEND
"#       limitations under the License."                                        TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       File            %s"                                                    TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     Run all test executables build/test/unit/*.test" TESTRUN_LINEEND
"#                       Run the whole folder, until an error occurs."          TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       MODE         FAIL ON ERROR (Fail on first test error)" TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       LOGFILE      build/test/log/unittest.<time>.log" TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           ./%s /path/to/project"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    bash, touch, chmod, ls, wc, date"                      TESTRUN_LINEEND
"#"TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"echo \"-------------------------------------------------------\""              TESTRUN_LINEEND
"echo \"               SIMPLE UNIT TESTING\""                                   TESTRUN_LINEEND
"echo \"-------------------------------------------------------\""              TESTRUN_LINEEND
TESTRUN_LINEEND
"start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")"                           TESTRUN_LINEEND
TESTRUN_LINEEND
"# SET A LOGFILE"                                                               TESTRUN_LINEEND
"LOGFILE=\"build/test/log/unit_\".$start_time.\"log\""                          TESTRUN_LINEEND
"echo \" (log)   $start_time\" > $LOGFILE"                                      TESTRUN_LINEEND
"touch $LOGFILE"                                                                TESTRUN_LINEEND
"chmod a+w $LOGFILE"                                                            TESTRUN_LINEEND
TESTRUN_LINEEND
"# SET THE FOLDER"                                                              TESTRUN_LINEEND
"FOLDER=\"build/test/unit\""                                                    TESTRUN_LINEEND
TESTRUN_LINEEND
"echo \"-------------------------------------------------------\" >> $LOGFILE"  TESTRUN_LINEEND
"echo \"               REPORT UNIT TESTING\"                      >> $LOGFILE"  TESTRUN_LINEEND
"echo \"-------------------------------------------------------\" >> $LOGFILE"  TESTRUN_LINEEND
TESTRUN_LINEEND
"# RUN THE RUNNER"                                                              TESTRUN_LINEEND
"sh ./tests/./tools/testrun_runner.sh  $LOGFILE $FOLDER FAIL_ON_ERROR"          TESTRUN_LINEEND
"RESULT=$?"                                                                     TESTRUN_LINEEND
TESTRUN_LINEEND
"end_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")"                             TESTRUN_LINEEND
""TESTRUN_LINEEND
"# FINISH THE REPORT"                                                           TESTRUN_LINEEND
"echo \"-------------------------------------------------------\">> $LOGFILE"   TESTRUN_LINEEND
"echo \"DONE \\t UNIT TEST RUN\"  >> $LOGFILE"                                  TESTRUN_LINEEND
"if [ $RESULT -eq 0 ]; then"                                                    TESTRUN_LINEEND
"        echo \"RESULT\\t SUCCESS\"  >> $LOGFILE"                               TESTRUN_LINEEND
"else"                                                                          TESTRUN_LINEEND
"        echo \"RESULT\\t FAILURE\"  >> $LOGFILE"                               TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
"echo \"START \\t $start_time\" >> $LOGFILE"                                    TESTRUN_LINEEND
"echo \"END   \\t $end_time\" >> $LOGFILE"                                      TESTRUN_LINEEND
"echo \"-------------------------------------------------------\">> $LOGFILE"   TESTRUN_LINEEND
TESTRUN_LINEEND
"# DUMP THE REPORT"                                                             TESTRUN_LINEEND
"cat $LOGFILE"                                                                  TESTRUN_LINEEND
"echo \"\""                                                                     TESTRUN_LINEEND
"exit $RESULT"                                                                  TESTRUN_LINEEND
,config.project.path.tests.tools.unit_script,
date,
config.project.path.tests.tools.unit_script,
config.project.path.tests.tools.unit_script,
date);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));

        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_script_coverage_tests_content(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        testrun_config config = testrun_config_default();

        char *result = testrun_lib_script_coverage_tests_content(config);

        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);

        char path_src[PATH_MAX];
        char path_unit[PATH_MAX];

        bzero(path_src,         PATH_MAX);
        bzero(path_unit,        PATH_MAX);

        testrun(testrun_path_project_to_unit_tests(path_unit, PATH_MAX,
                &config));

        testrun(testrun_path_project_to_source(path_src, PATH_MAX,
                &config));

snprintf(expect, size,
"#!/usr/bin/env bash"                                                           TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Copyright 2017 Markus Toepfer"                                         TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Licensed under the Apache License, Version 2.0 (the \"License\");"     TESTRUN_LINEEND
"#       you may not use this file except in compliance with the License."      TESTRUN_LINEEND
"#       You may obtain a copy of the License at"                               TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#               http://www.apache.org/licenses/LICENSE-2.0"                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Unless required by applicable law or agreed to in writing, software"   TESTRUN_LINEEND
"#       distributed under the License is distributed on an \"AS IS\" BASIS,"   TESTRUN_LINEEND
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."TESTRUN_LINEEND
"#       See the License for the specific language governing permissions and"   TESTRUN_LINEEND
"#       limitations under the License."                                        TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       File            %s"                                                    TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     Count functions of folder src against their counterparts"TESTRUN_LINEEND
"#                       in the unit test folder."                              TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       CONVENTION"                                            TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       Each function in any file of the source folder located"TESTRUN_LINEEND
"#                       \"%s\""                                                TESTRUN_LINEEND
"#                       will have a corresponding test function using the same"TESTRUN_LINEEND
"#                       name in any other file of the unit test folder located"TESTRUN_LINEEND
"#                       \"%s\","                                               TESTRUN_LINEEND
"#                       with a function name prefix of"                        TESTRUN_LINEEND
"#                       \"%s\"."                                               TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       EXAMPLE      function | %sfunction"                    TESTRUN_LINEEND
"#"TESTRUN_LINEEND
"#                       NOTE         This simple coverage test just covers the"TESTRUN_LINEEND
"#                                    observance of the given coding convention."TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           ./%s /path/to/project"                                 TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    bash, ctags, awk, sed, grep"                           TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S\")"                               TESTRUN_LINEEND
"PREFIX=\"%s\""                                                                 TESTRUN_LINEEND
TESTRUN_LINEEND
"LIBDIR=$1"                                                                     TESTRUN_LINEEND
"SRCDIR=$1/%s"                                                                  TESTRUN_LINEEND
"TESTDIR=$1/%s"                                                                 TESTRUN_LINEEND
TESTRUN_LINEEND
"# SET A LOGFILE"                                                               TESTRUN_LINEEND
"LOGFILE=\"$LIBDIR/build/test/log/coverage_\".$start_time.\"log\""              TESTRUN_LINEEND
"touch $LOGFILE"                                                                TESTRUN_LINEEND
"chmod a+w $LOGFILE"                                                            TESTRUN_LINEEND
TESTRUN_LINEEND
"echo \"-------------------------------------------------------\" >> $LOGFILE"  TESTRUN_LINEEND
"echo \"               REPORT COVERAGE TESTING\"                  >> $LOGFILE"  TESTRUN_LINEEND
"echo \"-------------------------------------------------------\" >> $LOGFILE"  TESTRUN_LINEEND
"echo \"   TIME \\t $start_time\" >> $LOGFILE"                                  TESTRUN_LINEEND
"echo \"\" >> $LOGFILE"                                                         TESTRUN_LINEEND
TESTRUN_LINEEND
"# GENERATE CTAGS SOURCE"                                                       TESTRUN_LINEEND
"cd $SRCDIR"                                                                    TESTRUN_LINEEND
"if [ $? -ne 0 ]; then"                                                         TESTRUN_LINEEND
"        exit 1"                                                                TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
"ctags --c-types=f -R"                                                          TESTRUN_LINEEND
"# remove the ctags stuff, to leave just the function lines"                    TESTRUN_LINEEND
"sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions"                                TESTRUN_LINEEND
"# remove anything but the function names"                                      TESTRUN_LINEEND
"awk '{print $1 }' $SRCDIR/functions > \\"                                      TESTRUN_LINEEND
"        $SRCDIR/functionNames"                                                 TESTRUN_LINEEND
"# count the lines"                                                             TESTRUN_LINEEND
"sourceFkt=\"$(cat functions | wc -l)\""                                        TESTRUN_LINEEND
"echo \"   count source\\t\" $sourceFkt >> $LOGFILE"                            TESTRUN_LINEEND
TESTRUN_LINEEND
"# GENERATE CTAGS TESTS"                                                        TESTRUN_LINEEND
"cd $TESTDIR"                                                                   TESTRUN_LINEEND
"if [ $? -ne 0 ]; then"                                                         TESTRUN_LINEEND
"        exit 1"                                                                TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
"ctags --c-types=f -R"                                                          TESTRUN_LINEEND
"# remove the ctags stuff, to leave just the function lines"                    TESTRUN_LINEEND
"sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions"                                TESTRUN_LINEEND
"# remove anything but the function names"                                      TESTRUN_LINEEND
"awk '{print $1 }' $TESTDIR/functions > \\"                                     TESTRUN_LINEEND
"        $TESTDIR/functionNames"                                                TESTRUN_LINEEND
"# count the lines"                                                             TESTRUN_LINEEND
"testFkt=\"$(cat functions | grep -i ^$PREFIX | wc -l)\""                       TESTRUN_LINEEND
"echo \"   count tests\\t\" $testFkt >> $LOGFILE"                               TESTRUN_LINEEND
TESTRUN_LINEEND
"echo \"\\nUNTESTED: \" >> $LOGFILE"                                            TESTRUN_LINEEND
"# Found functions:"                                                            TESTRUN_LINEEND
"while read line;"                                                              TESTRUN_LINEEND
"do"                                                                            TESTRUN_LINEEND
"        grep -n '^test_'$line'$' $TESTDIR/functionNames > \\"                  TESTRUN_LINEEND
"        /dev/null || echo $line >> $LOGFILE"                                   TESTRUN_LINEEND
"done < $SRCDIR/functionNames"                                                  TESTRUN_LINEEND
TESTRUN_LINEEND
"if [ $sourceFkt != 0 ]; then"                                                  TESTRUN_LINEEND
"        echo \"............................................\"  >> $LOGFILE"    TESTRUN_LINEEND
"        echo \"COVERAGE: $sourceFkt $testFkt\" | \\"                           TESTRUN_LINEEND
"        awk '{ printf $1 \" %%.2f %%%% \\n\", $3/$2*100}' >> $LOGFILE"         TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
TESTRUN_LINEEND
"cat $LOGFILE"                                                                  TESTRUN_LINEEND
"echo \"-------------------------------------------------------\""              TESTRUN_LINEEND
"echo \"\""TESTRUN_LINEEND
TESTRUN_LINEEND
"# cleanup remove the files we created"                                         TESTRUN_LINEEND
"rm $SRCDIR/tags"                                                               TESTRUN_LINEEND
"rm $SRCDIR/functions"                                                          TESTRUN_LINEEND
"rm $SRCDIR/functionNames"                                                      TESTRUN_LINEEND
"rm $TESTDIR/tags"                                                              TESTRUN_LINEEND
"rm $TESTDIR/functions"                                                         TESTRUN_LINEEND
"rm $TESTDIR/functionNames"                                                     TESTRUN_LINEEND
,config.project.path.tests.tools.coverage_script,
date,
config.project.path.tests.tools.coverage_script,
path_src,
path_unit,
config.format.prefix.unit_test,
config.format.prefix.unit_test,
config.project.path.tests.tools.coverage_script,
date,
config.format.prefix.unit_test,
path_src,
path_unit);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));

        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        // -------------------------------------------------------------
        // Check config
        // -------------------------------------------------------------
        testrun(testrun_path_project_to_unit_tests(path_unit, PATH_MAX,
                &config));

        testrun(testrun_path_project_to_source(path_src, PATH_MAX,
                &config));

        config.project.path.tests.tools.coverage_script = "test";
        config.format.prefix.unit_test = "prefix";
        config.project.path.source = "source";
        config.project.path.tests.unit = "ut";

        testrun(testrun_path_project_to_unit_tests(path_unit, PATH_MAX,
                &config));

        testrun(testrun_path_project_to_source(path_src, PATH_MAX,
                &config));

        bzero(expect, size);

snprintf(expect, size,
"#!/usr/bin/env bash"                                                           TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Copyright 2017 Markus Toepfer"                                         TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Licensed under the Apache License, Version 2.0 (the \"License\");"     TESTRUN_LINEEND
"#       you may not use this file except in compliance with the License."      TESTRUN_LINEEND
"#       You may obtain a copy of the License at"                               TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#               http://www.apache.org/licenses/LICENSE-2.0"                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Unless required by applicable law or agreed to in writing, software"   TESTRUN_LINEEND
"#       distributed under the License is distributed on an \"AS IS\" BASIS,"   TESTRUN_LINEEND
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."TESTRUN_LINEEND
"#       See the License for the specific language governing permissions and"   TESTRUN_LINEEND
"#       limitations under the License."                                        TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       File            %s"                                                    TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     Count functions of folder src against their counterparts"TESTRUN_LINEEND
"#                       in the unit test folder."                              TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       CONVENTION"                                            TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       Each function in any file of the source folder located"TESTRUN_LINEEND
"#                       \"%s\""                                                TESTRUN_LINEEND
"#                       will have a corresponding test function using the same"TESTRUN_LINEEND
"#                       name in any other file of the unit test folder located"TESTRUN_LINEEND
"#                       \"%s\","                                               TESTRUN_LINEEND
"#                       with a function name prefix of"                        TESTRUN_LINEEND
"#                       \"%s\"."                                               TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       EXAMPLE      function | %sfunction"                    TESTRUN_LINEEND
"#"TESTRUN_LINEEND
"#                       NOTE         This simple coverage test just covers the"TESTRUN_LINEEND
"#                                    observance of the given coding convention."TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           ./%s /path/to/project"                                 TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    bash, ctags, awk, sed, grep"                           TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S\")"                               TESTRUN_LINEEND
"PREFIX=\"%s\""                                                                 TESTRUN_LINEEND
TESTRUN_LINEEND
"LIBDIR=$1"                                                                     TESTRUN_LINEEND
"SRCDIR=$1/%s"                                                                  TESTRUN_LINEEND
"TESTDIR=$1/%s"                                                                 TESTRUN_LINEEND
TESTRUN_LINEEND
"# SET A LOGFILE"                                                               TESTRUN_LINEEND
"LOGFILE=\"$LIBDIR/build/test/log/coverage_\".$start_time.\"log\""              TESTRUN_LINEEND
"touch $LOGFILE"                                                                TESTRUN_LINEEND
"chmod a+w $LOGFILE"                                                            TESTRUN_LINEEND
TESTRUN_LINEEND
"echo \"-------------------------------------------------------\" >> $LOGFILE"  TESTRUN_LINEEND
"echo \"               REPORT COVERAGE TESTING\"                  >> $LOGFILE"  TESTRUN_LINEEND
"echo \"-------------------------------------------------------\" >> $LOGFILE"  TESTRUN_LINEEND
"echo \"   TIME \\t $start_time\" >> $LOGFILE"                                  TESTRUN_LINEEND
"echo \"\" >> $LOGFILE"                                                         TESTRUN_LINEEND
TESTRUN_LINEEND
"# GENERATE CTAGS SOURCE"                                                       TESTRUN_LINEEND
"cd $SRCDIR"                                                                    TESTRUN_LINEEND
"if [ $? -ne 0 ]; then"                                                         TESTRUN_LINEEND
"        exit 1"                                                                TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
"ctags --c-types=f -R"                                                          TESTRUN_LINEEND
"# remove the ctags stuff, to leave just the function lines"                    TESTRUN_LINEEND
"sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions"                                TESTRUN_LINEEND
"# remove anything but the function names"                                      TESTRUN_LINEEND
"awk '{print $1 }' $SRCDIR/functions > \\"                                      TESTRUN_LINEEND
"        $SRCDIR/functionNames"                                                 TESTRUN_LINEEND
"# count the lines"                                                             TESTRUN_LINEEND
"sourceFkt=\"$(cat functions | wc -l)\""                                        TESTRUN_LINEEND
"echo \"   count source\\t\" $sourceFkt >> $LOGFILE"                            TESTRUN_LINEEND
TESTRUN_LINEEND
"# GENERATE CTAGS TESTS"                                                        TESTRUN_LINEEND
"cd $TESTDIR"                                                                   TESTRUN_LINEEND
"if [ $? -ne 0 ]; then"                                                         TESTRUN_LINEEND
"        exit 1"                                                                TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
"ctags --c-types=f -R"                                                          TESTRUN_LINEEND
"# remove the ctags stuff, to leave just the function lines"                    TESTRUN_LINEEND
"sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions"                                TESTRUN_LINEEND
"# remove anything but the function names"                                      TESTRUN_LINEEND
"awk '{print $1 }' $TESTDIR/functions > \\"                                     TESTRUN_LINEEND
"        $TESTDIR/functionNames"                                                TESTRUN_LINEEND
"# count the lines"                                                             TESTRUN_LINEEND
"testFkt=\"$(cat functions | grep -i ^$PREFIX | wc -l)\""                       TESTRUN_LINEEND
"echo \"   count tests\\t\" $testFkt >> $LOGFILE"                               TESTRUN_LINEEND
TESTRUN_LINEEND
"echo \"\\nUNTESTED: \" >> $LOGFILE"                                            TESTRUN_LINEEND
"# Found functions:"                                                            TESTRUN_LINEEND
"while read line;"                                                              TESTRUN_LINEEND
"do"                                                                            TESTRUN_LINEEND
"        grep -n '^test_'$line'$' $TESTDIR/functionNames > \\"                  TESTRUN_LINEEND
"        /dev/null || echo $line >> $LOGFILE"                                   TESTRUN_LINEEND
"done < $SRCDIR/functionNames"                                                  TESTRUN_LINEEND
TESTRUN_LINEEND
"if [ $sourceFkt != 0 ]; then"                                                  TESTRUN_LINEEND
"        echo \"............................................\"  >> $LOGFILE"    TESTRUN_LINEEND
"        echo \"COVERAGE: $sourceFkt $testFkt\" | \\"                           TESTRUN_LINEEND
"        awk '{ printf $1 \" %%.2f %%%% \\n\", $3/$2*100}' >> $LOGFILE"         TESTRUN_LINEEND
"fi"                                                                            TESTRUN_LINEEND
TESTRUN_LINEEND
"cat $LOGFILE"                                                                  TESTRUN_LINEEND
"echo \"-------------------------------------------------------\""              TESTRUN_LINEEND
"echo \"\""TESTRUN_LINEEND
TESTRUN_LINEEND
"# cleanup remove the files we created"                                         TESTRUN_LINEEND
"rm $SRCDIR/tags"                                                               TESTRUN_LINEEND
"rm $SRCDIR/functions"                                                          TESTRUN_LINEEND
"rm $SRCDIR/functionNames"                                                      TESTRUN_LINEEND
"rm $TESTDIR/tags"                                                              TESTRUN_LINEEND
"rm $TESTDIR/functions"                                                         TESTRUN_LINEEND
"rm $TESTDIR/functionNames"                                                     TESTRUN_LINEEND
,config.project.path.tests.tools.coverage_script,
date,
config.project.path.tests.tools.coverage_script,
path_src,
path_unit,
config.format.prefix.unit_test,
config.format.prefix.unit_test,
config.project.path.tests.tools.coverage_script,
date,
config.format.prefix.unit_test,
path_src,
path_unit);

        testrun(strlen(result) != strlen(expect), "reconfigured");
        testrun(strncmp(result, expect, strlen(expect))!= 0, "reconfigured");
        result = testrun_string_free(result);
        result = testrun_lib_script_coverage_tests_content(config);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strlen(result) == strlen(expect), "reconfiguration applied");
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_script_loc_tests_content(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        testrun_config config = testrun_config_default();

        char *result = testrun_lib_script_coverage_tests_content(config);

        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);

        char path_src[PATH_MAX];
        char path_unit[PATH_MAX];
        char path_acceptance[PATH_MAX];

        bzero(path_src,         PATH_MAX);
        bzero(path_unit,        PATH_MAX);
        bzero(path_acceptance,  PATH_MAX);

        testrun(testrun_path_project_to_unit_tests(path_unit, PATH_MAX,
                &config));

        testrun(testrun_path_project_to_source(path_src, PATH_MAX,
                &config));

        testrun(testrun_path_project_to_acceptance_tests(
                path_acceptance, PATH_MAX, &config));

        result = testrun_lib_script_loc_tests_content(config);

        snprintf(expect, size,
"#!/usr/bin/env bash"                                                           TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Copyright 2017 Markus Toepfer"                                         TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Licensed under the Apache License, Version 2.0 (the \"License\");"     TESTRUN_LINEEND
"#       you may not use this file except in compliance with the License."      TESTRUN_LINEEND
"#       You may obtain a copy of the License at"                               TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#               http://www.apache.org/licenses/LICENSE-2.0"                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Unless required by applicable law or agreed to in writing, software"   TESTRUN_LINEEND
"#       distributed under the License is distributed on an \"AS IS\" BASIS,"   TESTRUN_LINEEND
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."TESTRUN_LINEEND
"#       See the License for the specific language governing permissions and"   TESTRUN_LINEEND
"#       limitations under the License."                                        TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       File            %s"                                                    TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     Count the lines of src and unit | acceptance tests."   TESTRUN_LINEEND
"#                       This file uses no error checking."                     TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           ./%s /path/to/project"                                 TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    bash, find, xargs, wc"                                 TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"echo \"-------------------------------------------------------\""              TESTRUN_LINEEND
"echo \"               SIMPLE LOC COUNTER\""                                    TESTRUN_LINEEND
"echo \"-------------------------------------------------------\""              TESTRUN_LINEEND
"echo \"(LOC) src\""                                                            TESTRUN_LINEEND
"find $1/%s -name '*.c' | xargs wc -l"                                          TESTRUN_LINEEND
"echo \"(LOC) tests/unit\""                                                     TESTRUN_LINEEND
"find $1/%s -name '*.c' | xargs wc -l"                                          TESTRUN_LINEEND
"echo \"(LOC) tests/acceptance\""                                               TESTRUN_LINEEND
"find $1/%s -name '*.c' | xargs wc -l"                                          TESTRUN_LINEEND
"echo \"-------------------------------------------------------\""              TESTRUN_LINEEND
"echo \"\""                                                                     TESTRUN_LINEEND
,config.project.path.tests.tools.loc_script,
date,
config.project.path.tests.tools.loc_script,
config.project.path.tests.tools.loc_script,
date,
path_src, path_unit, path_acceptance);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_c_file_content() {

        size_t size = 2500;
        char expect[size];
        bzero(expect, size);

        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);

        char *result = NULL;
        char *name   = "test";

        testrun_config config = testrun_config_default();

        // -------------------------------------------------------------
        // Positive test - HEADER
        // -------------------------------------------------------------


        result = testrun_lib_c_file_content(name, TESTRUN_HEADER, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s.h"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        "#ifndef %s_h"                TESTRUN_LINEEND
        "#define %s_h"                TESTRUN_LINEEND
        TESTRUN_LINEEND
        "#endif /* %s_h */" TESTRUN_LINEEND,
        name, date, name, name, name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - SOURCE
        // -------------------------------------------------------------

        result = testrun_lib_c_file_content(name, TESTRUN_SOURCE, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s.c"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        "#include \".././include/%s.h\""  TESTRUN_LINEEND
        TESTRUN_LINEEND,
        name, date, name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - TESTS
        // -------------------------------------------------------------

        result = testrun_lib_c_file_content(name, TESTRUN_TEST, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s_test.c"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "#include \".././tools/testrun.h\"" TESTRUN_LINEEND
        "#include \"../.././src/%s.c\"" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST HELPER                                                     #HELPER" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST CASES                                                      #CASES" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/**" TESTRUN_LINEEND
        "int test_ ... (){" TESTRUN_LINEEND
        "        testrun(true);" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        return testrun_log_success();" TESTRUN_LINEEND
        "}" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST CLUSTER                                                    #CLUSTER" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "int all_tests() {" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "        testrun_init();" TESTRUN_LINEEND
        "        //testrun_test(test_);" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "        return testrun_counter;" TESTRUN_LINEEND
        "}" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST EXECUTION                                                  #EXEC" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "testrun_run(all_tests);" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        name, date, name);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);



        // -------------------------------------------------------------
        // Configuration changes
        // -------------------------------------------------------------

        // move include header to project/src/include
        config.project.path.to_include = ".";
        // move tests to project/src/tests
        config.project.path.tests.to_project = ".";
        // move tests tools to project/tools
        config.project.path.tests.to_tools = "..";


        // -------------------------------------------------------------
        // Configuration changes (No change in header)
        // -------------------------------------------------------------

        result = testrun_lib_c_file_content(name, TESTRUN_HEADER, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s.h"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        "#ifndef %s_h"                TESTRUN_LINEEND
        "#define %s_h"                TESTRUN_LINEEND
        TESTRUN_LINEEND
        "#endif /* %s_h */" TESTRUN_LINEEND,
        name, date, name, name, name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);


        // -------------------------------------------------------------
        // Configuration changes (source to header path)
        // -------------------------------------------------------------

        result = testrun_lib_c_file_content(name, TESTRUN_SOURCE, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s.c"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        "#include \".././include/%s.h\""  TESTRUN_LINEEND
        TESTRUN_LINEEND,
        name, date, name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Configuration changes (tests to tools path, tests to src path
        // -------------------------------------------------------------

        result = testrun_lib_c_file_content(name, TESTRUN_TEST, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s_test.c"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "#include \"../../tools/testrun.h\"" TESTRUN_LINEEND
        "#include \"../.././src/%s.c\"" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST HELPER                                                     #HELPER" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST CASES                                                      #CASES" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/**" TESTRUN_LINEEND
        "int test_ ... (){" TESTRUN_LINEEND
        "        testrun(true);" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        return testrun_log_success();" TESTRUN_LINEEND
        "}" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST CLUSTER                                                    #CLUSTER" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "int all_tests() {" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "        testrun_init();" TESTRUN_LINEEND
        "        //testrun_test(test_);" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "        return testrun_counter;" TESTRUN_LINEEND
        "}" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST EXECUTION                                                  #EXEC" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "testrun_run(all_tests);" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        name, date, name);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        date = testrun_string_free(date);
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

       testrun_test(test_testrun_lib_testrun_header_content);
       testrun_test(test_testrun_lib_script_folder_runner_content);
       testrun_test(test_testrun_lib_script_acceptance_tests_content);
       testrun_test(test_testrun_lib_script_unit_tests_content);
       testrun_test(test_testrun_lib_script_coverage_tests_content);
       testrun_test(test_testrun_lib_script_loc_tests_content);
       testrun_test(test_testrun_lib_c_file_content);

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
