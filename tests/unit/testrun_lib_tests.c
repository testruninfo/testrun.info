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
"/***\n"
"        ------------------------------------------------------------------------\n"
"\n"
"        Copyright 2017 Markus Toepfer\n"
"\n"
"        Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"        you may not use this file except in compliance with the License.\n"
"        You may obtain a copy of the License at\n"
"\n"
"                http://www.apache.org/licenses/LICENSE-2.0\n"
"\n"
"        Unless required by applicable law or agreed to in writing, software\n"
"        distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"        See the License for the specific language governing permissions and\n"
"        limitations under the License.\n"
"\n"
"        This file is part of the testrun project. http://testrun.info\n"
"\n"
"        ------------------------------------------------------------------------\n"
"*//**\n"
"        @file           testrun.h\n"
"        @author         Markus Toepfer\n"
"        @date           %s\n"
"\n"
"        @ingroup        testrun\n"
"\n"
"        @brief          Simple serial test execution framework.\n"
"\n"
"\n"
"        ------------------------------------------------------------------------\n"
"*/\n"
"\n"
"#ifndef testrun_h\n"
"#define testrun_h\n"
"\n"
"#include <unistd.h>     /* C89/C90 */\n"
"#include <stdlib.h>     /* C89/C90 */\n"
"#include <stdio.h>      /* C89/C90 */\n"
"#include <string.h>     /* C89/C90 */\n"
"#include <errno.h>      /* C89/C90 */\n"
"#include <time.h>       /* C89/C90 */\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_errno() \\\n"
"        (errno == 0 ? \"NONE\" :  strerror(errno))\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Log a failure. Failure: Inability to perform a function as expected.\n"
"*/\n"
"#define testrun_log_failure(msg, ...) \\\n"
"        fprintf(stderr, \"\\t[FAIL]\\t%%s line:%%d errno:%%s message: \" msg \"\\n\",\\\n"
"               __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Log an error. Error: Difference between expected and actual result.\n"
"*/\n"
"#define testrun_log_error(msg, ...) \\\n"
"        fprintf(stderr, \"\\t[ERROR]\\t%%s line:%%d errno:%%s message: \" msg \"\\n\",\\\n"
"        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log_success(msg, ...) \\\n"
"        fprintf(stdout, \"\\t[OK] \\t%%s \" msg \"\\n\", __FUNCTION__, ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log(msg, ...) \\\n"
"        fprintf(stdout, \"\\t\" msg \"\\n\", ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log_function_info(msg, ...) \\\n"
"        fprintf(stdout, \"\\t[INFO] \\t%%s line:%%d message: \" msg \"\\n\", \\\n"
"                __FUNCTION__, __LINE__, ##__VA_ARGS__)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_log_clock(start, end) \\\n"
"        fprintf(stdout, \"\\tClock ticks function: ( %%s ) | %%f s | %%.0f ms \\n\", \\\n"
"        __func__, \\\n"
"        ((double)(end - start)) / CLOCKS_PER_SEC, \\\n"
"        (((double)(end - start)) / CLOCKS_PER_SEC ) * 1000)\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"#define testrun_init()  \\\n"
"        int result = 0; \\\n"
"        int testrun_counter = 0;\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Run a single atomar test. Return the surrounding block on error.\n"
"        This function will leave the context block running on error. The\n"
"        Mindset is a defused assert. LEAVE THE FUNCTION NOT THE PROGRAM.\n"
"\n"
"        @param test     boolean decision input.\n"
"        @returns        the calling function on error with -1\n"
"*/\n"
"#define testrun_check(test, ... )\\\n"
"        if (!(test)) { testrun_log_error(__VA_ARGS__);  return -1;}\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Alias to @see testrun_check.\n"
"*/\n"
"#define testrun(test, ...)\\\n"
"        testrun_check(test, __VA_ARGS__ )\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Run a single test (execute a function pointer. Runs a test function.\n"
"        On non negative return value of the function run, a testrun_counter\n"
"        is increased, on negative result, the negative result will be returned.\n"
"\n"
"        @param test     function pointer of the test to run\n"
"        @NOTE           The surrounding block is left on negative result of the\n"
"                        function pointer execution.\n"
"*/\n"
"#define testrun_test(test)\\\n"
"        result = test(); testrun_counter++; if (result < 0) return result;\n"
"\n"
"/*----------------------------------------------------------------------------*/\n"
"\n"
"/**\n"
"        Runs a function pointer, which SHALL contain the test function pointers\n"
"        to run. The function pointer is wrapped in a main procedure, which and\n"
"        allows indepentent testruns of the input testcluster over external\n"
"        execution.\n"
"\n"
"        A clock will be started, as soon as the main is executed and the the\n"
"        time is stopped again, at the end of the execution. The difference\n"
"        will be printed and is the runtime of the whole input testcluster.\n"
"\n"
"        A run will fail, as soon as one of the tests in the testcluster fails.\n"
"        (Fail on first) or will run all functions dependent on the testsetup.\n"
"\n"
"        @param testcluster      function pointer to be executed.\n"
"*/\n"
"#define testrun_run(testcluster) int main(int argc, char *argv[]) {\\\n"
"        argc = argc;\\\n"
"        clock_t start1_t, end1_t; \\\n"
"        start1_t = clock(); \\\n"
"        testrun_log(\"\\ntestrun\\t%%s\", argv[0]);\\\n"
"        int64_t result = testcluster();\\\n"
"        if (result > 0) \\\n"
"                testrun_log(\"ALL TESTS RUN (%%jd tests)\", result);\\\n"
"        end1_t = clock(); \\\n"
"        testrun_log_clock(start1_t, end1_t); \\\n"
"        testrun_log();\\\n"
"        result >=0 ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE); \\\n"
"}\n"
"\n"
"/*--------------- For EXAMPLE code check http://testrun.info -----------------*/\n"
"\n"
"#endif /* testrun_h */\n",
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
"#!/usr/bin/env bash\n"
"#\n"
"#       Copyright 2017 Markus Toepfer\n"
"#\n"
"#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"#       you may not use this file except in compliance with the License.\n"
"#       You may obtain a copy of the License at\n"
"#\n"
"#               http://www.apache.org/licenses/LICENSE-2.0\n"
"#\n"
"#       Unless required by applicable law or agreed to in writing, software\n"
"#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"#       See the License for the specific language governing permissions and\n"
"#       limitations under the License.\n"
"#\n"
"#       ------------------------------------------------------------------------\n"
"#\n"
"#       File            testrun_runner.sh\n"
"#       Authors         Markus Toepfer\n"
"#       Date            %s\n"
"#\n"
"#       Project         testrun_runner.sh\n"
"#\n"
"#       Description     Run each test.test of a folder and log Ok or NOK\n"
"#                       for each executed testfile of the folder.\n"
"#\n"
"#                       EXAMPLE OUTPUT\n"
"#\n"
"#                       [OK]  1/5 filename1.test\n"
"#                       [NOK] 2/5 filename2.test\n"
"#\n"
"#                       MODES\n"
"#\n"
"#                       (1) RUN ALL TESTS (log but ignore errors)\n"
"#                           use script with 2 parameters\n"
"#                           e.g. ./testrun_runner.sh logfile /path\n"
"#\n"
"#                           This mode will not return a test failure and\n"
"#                           may be used to run all tests and return success\n"
"#                           if all tests was run. (test results are logged)\n"
"#\n"
"#                       (2) FAIL ON ERROR (Fail on first error)\n"
"#                           use script with 3 parameters\n"
"#                           e.g. ./testrun_runner.sh logfile /path 1\n"
"#\n"
"#                           This mode returns -1 on the first test failure.\n"
"#\n"
"#                       PARAMETER\n"
"#\n"
"#                       (1) path to logfile destination\n"
"#                       (2) path to folder with test cases\n"
"#\n"
"#\n"
"#       Usage           ./testrun_runner.sh /path/to/logfile /path/to/test/dir\n"
"#\n"
"#       Dependencies    bash, tail, ls, grep, wc\n"
"#\n"
"#       Last changed    %s\n"
"#       ------------------------------------------------------------------------\n"
"\n"
"\n"
"#       ------------------------------------------------------------------------\n"
"#       CONFIGURE SCRIPT BASED ON PRESET VARIBALES OR ON INPUT\n"
"#       ------------------------------------------------------------------------\n"
"\n"
"if [ -z $1 ]; then\n"
"        echo \"ERROR ... NO LOGFILE INPUT TO SRCIPT\"\n"
"        exit 1\n"
"fi\n"
"LOGFILE=$1\n"
"\n"
"if [ -z $2 ]; then\n"
"        echo \"ERROR ... FOLDER INPUT TO SRCIPT\"\n"
"        exit 1\n"
"fi\n"
"FOLDER=$2\n"
"\n"
"FAIL_ON_ERROR=0\n"
"if [ ! -z $3 ]; then\n"
"        FAIL_ON_ERROR=1\n"
"fi\n"
"\n"
"if [ ! -w $LOGFILE ]; then\n"
"        echo \"ERROR ... LOGFILE NOT WRITABLE\"\n"
"        exit 1\n"
"fi\n"
"\n"
"#       ------------------------------------------------------------------------\n"
"#       PERFORM TESTRUN\n"
"#       ------------------------------------------------------------------------\n"
"\n"
"FILES=`ls  $FOLDER/ | grep \"\\.test\" | wc -l`\n"
"if [ $? -ne 0 ]; then\n"
"        echo \"ERROR ... could not count files of $FOLDER\"\n"
"        exit 1\n"
"fi\n"
"c=0\n"
"\n"
"if [ $FILES -eq 0 ]; then\n"
"        exit 0\n"
"fi\n"
"\n"
"for i in $FOLDER/*.test\n"
"do\n"
"        c=$((c+1))\n"
"\n"
"        # RUN EXECUTABLE\n"
"        $i 2>&1 >> $LOGFILE\n"
"\n"
"        # CHECK RETURN OF EXECUTABLE\n"
"        if [ $? -ne 0 ]; then\n"
"\n"
"                echo \"NOK\\t(\"$c\"/\"$FILES\")\\t\"$i\n"
"\n"
"                if [ $FAIL_ON_ERROR -eq 1 ]; then\n"
"                        exit 1\n"
"                fi\n"
"        else\n"
"                echo \"OK\\t(\"$c\"/\"$FILES\")\\t\"$i\n"
"        fi\n"
"done\n"
"exit 0\n"
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
"#!/usr/bin/env bash\n"
"#\n"
"#       Copyright 2017 Markus Toepfer\n"
"#\n"
"#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"#       you may not use this file except in compliance with the License.\n"
"#       You may obtain a copy of the License at\n"
"#\n"
"#               http://www.apache.org/licenses/LICENSE-2.0\n"
"#\n"
"#       Unless required by applicable law or agreed to in writing, software\n"
"#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"#       See the License for the specific language governing permissions and\n"
"#       limitations under the License.\n"
"#\n"
"#       ------------------------------------------------------------------------\n"
"#\n"
"#       File            %s\n"
"#       Authors         Markus Toepfer\n"
"#       Date            %s\n"
"#\n"
"#       Project         %s\n"
"#\n"
"#       Description     Run all test executables build/test/acceptance/*.test\n"
"#                       Run the whole folder, until an error occurs.\n"
"#\n"
"#                       MODE         FAIL ON ERROR (Fail on first test error)\n"
"#\n"
"#                       LOGFILE      build/test/log/acceptancetest.<time>.log\n"
"#\n"
"#\n"
"#       Usage           ./%s /path/to/project\n"
"#\n"
"#       Dependencies    bash, touch, chmod, ls, wc, date\n"
"#\n"
"#       Last changed    %s\n"
"#       ------------------------------------------------------------------------\n"
"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"               SIMPLE ACCEPTANCE TESTING\"\n"
"echo \"-------------------------------------------------------\"\n"
"\n"
"start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
"\n"
"# SET A LOGFILE\n"
"LOGFILE=\"build/tests/log/acceptance_\".$start_time.\"log\"\n"
"echo \" (log)   $start_time\" > $LOGFILE\n"
"touch $LOGFILE\n"
"chmod a+w $LOGFILE\n"
"\n"
"# SET THE FOLDER\n"
"FOLDER=\"build/tests/acceptance\"\n"
"\n"
"echo \"-------------------------------------------------------\" >> $LOGFILE\n"
"echo \"               REPORT ACCEPTANCE TESTING\"                >> $LOGFILE\n"
"echo \"-------------------------------------------------------\" >> $LOGFILE\n"
"\n"
"# RUN THE RUNNER\n"
"sh tests/tools/testrun_runner.sh  $LOGFILE $FOLDER FAIL_ON_ERROR\n"
"RESULT=$?\n"
"\n"
"end_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
"\n"
"# FINISH THE REPORT\n"
"echo \"-------------------------------------------------------\">> $LOGFILE\n"
"echo \"DONE \\t ACCEPTANCE TEST RUN\"  >> $LOGFILE\n"
"if [ $RESULT -eq 0 ]; then\n"
"        echo \"RESULT\\t SUCCESS\"  >> $LOGFILE\n"
"else\n"
"        echo \"RESULT\\t FAILURE\"  >> $LOGFILE\n"
"fi\n"
"echo \"START \\t $start_time\" >> $LOGFILE\n"
"echo \"END   \\t $end_time\" >> $LOGFILE\n"
"echo \"-------------------------------------------------------\">> $LOGFILE\n"
"\n"
"# DUMP THE REPORT\n"
"cat $LOGFILE\n"
"echo \"\"\n"
"exit $RESULT\n"
,TESTRUN_SCRIPT_ACCEPTANCE,
date,
TESTRUN_SCRIPT_ACCEPTANCE,
TESTRUN_SCRIPT_ACCEPTANCE,
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
"#!/usr/bin/env bash\n"
"#\n"
"#       Copyright 2017 Markus Toepfer\n"
"#\n"
"#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"#       you may not use this file except in compliance with the License.\n"
"#       You may obtain a copy of the License at\n"
"#\n"
"#               http://www.apache.org/licenses/LICENSE-2.0\n"
"#\n"
"#       Unless required by applicable law or agreed to in writing, software\n"
"#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"#       See the License for the specific language governing permissions and\n"
"#       limitations under the License.\n"
"#\n"
"#       ------------------------------------------------------------------------\n"
"#\n"
"#       File            %s\n"
"#       Authors         Markus Toepfer\n"
"#       Date            %s\n"
"#\n"
"#       Project         %s\n"
"#\n"
"#       Description     Run all test executables build/test/unit/*.test\n"
"#                       Run the whole folder, until an error occurs.\n"
"#\n"
"#                       MODE         FAIL ON ERROR (Fail on first test error)\n"
"#\n"
"#                       LOGFILE      build/test/log/unittest.<time>.log\n"
"#\n"
"#\n"
"#       Usage           ./%s /path/to/project\n"
"#\n"
"#       Dependencies    bash, touch, chmod, ls, wc, date\n"
"#\n"
"#       Last changed    %s\n"
"#       ------------------------------------------------------------------------\n"
"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"               SIMPLE UNIT TESTING\"\n"
"echo \"-------------------------------------------------------\"\n"
"\n"
"start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
"\n"
"# SET A LOGFILE\n"
"LOGFILE=\"build/tests/log/unit_\".$start_time.\"log\"\n"
"echo \" (log)   $start_time\" > $LOGFILE\n"
"touch $LOGFILE\n"
"chmod a+w $LOGFILE\n"
"\n"
"# SET THE FOLDER\n"
"FOLDER=\"build/tests/unit\"\n"
"\n"
"echo \"-------------------------------------------------------\" >> $LOGFILE\n"
"echo \"               REPORT UNIT TESTING\"                      >> $LOGFILE\n"
"echo \"-------------------------------------------------------\" >> $LOGFILE\n"
"\n"
"# RUN THE RUNNER\n"
"sh tests/tools/testrun_runner.sh  $LOGFILE $FOLDER FAIL_ON_ERROR\n"
"RESULT=$?\n"
"\n"
"end_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
"\n"
"# FINISH THE REPORT\n"
"echo \"-------------------------------------------------------\">> $LOGFILE\n"
"echo \"DONE \\t UNIT TEST RUN\"  >> $LOGFILE\n"
"if [ $RESULT -eq 0 ]; then\n"
"        echo \"RESULT\\t SUCCESS\"  >> $LOGFILE\n"
"else\n"
"        echo \"RESULT\\t FAILURE\"  >> $LOGFILE\n"
"fi\n"
"echo \"START \\t $start_time\" >> $LOGFILE\n"
"echo \"END   \\t $end_time\" >> $LOGFILE\n"
"echo \"-------------------------------------------------------\">> $LOGFILE\n"
"\n"
"# DUMP THE REPORT\n"
"cat $LOGFILE\n"
"echo \"\"\n"
"exit $RESULT\n"
,TESTRUN_SCRIPT_UNIT,
date,
TESTRUN_SCRIPT_UNIT,
TESTRUN_SCRIPT_UNIT,
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

