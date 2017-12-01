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

        char *result = NULL;

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

/*----------------------------------------------------------------------------*/

int test_testrun_lib_makefile_content(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        testrun_config config = testrun_config_default();

        char *result = testrun_lib_makefile_content(config);
        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);


        snprintf(expect, size,
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
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
"#       File            makefile"                                              TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     This makefile defines project specific parameter."     TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       These parameter are:"                                  TESTRUN_LINEEND
"#                       (1) used compiler and special flags"                   TESTRUN_LINEEND
"#                       (2) name and version"                                  TESTRUN_LINEEND
"#                       (3) installation prefix"                               TESTRUN_LINEEND
"#                       (4) used libraries"                                    TESTRUN_LINEEND
"#                       (5) general makefiles used"                            TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           make"                                                  TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    make & compiler"                                       TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"CC = gcc"                                                                      TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"PROJECT         := %s"                                                         TESTRUN_LINEEND
"VERSION         := 0.0.1"                                                      TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"# project path recalculation (if used included from parent make)"              TESTRUN_LINEEND
"PROJECTMK       := $(abspath $(lastword $(MAKEFILE_LIST)))"                    TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"# base directory for installation"                                             TESTRUN_LINEEND
"PREFIX          := /usr/local"                                                 TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"# LIBS USED (uncommented example includes)"                                    TESTRUN_LINEEND
"#LIBS           += `pkg-config --libs libsystemd`"                             TESTRUN_LINEEND
"#LIBS           += `pkg-config --libs uuid`"                                   TESTRUN_LINEEND
"#LIBS           += `pkg-config --libs openssl`"                                TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"# EXTRA CFLAGS (example)"                                                      TESTRUN_LINEEND
"MODCFLAGS       += -std=gnu11"                                                 TESTRUN_LINEEND
"#MODCFLAGS      += -fopenmp"                                                   TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"#GCC only flags (example)"                                                     TESTRUN_LINEEND
"#MODCFLAGS      += -rdynamic"                                                  TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"#TMP FILE DEFINITION"                                                          TESTRUN_LINEEND
"TESTS_TMP_FILES = $(wildcard /tmp/test_*)"                                     TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"# INCLUDE BASE MAKEFILE"                                                       TESTRUN_LINEEND
"include testrun_makefile.main"                                                 TESTRUN_LINEEND
"include testrun_makefile.test"                                                 TESTRUN_LINEEND
,date,
config.project.name,
date,
config.project.name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        // -------------------------------------------------------------
        // Configuration changes
        // -------------------------------------------------------------

        config.project.name = "test";
             snprintf(expect, size,
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
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
"#       File            makefile"                                              TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     This makefile defines project specific parameter."     TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       These parameter are:"                                  TESTRUN_LINEEND
"#                       (1) used compiler and special flags"                   TESTRUN_LINEEND
"#                       (2) name and version"                                  TESTRUN_LINEEND
"#                       (3) installation prefix"                               TESTRUN_LINEEND
"#                       (4) used libraries"                                    TESTRUN_LINEEND
"#                       (5) general makefiles used"                            TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           make"                                                  TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    make & compiler"                                       TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"CC = gcc"                                                                      TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"PROJECT         := %s"                                                         TESTRUN_LINEEND
"VERSION         := 0.0.1"                                                      TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"# project path recalculation (if used included from parent make)"              TESTRUN_LINEEND
"PROJECTMK       := $(abspath $(lastword $(MAKEFILE_LIST)))"                    TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"# base directory for installation"                                             TESTRUN_LINEEND
"PREFIX          := /usr/local"                                                 TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"# LIBS USED (uncommented example includes)"                                    TESTRUN_LINEEND
"#LIBS           += `pkg-config --libs libsystemd`"                             TESTRUN_LINEEND
"#LIBS           += `pkg-config --libs uuid`"                                   TESTRUN_LINEEND
"#LIBS           += `pkg-config --libs openssl`"                                TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"# EXTRA CFLAGS (example)"                                                      TESTRUN_LINEEND
"MODCFLAGS       += -std=gnu11"                                                 TESTRUN_LINEEND
"#MODCFLAGS      += -fopenmp"                                                   TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"#GCC only flags (example)"                                                     TESTRUN_LINEEND
"#MODCFLAGS      += -rdynamic"                                                  TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"#TMP FILE DEFINITION"                                                          TESTRUN_LINEEND
"TESTS_TMP_FILES = $(wildcard /tmp/test_*)"                                     TESTRUN_LINEEND
""                                                                              TESTRUN_LINEEND
"# INCLUDE BASE MAKEFILE"                                                       TESTRUN_LINEEND
"include testrun_makefile.main"                                                 TESTRUN_LINEEND
"include testrun_makefile.test"                                                 TESTRUN_LINEEND
,date,
config.project.name,
date,
config.project.name);

        testrun(strlen(result) != strlen(expect));
        result = testrun_string_free(result);
        result = testrun_lib_makefile_content(config);
        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_makefile_main_content(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        testrun_config config = testrun_config_default();

        char *result = testrun_lib_makefile_main_content(config);
        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);


        char *all_target = NULL;
        char *install    = NULL;
        char *uninstall  = NULL;

        switch (config.project.type){

                case TESTRUN_LIB:
                        all_target = "all_lib";
                        install    = "install_lib";
                        uninstall  = "uinstall_lib";
                        break;
                case TESTRUN_EXEC:
                        all_target = "all_exec";
                        install    = "install_exec";
                        uninstall  = "uinstall_exec";
                        break;
                case TESTRUN_SERVICE:
                        all_target = "all_exec";
                        install    = "install_service";
                        uninstall  = "uinstall_service";
                        break;
                default:
                        testrun(false);
        }

        char path_src[PATH_MAX];
        char script_install[PATH_MAX];
        char script_uninstall[PATH_MAX];
        char doxygen_config[PATH_MAX];

        bzero(path_src, PATH_MAX);
        bzero(script_install, PATH_MAX);
        bzero(script_uninstall, PATH_MAX);
        bzero(doxygen_config, PATH_MAX);

        testrun(testrun_path_project_to_source(
                path_src, PATH_MAX, &config));

        testrun(testrun_path_script_service_install(
                script_install, PATH_MAX, &config));

        testrun(testrun_path_script_service_uninstall(
                script_uninstall, PATH_MAX, &config));

        testrun(testrun_path_doxygen_config(
                doxygen_config, PATH_MAX, &config));


        snprintf(expect, size,
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
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
"#       File            testrun_makefile.main"                                 TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     Generic makefile for testrun based projects."          TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       Target of this makefile is an independent library"     TESTRUN_LINEEND
"#                       or executable to be installed at either PREFIX/lib"    TESTRUN_LINEEND
"#                       or PREFIX/bin."                                        TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           SHOULD be used included by parent makefile"            TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    testrun (makefile & service scripts), doxygen (if used)"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"# Switch on colors"                                                            TESTRUN_LINEEND
"GCC_COLORS ?= 'gcc colors available, use them!'"                               TESTRUN_LINEEND
"export GCC_COLORS"                                                             TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- PARAMETER DEFINITION --------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"# CFLAGS"                                                                      TESTRUN_LINEEND
"# -g            enable Debugging symbols"                                      TESTRUN_LINEEND
"# -Ox           code optimization"                                             TESTRUN_LINEEND
"# -Wall         enable Warnings"                                               TESTRUN_LINEEND
"# -Wextra       additional Warnings"                                           TESTRUN_LINEEND
TESTRUN_LINEEND
"CFLAGS\t\t= -c -Wall -Wextra -fPIC"                                            TESTRUN_LINEEND
"CFLAGS\t\t+= $(EXTRAHEADER)"                                                   TESTRUN_LINEEND
"CFLAGS\t\t+= $(MODCFLAGS)"                                                     TESTRUN_LINEEND
TESTRUN_LINEEND
"PROJECTPATH\t:= $(abspath $(dir $(PROJECTMK)))"                                TESTRUN_LINEEND
"DIRNAME\t\t:= $(notdir $(patsubst %%/,%%,$(dir $(PROJECTMK))))"                TESTRUN_LINEEND
TESTRUN_LINEEND
"LIBNAME\t\t:= lib$(DIRNAME)"                                                   TESTRUN_LINEEND
"LIBNAMEPC\t:= $(LIBNAME).pc"                                                   TESTRUN_LINEEND
TESTRUN_LINEEND
"INSTALL\t\t:= install"                                                         TESTRUN_LINEEND
TESTRUN_LINEEND
"INCDIR\t\t:= $(PREFIX)/include/$(DIRNAME)"                                     TESTRUN_LINEEND
"LIBDIR\t\t:= $(PREFIX)/lib"                                                    TESTRUN_LINEEND
"EXECDIR\t\t:= $(PREFIX)/bin"                                                   TESTRUN_LINEEND
"PRODIR\t\t:= $(LIBDIR)/$(DIRNAME)"                                             TESTRUN_LINEEND
TESTRUN_LINEEND
"EXECUTABLE\t= bin/$(DIRNAME)"                                                  TESTRUN_LINEEND
TESTRUN_LINEEND
"CFLAGS\t\t+= -Iinclude"                                                        TESTRUN_LINEEND
TESTRUN_LINEEND
"MODMAKE\t\t:= $(patsubst %%,%s/%%/mod.mk,$(MODULES))"                          TESTRUN_LINEEND
TESTRUN_LINEEND
"STATIC\t\t= build/lib$(DIRNAME).a"                                             TESTRUN_LINEEND
"SHARED\t\t= $(patsubst %%.a,%%.so,$(STATIC))"                                  TESTRUN_LINEEND
TESTRUN_LINEEND
"# Source and object files to compile"                                          TESTRUN_LINEEND
"SOURCES\t\t= $(wildcard %s/**/*.c %s/*.c)"                                     TESTRUN_LINEEND
"OBJECTS\t\t= $(patsubst %%.c,%%.o,$(SOURCES))"                                 TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- DEFAULT MAKE RULES ----------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"%%.o : %%.c"                                                                   TESTRUN_LINEEND
"\t@echo \" (CC)    $@\""                                                       TESTRUN_LINEEND
"\t@$(CC) $(CFLAGS) -g -o $@ -c $< $(LIBS)"                                     TESTRUN_LINEEND
TESTRUN_LINEEND
"all:\t\t%s"                                                                    TESTRUN_LINEEND
"install:\t%s"                                                                  TESTRUN_LINEEND
"uninstall:\t%s"                                                                TESTRUN_LINEEND
TESTRUN_LINEEND
"all_lib:\tstart lib tests pkgconfig done"                                      TESTRUN_LINEEND
"all_exec:\tstart lib tests $(EXECUTABLE) done"                                 TESTRUN_LINEEND
"all_service:\tall_exec"                                                        TESTRUN_LINEEND
TESTRUN_LINEEND
"lib:\t\tbuild sources"                                                         TESTRUN_LINEEND
"sources:\tbuild $(STATIC) $(SHARED)"                                           TESTRUN_LINEEND
TESTRUN_LINEEND
"$(STATIC):  $(OBJECTS)"                                                        TESTRUN_LINEEND
"\t@echo \" (AR)    $@ $(OBJECTS)\""                                            TESTRUN_LINEEND
"\t@ar rcs $@ $(OBJECTS)"TESTRUN_LINEEND
"\t@ranlib $@"TESTRUN_LINEEND
TESTRUN_LINEEND
"$(SHARED): $(STATIC) $(OBJECTS)"                                               TESTRUN_LINEEND
"\t@echo \" (CC)    $@ $(OBJECTS)\""                                            TESTRUN_LINEEND
"\t@$(CC) -shared -o $@ $(OBJECTS) $(LIBS)"                                     TESTRUN_LINEEND
TESTRUN_LINEEND
"$(EXECUTABLE): $(OBJECTS)"                                                     TESTRUN_LINEEND
"\t@echo \" (CC)    $@ $(OBJECTS)\""                                            TESTRUN_LINEEND
"\t$(CC) -o $@ $(STATIC) $(LIBS)"                                               TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- BUILD & CLEANUP -----------------------------------------------------" TESTRUN_LINEEND
TESTRUN_LINEEND
"build:"                                                                        TESTRUN_LINEEND
"\t@mkdir -p bin"                                                               TESTRUN_LINEEND
"\t@mkdir -p build"                                                             TESTRUN_LINEEND
"\t@mkdir -p build/test"                                                        TESTRUN_LINEEND
"\t@mkdir -p build/test/omp"                                                    TESTRUN_LINEEND
"\t@mkdir -p build/test/omp/unit"                                               TESTRUN_LINEEND
"\t@mkdir -p build/test/omp/acceptance"                                         TESTRUN_LINEEND
"\t@mkdir -p build/test/unit"                                                   TESTRUN_LINEEND
"\t@mkdir -p build/test/acceptance"                                             TESTRUN_LINEEND
"\t@mkdir -p build/test/log"                                                    TESTRUN_LINEEND
"\t@echo \" (MK)    directories for build\""                                    TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: clean"                                                                 TESTRUN_LINEEND
"clean:"TESTRUN_LINEEND
"\t@echo \" (CLEAN) $(LIBNAME)\""                                               TESTRUN_LINEEND
"\t@rm -rf build bin doxygen/documentation $(OBJECTS) $(TESTS_OBJECTS) \\"      TESTRUN_LINEEND
"\t\t$(LIBNAMEPC) $(TESTS_TMP_FILES)"                                           TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- DOCUMENATION -------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"#NOTE requires doxygen"
".PHONY: documentation"                                                         TESTRUN_LINEEND
"documentation:"                                                                TESTRUN_LINEEND
"\tdoxygen %s"                                                                  TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: pkgconfig"                                                             TESTRUN_LINEEND
"pkgconfig:"                                                                    TESTRUN_LINEEND
"\t@echo 'prefix='$(PREFIX)                     >  $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'exec_prefix=$${prefix}'               >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'libdir=$${prefix}/lib'                >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'includedir=$${prefix}/include'        >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo ''                                     >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Name: '$(LIBNAME)                     >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Description: '                        >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Version: '$(VERSION)                  >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'URL: '$(PROJECT_URL)                  >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Libs: -L$${libdir} -l'$(DIRNAME)      >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Cflags: -I$${includedir}'             >> $(LIBNAMEPC)"                TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- INSTALLATION -------------------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"# Installation as a library ------------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"install_lib: $(SHARED) $(STATIC)"                                              TESTRUN_LINEEND
"\t@echo \" (OK)    installed $(LIBNAME) to $(LIBDIR)\""                        TESTRUN_LINEEND
"\t@mkdir -p $(PRODIR)"                                                         TESTRUN_LINEEND
"\t@mkdir -p $(LIBDIR)/pkgconfig"                                               TESTRUN_LINEEND
"\t@mkdir -p $(INCDIR)"                                                         TESTRUN_LINEEND
"\t@$(INSTALL) -m 0644 -t $(INCDIR) $(shell find include -name \"*.h\")"        TESTRUN_LINEEND
"\t@$(INSTALL) -m 0755 $(SHARED) $(PRODIR)"                                     TESTRUN_LINEEND
"\t@$(INSTALL) -m 0755 $(STATIC) $(PRODIR)"                                     TESTRUN_LINEEND
"\t@$(INSTALL) -m 0644 $(LIBNAMEPC) $(LIBDIR)/pkgconfig"                        TESTRUN_LINEEND
"\t@ldconfig $(PRODIR)"                                                         TESTRUN_LINEEND
TESTRUN_LINEEND
"uninstall_lib:"                                                                TESTRUN_LINEEND
"\t@echo \" (OK)    uninstalled $(LIBNAME) from $(LIBDIR)\""                    TESTRUN_LINEEND
"\t@rm -rf $(INCDIR)"                                                           TESTRUN_LINEEND
"\t@rm -rf $(PRODIR)"                                                           TESTRUN_LINEEND
"\t@rm -rf $(PRODIR)/$(LIBNAME).a"                                              TESTRUN_LINEEND
"\t@rm -rf $(PRODIR)/$(LIBNAME).so"                                             TESTRUN_LINEEND
"\t@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)"                                    TESTRUN_LINEEND
TESTRUN_LINEEND
"# Installation as an executable --------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"install_exec: $(SHARED) $(STATIC)"                                             TESTRUN_LINEEND
"\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\""                       TESTRUN_LINEEND
"\t@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)"                               TESTRUN_LINEEND
TESTRUN_LINEEND
"uninstall_exec:"                                                               TESTRUN_LINEEND
"\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\""                   TESTRUN_LINEEND
"\t@rm -rf $(EXECDIR)/$(DIRNAME)"                                               TESTRUN_LINEEND
TESTRUN_LINEEND
"# Installation as a service (outsourced to script)--------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"install_service: $(EXECUTABLE)"                                                TESTRUN_LINEEND
"\t%s"                                                                          TESTRUN_LINEEND
TESTRUN_LINEEND
"uninstall_service:"                                                            TESTRUN_LINEEND
"\t%s"                                                                          TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- INFORMATION PRINTING -----------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"# print out a variable of the make file (e.g. \"make print-PROJECTPATH\")"     TESTRUN_LINEEND
".PHONY: print"                                                                 TESTRUN_LINEEND
"print-%%  : ; @echo $* = $($*)"                                                TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: start"                                                                 TESTRUN_LINEEND
"start:"                                                                        TESTRUN_LINEEND
"\t@echo \"\\n (HINT)    $(PROJECT) \\t\\t ==> running make\\n\""               TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: done"                                                                  TESTRUN_LINEEND
"done:"                                                                         TESTRUN_LINEEND
"\t@echo"                                                                       TESTRUN_LINEEND
"\t@echo \" (DONE)  make $(PROJECT)\""                                          TESTRUN_LINEEND
"\t@echo \" (HINT)  with unit testing      ==> 'make tested'\""                 TESTRUN_LINEEND
"\t@echo \" (HINT)  perform installation   ==> 'sudo make install\\n\""         TESTRUN_LINEEND
"\t@echo \" (HINT)  generate documentation ==> 'make documentation\\n\""        TESTRUN_LINEEND
,date,
config.project.name,
date,
path_src, path_src, path_src,
all_target,
install,
uninstall,
doxygen_config, script_install, script_uninstall);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        // -------------------------------------------------------------
        // Configuration changes
        // -------------------------------------------------------------

        config.project.type = TESTRUN_SERVICE;
        all_target = "all_exec";
        install    = "install_service";
        uninstall  = "uinstall_service";


        snprintf(expect, size,
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
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
"#       File            testrun_makefile.main"                                 TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     Generic makefile for testrun based projects."          TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       Target of this makefile is an independent library"     TESTRUN_LINEEND
"#                       or executable to be installed at either PREFIX/lib"    TESTRUN_LINEEND
"#                       or PREFIX/bin."                                        TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           SHOULD be used included by parent makefile"            TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    testrun (makefile & service scripts), doxygen (if used)"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"# Switch on colors"                                                            TESTRUN_LINEEND
"GCC_COLORS ?= 'gcc colors available, use them!'"                               TESTRUN_LINEEND
"export GCC_COLORS"                                                             TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- PARAMETER DEFINITION --------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"# CFLAGS"                                                                      TESTRUN_LINEEND
"# -g            enable Debugging symbols"                                      TESTRUN_LINEEND
"# -Ox           code optimization"                                             TESTRUN_LINEEND
"# -Wall         enable Warnings"                                               TESTRUN_LINEEND
"# -Wextra       additional Warnings"                                           TESTRUN_LINEEND
TESTRUN_LINEEND
"CFLAGS\t\t= -c -Wall -Wextra -fPIC"                                            TESTRUN_LINEEND
"CFLAGS\t\t+= $(EXTRAHEADER)"                                                   TESTRUN_LINEEND
"CFLAGS\t\t+= $(MODCFLAGS)"                                                     TESTRUN_LINEEND
TESTRUN_LINEEND
"PROJECTPATH\t:= $(abspath $(dir $(PROJECTMK)))"                                TESTRUN_LINEEND
"DIRNAME\t\t:= $(notdir $(patsubst %%/,%%,$(dir $(PROJECTMK))))"                TESTRUN_LINEEND
TESTRUN_LINEEND
"LIBNAME\t\t:= lib$(DIRNAME)"                                                   TESTRUN_LINEEND
"LIBNAMEPC\t:= $(LIBNAME).pc"                                                   TESTRUN_LINEEND
TESTRUN_LINEEND
"INSTALL\t\t:= install"                                                         TESTRUN_LINEEND
TESTRUN_LINEEND
"INCDIR\t\t:= $(PREFIX)/include/$(DIRNAME)"                                     TESTRUN_LINEEND
"LIBDIR\t\t:= $(PREFIX)/lib"                                                    TESTRUN_LINEEND
"EXECDIR\t\t:= $(PREFIX)/bin"                                                   TESTRUN_LINEEND
"PRODIR\t\t:= $(LIBDIR)/$(DIRNAME)"                                             TESTRUN_LINEEND
TESTRUN_LINEEND
"EXECUTABLE\t= bin/$(DIRNAME)"                                                  TESTRUN_LINEEND
TESTRUN_LINEEND
"CFLAGS\t\t+= -Iinclude"                                                        TESTRUN_LINEEND
TESTRUN_LINEEND
"MODMAKE\t\t:= $(patsubst %%,%s/%%/mod.mk,$(MODULES))"                          TESTRUN_LINEEND
TESTRUN_LINEEND
"STATIC\t\t= build/lib$(DIRNAME).a"                                             TESTRUN_LINEEND
"SHARED\t\t= $(patsubst %%.a,%%.so,$(STATIC))"                                  TESTRUN_LINEEND
TESTRUN_LINEEND
"# Source and object files to compile"                                          TESTRUN_LINEEND
"SOURCES\t\t= $(wildcard %s/**/*.c %s/*.c)"                                     TESTRUN_LINEEND
"OBJECTS\t\t= $(patsubst %%.c,%%.o,$(SOURCES))"                                 TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- DEFAULT MAKE RULES ----------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"%%.o : %%.c"                                                                   TESTRUN_LINEEND
"\t@echo \" (CC)    $@\""                                                       TESTRUN_LINEEND
"\t@$(CC) $(CFLAGS) -g -o $@ -c $< $(LIBS)"                                     TESTRUN_LINEEND
TESTRUN_LINEEND
"all:\t\t%s"                                                                    TESTRUN_LINEEND
"install:\t%s"                                                                  TESTRUN_LINEEND
"uninstall:\t%s"                                                                TESTRUN_LINEEND
TESTRUN_LINEEND
"all_lib:\tstart lib tests pkgconfig done"                                      TESTRUN_LINEEND
"all_exec:\tstart lib tests $(EXECUTABLE) done"                                 TESTRUN_LINEEND
"all_service:\tall_exec"                                                        TESTRUN_LINEEND
TESTRUN_LINEEND
"lib:\t\tbuild sources"                                                         TESTRUN_LINEEND
"sources:\tbuild $(STATIC) $(SHARED)"                                           TESTRUN_LINEEND
TESTRUN_LINEEND
"$(STATIC):  $(OBJECTS)"                                                        TESTRUN_LINEEND
"\t@echo \" (AR)    $@ $(OBJECTS)\""                                            TESTRUN_LINEEND
"\t@ar rcs $@ $(OBJECTS)"TESTRUN_LINEEND
"\t@ranlib $@"TESTRUN_LINEEND
TESTRUN_LINEEND
"$(SHARED): $(STATIC) $(OBJECTS)"                                               TESTRUN_LINEEND
"\t@echo \" (CC)    $@ $(OBJECTS)\""                                            TESTRUN_LINEEND
"\t@$(CC) -shared -o $@ $(OBJECTS) $(LIBS)"                                     TESTRUN_LINEEND
TESTRUN_LINEEND
"$(EXECUTABLE): $(OBJECTS)"                                                     TESTRUN_LINEEND
"\t@echo \" (CC)    $@ $(OBJECTS)\""                                            TESTRUN_LINEEND
"\t$(CC) -o $@ $(STATIC) $(LIBS)"                                               TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- BUILD & CLEANUP -----------------------------------------------------" TESTRUN_LINEEND
TESTRUN_LINEEND
"build:"                                                                        TESTRUN_LINEEND
"\t@mkdir -p bin"                                                               TESTRUN_LINEEND
"\t@mkdir -p build"                                                             TESTRUN_LINEEND
"\t@mkdir -p build/test"                                                        TESTRUN_LINEEND
"\t@mkdir -p build/test/omp"                                                    TESTRUN_LINEEND
"\t@mkdir -p build/test/omp/unit"                                               TESTRUN_LINEEND
"\t@mkdir -p build/test/omp/acceptance"                                         TESTRUN_LINEEND
"\t@mkdir -p build/test/unit"                                                   TESTRUN_LINEEND
"\t@mkdir -p build/test/acceptance"                                             TESTRUN_LINEEND
"\t@mkdir -p build/test/log"                                                    TESTRUN_LINEEND
"\t@echo \" (MK)    directories for build\""                                    TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: clean"                                                                 TESTRUN_LINEEND
"clean:"TESTRUN_LINEEND
"\t@echo \" (CLEAN) $(LIBNAME)\""                                               TESTRUN_LINEEND
"\t@rm -rf build bin doxygen/documentation $(OBJECTS) $(TESTS_OBJECTS) \\"      TESTRUN_LINEEND
"\t\t$(LIBNAMEPC) $(TESTS_TMP_FILES)"                                           TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- DOCUMENATION -------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"#NOTE requires doxygen"
".PHONY: documentation"                                                         TESTRUN_LINEEND
"documentation:"                                                                TESTRUN_LINEEND
"\tdoxygen %s"                                                                  TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: pkgconfig"                                                             TESTRUN_LINEEND
"pkgconfig:"                                                                    TESTRUN_LINEEND
"\t@echo 'prefix='$(PREFIX)                     >  $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'exec_prefix=$${prefix}'               >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'libdir=$${prefix}/lib'                >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'includedir=$${prefix}/include'        >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo ''                                     >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Name: '$(LIBNAME)                     >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Description: '                        >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Version: '$(VERSION)                  >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'URL: '$(PROJECT_URL)                  >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Libs: -L$${libdir} -l'$(DIRNAME)      >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Cflags: -I$${includedir}'             >> $(LIBNAMEPC)"                TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- INSTALLATION -------------------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"# Installation as a library ------------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"install_lib: $(SHARED) $(STATIC)"                                              TESTRUN_LINEEND
"\t@echo \" (OK)    installed $(LIBNAME) to $(LIBDIR)\""                        TESTRUN_LINEEND
"\t@mkdir -p $(PRODIR)"                                                         TESTRUN_LINEEND
"\t@mkdir -p $(LIBDIR)/pkgconfig"                                               TESTRUN_LINEEND
"\t@mkdir -p $(INCDIR)"                                                         TESTRUN_LINEEND
"\t@$(INSTALL) -m 0644 -t $(INCDIR) $(shell find include -name \"*.h\")"        TESTRUN_LINEEND
"\t@$(INSTALL) -m 0755 $(SHARED) $(PRODIR)"                                     TESTRUN_LINEEND
"\t@$(INSTALL) -m 0755 $(STATIC) $(PRODIR)"                                     TESTRUN_LINEEND
"\t@$(INSTALL) -m 0644 $(LIBNAMEPC) $(LIBDIR)/pkgconfig"                        TESTRUN_LINEEND
"\t@ldconfig $(PRODIR)"                                                         TESTRUN_LINEEND
TESTRUN_LINEEND
"uninstall_lib:"                                                                TESTRUN_LINEEND
"\t@echo \" (OK)    uninstalled $(LIBNAME) from $(LIBDIR)\""                    TESTRUN_LINEEND
"\t@rm -rf $(INCDIR)"                                                           TESTRUN_LINEEND
"\t@rm -rf $(PRODIR)"                                                           TESTRUN_LINEEND
"\t@rm -rf $(PRODIR)/$(LIBNAME).a"                                              TESTRUN_LINEEND
"\t@rm -rf $(PRODIR)/$(LIBNAME).so"                                             TESTRUN_LINEEND
"\t@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)"                                    TESTRUN_LINEEND
TESTRUN_LINEEND
"# Installation as an executable --------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"install_exec: $(SHARED) $(STATIC)"                                             TESTRUN_LINEEND
"\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\""                       TESTRUN_LINEEND
"\t@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)"                               TESTRUN_LINEEND
TESTRUN_LINEEND
"uninstall_exec:"                                                               TESTRUN_LINEEND
"\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\""                   TESTRUN_LINEEND
"\t@rm -rf $(EXECDIR)/$(DIRNAME)"                                               TESTRUN_LINEEND
TESTRUN_LINEEND
"# Installation as a service (outsourced to script)--------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"install_service: $(EXECUTABLE)"                                                TESTRUN_LINEEND
"\t%s"                                                                          TESTRUN_LINEEND
TESTRUN_LINEEND
"uninstall_service:"                                                            TESTRUN_LINEEND
"\t%s"                                                                          TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- INFORMATION PRINTING -----------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"# print out a variable of the make file (e.g. \"make print-PROJECTPATH\")"     TESTRUN_LINEEND
".PHONY: print"                                                                 TESTRUN_LINEEND
"print-%%  : ; @echo $* = $($*)"                                                TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: start"                                                                 TESTRUN_LINEEND
"start:"                                                                        TESTRUN_LINEEND
"\t@echo \"\\n (HINT)    $(PROJECT) \\t\\t ==> running make\\n\""               TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: done"                                                                  TESTRUN_LINEEND
"done:"                                                                         TESTRUN_LINEEND
"\t@echo"                                                                       TESTRUN_LINEEND
"\t@echo \" (DONE)  make $(PROJECT)\""                                          TESTRUN_LINEEND
"\t@echo \" (HINT)  with unit testing      ==> 'make tested'\""                 TESTRUN_LINEEND
"\t@echo \" (HINT)  perform installation   ==> 'sudo make install\\n\""         TESTRUN_LINEEND
"\t@echo \" (HINT)  generate documentation ==> 'make documentation\\n\""        TESTRUN_LINEEND
,date,
config.project.name,
date,
path_src, path_src, path_src,
all_target,
install,
uninstall,
doxygen_config, script_install, script_uninstall);

        testrun(strlen(result) != strlen(expect));
        result = testrun_string_free(result);
        result = testrun_lib_makefile_main_content(config);
        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        config.project.type = TESTRUN_EXEC;
        all_target = "all_exec";
        install    = "install_exec";
        uninstall  = "uinstall_exec";


        config.project.path.to_source = "../../..";
        config.project.path.source = "123";
        config.project.doxygen.filename = "AAA";

        config.project.service.install_script = "XXX";
        config.project.service.uninstall_script = "YYY";

        testrun(testrun_path_project_to_source(
                path_src, PATH_MAX, &config));

        testrun(testrun_path_script_service_install(
                script_install, PATH_MAX, &config));

        testrun(testrun_path_script_service_uninstall(
                script_uninstall, PATH_MAX, &config));

        testrun(testrun_path_doxygen_config(
                doxygen_config, PATH_MAX, &config));



        snprintf(expect, size,
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
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
"#       File            testrun_makefile.main"                                 TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     Generic makefile for testrun based projects."          TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       Target of this makefile is an independent library"     TESTRUN_LINEEND
"#                       or executable to be installed at either PREFIX/lib"    TESTRUN_LINEEND
"#                       or PREFIX/bin."                                        TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           SHOULD be used included by parent makefile"            TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    testrun (makefile & service scripts), doxygen (if used)"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"# Switch on colors"                                                            TESTRUN_LINEEND
"GCC_COLORS ?= 'gcc colors available, use them!'"                               TESTRUN_LINEEND
"export GCC_COLORS"                                                             TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- PARAMETER DEFINITION --------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"# CFLAGS"                                                                      TESTRUN_LINEEND
"# -g            enable Debugging symbols"                                      TESTRUN_LINEEND
"# -Ox           code optimization"                                             TESTRUN_LINEEND
"# -Wall         enable Warnings"                                               TESTRUN_LINEEND
"# -Wextra       additional Warnings"                                           TESTRUN_LINEEND
TESTRUN_LINEEND
"CFLAGS\t\t= -c -Wall -Wextra -fPIC"                                            TESTRUN_LINEEND
"CFLAGS\t\t+= $(EXTRAHEADER)"                                                   TESTRUN_LINEEND
"CFLAGS\t\t+= $(MODCFLAGS)"                                                     TESTRUN_LINEEND
TESTRUN_LINEEND
"PROJECTPATH\t:= $(abspath $(dir $(PROJECTMK)))"                                TESTRUN_LINEEND
"DIRNAME\t\t:= $(notdir $(patsubst %%/,%%,$(dir $(PROJECTMK))))"                TESTRUN_LINEEND
TESTRUN_LINEEND
"LIBNAME\t\t:= lib$(DIRNAME)"                                                   TESTRUN_LINEEND
"LIBNAMEPC\t:= $(LIBNAME).pc"                                                   TESTRUN_LINEEND
TESTRUN_LINEEND
"INSTALL\t\t:= install"                                                         TESTRUN_LINEEND
TESTRUN_LINEEND
"INCDIR\t\t:= $(PREFIX)/include/$(DIRNAME)"                                     TESTRUN_LINEEND
"LIBDIR\t\t:= $(PREFIX)/lib"                                                    TESTRUN_LINEEND
"EXECDIR\t\t:= $(PREFIX)/bin"                                                   TESTRUN_LINEEND
"PRODIR\t\t:= $(LIBDIR)/$(DIRNAME)"                                             TESTRUN_LINEEND
TESTRUN_LINEEND
"EXECUTABLE\t= bin/$(DIRNAME)"                                                  TESTRUN_LINEEND
TESTRUN_LINEEND
"CFLAGS\t\t+= -Iinclude"                                                        TESTRUN_LINEEND
TESTRUN_LINEEND
"MODMAKE\t\t:= $(patsubst %%,../../../123/%%/mod.mk,$(MODULES))"                 TESTRUN_LINEEND
TESTRUN_LINEEND
"STATIC\t\t= build/lib$(DIRNAME).a"                                             TESTRUN_LINEEND
"SHARED\t\t= $(patsubst %%.a,%%.so,$(STATIC))"                                  TESTRUN_LINEEND
TESTRUN_LINEEND
"# Source and object files to compile"                                          TESTRUN_LINEEND
"SOURCES\t\t= $(wildcard ../../../123/**/*.c ../../../123/*.c)"                   TESTRUN_LINEEND
"OBJECTS\t\t= $(patsubst %%.c,%%.o,$(SOURCES))"                                 TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- DEFAULT MAKE RULES ----------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"%%.o : %%.c"                                                                   TESTRUN_LINEEND
"\t@echo \" (CC)    $@\""                                                       TESTRUN_LINEEND
"\t@$(CC) $(CFLAGS) -g -o $@ -c $< $(LIBS)"                                     TESTRUN_LINEEND
TESTRUN_LINEEND
"all:\t\t%s"                                                                    TESTRUN_LINEEND
"install:\t%s"                                                                  TESTRUN_LINEEND
"uninstall:\t%s"                                                                TESTRUN_LINEEND
TESTRUN_LINEEND
"all_lib:\tstart lib tests pkgconfig done"                                      TESTRUN_LINEEND
"all_exec:\tstart lib tests $(EXECUTABLE) done"                                 TESTRUN_LINEEND
"all_service:\tall_exec"                                                        TESTRUN_LINEEND
TESTRUN_LINEEND
"lib:\t\tbuild sources"                                                         TESTRUN_LINEEND
"sources:\tbuild $(STATIC) $(SHARED)"                                           TESTRUN_LINEEND
TESTRUN_LINEEND
"$(STATIC):  $(OBJECTS)"                                                        TESTRUN_LINEEND
"\t@echo \" (AR)    $@ $(OBJECTS)\""                                            TESTRUN_LINEEND
"\t@ar rcs $@ $(OBJECTS)"TESTRUN_LINEEND
"\t@ranlib $@"TESTRUN_LINEEND
TESTRUN_LINEEND
"$(SHARED): $(STATIC) $(OBJECTS)"                                               TESTRUN_LINEEND
"\t@echo \" (CC)    $@ $(OBJECTS)\""                                            TESTRUN_LINEEND
"\t@$(CC) -shared -o $@ $(OBJECTS) $(LIBS)"                                     TESTRUN_LINEEND
TESTRUN_LINEEND
"$(EXECUTABLE): $(OBJECTS)"                                                     TESTRUN_LINEEND
"\t@echo \" (CC)    $@ $(OBJECTS)\""                                            TESTRUN_LINEEND
"\t$(CC) -o $@ $(STATIC) $(LIBS)"                                               TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- BUILD & CLEANUP -----------------------------------------------------" TESTRUN_LINEEND
TESTRUN_LINEEND
"build:"                                                                        TESTRUN_LINEEND
"\t@mkdir -p bin"                                                               TESTRUN_LINEEND
"\t@mkdir -p build"                                                             TESTRUN_LINEEND
"\t@mkdir -p build/test"                                                        TESTRUN_LINEEND
"\t@mkdir -p build/test/omp"                                                    TESTRUN_LINEEND
"\t@mkdir -p build/test/omp/unit"                                               TESTRUN_LINEEND
"\t@mkdir -p build/test/omp/acceptance"                                         TESTRUN_LINEEND
"\t@mkdir -p build/test/unit"                                                   TESTRUN_LINEEND
"\t@mkdir -p build/test/acceptance"                                             TESTRUN_LINEEND
"\t@mkdir -p build/test/log"                                                    TESTRUN_LINEEND
"\t@echo \" (MK)    directories for build\""                                    TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: clean"                                                                 TESTRUN_LINEEND
"clean:"TESTRUN_LINEEND
"\t@echo \" (CLEAN) $(LIBNAME)\""                                               TESTRUN_LINEEND
"\t@rm -rf build bin doxygen/documentation $(OBJECTS) $(TESTS_OBJECTS) \\"      TESTRUN_LINEEND
"\t\t$(LIBNAMEPC) $(TESTS_TMP_FILES)"                                           TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- DOCUMENATION -------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"#NOTE requires doxygen"
".PHONY: documentation"                                                         TESTRUN_LINEEND
"documentation:"                                                                TESTRUN_LINEEND
"\tdoxygen ./doxygen/AAA"                                                       TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: pkgconfig"                                                             TESTRUN_LINEEND
"pkgconfig:"                                                                    TESTRUN_LINEEND
"\t@echo 'prefix='$(PREFIX)                     >  $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'exec_prefix=$${prefix}'               >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'libdir=$${prefix}/lib'                >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'includedir=$${prefix}/include'        >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo ''                                     >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Name: '$(LIBNAME)                     >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Description: '                        >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Version: '$(VERSION)                  >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'URL: '$(PROJECT_URL)                  >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Libs: -L$${libdir} -l'$(DIRNAME)      >> $(LIBNAMEPC)"                TESTRUN_LINEEND
"\t@echo 'Cflags: -I$${includedir}'             >> $(LIBNAMEPC)"                TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- INSTALLATION -------------------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"# Installation as a library ------------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"install_lib: $(SHARED) $(STATIC)"                                              TESTRUN_LINEEND
"\t@echo \" (OK)    installed $(LIBNAME) to $(LIBDIR)\""                        TESTRUN_LINEEND
"\t@mkdir -p $(PRODIR)"                                                         TESTRUN_LINEEND
"\t@mkdir -p $(LIBDIR)/pkgconfig"                                               TESTRUN_LINEEND
"\t@mkdir -p $(INCDIR)"                                                         TESTRUN_LINEEND
"\t@$(INSTALL) -m 0644 -t $(INCDIR) $(shell find include -name \"*.h\")"        TESTRUN_LINEEND
"\t@$(INSTALL) -m 0755 $(SHARED) $(PRODIR)"                                     TESTRUN_LINEEND
"\t@$(INSTALL) -m 0755 $(STATIC) $(PRODIR)"                                     TESTRUN_LINEEND
"\t@$(INSTALL) -m 0644 $(LIBNAMEPC) $(LIBDIR)/pkgconfig"                        TESTRUN_LINEEND
"\t@ldconfig $(PRODIR)"                                                         TESTRUN_LINEEND
TESTRUN_LINEEND
"uninstall_lib:"                                                                TESTRUN_LINEEND
"\t@echo \" (OK)    uninstalled $(LIBNAME) from $(LIBDIR)\""                    TESTRUN_LINEEND
"\t@rm -rf $(INCDIR)"                                                           TESTRUN_LINEEND
"\t@rm -rf $(PRODIR)"                                                           TESTRUN_LINEEND
"\t@rm -rf $(PRODIR)/$(LIBNAME).a"                                              TESTRUN_LINEEND
"\t@rm -rf $(PRODIR)/$(LIBNAME).so"                                             TESTRUN_LINEEND
"\t@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)"                                    TESTRUN_LINEEND
TESTRUN_LINEEND
"# Installation as an executable --------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"install_exec: $(SHARED) $(STATIC)"                                             TESTRUN_LINEEND
"\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\""                       TESTRUN_LINEEND
"\t@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)"                               TESTRUN_LINEEND
TESTRUN_LINEEND
"uninstall_exec:"                                                               TESTRUN_LINEEND
"\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\""                   TESTRUN_LINEEND
"\t@rm -rf $(EXECDIR)/$(DIRNAME)"                                               TESTRUN_LINEEND
TESTRUN_LINEEND
"# Installation as a service (outsourced to script)--------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"install_service: $(EXECUTABLE)"                                                TESTRUN_LINEEND
"\t./config/install/XXX"                                                        TESTRUN_LINEEND
TESTRUN_LINEEND
"uninstall_service:"                                                            TESTRUN_LINEEND
"\t./config/install/YYY"                                                        TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# ----- INFORMATION PRINTING -----------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"# print out a variable of the make file (e.g. \"make print-PROJECTPATH\")"     TESTRUN_LINEEND
".PHONY: print"                                                                 TESTRUN_LINEEND
"print-%%  : ; @echo $* = $($*)"                                                TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: start"                                                                 TESTRUN_LINEEND
"start:"                                                                        TESTRUN_LINEEND
"\t@echo \"\\n (HINT)    $(PROJECT) \\t\\t ==> running make\\n\""               TESTRUN_LINEEND
TESTRUN_LINEEND
".PHONY: done"                                                                  TESTRUN_LINEEND
"done:"                                                                         TESTRUN_LINEEND
"\t@echo"                                                                       TESTRUN_LINEEND
"\t@echo \" (DONE)  make $(PROJECT)\""                                          TESTRUN_LINEEND
"\t@echo \" (HINT)  with unit testing      ==> 'make tested'\""                 TESTRUN_LINEEND
"\t@echo \" (HINT)  perform installation   ==> 'sudo make install\\n\""         TESTRUN_LINEEND
"\t@echo \" (HINT)  generate documentation ==> 'make documentation\\n\""        TESTRUN_LINEEND
,date,
config.project.name,
date,
all_target,
install,
uninstall);

        testrun(strlen(result) != strlen(expect));
        result = testrun_string_free(result);
        result = testrun_lib_makefile_main_content(config);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_makefile_test_content(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        testrun_config config = testrun_config_default();

        char *result = testrun_lib_makefile_test_content(config);
        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);

        char script_unit[PATH_MAX];
        char script_acceptance[PATH_MAX];
        char script_coverage[PATH_MAX];
        char script_loc[PATH_MAX];

        bzero(script_unit,      PATH_MAX);
        bzero(script_acceptance,PATH_MAX);
        bzero(script_coverage,  PATH_MAX);
        bzero(script_loc,       PATH_MAX);

        // load script pathes out of config
        testrun(testrun_path_script_unit_tests(
                script_unit, PATH_MAX, &config));

        testrun(testrun_path_script_acceptance_tests(
                script_acceptance, PATH_MAX, &config));

        testrun(testrun_path_script_coverage_tests(
                script_coverage, PATH_MAX, &config));

        testrun(testrun_path_script_loc_tests(
                script_loc, PATH_MAX, &config));

        snprintf(expect, size,
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
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
"#       File            testrun_makefile.test"                                 TESTRUN_LINEEND
"#       Authors         Markus Toepfer"                                        TESTRUN_LINEEND
"#       Date            %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Project         %s"                                                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Description     Makefile extension for the testrun enabled projects."  TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       The following part contains all required functionality"TESTRUN_LINEEND
"#                       to use the testrun tools via a makefile. It may be"    TESTRUN_LINEEND
"#                       seen as a makefile integrated testrunner framework."   TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       in particular:"                                        TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                            \"make clean && make tested\""                    TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       may be used to build all sources as well as tests from"TESTRUN_LINEEND
"#                       scratch and perform an integrated testrun over all after"TESTRUN_LINEEND
"#                       compilation."                                          TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       Following folder structure is required"                TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                            tests MUST be located at tests/"                  TESTRUN_LINEEND
"#                            build MUST be located at build/"                  TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#                       ALL TEST SCRIPTS MAY BE EXCHANGED WITH CUSTOM RUNNERS" TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Usage           SHOULD be used included by parent makefile"            TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Dependencies    testrun scripts, lib for OpenMP (if used for testing)"TESTRUN_LINEEND
"#"                                                                             TESTRUN_LINEEND
"#       Last changed    %s"                                                    TESTRUN_LINEEND
"#       ------------------------------------------------------------------------"TESTRUN_LINEEND
TESTRUN_LINEEND
"# (1) TESTRUN SOURCE DEFINITION --------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"TESTS_OMP_SRC\t= $(wildcard tests/**/*%s.c tests/*%s.c)"                       TESTRUN_LINEEND
"TESTS_OMP_TARGET= $(patsubst %%.c,%%%s,$(TESTS_OMP_SRC))"                      TESTRUN_LINEEND
"TESTS_SOURCES   = $(wildcard tests/**/*%s.c tests/*%s.c)"                      TESTRUN_LINEEND
"TESTS_TARGET    = $(patsubst %%.c,%%%s,$(TESTS_SOURCES))"                      TESTRUN_LINEEND
TESTRUN_LINEEND
"# (2) TESTRUN MAKE RULES ---------------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"# ALL IN ONE CALL (compile source, test and run test)"                         TESTRUN_LINEEND
"tested:\ttests_build all testrun done"                                         TESTRUN_LINEEND
TESTRUN_LINEEND
"# ensure the build folder are available"                                       TESTRUN_LINEEND
"tests_build:"                                                                  TESTRUN_LINEEND
"\t@mkdir -p build/test"                                                        TESTRUN_LINEEND
"\t@mkdir -p build/test/omp"                                                    TESTRUN_LINEEND
"\t@mkdir -p build/test/omp/unit"                                               TESTRUN_LINEEND
"\t@mkdir -p build/test/omp/acceptance"                                         TESTRUN_LINEEND
"\t@mkdir -p build/test/unit"                                                   TESTRUN_LINEEND
"\t@mkdir -p build/test/acceptance"                                             TESTRUN_LINEEND
"\t@mkdir -p build/test/log"                                                    TESTRUN_LINEEND
"\t@echo \" (MK)    directories for test under build\""                         TESTRUN_LINEEND
TESTRUN_LINEEND
"# compile tests only"                                                          TESTRUN_LINEEND
"tests:\ttests-resources $(TESTS_TARGET)"                                       TESTRUN_LINEEND
TESTRUN_LINEEND
"# compile omp_tests only"                                                      TESTRUN_LINEEND
"tests_omp:\ttests-resources $(TESTS_OMP_TARGET)"                               TESTRUN_LINEEND
TESTRUN_LINEEND
"# copy test resources to build"                                                TESTRUN_LINEEND
"tests-resources:"                                                              TESTRUN_LINEEND
"\t@echo \" (CP)    tests/resources\""                                          TESTRUN_LINEEND
"\t@cp -r tests/resources build/test"                                           TESTRUN_LINEEND
TESTRUN_LINEEND
"# build all executable tests under build/tests"                                TESTRUN_LINEEND
"$(TESTS_TARGET): $(TESTS_SOURCES)"                                             TESTRUN_LINEEND
"\t@echo \" (CC)    $(@)\""                                                     TESTRUN_LINEEND
"\t@$(CC) $(MODCFLAGS) $(patsubst %%%s,%%.c,$(@)) \\"                           TESTRUN_LINEEND
"\t\t-ldl $(STATIC) -Wl,-rpath=$(RPATH) \\"                                     TESTRUN_LINEEND
"\t\t-g -o $(subst tests/,build/test/,$(@)) $(LIBS)"                            TESTRUN_LINEEND
TESTRUN_LINEEND
"# build all parallel executable tests under build/tests"                       TESTRUN_LINEEND
"$(TESTS_OMP_TARGET): $(TESTS_OMP_SRC)"                                         TESTRUN_LINEEND
"\t@echo \" (CC)    $(@)\""                                                     TESTRUN_LINEEND
"\t@$(CC) $(MODCFLAGS) -fopenmp $(patsubst %%%s,%%.c,$(@)) \\"                  TESTRUN_LINEEND
"\t\t-ldl $(STATIC) -Wl,-rpath=$(RPATH) \\"                                     TESTRUN_LINEEND
"\t\t-g -o $(subst tests/,build/test/omp/,$(@)) $(LIBS)"                        TESTRUN_LINEEND
TESTRUN_LINEEND
"# build a specific executable test (testname) under build/tests"               TESTRUN_LINEEND
"# NOTE: May be usefull for module development in large projects"               TESTRUN_LINEEND
"test:"                                                                         TESTRUN_LINEEND
"\t@echo \" (CC)    $(testname)\""                                              TESTRUN_LINEEND
"\t@$(CC) $(MODCFLAGS) $(patsubst build/test/%%%s, \\"                          TESTRUN_LINEEND
"\t\ttests/%%.c,$(testname)) -ldl $(STATIC) -Wl,-rpath=$(RPATH) -g -o\\"        TESTRUN_LINEEND
"\t\t$(patsubst tests/%%.c,build/test/%%%s,$(testname)) $(LIBS)"                TESTRUN_LINEEND
TESTRUN_LINEEND
TESTRUN_LINEEND
"# (3) TESTRUN runners ------------------------------------------------------"  TESTRUN_LINEEND
TESTRUN_LINEEND
"# ACCEPTANCE TEST script invocation"                                           TESTRUN_LINEEND
".PHONY: testrun-acceptance"                                                    TESTRUN_LINEEND
"testrun-acceptance:"                                                           TESTRUN_LINEEND
"\tsh %s"                                                                       TESTRUN_LINEEND
TESTRUN_LINEEND
"# UNIT TEST script invocation"                                                 TESTRUN_LINEEND
".PHONY: testrun-unit"                                                          TESTRUN_LINEEND
"testrun-unit:"                                                                 TESTRUN_LINEEND
"\tsh %s"                                                                       TESTRUN_LINEEND
TESTRUN_LINEEND
 "# COVERAGE TEST script invocation"                                            TESTRUN_LINEEND
".PHONY: testrun-coverage"                                                      TESTRUN_LINEEND
"testrun-coverage:"                                                             TESTRUN_LINEEND
"\tsh %s $(PROJECTPATH)"                                                        TESTRUN_LINEEND
TESTRUN_LINEEND
"# LOC TEST script invocation"                                                  TESTRUN_LINEEND
".PHONY: testrun-loc"                                                           TESTRUN_LINEEND
"testrun-loc:"TESTRUN_LINEEND
"\tsh %s $(PROJECTPATH)"                                                        TESTRUN_LINEEND
TESTRUN_LINEEND
"# TESTRUN all scripts"                                                         TESTRUN_LINEEND
".PHONY: testrun"TESTRUN_LINEEND
"testrun: $(TESTS_EXEC)"TESTRUN_LINEEND
"\t@echo \" (HINT)  $(PROJECT) \\t\\t\\t==> running tests\\n\""                 TESTRUN_LINEEND
"\tsh %s"                                                                       TESTRUN_LINEEND
"\tsh %s"                                                                       TESTRUN_LINEEND
"\tsh %s $(PROJECTPATH)"                                                        TESTRUN_LINEEND
"\tsh %s $(PROJECTPATH)"                                                        TESTRUN_LINEEND
,date,
config.project.name,
date,
config.format.suffix.tests_source_omp,
config.format.suffix.tests_source_omp,
config.format.extensions.testexec,
config.format.suffix.tests_source,
config.format.suffix.tests_source,
config.format.extensions.testexec,

config.format.extensions.testexec,
config.format.extensions.testexec,
config.format.extensions.testexec, config.format.extensions.testexec,

script_acceptance,
script_unit,
script_coverage,
script_loc,
script_unit,
script_acceptance,
script_coverage,
script_loc
);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));

        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
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

       testrun_test(test_testrun_lib_makefile_content);
       testrun_test(test_testrun_lib_makefile_main_content);
       testrun_test(test_testrun_lib_makefile_test_content);

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