snprintf(expect, size,
"#!/usr/bin/env bash\n"
"#\n"
"#       Copyright 2017 Markus Toepfer\n"
"#\n"
"#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"#       you may not use this file except in compliance with the License.\n"
"#       You may obtain a copy of the License at\n"
"#\n"
"#               http://www.apache.org/licenses/LICENSE-2.0\n"
"#\n"
"#       Unless required by applicable law or agreed to in writing, software\n"
"#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"#       See the License for the specific language governing permissions and\n"
"#       limitations under the License.\n"
"#\n"
"#       ------------------------------------------------------------------------\n"
"#\n"
"#       File            %s\n"
"#       Authors         Markus Toepfer\n"
"#       Date            %s\n"
"#\n"
"#       Project         %s\n"
"#\n"
"#       Description     Count functions of folder src against their counterparts\n"
"#                       in the unit test folder.\n"
"#\n"
"#                       CONVENTION\n"
"#\n"
"#                       Each function in any file of the source folder src\n"
"#                       will have a corresponding test function prefixed\n"
"#                       with \"test_\" in any unit test file.\n"
"#\n"
"#                       EXAMPLE      function | test_function\n"
"#\n"
"#                       NOTE         This simple coverage test just covers the\n"
"#                                    observance of the given coding convention.\n"
"#\n"
"#\n"
"#       Usage           ./%s /path/to/project\n"
"#\n"
"#       Dependencies    bash, ctags, awk, sed, grep\n"
"#\n"
"#       Last changed    %s\n"
"#       ------------------------------------------------------------------------\n"
"\n"
"\n"
"start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S\")\n"
"PREFIX=\"test_\"\n"
"\n"
"LIBDIR=$1\n"
"SRCDIR=$1/src\n"
"TESTDIR=$1/tests\n"
"\n"
"# SET A LOGFILE\n"
"LOGFILE=\"$LIBDIR/build/tests/log/coverage_\".$start_time.\"log\"\n"
"touch $LOGFILE\n"
"chmod a+w $LOGFILE\n"
"\n"
"echo \"-------------------------------------------------------\" >> $LOGFILE\n"
"echo \"               REPORT COVERAGE TESTING\"                  >> $LOGFILE\n"
"echo \"-------------------------------------------------------\" >> $LOGFILE\n"
"echo \"   TIME \\t $start_time\" >> $LOGFILE\n"
"echo \"\" >> $LOGFILE\n"
"\n"
"# GENERATE CTAGS SOURCE\n"
"cd $SRCDIR\n"
"if [ $? -ne 0 ]; then\n"
"        exit 1\n"
"fi\n"
"ctags --c-types=f -R\n"
"# remove the ctags stuff, to leave just the function lines\n"
"sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions\n"
"# remove anything but the function names\n"
"awk '{print $1 }' $SRCDIR/functions > \\\n"
"        $SRCDIR/functionNames\n"
"# count the lines\n"
"sourceFkt=\"$(cat functions | wc -l)\"\n"
"echo \"   count source\\t\" $sourceFkt >> $LOGFILE\n"
"\n"
"# GENERATE CTAGS TESTS\n"
"cd $TESTDIR\n"
"if [ $? -ne 0 ]; then\n"
"        exit 1\n"
"fi\n"
"ctags --c-types=f -R\n"
"# remove the ctags stuff, to leave just the function lines\n"
"sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions\n"
"# remove anything but the function names\n"
"awk '{print $1 }' $TESTDIR/functions > \\\n"
"        $TESTDIR/functionNames\n"
"# count the lines\n"
"testFkt=\"$(cat functions | grep -i ^$PREFIX | wc -l)\"\n"
"echo \"   count tests\\t\" $testFkt >> $LOGFILE\n"
"\n"
"echo \"\\nUNTESTED: \" >> $LOGFILE\n"
"# Found functions:\n"
"while read line;\n"
"do\n"
"        grep -n '^test_'$line'$' $TESTDIR/functionNames > \\\n"
"        /dev/null || echo $line >> $LOGFILE\n"
"done < $SRCDIR/functionNames\n"
"\n"
"if [ $sourceFkt != 0 ]; then\n"
"        echo \"............................................\"  >> $LOGFILE\n"
"        echo \"COVERAGE: $sourceFkt $testFkt\" | \\\n"
"        awk '{ printf $1 \" %%.2f %%%% \\n\", $3/$2*100}' >> $LOGFILE\n"
"fi\n"
"\n"
"cat $LOGFILE\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"\"\n"
"\n"
"# cleanup remove the files we created\n"
"rm $SRCDIR/tags\n"
"rm $SRCDIR/functions\n"
"rm $SRCDIR/functionNames\n"
"rm $TESTDIR/tags\n"
"rm $TESTDIR/functions\n"
"rm $TESTDIR/functionNames\n"
,TESTRUN_SCRIPT_COVERAGE,
date,
TESTRUN_SCRIPT_COVERAGE,
TESTRUN_SCRIPT_COVERAGE,
date
);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));

        testrun(strlen(result) == strlen(expect));
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

        result = testrun_lib_script_loc_tests_content(config);

        snprintf(expect, size,
"#!/usr/bin/env bash\n"
"#\n"
"#       Copyright 2017 Markus Toepfer\n"
"#\n"
"#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"#       you may not use this file except in compliance with the License.\n"
"#       You may obtain a copy of the License at\n"
"#\n"
"#               http://www.apache.org/licenses/LICENSE-2.0\n"
"#\n"
"#       Unless required by applicable law or agreed to in writing, software\n"
"#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"#       See the License for the specific language governing permissions and\n"
"#       limitations under the License.\n"
"#\n"
"#       ------------------------------------------------------------------------\n"
"#\n"
"#       File            %s\n"
"#       Authors         Markus Toepfer\n"
"#       Date            %s\n"
"#\n"
"#       Project         %s\n"
"#\n"
"#       Description     Count the lines of src and unit | acceptance tests.\n"
"#                       This file uses no error checking.\n"
"#\n"
"#       Usage           ./%s /path/to/project\n"
"#\n"
"#       Dependencies    bash, find, xargs, wc\n"
"#\n"
"#       Last changed    %s\n"
"#       ------------------------------------------------------------------------\n"
"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"               SIMPLE LOC COUNTER\"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"(LOC) src\"\n"
"find $1/src -name '*.c' | xargs wc -l\n"
"echo \"(LOC) tests/unit\"\n"
"find $1/tests/unit -name '*.c' | xargs wc -l\n"
"echo \"(LOC) tests/acceptance\"\n"
"find $1/tests/acceptance -name '*.c' | xargs wc -l\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"\"\n"
,TESTRUN_SCRIPT_LOC,
date,
TESTRUN_SCRIPT_LOC,
TESTRUN_SCRIPT_LOC,
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

int test_testrun_lib_script_gprof_tests_content(){

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

        result = testrun_lib_script_gprof_tests_content(config);

        snprintf(expect, size,
"#!/usr/bin/env bash\n"
"#\n"
"#       Copyright 2017 Markus Toepfer\n"
"#\n"
"#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"#       you may not use this file except in compliance with the License.\n"
"#       You may obtain a copy of the License at\n"
"#\n"
"#               http://www.apache.org/licenses/LICENSE-2.0\n"
"#\n"
"#       Unless required by applicable law or agreed to in writing, software\n"
"#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"#       See the License for the specific language governing permissions and\n"
"#       limitations under the License.\n"
"#\n"
"#       ------------------------------------------------------------------------\n"
"#\n"
"#       File            %s\n"
"#       Authors         Markus Toepfer\n"
"#       Date            %s\n"
"#\n"
"#       Project         %s\n"
"#\n"
"#       Description     Run gprof based analysis tests on all test cases.\n"
"#\n"
"#       Usage           ./%s /path/to/project\n"
"#\n"
"#       Dependencies    bash, gprof\n"
"#\n"
"#       Last changed    %s\n"
"#       ------------------------------------------------------------------------\n"
"\n"
"start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S\")\n"
"\n"
"LOGFOLDER=\"build/tests/log\"\n"
"\n"
"# SET A LOGFILE\n"
"LOGFILE=$LOGFOLDER\"/gprof_\".$start_time.\"log\"\n"
"echo \" (log)   $start_time\" > $LOGFILE\n"
"touch $LOGFILE\n"
"chmod a+w $LOGFILE\n"
"\n"
"# SET THE FOLDER\n"
"FOLDER=\"build/tests/unit\"\n"
"SRC_FOLDER=\"tests/unit\"\n"
"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"               GPROF RUNNER\"\n"
"echo \"-------------------------------------------------------\"\n"
"\n"
"# Execute the test once and profile the execution\n"
"for test in $FOLDER/*.test; do\n"
"        name=${test##*/}\n"
"        echo \"Profiling\" $name\n"
"    $test\n"
"    gprof $test gmon.out > $name.profile\n"
"done\n"
"\n"
"# move profile to build/tests/logs\n"
"mv *.profile $LOGFOLDER\n"
"exit 0\n"
"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"\"\n"
,TESTRUN_SCRIPT_GPROF,
date,
TESTRUN_SCRIPT_GPROF,
TESTRUN_SCRIPT_GPROF,
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

int test_testrun_lib_script_gcov_tests_content(){

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

        result = testrun_lib_script_gcov_tests_content(config);

        snprintf(expect, size,
"#!/usr/bin/env bash\n"
"#\n"
"#       Copyright 2017 Markus Toepfer\n"
"#\n"
"#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"#       you may not use this file except in compliance with the License.\n"
"#       You may obtain a copy of the License at\n"
"#\n"
"#               http://www.apache.org/licenses/LICENSE-2.0\n"
"#\n"
"#       Unless required by applicable law or agreed to in writing, software\n"
"#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"#       See the License for the specific language governing permissions and\n"
"#       limitations under the License.\n"
"#\n"
"#       ------------------------------------------------------------------------\n"
"#\n"
"#       File            %s\n"
"#       Authors         Markus Toepfer\n"
"#       Date            %s\n"
"#\n"
"#       Project         %s\n"
"#\n"
"#       Description     Run gcov based coverage tests on all test cases.\n"
"#\n"
"#       Usage           ./%s /path/to/project\n"
"#\n"
"#       Dependencies    bash, gcov\n"
"#\n"
"#       Last changed    %s\n"
"#       ------------------------------------------------------------------------\n"
"\n"
"start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S\")\n"
"\n"
"LOGFOLDER=\"build/tests/log\"\n"
"\n"
"# SET A LOGFILE\n"
"LOGFILE=$LOGFOLDER\"/gcov_\".$start_time.\"log\"\n"
"echo \" (log)   $start_time\" > $LOGFILE\n"
"touch $LOGFILE\n"
"chmod a+w $LOGFILE\n"
"\n"
"# SET THE FOLDER\n"
"FOLDER=\"build/tests/unit\"\n"
"SRC_FOLDER=\"tests/unit\"\n"
"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"               GCOV RUNNER\"\n"
"echo \"-------------------------------------------------------\"\n"
"\n"
"# RUN THE RUNNER\n"
"#sh ./tests/./tools/testrun_runner.sh  $LOGFILE $FOLDER FAIL_ON_ERROR\n"
"#RESULT=$?\n"
"\n"
"for test in $FOLDER/*.test; do\n"
"    $test\n"
"done\n"
"\n"
"FILES=`ls  $FOLDER/ | grep \".test\" | wc -l`\n"
"if [ $? -ne 0 ]; then\n"
"        echo \"ERROR ... could not count files of $FOLDER\"\n"
"        exit 1\n"
"fi\n"
"c=0\n"
"\n"
"if [ $FILES -eq 0 ]; then\n"
"        exit 0\n"
"fi\n"
"\n"
"for i in $SRC_FOLDER/*_tests.c\n"
"do\n"
"        # RUN GCOV\n"
"        echo $i\n"
"        gcov $i\n"
"done\n"
"\n"
"# move coverage to build/tests/logs\n"
"mv *.gcov $LOGFOLDER\n"
"exit 0\n"
"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"\"\n"
,TESTRUN_SCRIPT_GCOV,
date,
TESTRUN_SCRIPT_GCOV,
TESTRUN_SCRIPT_GCOV,
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
        "/***\n"
        "        ------------------------------------------------------------------------\n"
        "\n"
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]\n"
        "        All rights reserved.\n"
        "\n"
        "        ------------------------------------------------------------------------\n"
        "*//**\n"
        "        @file           %s.h\n"
        "        @author         [AUTHOR]\n"
        "        @date           %s\n"
        "\n"
        "        @ingroup        [PROJECT]\n"
        "\n"
        "        @brief\n"
        "\n"
        "\n"
        "        ------------------------------------------------------------------------\n"
        "*/\n"
        "#ifndef %s_h\n"
        "#define %s_h\n"
        "\n"
        "#endif /* %s_h */\n",
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
        "/***\n"
        "        ------------------------------------------------------------------------\n"
        "\n"
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]\n"
        "        All rights reserved.\n"
        "\n"
        "        ------------------------------------------------------------------------\n"
        "*//**\n"
        "        @file           %s.c\n"
        "        @author         [AUTHOR]\n"
        "        @date           %s\n"
        "\n"
        "        @ingroup        [PROJECT]\n"
        "\n"
        "        @brief\n"
        "\n"
        "\n"
        "        ------------------------------------------------------------------------\n"
        "*/\n"
        "#include \"../include/%s.h\"\n",name, date, name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - TESTS
        // -------------------------------------------------------------

        result = testrun_lib_c_file_content(name, TESTRUN_TEST, &config);
        snprintf(expect, size,
        "/***\n"
        "        ------------------------------------------------------------------------\n"
        "\n"
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]\n"
        "        All rights reserved.\n"
        "\n"
        "        ------------------------------------------------------------------------\n"
        "*//**\n"
        "        @file           %s_test.c\n"
        "        @author         [AUTHOR]\n"
        "        @date           %s\n"
        "\n"
        "        @ingroup        [PROJECT]\n"
        "\n"
        "        @brief\n"
        "\n"
        "\n"
        "        ------------------------------------------------------------------------\n"
        "*/\n"
        "\n"
        "#include \"../tools/testrun.h\"\n"
        "#include \"../../src/%s.c\"\n"
        "\n"
        "/*\n"
        " *      ------------------------------------------------------------------------\n"
        " *\n"
        " *      TEST HELPER                                                     #HELPER\n"
        " *\n"
        " *      ------------------------------------------------------------------------\n"
        " */\n"
        "\n"
        "/*----------------------------------------------------------------------------*/\n"
        "\n"
        "/*\n"
        " *      ------------------------------------------------------------------------\n"
        " *\n"
        " *      TEST CASES                                                      #CASES\n"
        " *\n"
        " *      ------------------------------------------------------------------------\n"
        " */\n"
        "\n"
        "/*----------------------------------------------------------------------------*/\n"
        "\n"
        "int test_case(){\n"
        "        testrun(1 == 1);\n"
        "\n"
        "        return testrun_log_success();\n"
        "}\n"
        "\n"
        "/*----------------------------------------------------------------------------*/\n"
        "\n"
        "/*\n"
        " *      ------------------------------------------------------------------------\n"
        " *\n"
        " *      TEST CLUSTER                                                    #CLUSTER\n"
        " *\n"
        " *      ------------------------------------------------------------------------\n"
        " */\n"
        "\n"
        "int all_tests() {\n"
        "\n"
        "        testrun_init();\n"
        "        testrun_test(test_case);\n"
        "\n"
        "        return testrun_counter;\n"
        "}\n"
        "\n"
        "/*\n"
        " *      ------------------------------------------------------------------------\n"
        " *\n"
        " *      TEST EXECUTION                                                  #EXEC\n"
        " *\n"
        " *      ------------------------------------------------------------------------\n"
        " */\n"
        "\n"
        "testrun_run(all_tests);\n"
        "\n"
        ,name, date, name);
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
"#       ------------------------------------------------------------------------\n"
"#\n"
"#       Copyright 2017 Markus Toepfer\n"
"#\n"
"#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"#       you may not use this file except in compliance with the License.\n"
"#       You may obtain a copy of the License at\n"
"#\n"
"#               http://www.apache.org/licenses/LICENSE-2.0\n"
"#\n"
"#       Unless required by applicable law or agreed to in writing, software\n"
"#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"#       See the License for the specific language governing permissions and\n"
"#       limitations under the License.\n"
"#\n"
"#       ------------------------------------------------------------------------\n"
"#\n"
"#       File            makefile\n"
"#       Authors         Markus Toepfer\n"
"#       Date            %s\n"
"#\n"
"#       Project         %s\n"
"#\n"
"#       Description     This makefile defines project specific parameter.\n"
"#\n"
"#                       These parameter are:\n"
"#                       (1) used compiler and special flags\n"
"#                       (2) name and version\n"
"#                       (3) installation prefix\n"
"#                       (4) used libraries\n"
"#                       (5) general makefiles used\n"
"#\n"
"#       Usage           make\n"
"#\n"
"#       Dependencies    make & compiler\n"
"#\n"
"#       Last changed    %s\n"
"#       ------------------------------------------------------------------------\n"
"\n"
"CC = gcc\n"
"\n"
"PROJECT         := %s\n"
"VERSION         := 0.0.1\n"
"\n"
"# project path recalculation (if used included from parent make)\n"
"PROJECTMK       := $(abspath $(lastword $(MAKEFILE_LIST)))\n"
"\n"
"# base directory for installation\n"
"PREFIX          := /usr/local\n"
"\n"
"# LIBS USED (uncommented example includes)\n"
"#LIBS           += `pkg-config --libs libsystemd`\n"
"#LIBS           += `pkg-config --libs uuid`\n"
"#LIBS           += `pkg-config --libs openssl`\n"
"\n"
"# EXTRA CFLAGS (example)\n"
"MODCFLAGS       += -std=gnu11\n"
"MODCFLAGS       += -rdynamic\n"
"#MODCFLAGS      += -fopenmp\n"
"\n"
"# EXTRA LFLAGS (example)\n"
"#MODLFLAGS      += -pthread\n"
"\n"
"#TMP FILE DEFINITION\n"
"TESTS_TMP_FILES = $(wildcard /tmp/test_*)\n"
"\n"
"# INCLUDE BASE MAKEFILE\n"
"include makefile_general.mk\n"
,date,
config.project.name,
date,
config.project.name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_makefile_general_content(){

        size_t size = 50000;
        char expect[size];
        bzero(expect, size);

        testrun_config config = testrun_config_default();

        char *result = testrun_lib_makefile_general_content(config);
        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);


        char *all_target = NULL;
        char *install    = NULL;
        char *uninstall  = NULL;

        switch (config.project.type){

                case TESTRUN_LIB:
                        all_target = "all_lib";
                        install    = "install_lib";
                        uninstall  = "uninstall_lib";
                        break;
                case TESTRUN_EXEC:
                        all_target = "all_exec";
                        install    = "install_exec";
                        uninstall  = "uninstall_exec";
                        break;
                case TESTRUN_SERVICE:
                        all_target = "all_exec";
                        install    = "install_service";
                        uninstall  = "uninstall_service";
                        break;
                default:
                        testrun(false);
        }

        snprintf(expect, size,
"#       ------------------------------------------------------------------------\n"
"#\n"
"#       Copyright 2017 Markus Toepfer\n"
"#\n"
"#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"#       you may not use this file except in compliance with the License.\n"
"#       You may obtain a copy of the License at\n"
"#\n"
"#               http://www.apache.org/licenses/LICENSE-2.0\n"
"#\n"
"#       Unless required by applicable law or agreed to in writing, software\n"
"#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"#       See the License for the specific language governing permissions and\n"
"#       limitations under the License.\n"
"#\n"
"#       ------------------------------------------------------------------------\n"
"#\n"
"#       File            makefile_general.mk\n"
"#       Authors         Markus Toepfer\n"
"#       Date            %s\n"
"#\n"
"#       Project         [PROJECT]\n"
"#\n"
"#       Description     Generic makefile for testrun based projects.\n"
"#\n"
"#                       Target of this makefile is an independent library\n"
"#                       or executable to be installed at either PREFIX/lib\n"
"#                       or PREFIX/bin.\n"
"#\n"
"#                       The TESTING part contains all required functionality\n"
"#                       to use the testrun tools via a makefile. It may be\n"
"#                       seen as a makefile integrated testrunner framework.\n"
"#\n"
"#                       in particular:\n"
"#\n"
"#                           \"make clean && make tested\"\n"
"#\n"
"#                       may be used to build all sources as well as tests from\n"
"#                       scratch and perform an integrated testrun over all after\n"
"#                       compilation.\n"
"#\n"
"#                           \"make testrun-gcov\"\n"
"#\n"
"#                       may be used to rebuild the whole project with gcov\n"
"#                       coverage testing flag enabled.\n"
"#\n"
"#                           \"make testrun-gprof\"\n"
"#\n"
"#                       may be used to rebuild the whole project with gprof\n"
"#                       profiling flags enabled.\n"
"#\n"
"#                       Following folder structure is required\n"
"#\n"
"#                           sources MUST be located at src/\n"
"#                           inludes MUST be located at include/\n"
"#                           tests   MUST be located at tests/\n"
"#                           build   MUST be located at build/\n"
"#\n"
"#                       ALL TEST SCRIPTS MAY BE EXCHANGED WITH CUSTOM RUNNERS\n"
"#\n"
"#       Usage           SHOULD be used included by parent makefile\n"
"#\n"
"#       Dependencies    testrun (makefile & service scripts), doxygen (if used)\n"
"#\n"
"#       Last changed    %s\n"
"#       ------------------------------------------------------------------------\n"
"\n"
"# Switch on colors\n"
"GCC_COLORS ?= 'gcc colors available, use them!'\n"
"export GCC_COLORS\n"
"\n"
"# ----- Compiler flags -----------------------------------------------------\n"
"\n"
"CFLAGS\t\t\t= -Wall -Wextra -fPIC -Iinclude\n"
"\n"
"CFLAGS\t\t\t+= $(MODCFLAGS)\n"
"LFLAGS\t\t\t+= $(MODLFLAGS)\n"
"\n"
"# ----- Project path calculation (if used included) ------------------------\n"
"\n"
"PROJECTPATH\t\t:= $(abspath $(dir $(PROJECTMK)))\n"
"DIRNAME\t\t:= $(notdir $(patsubst %%/,%%,$(dir $(PROJECTMK))))\n"
"\n"
"# ----- Package config setup -----------------------------------------------\n"
"\n"
"LIBNAME\t\t\t:= lib$(DIRNAME)\n"
"LIBNAMEPC\t\t:= $(LIBNAME).pc\n"
"\n"
"INCDIR\t\t\t:= $(PREFIX)/include/$(DIRNAME)\n"
"LIBDIR\t\t\t:= $(PREFIX)/lib\n"
"EXECDIR\t\t\t:= $(PREFIX)/bin\n"
"\n"
"# ----- TARGETS ------------------------------------------------------------\n"
"\n"
"INSTALL\t\t\t:= install\n"
"\n"
"EXECUTABLE\t\t= bin/$(DIRNAME)\n"
"\n"
"STATIC\t\t\t= build/lib$(DIRNAME).a\n"
"SHARED\t\t\t= $(patsubst %%.a,%%.so,$(STATIC))\n"
"\n"
"# Source and object files to compile\n"
"HEADERS\t\t\t= $(wildcard include/*.h)\n"
"SOURCES\t\t\t= $(wildcard src/**/*.c src/*.c)\n"
"OBJECTS\t\t\t= $(patsubst %%.c,%%.o,$(SOURCES))\n"
"\n"
"# Test sources and targets\n"
"TESTS_SOURCES\t= $(wildcard tests/**/*_tests.c tests/*_tests.c)\n"
"TESTS_TARGET\t= $(patsubst tests/%%.c, build/tests/%%.test, $(TESTS_SOURCES))\n"
"\n"
"# GCOV support\n"
"GCOV_FILES\t\t= $(patsubst  %%.c,%%.gcno,$(SOURCES))\n"
"GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcov,$(SOURCES))\n"
"GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcda,$(SOURCES))\n"
"GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcno,$(TESTS_SOURCES))\n"
"GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcda,$(TESTS_SOURCES))\n"
"\n"
"ifdef USE_GCOV\n"
"CFLAGS += -fprofile-arcs -ftest-coverage\n"
"LFLAGS += -lgcov --coverage\n"
"endif\n"
"\n"
"ifdef USE_GPROF\n"
"CFLAGS += -pg\n"
"endif\n"
"\n"
"# ----- DEFAULT MAKE RULES -------------------------------------------------\n"
"\n"
"%%.o : %%.c $(HEADERS)\n"
"\t@echo \" (CC)    $@\"\n"
"\t@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)\n"
"\n"
"%%_tests.o : %%_tests.c\n"
"\t@echo \" (CC)    $@\"\n"
"\t@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)\n"
"\n"
"all:\t\t\t%s\n"
"install:\t\t%s\n"
"uninstall:\t\t%s\n"
"\n"
"all_lib:\t\tstart lib tests pkgconfig done\n"
"all_exec:\t\tstart lib tests $(EXECUTABLE) done\n"
"all_service:\tall_exec\n"
"\n"
"lib:\t\t\tbuild sources\n"
"sources:\t\tbuild $(STATIC) $(SHARED)\n"
"tests:\t\t\ttests-resources $(TESTS_TARGET)\n"
"\n"
"$(STATIC):  $(OBJECTS)\n"
"\t@echo \" (AR)    $@ $(OBJECTS)\"\n"
"\t@ar rcs $@ $(OBJECTS)\n"
"\t@ranlib $@\n"
"\n"
"$(SHARED): $(STATIC) $(OBJECTS)\n"
"\t@echo \" (CC)    $@ $(OBJECTS)\"\n"
"\t@$(CC) -shared -o $@ $(OBJECTS) $(LIBS) $(LFLAGS)\n"
"\n"
"$(EXECUTABLE): $(OBJECTS)\n"
"\t@echo \" (CC)    $@ $(OBJECTS)\"\n"
"\t$(CC) -o $@ $(STATIC) $(LIBS) $(LFLAGS)\n"
"\n"
"\n"
"# ----- BUILD & CLEANUP ----------------------------------------------------\n"
"\n"
"build:\n"
"\t@mkdir -p bin\n"
"\t@mkdir -p build\n"
"\t@mkdir -p build/tests\n"
"\t@mkdir -p build/tests/unit\n"
"\t@mkdir -p build/tests/acceptance\n"
"\t@mkdir -p build/tests/log\n"
"\t@echo \" (MK)    directories for build\"\n"
"\n"
".PHONY: clean\n"
"clean:\n"
"\t@echo \" (CLEAN) $(LIBNAME)\"\n"
"\t@rm -rf build bin doxygen/documentation $(OBJECTS) $(TESTS_OBJECTS) \\n"
"\t\t$(LIBNAMEPC) $(TESTS_TMP_FILES) $(GCOV_FILES) *.gcov *.profile *.pc *.out\n"
"\n"
"\n"
"# ----- DOCUMENATION -------------------------------------------------------\n"
"\n"
"#NOTE requires doxygen.PHONY: documentation\n"
"documentation:\n"
"\tdoxygen ./doxygen/doxygen.config\n"
"\n"
"\n"
"# ----- INFORMATION PRINTING -----------------------------------------------\n"
"\n"
"# print out a variable of the make file (e.g. \"make print-PROJECTPATH\")\n"
".PHONY: print\n"
"print-%%  : ; @echo $* = $($*)\n"
"\n"
".PHONY: start\n"
"start:\n"
"\t@echo \"\\n (HINT)    $(PROJECT) \t\t ==> running make\\n\"\n"
"\n"
".PHONY: done\n"
"done:\n"
"\t@echo\n"
"\t@echo \" (DONE)  make $(PROJECT)\"\n"
"\t@echo \" (HINT)  with unit testing      ==> 'make tested'\"\n"
"\t@echo \" (HINT)  perform installation   ==> 'sudo make install\\n\"\n"
"\t@echo \" (HINT)  generate documentation ==> 'make documentation\\n\"\n"
"\n"
"\n"
"# ----- TESTING ------------------------------------------------------------\n"
"\n"
"# ALL IN ONE CALL (compile source, test and run test)\n"
"tested: all testrun done\n"
"\n"
"# copy test resources to build\n"
"tests-resources:\n"
"\t@echo \" (CP)    tests/resources\"\n"
"\t@cp -r tests/resources build/tests\n"
"\n"
"build/tests/acceptance/%%_tests.test: tests/acceptance/%%_tests.o\n"
"\t@echo \" (CC)    $(@)\"\n"
"\t@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)\n"
"\n"
"build/tests/unit/%%_tests.test: tests/unit/%%_tests.o\n"
"\t@echo \" (CC)    $(@)\"\n"
"\t@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)\n"
"\n"
"# build a specific executable test (testname) under build/tests\n"
"# NOTE: May be usefull for module development in large projects\n"
"test:\n"
"\t@echo \" (CC)    $(testname)\"\n"
"\t@$(CC) $(CFLAGS) $(LFLAGS) $(patsubst build/tests/%%.test, \\\n"
"\t\ttests/%%.c,$(testname)) -ldl $(STATIC) -Wl,-rpath=$(RPATH) -g -o\\\n"
"\t\t$(patsubst tests/%%.c,build/tests/%%.test,$(testname)) $(LIBS)\n"
"\n"
"# TESTRUN runners ----------------------------------------------------------\n"
"\n"
"# ACCEPTANCE TEST script invocation\n"
".PHONY: testrun-acceptance\n"
"testrun-acceptance:\n"
"\tsh tests/tools/testrun_simple_acceptance_tests.sh\n"
"\n"
"# UNIT TEST script invocation\n"
".PHONY: testrun-unit\n"
"testrun-unit:\n"
"\tsh tests/tools/testrun_simple_unit_tests.sh\n"
"\n"
"# COVERAGE TEST script invocation\n"
".PHONY: testrun-coverage\n"
"testrun-coverage:\n"
"\tsh tests/tools/testrun_simple_coverage_tests.sh $(PROJECTPATH)\n"
"\n"
"# LOC TEST script invocation\n"
".PHONY: testrun-loc\n"
"testrun-loc:\n"
"\tsh tests/tools/testrun_simple_loc.sh $(PROJECTPATH)\n"
"\n"
"# TESTRUN all scripts\n"
".PHONY: testrun\n"
"testrun:\n"
"\t@echo \" (HINT)  $(PROJECT) \\t\\t\\t==> running tests\\n\"\n"
"\tsh tests/tools/testrun_simple_unit_tests.sh\n"
"\tsh tests/tools/testrun_simple_acceptance_tests.sh\n"
"\tsh tests/tools/testrun_simple_coverage_tests.sh $(PROJECTPATH)\n"
"\tsh tests/tools/testrun_simple_loc.sh $(PROJECTPATH)\n"
"\n"
"# TESTRUN gcov -------------------------------------------------------------\n"
"\n"
"testrun-gcov: clean\n"
"\tmake USE_GCOV=1 all\n"
"\tsh tests/tools/testrun_gcov.sh $(PROJECTPATH)\n"
"\n"
"# TESTRUN gprof ------------------------------------------------------------\n"
"\n"
"testrun-gprof: clean\n"
"\tmake USE_GPROF=1 all\n"
"\tsh tests/tools/testrun_gprof.sh $(PROJECTPATH)\n"
"\n"
"# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------\n"
"\n"
".PHONY: pkgconfig\n"
"pkgconfig:\n"
"\t@echo 'prefix='$(PREFIX)                        >  $(LIBNAMEPC)\n"
"\t@echo 'exec_prefix=$${prefix}'                  >> $(LIBNAMEPC)\n"
"\t@echo 'libdir='$(LIBDIR)                        >> $(LIBNAMEPC)\n"
"\t@echo 'includedir='$(INCDIR)                    >> $(LIBNAMEPC)\n"
"\t@echo ''                                        >> $(LIBNAMEPC)\n"
"\t@echo 'Name: '$(LIBNAME)                        >> $(LIBNAMEPC)\n"
"\t@echo 'Description: '                           >> $(LIBNAMEPC)\n"
"\t@echo 'Version: '$(VERSION)                     >> $(LIBNAMEPC)\n"
"\t@echo 'URL: http://testrun.info'                >> $(LIBNAMEPC)\n"
"\t@echo 'Libs: -L$${libdir} -l'$(DIRNAME)         >> $(LIBNAMEPC)\n"
"\t@echo 'Cflags: -I$${includedir}'                >> $(LIBNAMEPC)\n"
"\n"
"# ----- INSTALLATION -------------------------------------------------------\n"
"\n"
"# Installation as a library ------------------------------------------------\n"
"\n"
"install_lib: $(SHARED) $(STATIC)\n"
"\t@echo \" (OK)    installed $(LIBNAME) to $(LIBDIR)\"\n"
"\t@mkdir -p $(LIBDIR)/pkgconfig\n"
"\t@mkdir -p $(INCDIR)\n"
"\t@$(INSTALL) -m 0644 -t $(INCDIR) $(shell find include -name \"*.h\")\n"
"\t@$(INSTALL) -m 0755 $(SHARED) $(LIBDIR)\n"
"\t@$(INSTALL) -m 0755 $(STATIC) $(LIBDIR)\n"
"\t@$(INSTALL) -m 0644 $(LIBNAMEPC) $(LIBDIR)/pkgconfig\n"
"\t@ldconfig\n"
"\n"
"uninstall_lib:\n"
"\t@echo \" (OK)    uninstalled $(LIBNAME) from $(LIBDIR)\"\n"
"\t@rm -rf $(INCDIR)\n"
"\t@rm -rf $(LIBDIR)/$(LIBNAME).a\n"
"\t@rm -rf $(LIBDIR)/$(LIBNAME).so\n"
"\t@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)\n"
"\n"
"# Installation as an executable --------------------------------------------\n"
"\n"
"install_exec: $(SHARED) $(STATIC)\n"
"\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\"\n"
"\t@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)\n"
"\n"
"uninstall_exec:\n"
"\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\"\n"
"\t@rm -rf $(EXECDIR)/$(DIRNAME)\n"
"\n"
"# Installation as a service (outsourced to script)--------------------------\n"
"\n"
"install_service: $(EXECUTABLE)\n"
"\t./config/install/install.sh\n"
"\n"
"uninstall_service:\n"
"\t./config/install/uninstall.sh\n",
date, date,all_target,install,uninstall);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_script_service_install_content(){

        size_t size = 50000;
        char expect[size];
        bzero(expect, size);

        testrun_config config = testrun_config_default();

        char *result = testrun_lib_script_service_install_content(config);
        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);

        snprintf(expect, size,
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright 2017 Markus Toepfer\n"
        "#\n"
        "#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
        "#       you may not use this file except in compliance with the License.\n"
        "#       You may obtain a copy of the License at\n"
        "#\n"
        "#               http://www.apache.org/licenses/LICENSE-2.0\n"
        "#\n"
        "#       Unless required by applicable law or agreed to in writing, software\n"
        "#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
        "#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
        "#       See the License for the specific language governing permissions and\n"
        "#       limitations under the License.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Install and enable the service.\n"
        "#\n"
        "#                       CONVENTION\n"
        "#\n"
        "#                       (1) Installation will be done at:\n"
        "#                           /usr/local/bin\n"
        "#\n"
        "#                       (2) Configuration files will be placed at\n"
        "#                           /etc/%s\n"
        "#\n"
        "#                       (3) Service definition will be placed at\n"
        "#                           /etc/systemd/system/%s.service\n"
        "#\n"
        "#                       (4) Service socket definition will be placed at\n"
        "#                           /etc/systemd/system/%s.socket\n"
        "#\n"
        "#                       THIS SCRIPT MUST BE RUN AS ROOT.\n"
        "#\n"
        "#       Usage           ./%s /path/to/project\n"
        "#\n"
        "#       Dependencies    bash systemctl grep cp\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "MODNAME=%s\n"
        "CONFIGDIR=\"/etc/$MODNAME\"\n"
        "\n"
        "# Make sure only root can run our script\n"
        "if [[ $EUID -ne 0 ]]; then\n"
        "        echo \"\"\n"
        "        echo \"This script must be run as root\" 1>&2\n"
        "        echo \"\"\n"
        "        exit 1\n"
        "fi\n"
        "\n"
        "DIR=\"$( cd \"$( dirname \"${BASH_SOURCE[0]}\" )\" && pwd )\"\n"
        "cd $DIR\n"
        "\n"
        "# ----------------------------------------------------------------------------\n"
        "#       CHECK SOCKET CONFIG DONE\n"
        "# ----------------------------------------------------------------------------\n"
        "\n"
        "grep '^ListenStream' $MODNAME.socket\n"
        "if [[ $? -eq 1 ]]; then\n"
        "        echo \"\"\n"
        "        echo \"YOU MUST CONFIGURE $MODNAME.socket before running install.\"\n"
        "        echo \"ListenStream not configured. Socket activated run will not work.\"\n"
        "        echo \"\"\n"
        "        echo \"EDIT config/install/$MODNAME.socket and enable a socket port\"\n"
        "        exit 1\n"
        "fi\n"
        "\n"
        "# ----------------------------------------------------------------------------\n"
        "#       COPY REQUIRED FILES\n"
        "# ----------------------------------------------------------------------------\n"
        "\n"
        "echo \"... copy executable to /usr/local/bin\"\n"
        "cp ${DIR}/../../bin/$MODNAME /usr/local/bin/$MODNAME\n"
        "if [ $? -eq 0 ]; then\n"
        "        echo \"copied $MODNAME to /usr/local/bin/\"\n"
        "else\n"
        "        echo \"FAILURE ... stopping, please check manually.\"\n"
        "        exit 1\n"
        "fi\n"
        "\n"
        "echo \"... copying systemd service descriptions.\"\n"
        "cp ${DIR}/../install/$MODNAME* /etc/systemd/system/\n"
        "if [ $? -eq 0 ]; then\n"
        "        echo \"copied systemd descriptions to /etc/systemd/system/\"\n"
        "else\n"
        "        echo \"FAILURE ... stopping, please check manually.\"\n"
        "        exit 1\n"
        "fi\n"
        "\n"
        "# ----------------------------------------------------------------------------\n"
        "#       ENABLE THE SOCKET\n"
        "# ----------------------------------------------------------------------------\n"
        "\n"
        "echo \"... enabling socket service\"\n"
        "\n"
        "systemctl enable $MODNAME.socket\n"
        "if [ $? -eq 0 ]; then\n"
        "        echo \"$MODNAME.socket enabled\"\n"
        "        systemctl start $MODNAME.socket\n"
        "        if [ $? -eq 0 ]; then\n"
        "                echo \"$MODNAME.socket started\"\n"
        "        else\n"
        "                echo \"$MODNAME socket not started ... stopping.\"\n"
        "                exit 1\n"
        "        fi\n"
        "else\n"
        "        echo \"$MODNAME socket not enabled ... stopping\"\n"
        "        exit 1\n"
        "fi\n"
        "\n"
        "echo \"... enabling service service\"\n"
        "\n"
        "# ----------------------------------------------------------------------------\n"
        "#       ENABLE THE SERVICE\n"
        "# ----------------------------------------------------------------------------\n"
        "\n"
        "systemctl enable $MODNAME.service\n"
        "if [ $? -eq 0 ]; then\n"
        "        echo \"$MODNAME.service enabled\"\n"
        "        systemctl start $MODNAME.socket\n"
        "        if [ $? -eq 0 ]; then\n"
        "                echo \"$MODNAME.service started\"\n"
        "                echo \"check with 'systemctl status $MODNAME.service'\"\n"
        "        else\n"
        "                echo \"$MODNAME serivce not started ... stopping.\"\n"
        "                exit 1\n"
        "        fi\n"
        "else\n"
        "        echo \"$MODNAME serivce not enabled ... stopping.\"\n"
        "        exit 1\n"
        "fi\n"
        "\n"
        "# ----------------------------------------------------------------------------\n"
        "#       COPY ADDITIONAL SERVICE CONFIGURATION DATA (EXAMPLE)\n"
        "# ----------------------------------------------------------------------------\n"
        "\n"
        "# copy additional configuration data\n"
        "mkdir -p $CONFIGDIR\n"
        "mkdir -p $CONFIGDIR\"/data\"\n"
        "cp -r ${DIR}/../config/data/*      $CONFIGDIR\"/data\" 2>/dev/null || :\n"
        "cp -r ${DIR}/../config/*.config  $CONFIGDIR 2>/dev/null || :\n"
        "cp -r ${DIR}/../config/*.conf    $CONFIGDIR 2>/dev/null || :\n"
        "cp -r ${DIR}/../config/*.ini     $CONFIGDIR 2>/dev/null || :\n"
        "cp -r ${DIR}/../config/*.cfg     $CONFIGDIR 2>/dev/null || :\n"
        , TESTRUN_FILE_SERVICE_INSTALL,
        date,
        config.project.name,
        config.project.name,
        config.project.name,
        config.project.name,
        TESTRUN_FILE_SERVICE_INSTALL,
        date,
        config.project.name
        );

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));

        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_script_service_uninstall_content(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        testrun_config config = testrun_config_default();

        char *result = testrun_lib_script_service_uninstall_content(config);
        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);

        snprintf(expect, size,
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright 2017 Markus Toepfer\n"
        "#\n"
        "#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
        "#       you may not use this file except in compliance with the License.\n"
        "#       You may obtain a copy of the License at\n"
        "#\n"
        "#               http://www.apache.org/licenses/LICENSE-2.0\n"
        "#\n"
        "#       Unless required by applicable law or agreed to in writing, software\n"
        "#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
        "#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
        "#       See the License for the specific language governing permissions and\n"
        "#       limitations under the License.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Disable and deinstall the service.\n"
        "#\n"
        "#                       THIS SCRIPT MUST BE RUN AS ROOT.\n"
        "#\n"
        "#       Usage           ./%s /path/to/project\n"
        "#\n"
        "#       Dependencies    bash systemctl grep cp\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "MODNAME=%s\n"
        "CONFIGDIR=\"/etc/$MODNAME\"\n"
        "\n"
        "# Make sure only root can run our script\n"
        "if [[ $EUID -ne 0 ]]; then\n"
        "        echo \"This script must be run as root\" 1>&2\n"
        "        exit 1\n"
        "fi\n"
        "\n"
        "# ----------------------------------------------------------------------------\n"
        "#       SERVICE SHUTDOWN\n"
        "# ----------------------------------------------------------------------------\n"
        "\n"
        "echo \"Stopping $MODNAME.service\"\n"
        "systemctl stop $MODNAME.service\n"
        "if [ $? -eq 0 ]; then\n"
        "        echo \"... service stopped.\";\n"
        "else\n"
        "        echo \"... service was not running.\"\n"
        "fi\n"
        "\n"
        "echo \"Disabling $MODNAME.service\"\n"
        "systemctl disable $MODNAME.service\n"
        "if [ $? -eq 0 ]; then\n"
        "        echo \"... service disabled.\";\n"
        "else\n"
        "        echo \"... service was not enabled.\"\n"
        "fi\n"
        "\n"
        "# ----------------------------------------------------------------------------\n"
        "#       SOCKET SHUTDOWN\n"
        "# ----------------------------------------------------------------------------\n"
        "\n"
        "echo \"Stopping $MODNAME.socket\"\n"
        "systemctl stop $MODNAME.socket\n"
        "if [ $? -eq 0 ]; then\n"
        "        echo \"... socket stopped.\";\n"
        "else\n"
        "        echo \"... socket was not running.\"\n"
        "fi\n"
        "\n"
        "echo \"Disabling $MODNAME.socket\"\n"
        "systemctl disable $MODNAME.socket\n"
        "if [ $? -eq 0 ]; then\n"
        "        echo \"... socket disabled.\";\n"
        "else\n"
        "        echo \"... socket was not enabled.\"\n"
        "fi\n"
        "\n"
        "# ----------------------------------------------------------------------------\n"
        "#       REMOVE CONFIG\n"
        "# ----------------------------------------------------------------------------\n"
        "\n"
        "echo \"Removing systemd files\"\n"
        "rm -rf /etc/systemd/system/$MODNAME*\n"
        "if [ $? -eq 0 ]; then\n"
        "        echo \"... removed /etc/systemd/system/$MODNAME*\";\n"
        "else\n"
        "        echo \"... WARN check and remove /etc/systemd/system/$MODNAME*\";\n"
        "fi\n"
        "echo \"uninstall done.\"\n"
        "\n"
        "echo \"Removing executable files\"\n"
        "rm -rf /usr/local/bin/$MODNAME\n"
        "if [ $? -eq 0 ]; then\n"
        "        echo \"... removed /usr/local/bin/$MODNAME\";\n"
        "else\n"
        "        echo \"... WARN check and remove /usr/local/bin/$MODNAME\";\n"
        "fi\n"
        "echo \"uninstall done.\"\n"
        "\n"
        "echo \"Removing config files\"\n"
        "rm -rf /etc/$MODNAME*\n"
        "if [ $? -eq 0 ]; then\n"
        "        echo \"... removed /etc/$MODNAME*\";\n"
        "else\n"
        "        echo \"... WARN check and remove /etc/$MODNAME*\";\n"
        "fi\n"
        "echo \"uninstall done.\"\n"
        "\n"
        "# ----------------------------------------------------------------------------\n"
        "#       RELOAD SYSTEMCTL DAEMON\n"
        "# ----------------------------------------------------------------------------\n"
        "\n"
        "systemctl daemon-reload\n"
        "echo \"Systemd daemon reload performed.\"\n"
        , TESTRUN_FILE_SERVICE_UNINSTALL,
        date,
        config.project.name,
        TESTRUN_FILE_SERVICE_UNINSTALL,
        date,
        config.project.name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));

        testrun(strlen(result) == strlen(expect));
        testrun(strncmp(result, expect, strlen(expect))== 0);

        date   = testrun_string_free(date);
        result = testrun_string_free(result);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_path(){

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        DIR *dp;

        testrun_config config = testrun_config_default();
        config.project.path = "./build/test/check";

        testrun(!testrun_lib_create_path(NULL, 0, NULL, NULL));
        testrun(!testrun_lib_create_path(
                NULL, PATH_MAX, &config, "config"));
        testrun(!testrun_lib_create_path(
                buffer, PATH_MAX, NULL, "config"));
        testrun(!testrun_lib_create_path(
                buffer, PATH_MAX, &config, NULL));
        testrun(!testrun_lib_create_path(
                buffer, 0, &config, "config"));

        dp = opendir("./build/test/check/");
        testrun(!dp);

        dp = opendir("./build/test/check/config/");
        testrun(!dp);

        dp = opendir("./build/test/check/docs/");
        testrun(!dp);

        testrun(testrun_lib_create_path(
                buffer, PATH_MAX, &config, "config"));

        dp = opendir("./build/test/check/");
        testrun(dp);
        (void) closedir (dp);

        dp = opendir("./build/test/check/config/");
        testrun(dp);
        (void) closedir (dp);

        dp = opendir("./build/test/check/docs/");
        testrun(!dp);



        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_folder_structure(){

        DIR *dp;

        testrun_config config = testrun_config_default();
        config.project.path   = "./build/test/check";

        testrun(testrun_lib_create_folder_structure(&config));
        dp = opendir("./build/test/check/config/");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("./build/test/check/config/data");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("./build/test/check/copyright/");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("./build/test/check/docs/");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("./build/test/check/doxygen/");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("./build/test/check/include/");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("./build/test/check/src/");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("./build/test/check/tests/");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("./build/test/check/tests/acceptance");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("./build/test/check/tests/resources");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("./build/test/check/tests/tools");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("./build/test/check/tests/unit");
        testrun(dp);
        (void) closedir (dp);
        testrun(system("rm -rf ./build/test/check") == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_file_content_function(){

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char *path = NULL;

        testrun_config config = testrun_config_default();
        config.project.path = "./build/test";

        path = "./build/test/makefile.test";
        testrun( -1 == access(path, F_OK));

        testrun(testrun_lib_create_file_content_function(
                "makefile.test", &config, NULL,
                testrun_lib_makefile_content));

        testrun( -1 != access(path, F_OK), "failed to access tmp file");
        testrun(0 == unlink(path), "failed to delete tmp file");

        testrun(!testrun_lib_create_file_content_function(
                NULL, &config, NULL,
                testrun_lib_makefile_content));
        testrun(!testrun_lib_create_file_content_function(
                "makefile.test", NULL, NULL,
                testrun_lib_makefile_content));
        testrun(!testrun_lib_create_file_content_function(
                "makefile.test", &config, NULL,
                NULL));

        config.project.path = "./build/test/nodir";
        testrun(!testrun_lib_create_file_content_function(
                "makefile.test", &config, NULL,
                testrun_lib_makefile_content), "path not existing");

        config.project.path = "./build/test";
        testrun(testrun_lib_create_file_content_function(
                "makefile.test", &config, NULL,
                testrun_lib_makefile_content));

        testrun( -1 != access(path, F_OK), "failed to access tmp file");

        testrun(!testrun_lib_create_file_content_function(
                "makefile.test", &config, NULL,
                testrun_lib_makefile_content), "file extists");

        testrun(0 == unlink(path), "failed to delete tmp file");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_file(){

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char *content = "TESTCONTENT";
        char reread[1000];
        bzero(reread, 1000);

        char *path = NULL;
        FILE *file;

        testrun_config config = testrun_config_default();
        config.project.path = "./build/test";

        path = "./build/test/testfile";
        testrun( -1 == access(path, F_OK));

        testrun(testrun_lib_create_file(
                "testfile", content, &config, NULL));

        testrun( -1 != access(path, F_OK), "failed to access tmp file");

        testrun(!testrun_lib_create_file(
                "testfile", content, &config, NULL), "file exists");

        // reread the content
        file = fopen(path, "r");
        testrun(file);
        testrun(fgets(reread, 1000, file));
        fclose(file);

        testrun(strncmp(reread, content, strlen(content)) == 0);
        testrun(strlen(reread) == strlen(content));

        testrun(0 == unlink(path), "failed to delete tmp file");


        testrun(!testrun_lib_create_file(
                NULL, content, &config, NULL));
        testrun(!testrun_lib_create_file(
                "testfile", NULL, &config, NULL));
        testrun(!testrun_lib_create_file(
                "testfile", content, NULL, NULL));

        config.project.path = "./build/test/nodir";
        testrun(!testrun_lib_create_file(
                "testfile", content, &config, NULL), "path not existing");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_test_tools(){

        char *path = "./build/test/x";
        testrun_config config = testrun_config_default();

        DIR *dp;

        config.project.path = path;

        testrun(system("rm -rf ./build/test/x") == 0);

        testrun(testrun_lib_create_folder_structure(&config));
        dp = opendir("./build/test/x/tests/");
        testrun(dp, "check directory for test tools exists");
        (void) closedir (dp);


        testrun(!testrun_lib_create_test_tools(NULL));

        testrun( -1 == access(
                "./build/test/x/tests/tools/testrun.h",
                F_OK));
        testrun( -1 == access(
                "./build/test/x/tests/tools/testrun_runner.sh",
                F_OK));
        testrun( -1 == access(
                "./build/test/x/tests/tools/testrun_simple_acceptance_tests.sh",
                F_OK));
        testrun( -1 == access(
                "./build/test/x/tests/tools/testrun_simple_unit_tests.sh",
                F_OK));
        testrun( -1 == access(
                "./build/test/x/tests/tools/testrun_simple_coverage_tests.sh",
                F_OK));
        testrun( -1 == access(
                "./build/test/x/tests/tools/testrun_simple_loc.sh",
                F_OK));

        testrun(testrun_lib_create_test_tools(&config));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun.h",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_runner.sh",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_acceptance_tests.sh",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_unit_tests.sh",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_coverage_tests.sh",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_loc.sh",
                F_OK));

        testrun(!testrun_lib_create_test_tools(&config), "files exist");

        testrun(system("rm -rf ./build/test/x") == 0);

        testrun(!testrun_lib_create_test_tools(&config), "folder does not exist");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_module_files(){

        char *path = "./build/test/x";
        testrun_config config = testrun_config_default();

        DIR *dp;

        config.project.path = path;

        testrun(system("rm -rf ./build/test/x") == 0);

        testrun(testrun_lib_create_folder_structure(&config));
        dp = opendir("./build/test/x/tests/unit");
        testrun(dp, "check directory unit tests");
        (void) closedir (dp);
        dp = opendir("./build/test/x/include/");
        testrun(dp, "check directory include");
        (void) closedir (dp);
        dp = opendir("./build/test/x/src/");
        testrun(dp, "check directory src");
        (void) closedir (dp);


        testrun(!testrun_lib_create_module_files(NULL,   NULL));
        testrun(!testrun_lib_create_module_files("test", NULL));
        testrun(!testrun_lib_create_module_files(NULL,   &config));

        testrun( -1 == access(
                "./build/test/x/include/test.h",
                F_OK));
        testrun( -1 == access(
                "./build/test/x/src/test.c",
                F_OK));
        testrun( -1 == access(
                "./build/test/x/tests/unit/test_test.c",
                F_OK));

        testrun(testrun_lib_create_module_files("test", &config));

        testrun( -1 != access(
                "./build/test/x/include/test.h",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/src/test.c",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/tests/unit/test_tests.c",
                F_OK));

        testrun(!testrun_lib_create_module_files("test", &config),
                "files exist");

        testrun(system("rm -rf ./build/test/x") == 0);

        testrun(!testrun_lib_create_module_files("test", &config),
                "folder does not exist");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_config_files(){

        char *path = "./build/test/x";
        testrun_config config = testrun_config_default();
        config.project.name = "project";

        DIR *dp;

        config.project.path = path;

        testrun(system("rm -rf ./build/test/x") == 0);

        testrun(testrun_lib_create_folder_structure(&config));
        dp = opendir("./build/test/x/config");
        testrun(dp, "check directory config");
        (void) closedir (dp);
        dp = opendir("./build/test/x/config/data");
        testrun(dp, "check directory config data");
        (void) closedir (dp);

        // test with project service folder

        testrun(!testrun_lib_create_config_files(NULL));

        testrun( -1 == access(
                "./build/test/x/config/install/install.sh",
                F_OK));
        testrun( -1 == access(
                "./build/test/x/config/install/uninstall.sh",
                F_OK));
        testrun( -1 == access(
                "./build/test/x/config/install/project.service",
                F_OK));
        testrun( -1 == access(
                "./build/test/x/config/install/project.socket",
                F_OK));

        testrun(testrun_lib_create_config_files(&config));

        testrun( -1 != access(
                "./build/test/x/config/install/install.sh",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/config/install/uninstall.sh",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/config/install/project.service",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/config/install/project.socket",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/config/data/readme",
                F_OK));

        testrun(!testrun_lib_create_config_files(&config),
                "files exist");

        testrun(system("rm -rf ./build/test/x") == 0);

        testrun(!testrun_lib_create_config_files(&config),
                "folder does not exist");

        testrun(system("rm -rf ./build/test/x") == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_copyright_files(){

        testrun_config config = testrun_config_default();
        config.project.name = "project";

        char *path = "./build/test/x";
        char full[PATH_MAX];
        bzero(full, PATH_MAX);

        DIR *dp;

        config.project.path = path;
        path = "./build/test/x/copyright";

        testrun(system("rm -rf ./build/test/x") == 0);

        testrun(testrun_lib_create_folder_structure(&config));
        dp = opendir("./build/test/x/copyright");
        testrun(dp, "check directory copyright");
        (void) closedir (dp);

        testrun(!testrun_lib_create_copyright_files(NULL));

        testrun(testrun_lib_create_copyright_files(&config));


        testrun( -1 != access(
                "./build/test/x/copyright/copyright",
                F_OK));

        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_APACHE);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_GPL);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_LGPL);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_AGPL);
        testrun( -1 == access( full, F_OK));
        testrun(system("rm -rf ./build/test/x") == 0);

        config.copyright = testrun_copyright_apache_version_2("x", "y", NULL);
        testrun(testrun_lib_create_folder_structure(&config));
        dp = opendir("./build/test/x/copyright");
        testrun(dp, "check directory copyright");
        (void) closedir (dp);
        testrun(testrun_lib_create_copyright_files(&config));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_FILE_COPYRIGHT);
        testrun( -1 != access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_APACHE);
        testrun( -1 != access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_GPL);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_LGPL);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_AGPL);
        testrun( -1 == access( full, F_OK));
        testrun(system("rm -rf ./build/test/x") == 0);

        config.copyright = testrun_copyright_GPL_v3("x", "y", NULL, "z", GENERAL);
        testrun(testrun_lib_create_folder_structure(&config));
        dp = opendir("./build/test/x/copyright");
        testrun(dp, "check directory copyright");
        (void) closedir (dp);
        testrun(testrun_lib_create_copyright_files(&config));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_FILE_COPYRIGHT);
        testrun( -1 != access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_APACHE);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_GPL);
        testrun( -1 != access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_LGPL);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_AGPL);
        testrun( -1 == access( full, F_OK));
        testrun(system("rm -rf ./build/test/x") == 0);

        config.copyright = testrun_copyright_GPL_v3("x", "y", NULL, "z", LESSER);
        testrun(testrun_lib_create_folder_structure(&config));
        dp = opendir("./build/test/x/copyright");
        testrun(dp, "check directory copyright");
        (void) closedir (dp);
        testrun(testrun_lib_create_copyright_files(&config));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_FILE_COPYRIGHT);
        testrun( -1 != access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_APACHE);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_GPL);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_LGPL);
        testrun( -1 != access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_AGPL);
        testrun( -1 == access( full, F_OK));
        testrun(system("rm -rf ./build/test/x") == 0);

        config.copyright = testrun_copyright_GPL_v3("x", "y", NULL, "z", AFFERO);
        testrun(testrun_lib_create_folder_structure(&config));
        dp = opendir("./build/test/x/copyright");
        testrun(dp, "check directory copyright");
        (void) closedir (dp);
        testrun(testrun_lib_create_copyright_files(&config));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_FILE_COPYRIGHT);
        testrun( -1 != access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_APACHE);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_GPL);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_LGPL);
        testrun( -1 == access( full, F_OK));
        snprintf(full, PATH_MAX, "%s/%s", path, TESTRUN_COPYRIGHT_FILE_AGPL);
        testrun( -1 != access( full, F_OK));
        testrun(system("rm -rf ./build/test/x") == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_files(){

        char reread[1000];
        bzero(reread, 1000);

        char *path = "./build/test/x";
        testrun_config config = testrun_config_default();

        DIR *dp;

        config.project.path = path;

        testrun(system("rm -rf ./build/test/x") == 0);

        testrun(testrun_lib_create_folder_structure(&config));
        dp = opendir("./build/test/x/config");
        testrun(dp, "check directory config");
        (void) closedir (dp);
        dp = opendir("./build/test/x/config/data");
        testrun(dp, "check directory config data");
        (void) closedir (dp);

        testrun(!testrun_lib_create_files(NULL, NULL));
        testrun(!testrun_lib_create_files(NULL, "readme"));

        testrun(testrun_lib_create_files(&config, NULL));

        // makefiles
        testrun( -1 != access(
                "./build/test/x/makefile",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/makefile_general.mk",
                F_OK));

        // gitignore
        testrun( -1 != access(
                "./build/test/x/.gitignore",
                F_OK));

        // readme
        testrun( -1 != access(
                "./build/test/x/README.MD",
                F_OK));

        // copyright
        testrun( -1 != access(
                "./build/test/x/copyright/copyright",
                F_OK));

        // changelog
        testrun( -1 != access(
                "./build/test/x/docs/CHANGELOG",
                F_OK));

        // doxygen
        testrun( -1 != access(
                "./build/test/x/doxygen/doxygen.config",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/config/data/readme",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/config/install/install.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/config/install/uninstall.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/config/install/[PROJECT].socket",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/config/install/[PROJECT].service",
                F_OK));

        // tools

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun.h",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_runner.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_acceptance_tests.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_unit_tests.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_coverage_tests.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_loc.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_gcov.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_gprof.sh",
                F_OK));

        // module files

        testrun( -1 != access(
                "./build/test/x/include/[PROJECT].h",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/src/[PROJECT].c",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/unit/[PROJECT]_tests.c",
                F_OK));

        testrun(system("rm -rf ./build/test/x") == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_project(){

        char reread[1000];
        bzero(reread, 1000);

        char *path = "./build/test/x";
        testrun_config config = testrun_config_default();

        DIR *dp;
        FILE *file;

        config.project.path = path;

        testrun(system("rm -rf ./build/test/x") == 0);

        testrun(!testrun_lib_create_project(NULL, NULL));
        testrun(!testrun_lib_create_project(NULL, "readme"));

        testrun(testrun_lib_create_project(&config, "readme"));
        dp = opendir("./build/test/x/config");
        testrun(dp, "check directory config");
        (void) closedir (dp);
        dp = opendir("./build/test/x/config/data");
        testrun(dp, "check directory config data");
        (void) closedir (dp);

        // makefiles
        testrun( -1 != access(
                "./build/test/x/makefile",
                F_OK));
        testrun( -1 != access(
                "./build/test/x/makefile_general.mk",
                F_OK));

        // readme
        testrun( -1 != access(
                "./build/test/x/README.MD",
                F_OK));

        // copyright
        testrun( -1 != access(
                "./build/test/x/copyright/copyright",
                F_OK));

        // changelog
        testrun( -1 != access(
                "./build/test/x/docs/CHANGELOG",
                F_OK));

        // doxygen
        testrun( -1 != access(
                "./build/test/x/doxygen/doxygen.config",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/config/data/readme",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/config/install/install.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/config/install/uninstall.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/config/install/[PROJECT].socket",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/config/install/[PROJECT].service",
                F_OK));

        // tools

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun.h",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_runner.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_acceptance_tests.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_unit_tests.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_coverage_tests.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_simple_loc.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_gcov.sh",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/tools/testrun_gprof.sh",
                F_OK));

        // module files

        testrun( -1 != access(
                "./build/test/x/include/[PROJECT].h",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/src/[PROJECT].c",
                F_OK));

        testrun( -1 != access(
                "./build/test/x/tests/unit/[PROJECT]_tests.c",
                F_OK));

        // check readme content
        file = fopen("./build/test/x/README.MD", "r");
        testrun(file);
        testrun(fgets(reread, 1000, file));
        fclose(file);
        testrun(strncmp(reread, "readme", 6) == 0);
        testrun(strlen(reread) == 6);

        testrun(system("rm -rf ./build/test/x") == 0);


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
        testrun_test(test_testrun_lib_script_gcov_tests_content);
        testrun_test(test_testrun_lib_script_gprof_tests_content);
        testrun_test(test_testrun_lib_c_file_content);

        testrun_test(test_testrun_lib_makefile_content);
        testrun_test(test_testrun_lib_makefile_general_content);

        testrun_test(test_testrun_lib_script_service_install_content);
        testrun_test(test_testrun_lib_script_service_uninstall_content);

        testrun_test(test_testrun_lib_create_path);
        testrun_test(test_testrun_lib_create_folder_structure);

        testrun_test(test_testrun_lib_create_file_content_function);
        testrun_test(test_testrun_lib_create_file);

        testrun_test(test_testrun_lib_create_test_tools);
        testrun_test(test_testrun_lib_create_module_files);
        testrun_test(test_testrun_lib_create_config_files);
        testrun_test(test_testrun_lib_create_copyright_files);

        testrun_test(test_testrun_lib_create_files);
        testrun_test(test_testrun_lib_create_project);


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
