/***
        ------------------------------------------------------------------------

        Copyright 2018 Markus Toepfer

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.

        ------------------------------------------------------------------------
*//**
        @file           testrun_tools_test.c
        @author         Markus Toepfer
        @date           2018-07-10

        @ingroup        testrun_lib

        @brief          Unit tests for testrun_tools.


        ------------------------------------------------------------------------
*/

#include "../tools/testrun.h"
#include "../../src/testrun_tools.c"

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST HELPER                                                     #HELPER
 *
 *      ------------------------------------------------------------------------
 */

#include "helper_testrun_header_constant.c"
#include "helper_testrun_header_openmp_constant.c"

/*----------------------------------------------------------------------------*/

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------*/

int test_testrun_generate_header(){

        char *result = testrun_generate_header();
        testrun(result);
        testrun(0 == strncmp(result,
                helper_testrun_header,
                strlen(helper_testrun_header)));
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_header_openmp(){

        char *result = testrun_generate_header_openmp();
        testrun(result);
        testrun(0 == strncmp(result,
                helper_testrun_header_openmp,
                strlen(helper_testrun_header_openmp)));
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_script_simple_tests(){

        char *type         = "UNIT";
        char *project      = "project";
        char *file_name    = "script.sh";
        char *runner       = "runner.sh";
        char *path_logfile = "some/path";
        char *path_tests   = "tests/path";
        char *path_tools   = "tests/tools";

        char *result       = NULL;

        size_t size = 5000;
        char buffer[size];

        testrun(!testrun_generate_script_simple_tests(
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL));

        testrun(!testrun_generate_script_simple_tests(
                type,
                NULL,
                file_name,
                runner,
                path_logfile,
                path_tests,
                path_tools));

        testrun(!testrun_generate_script_simple_tests(
                NULL,
                project,
                file_name,
                runner,
                path_logfile,
                path_tests,
                path_tools));


        testrun(!testrun_generate_script_simple_tests(
                type,
                project,
                NULL,
                runner,
                path_logfile,
                path_tests,
                path_tools));

        testrun(!testrun_generate_script_simple_tests(
                type,
                project,
                file_name,
                NULL,
                path_logfile,
                path_tests,
                path_tools));

        testrun(!testrun_generate_script_simple_tests(
                type,
                project,
                file_name,
                runner,
                NULL,
                path_tests,
                path_tools));

        testrun(!testrun_generate_script_simple_tests(
                type,
                project,
                file_name,
                runner,
                path_logfile,
                NULL,
                path_tools));

        testrun(!testrun_generate_script_simple_tests(
                type,
                project,
                file_name,
                runner,
                path_logfile,
                path_tests,
                NULL));

        result = testrun_generate_script_simple_tests(
                type,
                project,
                file_name,
                runner,
                path_logfile,
                path_tests,
                path_tools);

        testrun(result);

        memset(buffer, 0, size);
        testrun(0 < snprintf(buffer, size,
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2017-11-30\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Run all test executables [PATH_TESTS]/*.test\n"
        "#                       Run the whole folder, until an error occurs.\n"
        "#\n"
        "#                       MODE         FAIL ON ERROR (Fail on first test error)\n"
        "#\n"
        "#                       LOGFILE      [PATH_LOGFILE]/%s.<time>.log\n"
        "#\n"
        "#\n"
        "#       Usage           ./%s /path/to/project\n"
        "#\n"
        "#       Dependencies    bash, touch, chmod, ls, wc, date\n"
        "#\n"
        "#       Last changed    2018-07-11\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "TEST_TYPE=\"%s\"\n"
        "FOLDER_LOGFILE=\"%s\"\n"
        "FOLDER_TESTS=\"%s\"\n"
        "RUNNER_SCRIPT=\"./%s/%s\"\n"
        "\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE $TEST_TYPE TESTING\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"$FOLDER_LOGFILE/$TEST_TYPE.$start_time.log\"\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               REPORT $TEST_TYPE TESTING\"                >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "# RUN THE RUNNER\n"
        "sh $RUNNER_SCRIPT $LOGFILE $FOLDER_TESTS FAIL_ON_ERROR\n"
        "RESULT=$?\n"
        "\n"
        "end_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# FINISH THE REPORT\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"DONE \\t $TEST_TYPE TEST RUN\" >> $LOGFILE\n"
        "if [ $RESULT -eq 0 ]; then\n"
        "        echo \"RESULT\\t SUCCESS\" >> $LOGFILE\n"
        "else\n"
        "        echo \"RESULT\\t FAILURE\" >> $LOGFILE\n"
        "fi\n"
        "echo \"START \\t $start_time\" >> $LOGFILE\n"
        "echo \"END   \\t $end_time\" >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "# DUMP THE REPORT ON SUCCESS\n"
        "if [ $RESULT -eq 0 ]; then\n"
        "        cat $LOGFILE\n"
        "        echo \"\"\n"
        "else\n"
        "        echo \"\"\n"
        "        echo \"$TEST_TYPE TEST FAILED\"\n"
        "        echo \"Logfile dump stopped to point to last error.\"\n"
        "fi\n"
        "exit $RESULT\n",
                bash_header,
                file_name,
                project,
                type,
                file_name,
                type,
                path_logfile,
                path_tests,
                path_tools,
                runner
                ));

        testrun(0 == strncmp(result, buffer, strlen(buffer)));
        free(result);

        // check with default setup

        type         = "UNIT";
        project      = "testrun_simple_unit_tests.sh";
        file_name    = "testrun_simple_unit_tests.sh";
        runner       = "testrun_runner.sh";
        path_logfile = "build/tests/log/";
        path_tests   = "build/tests/unit";
        path_tools   = "tests/tools";

        result = testrun_generate_script_simple_tests(
                type,
                project,
                file_name,
                runner,
                path_logfile,
                path_tests,
                path_tools);

        testrun(result);

        memset(buffer, 0, size);
        testrun(0 < snprintf(buffer, size,
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
        "#       File            testrun_simple_unit_tests.sh\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2017-11-30\n"
        "#\n"
        "#       Project         testrun_simple_unit_tests.sh\n"
        "#\n"
        "#       Description     Run all test executables [PATH_TESTS]/*.test\n"
        "#                       Run the whole folder, until an error occurs.\n"
        "#\n"
        "#                       MODE         FAIL ON ERROR (Fail on first test error)\n"
        "#\n"
        "#                       LOGFILE      [PATH_LOGFILE]/UNIT.<time>.log\n"
        "#\n"
        "#\n"
        "#       Usage           ./testrun_simple_unit_tests.sh /path/to/project\n"
        "#\n"
        "#       Dependencies    bash, touch, chmod, ls, wc, date\n"
        "#\n"
        "#       Last changed    2018-07-11\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "TEST_TYPE=\"UNIT\"\n"
        "FOLDER_LOGFILE=\"build/tests/log/\"\n"
        "FOLDER_TESTS=\"build/tests/unit\"\n"
        "RUNNER_SCRIPT=\"./tests/tools/testrun_runner.sh\"\n"
        "\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE $TEST_TYPE TESTING\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"$FOLDER_LOGFILE/$TEST_TYPE.$start_time.log\"\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               REPORT $TEST_TYPE TESTING\"                >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "# RUN THE RUNNER\n"
        "sh $RUNNER_SCRIPT $LOGFILE $FOLDER_TESTS FAIL_ON_ERROR\n"
        "RESULT=$?\n"
        "\n"
        "end_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# FINISH THE REPORT\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"DONE \\t $TEST_TYPE TEST RUN\" >> $LOGFILE\n"
        "if [ $RESULT -eq 0 ]; then\n"
        "        echo \"RESULT\\t SUCCESS\" >> $LOGFILE\n"
        "else\n"
        "        echo \"RESULT\\t FAILURE\" >> $LOGFILE\n"
        "fi\n"
        "echo \"START \\t $start_time\" >> $LOGFILE\n"
        "echo \"END   \\t $end_time\" >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "# DUMP THE REPORT ON SUCCESS\n"
        "if [ $RESULT -eq 0 ]; then\n"
        "        cat $LOGFILE\n"
        "        echo \"\"\n"
        "else\n"
        "        echo \"\"\n"
        "        echo \"$TEST_TYPE TEST FAILED\"\n"
        "        echo \"Logfile dump stopped to point to last error.\"\n"
        "fi\n"
        "exit $RESULT\n"));

        testrun(0 == strncmp(result, buffer, strlen(buffer)));
        free(result);

        /*
         *      -----------------------------------------------------------------
         *
         *      DEFAULT OUTPUTS TO COPY/PASTE TO ACTUAL SCRIPTS OF TESTRUN.INFO
         *
         *      -----------------------------------------------------------------
         */

        type         = "ACCEPTANCE";
        project      = "testrun_simple_acceptance_tests.sh";
        file_name    = "testrun_simple_acceptance_tests.sh";
        runner       = "./tests/tools/testrun_runner.sh";
        path_logfile = "build/tests/log/";
        path_tests   = "build/tests/acceptance";

        result = testrun_generate_script_simple_tests(
                type,
                project,
                file_name,
                runner,
                path_logfile,
                path_tests,
                path_tools);

        testrun(result);
        //testrun_log("%s", result);
        free(result);

        type         = "UNIT";
        project      = "testrun.info";
        file_name    = "testrun_simple_unit_tests.sh";
        runner       = "./tests/tools/testrun_runner.sh";
        path_logfile = "build/tests/log/";
        path_tests   = "build/tests/unit";

        result = testrun_generate_script_simple_tests(
                type,
                project,
                file_name,
                runner,
                path_logfile,
                path_tests,
                path_tools);

        testrun(result);
        //testrun_log("%s", result);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_script_runner(){

        char *project      = "project";
        char *file_name    = "runner.sh";
        char *result       = NULL;

        size_t size = 5000;
        char buffer[size];

        testrun(!testrun_generate_script_runner(NULL,    NULL));
        testrun(!testrun_generate_script_runner(project, NULL));
        testrun(!testrun_generate_script_runner(NULL,    file_name));

        result = testrun_generate_script_runner(project, file_name);
        testrun(result);
        //testrun_log("%s", result);

        memset(buffer, 0, size);
        testrun(0 < snprintf(buffer, size,
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2017-11-30\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Run each TEST.test of a folder and log Ok or NOK\n"
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
        "#       Last changed    2017-11-30\n"
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
        "exit 0\n",
                bash_header,
                file_name,
                project));

        testrun(0 == strncmp(result, buffer, strlen(buffer)));
        free(result);

        /*
         *      -----------------------------------------------------------------
         *
         *      DEFAULT OUTPUTS TO COPY/PASTE TO ACTUAL SCRIPTS OF TESTRUN.INFO
         *
         *      -----------------------------------------------------------------
         */

        project   = "testrun.info";
        file_name = "testrun_runner.sh";

        result = testrun_generate_script_runner(project, file_name);
        testrun(result);
        //testrun_log("%s", result);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_script_loc(){

        char *project      = "project";
        char *file_name    = "runner.sh";
        char *path_header  = "project/include";
        char *path_source  = "project/src";
        char *path_tests   = "project/tests/unit";
        char *result       = NULL;

        size_t size = 5000;
        char buffer[size];

        testrun(!testrun_generate_script_loc(
                NULL,
                NULL,
                NULL,
                NULL,
                NULL));

        testrun(!testrun_generate_script_loc(
                NULL,
                file_name,
                path_header,
                path_source,
                path_tests));

        testrun(!testrun_generate_script_loc(
                project,
                NULL,
                path_header,
                path_source,
                path_tests));

        testrun(!testrun_generate_script_loc(
                project,
                file_name,
                NULL,
                path_source,
                path_tests));

        testrun(!testrun_generate_script_loc(
                project,
                file_name,
                path_header,
                NULL,
                path_tests));

        testrun(!testrun_generate_script_loc(
                project,
                file_name,
                path_header,
                path_source,
                NULL));

        result = testrun_generate_script_loc(
                project,
                file_name,
                path_header,
                path_source,
                path_tests);

        testrun(result);

        memset(buffer, 0, size);
        testrun(0 < snprintf(buffer, size,
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2017-11-30\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Count the lines of header, src and tests.\n"
        "#                       This file uses no error checking.\n"
        "#\n"
        "#       Usage           ./%s /path/to/project\n"
        "#\n"
        "#       Dependencies    bash, find, xargs, wc\n"
        "#\n"
        "#       Last changed    2018-07-11\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "FOLDER_INC=\"%s\"\n"
        "FOLDER_SRC=\"%s\"\n"
        "FOLDER_TST=\"%s\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE LOC COUNTER\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"\"\n"
        "echo \"(LOC) HEADER\"\n"
        "find $1/$FOLDER_INC -name '*.h' | xargs wc -l\n"
        "echo \"\"\n"
        "echo \"(LOC) SOURCE\"\n"
        "find $1/$FOLDER_SRC -name '*.c' | xargs wc -l\n"
        "echo \"\"\n"
        "echo \"(LOC) TESTS\"\n"
        "find $1/$FOLDER_TST -name '*.c' | xargs wc -l\n"
        "echo \"\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"\"\n",
                bash_header,
                file_name,
                project,
                file_name,
                path_header,
                path_source,
                path_tests
                ));

        testrun(0 == strncmp(result, buffer, strlen(buffer)));
        free(result);

        /*
         *      -----------------------------------------------------------------
         *
         *      DEFAULT OUTPUTS TO COPY/PASTE TO ACTUAL SCRIPTS OF TESTRUN.INFO
         *
         *      -----------------------------------------------------------------
         */

        project     = "testrun.info";
        file_name   = "testrun_simple_loc.sh";
        path_header = "include";
        path_source = "src";
        path_tests  = "tests/unit";

        result = testrun_generate_script_loc(
                project,
                file_name,
                path_header,
                path_source,
                path_tests);

        testrun(result);
        //testrun_log("%s", result);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_script_coverage(){

        char *project      = "project";
        char *file_name    = "runner.sh";
        char *src_prefix   = "src_";
        char *test_prefix  = "test_";
        char *path_logfile = "project/build/tests/log";
        char *path_source  = "project/src";
        char *path_tests   = "project/tests/unit";
        char *result       = NULL;

        size_t size = 5000;
        char buffer[size];

        testrun(!testrun_generate_script_coverage(
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL));

        testrun(!testrun_generate_script_coverage(
                NULL,
                file_name,
                src_prefix,
                test_prefix,
                path_logfile,
                path_source,
                path_tests));

        testrun(!testrun_generate_script_coverage(
                project,
                NULL,
                src_prefix,
                test_prefix,
                path_logfile,
                path_source,
                path_tests));

        testrun(!testrun_generate_script_coverage(
                project,
                file_name,
                src_prefix,
                NULL,
                path_logfile,
                path_source,
                path_tests));

        testrun(!testrun_generate_script_coverage(
                project,
                file_name,
                src_prefix,
                test_prefix,
                NULL,
                path_source,
                path_tests));

        testrun(!testrun_generate_script_coverage(
                project,
                file_name,
                src_prefix,
                test_prefix,
                path_logfile,
                NULL,
                path_tests));

        testrun(!testrun_generate_script_coverage(
                project,
                file_name,
                src_prefix,
                test_prefix,
                path_logfile,
                path_source,
                NULL));

        result = testrun_generate_script_coverage(
                project,
                file_name,
                NULL,
                test_prefix,
                path_logfile,
                path_source,
                path_tests);

        testrun(result);

        memset(buffer, 0, size);
        testrun(0 < snprintf(buffer, size,
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2017-11-30\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Count functions of folder src vs unit test functions.\n"
        "#\n"
        "#                       CONVENTION\n"
        "#\n"
        "#                       Each function in any file of the source folder located\n"
        "#                       \"%s\"\n"
        "#                       will have a corresponding test function,\n"
        "#                       using the same name in a file of the unit tests located at\n"
        "#                       \"%s\",\n"
        "#                       with a function name prefix of\n"
        "#                       \"%s\".\n"
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
        "#       Last changed    2018-07-11\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "SRC_PREFIX=\"%s\"\n"
        "TEST_PREFIX=\"%s\"\n"
        "\n"
        "SRCDIR=\"$1/%s\"\n"
        "TESTDIR=\"$1/%s\"\n"
        "FOLDER_LOGFILE=\"$1/%s\"\n"
        ""
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"$FOLDER_LOGFILE/coverage.$start_time.log\"\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               REPORT COVERAGE TESTING\"                  >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"   TIME \t $start_time\" >> $LOGFILE\n"
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
        "awk '{print $1 }' $SRCDIR/functions > $SRCDIR/functionNamesAll\n"
        "\n"
        "# CUSTOMIZATION delete everything, which is not prefixed with custom src prefixes\n"
        "#cat $SRCDIR/functionNamesAll | sed -ne '/^$SRC_PREFIX_.*/p'   > $SRCDIR/functionNames\n"
        "#cat $SRCDIR/functionNamesAll | sed -ne '/^impl_.*/p' >> $SRCDIR/functionNames\n"
        "cat $SRCDIR/functionNamesAll >> $SRCDIR/functionNames\n"
        "\n"
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
        "awk '{print $1 }' $TESTDIR/functions > $TESTDIR/functionNames\n"
        "\n"
        "# count the lines\n"
        "testFkt=\"$(cat functions | grep -i ^$TEST_PREFIX | wc -l)\"\n"
        "echo \"   count tests\\t\" $testFkt >> $LOGFILE\n"
        "\n"
        "echo \"\nUNTESTED: \" >> $LOGFILE\n"
        "# Found functions:\n"
        "while read line;\n"
        "do\n"
        "        grep -n '^'$TEST_PREFIX$line'$' $TESTDIR/functionNames > \\\n"
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
        "rm $TESTDIR/functionNames\n",
                bash_header,
                file_name,
                project,
                path_source,
                path_tests,
                test_prefix,
                file_name,
                src_prefix,
                test_prefix,
                path_source,
                path_tests,
                path_logfile
                ));

        //testrun_log("%s", result);
        testrun(0 == strncmp(result, buffer, strlen(buffer)));

        free(result);


        /*
         *      -----------------------------------------------------------------
         *
         *      DEFAULT OUTPUTS TO COPY/PASTE TO ACTUAL SCRIPTS OF TESTRUN.INFO
         *
         *      -----------------------------------------------------------------
         */

        project      = "testrun.info";
        file_name    = "testrun_simple_coverage.sh";
        path_logfile = "build/tests/log";
        path_source  = "src";
        path_tests   = "tests/unit";
        test_prefix  = "test_";
        src_prefix   = NULL;

        result = testrun_generate_script_coverage(
                project,
                file_name,
                src_prefix,
                test_prefix,
                path_logfile,
                path_source,
                path_tests);

        testrun(result);
        //testrun_log("%s", result);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_script_gcov(){

        char *project      = "project";
        char *file_name    = "runner.sh";
        char *path_logfile = "project/build/tests/log";
        char *path_exec    = "project/src";
        char *path_source  = "project/tests/unit";
        char *exec_suffix  = NULL;
        char *src_suffix   = NULL;
        char *result       = NULL;

        size_t size = 5000;
        char buffer[size];

        testrun(!testrun_generate_script_gcov(
                NULL,
                file_name,
                path_logfile,
                path_exec,
                path_source,
                NULL,
                NULL));

        testrun(!testrun_generate_script_gcov(
                project,
                NULL,
                path_logfile,
                path_exec,
                path_source,
                NULL,
                NULL));

        testrun(!testrun_generate_script_gcov(
                project,
                file_name,
                NULL,
                path_exec,
                path_source,
                NULL,
                NULL));

        testrun(!testrun_generate_script_gcov(
                project,
                file_name,
                path_logfile,
                NULL,
                path_source,
                NULL,
                NULL));

        testrun(!testrun_generate_script_gcov(
                project,
                file_name,
                path_logfile,
                path_exec,
                NULL,
                NULL,
                NULL));

        result = testrun_generate_script_gcov(
                project,
                file_name,
                path_logfile,
                path_exec,
                path_source,
                NULL,
                NULL);

        testrun(result);

        memset(buffer, 0, size);
        testrun(0 < snprintf(buffer, size,
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2018-02-09\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Run gcov based coverage tests on all test cases.\n"
        "#\n"
        "#       Usage           ./%s /path/to/project\n"
        "#\n"
        "#       Dependencies    bash, gcov\n"
        "#\n"
        "#       Last changed    2018-07-11\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "FOLDER_TEST_EXEC=\"%s\"\n"
        "FOLDER_TEST_SRC=\"%s\"\n"
        "FOLDER_LOGFILE=\"%s\"\n"
        "TEST_EXEC_SUFFIX=\"%s\"\n"
        "TEST_SRC_SUFFIX=\"%s\"\n"
        ""
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"$FOLDER_LOGFILE/gcov.$start_time.log\"\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               GCOV RUNNER\" >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "for test in $FOLDER_TEST_EXEC/*$TEST_EXEC_SUFFIX; do\n"
        "    $test\n"
        "done\n"
        "\n"
        "FILES=`ls  $FOLDER_TEST_EXEC/ | grep $TEST_EXEC_SUFFIX | wc -l`\n"
        "if [ $? -ne 0 ]; then\n"
        "        echo \"ERROR ... could not count files of $FOLDER_TEST_EXEC\"\n"
        "        exit 1\n"
        "fi\n"
        "c=0\n"
        "\n"
        "if [ $FILES -eq 0 ]; then\n"
        "        exit 0\n"
        "fi\n"
        "\n"
        "for i in $FOLDER_TEST_SRC/*$TEST_SRC_SUFFIX.c\n"
        "do\n"
        "        # RUN GCOV\n"
        "        echo $i\n"
        "        gcov $i\n"
        "done\n"
        "\n"
        "# move coverage output to log folder\n"
        "mv *.gcov $FOLDER_LOGFILE\n"
        "exit 0\n",
                bash_header,
                file_name,
                project,
                file_name,
                path_exec,
                path_source,
                path_logfile,
                exec_suffix,
                src_suffix
                ));

        testrun(0 == strncmp(result, buffer, strlen(buffer)));
        //testrun_log("%s", result);
        free(result);

        /*
         *      -----------------------------------------------------------------
         *
         *      DEFAULT OUTPUTS TO COPY/PASTE TO ACTUAL SCRIPTS OF TESTRUN.INFO
         *
         *      -----------------------------------------------------------------
         */



        project      = "testrun.info";
        file_name    = "testrun_gcov.sh";
        path_logfile = "build/tests/log";
        path_exec    = "build/tests/unit";
        path_source  = "tests/unit";
        exec_suffix  = ".test";
        src_suffix   = "_tests";
        result       = NULL;

        result = testrun_generate_script_gcov(
                project,
                file_name,
                path_logfile,
                path_exec,
                path_source,
                exec_suffix,
                src_suffix);

        testrun(result);
        //testrun_log("%s", result);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_script_gprof(){

        char *project      = "project";
        char *file_name    = "runner.sh";
        char *path_logfile = "project/build/tests/log";
        char *path_exec    = "project/src";
        char *exec_suffix  = NULL;
        char *result       = NULL;

        size_t size = 5000;
        char buffer[size];

        testrun(!testrun_generate_script_gprof(
                NULL,
                file_name,
                path_logfile,
                path_exec,
                NULL));

        testrun(!testrun_generate_script_gprof(
                project,
                NULL,
                path_logfile,
                path_exec,
                NULL));

        testrun(!testrun_generate_script_gprof(
                project,
                file_name,
                NULL,
                path_exec,
                NULL));

        testrun(!testrun_generate_script_gprof(
                project,
                file_name,
                path_logfile,
                NULL,
                NULL));

        result = testrun_generate_script_gprof(
                project,
                file_name,
                path_logfile,
                path_exec,
                NULL);

        testrun(result);

        memset(buffer, 0, size);
        testrun(0 < snprintf(buffer, size,
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2018-02-09\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Run gprof based analysis tests on all test cases.\n"
        "#\n"
        "#       Usage           ./%s /path/to/project\n"
        "#\n"
        "#       Dependencies    bash, gprof\n"
        "#\n"
        "#       Last changed    2018-07-11\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "FOLDER_TEST_EXEC=\"%s\"\n"
        "FOLDER_LOGFILE=\"%s\"\n"
        "TEST_EXEC_SUFFIX=\"%s\"\n"
        ""
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"$FOLDER_LOGFILE/gprof.$start_time.log\"\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               GPROF RUNNER\" >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "# Execute the test once and profile the execution\n"
        "for test in $FOLDER_TEST_EXEC/*$TEST_EXEC_SUFFIX; do\n"
        "        name=${test##*/}"
        "        echo \"Profiling\" $name\n"
        "    $test\n"
        "    gprof $test gmon.out > $name.profile\n"
        "done\n"
        "\n"
        "# move profile to build/tests/logs\n"
        "mv *.profile $FOLDER_LOGFILE\n"
        "exit 0\n",
                bash_header,
                file_name,
                project,
                file_name,
                path_exec,
                path_logfile,
                exec_suffix
                ));

        testrun(0 == strncmp(result, buffer, strlen(buffer)));
        //testrun_log("%s", result);
        free(result);

        /*
         *      -----------------------------------------------------------------
         *
         *      DEFAULT OUTPUTS TO COPY/PASTE TO ACTUAL SCRIPTS OF TESTRUN.INFO
         *
         *      -----------------------------------------------------------------
         */



        project      = "testrun.info";
        file_name    = "testrun_gcov.sh";
        path_logfile = "build/tests/log";
        path_exec    = "build/tests/unit";
        exec_suffix  = ".test";
        result       = NULL;

        result = testrun_generate_script_gprof(
                project,
                file_name,
                path_logfile,
                path_exec,
                exec_suffix);

        testrun(result);
        //testrun_log("%s", result);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_makefile_common(){

        char *result             = NULL;
        char *project            = "project";
        char *file_name          = "file_name";
        char *path_bin           = "path_bin";
        char *path_build         = "path_build";
        char *path_include       = "path_include";
        char *path_source        = "path_source";
        char *path_tests         = "path_tests";
        char *path_tools         = "path_tools";
        char *path_doxygen       = "doxygen_docu";
        char *suffix_test_source = "suffix_test_source";
        char *suffix_test_exec   = "suffix_test_exec";
        char *script_unit        = "script_unit";
        char *script_acceptance  = "script_acceptance";
        char *script_coverage    = "script_coverage";
        char *script_loc         = "script_loc";
        char *script_gcov        = "script_gcov";
        char *script_gprof       = "script_gprof";

        size_t size = 20000;
        char buffer[size];

        result = testrun_generate_makefile_common(
                project,
                file_name,
                path_bin,
                path_build,
                path_include,
                path_source,
                path_tests,
                path_tools,
                path_doxygen,
                suffix_test_source,
                suffix_test_exec,
                script_unit,
                script_acceptance,
                script_coverage,
                script_loc,
                script_gcov,
                script_gprof,
                LIB);

        testrun(result);

        memset(buffer, 0, size);
        testrun(0 < snprintf(buffer, size,
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2018-02-18\n"
        "#\n"
        "#       Project         %s\n"
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
        "#                           \"make gcov\"\n"
        "#\n"
        "#                       may be used to rebuild the whole project with gcov\n"
        "#                       coverage testing flag enabled.\n"
        "#\n"
        "#                           \"make gprof\"\n"
        "#\n"
        "#                       may be used to rebuild the whole project with gprof\n"
        "#                       profiling flags enabled.\n"
        "#\n"
        "#                       Following folder structure is required\n"
        "#\n"
        "#                           bin     MUST be located at %s\n"
        "#                           build   MUST be located at %s\n"
        "#                           inludes MUST be located at %s\n"
        "#                           sources MUST be located at %s\n"
        "#                           tests   MUST be located at %s\n"
        "#\n"
        "#                       ALL TEST SCRIPTS MAY BE EXCHANGED WITH CUSTOM RUNNERS\n"
        "#\n"
        "#       Usage           SHOULD be used included by parent makefile\n"
        "#\n"
        "#       NOTE            aligned with tab width 4\n"
        "#\n"
        "#       Dependencies    testrun (makefile & service scripts), doxygen (if used)\n"
        "#\n"
        "#       Last changed    2018-07-12\n"
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
        "DIRNAME\t\t\t:= $(notdir $(patsubst %%/,%%,$(dir $(PROJECTMK))))\n"
        "\n"
        "# ----- Package config setup -----------------------------------------------\n"
        "\n"
        "LIBNAME\t\t\t:= lib$(DIRNAME)\n"
        "LIBNAMEPC\t\t:= $(LIBNAME).pc\n"
        "\n"
        "INCDIR\t\t\t:= $(PREFIX)/usr/local/include/$(DIRNAME)\n"
        "LIBDIR\t\t\t:= $(PREFIX)/usr/local/lib\n"
        "EXECDIR\t\t\t:= $(PREFIX)/usr/local/bin\n"
        "CONFDIR\t\t\t:= $(PREFIX)/etc/$(DIRNAME)\n"
        "SOCKDIR\t\t\t:= $(PREFIX)/etc/systemd/system\n"
        "\n"
        "# ----- TARGETS ------------------------------------------------------------\n"
        "\n"
        "INSTALL\t\t\t:= install\n"
        "\n"
        "EXECUTABLE\t\t= %s/$(DIRNAME)\n"
        "\n"
        "STATIC\t\t\t= %s/lib$(DIRNAME).a\n"
        "SHARED\t\t\t= $(patsubst %%.a,%%.so,$(STATIC))\n"
        "\n"
        "# Source and object files to compile\n"
        "HEADERS\t\t\t= $(wildcard %s/*.h)\n"
        "SOURCES\t\t\t= $(wildcard %s/**/*.c %s/*.c)\n"
        "OBJECTS\t\t\t= $(patsubst %%.c,%%.o,$(SOURCES))\n"
        "\n"
        "# Test sources and targets\n"
        "TESTS_SOURCES   = $(wildcard %s/**/*%s.c %s/*%s.c)\n"
        "TESTS_TARGET    = $(patsubst %s/%%.c, %s/tests/%%%s, $(TESTS_SOURCES))\n"
        "\n"
        "# GCOV support\n"
        "GCOV_FILES\t\t=  $(patsubst %%.c,%%.gcno,$(SOURCES))\n"
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
        "# ----- TEST_SCRIPTS -------------------------------------------------------\n"
        "\n"
        "TEST_TOOLS_FOLDER\t\t=%s\n"
        "TEST_SCRIPT_UNIT\t\t= $(TEST_TOOLS_FOLDER)/%s\n"
        "TEST_SCRIPT_ACCEPTANCE\t= $(TEST_TOOLS_FOLDER)/%s\n"
        "TEST_SCRIPT_COVERAGE\t=$(TEST_TOOLS_FOLDER)/%s\n"
        "TEST_SCRIPT_LOC\t\t\t= $(TEST_TOOLS_FOLDER)/%s\n"
        "TEST_SCRIPT_GCOV\t\t= $(TEST_TOOLS_FOLDER)/%s\n"
        "TEST_SCRIPT_GPROF\t\t= $(TEST_TOOLS_FOLDER)/%s\n"
        "\n"
        "# ----- DEFAULT MAKE RULES -------------------------------------------------\n"
        "\n"
        "%%.o : %%.c $(HEADERS)\n"
        "\t@echo \" (CC)    $@\"\n"
        "\t@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)\n"
        "\n"
        "%%%s.o : %%%s.c\n"
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
        "# ----- BUILD & CLEANUP ----------------------------------------------------\n"
        "\n"
        "build:\n"
        "\t@mkdir -p %s\n"
        "\t@mkdir -p %s\n"
        "\t@mkdir -p %s/tests\n"
        "\t@mkdir -p %s/tests/unit\n"
        "\t@mkdir -p %s/tests/acceptance\n"
        "\t@mkdir -p %s/tests/log\n"
        "\t@echo \" (MK)    directories for build\"\n"
        "\n"
        ".PHONY: clean\n"
        "clean:\n"
        "\t@echo \" (CLEAN) $(LIBNAME)\"\n"
        "\t@rm -rf %s %s %s/documentation $(OBJECTS) $(TESTS_OBJECTS) \\\n"
        "\t$(LIBNAMEPC) $(TESTS_TMP_FILES) $(GCOV_FILES) *.gcov *.profile *.pc *.out\n"
        "\n"
        "\n"
        "# ----- DOCUMENATION -------------------------------------------------------\n"
        "\n"
        "#NOTE requires doxygen.PHONY: documentation\n"
        "documentation:\n"
        "\tdoxygen %s/doxygen.config\n"
        "\n"
        "# ----- INFORMATION PRINTING -----------------------------------------------\n"
        "\n"
        "# print out a variable of the make file (e.g. \"make print-PROJECTPATH\")\n"
        ".PHONY: print\n"
        "print-%%  : ; @echo $* = $($*)\n"
        "\n"
        ".PHONY: start\n"
        "start:\n"
        "\t@echo \"\\n (HINT)    $(PROJECT)           ==> running make\\n\"\n"
        "\n"
        ".PHONY: done\n"
        "done:\n"
        "\t@echo\n"
        "\t@echo \" (DONE)  make $(PROJECT)\"\n"
        "\t@echo \" (HINT)  with unit testing      ==> 'make tested'\"\n"
        "\t@echo \" (HINT)  perform installation   ==> 'sudo make install\"\n"
        "\t@echo \" (HINT)  generate documentation ==> 'make documentation\"\n"
        "\n"
        "# ----- TESTING ------------------------------------------------------------\n"
        "\n"
        "# ALL IN ONE CALL (compile source, test and run test)\n"
        "tested: all testrun done\n"
        "\n"
        "# copy test resources to build\n"
        "tests-resources:\n"
        "\t@echo \" (CP)    tests/resources\"\n"
        "\t@cp -r %s/resources %s/tests\n"
        "\n"
        "%s/tests/acceptance/%%%s%s: %s/acceptance/%%%s.o\n"
        "\t@echo \" (CC)    $(@)\"\n"
        "\t@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)\n"
        "\n"
        "%s/tests/unit/%%%s%s: %s/unit/%%%s.o\n"
        "\t@echo \" (CC)    $(@)\"\n"
        "\t@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)\n"
        "\n"
        "# TESTRUN runners ----------------------------------------------------------\n"
        "\n"
        "# ACCEPTANCE TEST script invocation\n"
        ".PHONY: testrun-acceptance\n"
        "testrun-acceptance:\n"
        "\tsh $(TEST_SCRIPT_ACCEPTANCE)\n"
        "\n"
        "# UNIT TEST script invocation\n"
        ".PHONY: testrun-unit\n"
        "testrun-unit:\n"
        "\tsh $(TEST_SCRIPT_UNIT)\n"
        "\n"
        "# COVERAGE TEST script invocation\n"
        ".PHONY: testrun-coverage\n"
        "testrun-coverage:\n"
        "\tsh $(TEST_SCRIPT_COVERAGE) $(PROJECTPATH)\n"
        "\n"
        "# LOC TEST script invocation\n"
        ".PHONY: testrun-loc\n"
        "testrun-loc:\n"
        "\tsh $(TEST_SCRIPT_LOC) $(PROJECTPATH)\n"
        "\n"
        "# TESTRUN all scripts\n"
        ".PHONY: testrun\n"
        "testrun:\n"
        "\t@echo \" (HINT)  $(PROJECT) \\t\\t\\t==> running tests\\n\"\n"
        "\tsh $(TEST_SCRIPT_UNIT)\n"
        "\tsh $(TEST_SCRIPT_ACCEPTANCE)\n"
        "\tsh $(TEST_SCRIPT_COVERAGE) $(PROJECTPATH)\n"
        "\tsh $(TEST_SCRIPT_LOC) $(PROJECTPATH)\n"
        "\n"
        "# TESTRUN gcov -------------------------------------------------------------\n"
        "\n"
        ".PHONY: testrun-gcov\n"
        "testrun-gcov: clean\n"
        "\tmake USE_GCOV=1 all\n"
        "\tsh $(TEST_SCRIPT_GCOV) $(PROJECTPATH)\n"
        "\n"
        "# TESTRUN gprof ------------------------------------------------------------\n"
        "\n"
        ".PHONY: testrun-gprof\n"
        "testrun-gprof: clean\n"
        "\tmake USE_GPROF=1 all\n"
        "\tsh $(TEST_SCRIPT_PROF) $(PROJECTPATH)\n"
        "\n"
        "# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------\n"
        "\n"
        ".PHONY: pkgconfig\n"
        "pkgconfig:\n"
        "\t@echo 'prefix='$(PREFIX)'/usr/local/' >  $(LIBNAMEPC)\n"
        "\t@echo 'exec_prefix=$${prefix}' >> $(LIBNAMEPC)\n"
        "\t@echo 'libdir=$${prefix}/lib' >> $(LIBNAMEPC)\n"
        "\t@echo 'includedir=$${prefix}/include' >> $(LIBNAMEPC)\n"
        "\t@echo '' >> $(LIBNAMEPC)\n"
        "\t@echo 'Name: '$(LIBNAME) >> $(LIBNAMEPC)\n"
        "\t@echo 'Description: '$(PROJECT_DESC) >> $(LIBNAMEPC)\n"
        "\t@echo 'Version: '$(VERSION) >> $(LIBNAMEPC)\n"
        "\t@echo 'URL: ' $(PROJECT_URL) >> $(LIBNAMEPC)\n"
        "\t@echo 'Libs: -L$${libdir} -l'$(DIRNAME) >> $(LIBNAMEPC)\n"
        "\t@echo 'Cflags: -I$${includedir}' >> $(LIBNAMEPC)\n"
         "\n"
        "# ----- INSTALLATION -------------------------------------------------------\n"
        "\n"
        "# Installation as a library ------------------------------------------------\n"
        "\n"
        ".PHONY: install_lib\n"
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
        ".PHONY: uninstall_lib\n"
        "uninstall_lib:\n"
        "\t@echo \" (OK)    uninstalled $(LIBNAME) from $(LIBDIR)\"\n"
        "\t@rm -rf $(INCDIR)\n"
        "\t@rm -rf $(LIBDIR)/$(LIBNAME).a\n"
        "\t@rm -rf $(LIBDIR)/$(LIBNAME).so\n"
        "\t@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)\n"
        "\n"
        "# Installation as an executable --------------------------------------------\n"
        "\n"
        ".PHONY: install_exec\n"
        "install_exec: $(SHARED) $(STATIC)\n"
        "\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\"\n"
        "\t@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)\n"
        "\n"
        ".PHONY: uninstall_exec\n"
        "uninstall_exec:\n"
        "\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\"\n"
        "\t@rm -rf $(EXECDIR)/$(DIRNAME)\n"
        "\n"
        "# Installation as a service ------------------------------------------------\n"
        ".PHONY: install_service\n"
        "install_service: copy_service_files enable_service\n"
        "\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\"\n"
        "\n"
        ".PHONY: copy_service_files\n"
        "copy_service_files: $(EXECUTABLE) \n"
        "\t@echo \" (OK)    copied service files\"\n"
        "\t@mkdir -p $(SOCKDIR)\n"
        "\t@$(INSTALL) -m 0755 bin/$(DIRNAME)  $(EXECDIR)\n"
        "\t@$(INSTALL) -m 0755 -d $(SERVICE_DATA)/etc   $(CONFDIR)\n"
        "\t@$(INSTALL) -m 0644 $(SERVICE_DATA)/*.service $(SOCKDIR)\n"
        "\t@$(INSTALL) -m 0644 $(SERVICE_DATA)/*.socket $(SOCKDIR)\n"
        "\n"
        ".PHONY: enable_service\n"
        "enable_service:\n"
        "\t@# IF INSTALLATION IS DONE UNPREFIXED TO /etc, the service will be enabled \n"
        "\t@ifndef ($(PREFIX)) \\\n"
        "\t\t@echo \" (OK)    enable service\" \\\n"
        "\t\t$(shell systemctl enable $(DIRNAME).socket) \\\n"
        "\t\t$(shell systemctl start $(DIRNAME).socket) \\\n"
        "\t\t$(shell systemctl enable $(DIRNAME).service) \\\n"
        "\t\t$(shell systemctl start $(DIRNAME).service) \\\n"
        "\t\t$(shell systemctl daemon-reload) \\\n"
        "\t@endif\n"
        "\n"
        ".PHONY: delete_service_files\n"
        "delete_service_files: \n"
        "\t@echo \" (OK)    delete service files\"\n"
        "\t@rm -rf $(EXECDIR)/$(DIRNAME)\n"
        "\t@rm -rf $(CONFDIR)\n"
        "\t@rm -rf $(SOCKDIR)/$(DIRNAME)*\n"
        "\n"
        ".PHONY: disable_service\n"
        "disable_service:\n"
        "\t@# IF INSTALLATION WAS DONE UNPREFIXED TO /etc, the service will be disabled \n"
        "\t@ifndef ($(PREFIX)) \\\n"
        "\t\t@echo \" (OK)    disable service\" \\\n"
        "\t\t$(shell systemctl stop  $(DIRNAME).service) \\\n"
        "\t\t$(shell systemctl disable $(DIRNAME).service) \\\n"
        "\t\t$(shell systemctl stop $(DIRNAME).socket) \\\n"
        "\t\t$(shell systemctl disable $(DIRNAME).socket) \\\n"
        "\t\t$(shell systemctl daemon-reload) \\\n"
        "\t@endif\n"
        "\n"
        ".PHONY: uninstall_service\n"
        "uninstall_service: disable_service delete_service_files\n"
        "\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\"\n"
        ,
                bash_header,
                file_name,
                project,
                path_bin,
                path_build,
                path_include,
                path_source,
                path_tests,
                path_bin,
                path_build,
                path_include,
                path_source,
                path_source,
                path_tests,
                suffix_test_source,
                path_tests,
                suffix_test_source,
                path_tests,
                path_build,
                suffix_test_exec,
                path_tools,
                script_unit,
                script_acceptance,
                script_coverage,
                script_loc,
                script_gcov,
                script_gprof,
                suffix_test_source,
                suffix_test_source,
                "all_lib",
                "install_lib",
                "uninstall_lib",
                path_bin,
                path_build,
                path_build,
                path_build,
                path_build,
                path_build,
                path_bin,
                path_build,
                path_doxygen,
                path_doxygen,
                path_tests,
                path_build,
                path_build,
                suffix_test_source,
                suffix_test_exec,
                path_tests,
                suffix_test_source,
                path_build,
                suffix_test_source,
                suffix_test_exec,
                path_tests,
                suffix_test_source));

        //testrun_log("%s|%s", result, buffer);

        testrun(0 == strncmp(result, buffer, strlen(buffer)));
        free(result);

        /*
         *      -----------------------------------------------------------------
         *
         *      DEFAULT OUTPUTS TO COPY/PASTE TO ACTUAL SCRIPTS OF TESTRUN.INFO
         *
         *      -----------------------------------------------------------------
         */

        project            = "testrun_lib";
        file_name          = "makefile_common.mk";
        path_bin           = "bin";
        path_build         = "build";
        path_include       = "include";
        path_source        = "src";
        path_tests         = "tests";
        path_tools         = "tests/tools";
        path_doxygen       = "doxygen";
        suffix_test_source = "_tests";
        suffix_test_exec   = ".test";
        script_unit        = "tests/tools/testrun_simple_unit_tests.sh";
        script_acceptance  = "tests/tools/testrun_simple_acceptance_tests.sh";
        script_coverage    = "tests/tools/testrun_simple_coverage_tests.sh";
        script_loc         = "tests/tools/testrun_simple_loc.sh";
        script_gcov        = "tests/tools/testrun_gcov.sh";
        script_gprof       = "tests/tools/testrun_gprof.sh";

        result = testrun_generate_makefile_common(
                project,
                file_name,
                path_bin,
                path_build,
                path_include,
                path_source,
                path_tests,
                path_tools,
                path_doxygen,
                suffix_test_source,
                suffix_test_exec,
                script_unit,
                script_acceptance,
                script_coverage,
                script_loc,
                script_gcov,
                script_gprof,
                LIB);

        testrun(result);
        //testrun_log("%s", result);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_makefile(){

        char *result            = NULL;
        char *project           = "project";
        char *file_name         = "file_name";
        char *version           = "version";
        char *cflags            = "cflags";
        char *project_url       = "project_url";
        char *project_desc      = "project_desc";
        char *path_service      = "path_service";
        char *makefile_common  = "makefile_common";


        size_t size = 10000;
        char buffer[size];

        result = testrun_generate_makefile(
                project,
                file_name,
                version,
                cflags,
                project_url,
                project_desc,
                path_service,
                makefile_common);

        testrun(result);

        memset(buffer, 0, size);
        testrun(0 < snprintf(buffer, size,
        "%s"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Authors         ...\n"
        "#       Date            2018-02-18\n"
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
        "#       Last changed    2018-07-12\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "CC = gcc\n"
        "\n"
        "PROJECT\t\t\t:= %s\n"
        "VERSION\t\t\t:= %s\n"
        "\n"
        "# project path recalculation (if used included from a parent make)\n"
        "PROJECTMK\t\t:= $(abspath $(lastword $(MAKEFILE_LIST)))\n"
        "\n"
        "# prefix for base directory for installation (default is /)\n"
        "#PREFIX\t\t\t:= some_path\n"
        "\n"
        "# include all pkgconfig files available at PREFIX\n"
        "export PKG_CONFIG_PATH = $(PREFIX)/usr/local/lib/pkgconfig\n"
        "\n"
        "# LIBS USED (uncommented example includes)\n"
        "# ... will allow to include libs installed under PREFIX\n"
        "#LIBS\t\t\t+= `pkg-config --cflags --libs libtestrun.info`\n"
        "#LIBS\t\t\t+= `pkg-config --libs libsystemd`\n"
        "#LIBS\t\t\t+= `pkg-config --libs uuid`\n"
        "#LIBS\t\t\t+= `pkg-config --libs openssl`\n"
        "\n"
        "# MODULE BASED CFLAGS (example)\n"
        "MODCFLAGS\t\t+= %s\n"
        "\n"
        "# EXTRA CFLAGS (example parallel or other GCC custom flags)\n"
        "#MODCFLAGS\t\t+= -fopenmp\n"
        "#MODCFLAGS\t\t+= -rdynamic\n"
        "\n"
        "# EXTRA LFLAGS (example)\n"
        "#MODLFLAGS\t\t+= -pthread\n"
        "\n"
        "# PKG_CONFIG_DATA (used during LIBRARY install)\n"
        "PROJECT_URL\t\t= \"%s\"\n"
        "PROJECT_DESC\t= \"%s\"\n"
        "\n"
        "# SERVICE_CONFIG_DATA (used during SERVICE install)\n"
        "SERVICE_DATA\t= \"%s\"\n"
        "\n"
        "# TMP FILE DEFINITION\n"
        "TESTS_TMP_FILES\t= $(wildcard /tmp/test_*)\n"
        "\n"
        "# INCLUDE BASE MAKEFILE\n"
        "include %s\n"
        ,
                bash_header,
                file_name,
                project,
                project,
                version,
                cflags,
                project_url,
                project_desc,
                path_service,
                makefile_common
        ));

        testrun(0 == strncmp(result, buffer, strlen(buffer)));
        //testrun_log("%s", result);
        free(result);


        /*
         *      -----------------------------------------------------------------
         *
         *      DEFAULT OUTPUTS TO COPY/PASTE TO ACTUAL SCRIPTS OF TESTRUN.INFO
         *
         *      -----------------------------------------------------------------
         */

        result            = NULL;
        project           = "testrun_lib";
        file_name         = "makefile";
        version           = "1.0.0";
        cflags            = "-std=c11 -D _DEFAULT_SOURCE -D _POSIX_C_SOURCE=200809";
        project_url       = "http://testrun.info";
        project_desc      = "A small library to build testrunner frameworks.";
        path_service      = "config/install";
        makefile_common  = "makefile_common.mk";

        result = testrun_generate_makefile(
                project,
                file_name,
                version,
                cflags,
                project_url,
                project_desc,
                path_service,
                makefile_common);

        testrun(result);
        //testrun_log("%s", result);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_tools_default(){

        testrun_tools tools = testrun_tools_default();

        testrun(tools.testrun_header          == testrun_generate_header);
        testrun(tools.testrun_header_openmp   == testrun_generate_header_openmp);
        testrun(tools.testrun_simple_tests    == testrun_generate_script_simple_tests);
        testrun(tools.testrun_runner          == testrun_generate_script_runner);
        testrun(tools.testrun_loc             == testrun_generate_script_loc);
        testrun(tools.testrun_simple_coverage == testrun_generate_script_coverage);
        testrun(tools.testrun_gcov            == testrun_generate_script_gcov);
        testrun(tools.testrun_gprof           == testrun_generate_script_gprof);
        testrun(tools.makefile_configurable   == testrun_generate_makefile);
        testrun(tools.makefile_common         == testrun_generate_makefile_common);
        testrun(tools.gitignore               == testrun_generate_gitignore);
        testrun(tools.readme                  == testrun_generate_readme);
        testrun(tools.doxygen                 == testrun_generate_doxygen);
        testrun(tools.service_file            == testrun_generate_service_file);
        testrun(tools.socket_file             == testrun_generate_socket_file);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_doxygen(){

        char *expect = NULL;
        char *result = NULL;

        result = testrun_generate_doxygen(NULL, NULL, NULL, NULL);
        expect =        "DOXYFILE_ENCODING       = UTF-8\n"
                        "PROJECT_NAME            = (null)\n"
                        "PROJECT_NUMBER          = 0.0.1\n"
                        "PROJECT_LOGO            = (null)/logo.png\n"
                        "PROJECT_BRIEF           = (null)\n"
                        "OUTPUT_DIRECTORY        = (null)/documentation\n"
                        "CREATE_SUBDIRS          = NO\n"
                        "ALLOW_UNICODE_NAMES     = NO\n"
                        "OUTPUT_LANGUAGE         = English\n"
                        "MARKDOWN_SUPPORT        = YES\n"
                        "AUTOLINK_SUPPORT        = YES\n"
                        "USE_MDFILE_AS_MAINPAGE  = (null)\n"
                        "INPUT                   = (null)\n"
                        "INPUT_ENCODING          = UTF-8\n"
                        "FILE_PATTERNS           = *.h *.c *.js *.py *.sh\n"
                        "RECURSIVE               = YES\n"
                        "EXCLUDE_SYMLINKS        = YES\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        result = testrun_generate_doxygen("name", NULL, NULL, NULL);
        expect =        "DOXYFILE_ENCODING       = UTF-8\n"
                        "PROJECT_NAME            = name\n"
                        "PROJECT_NUMBER          = 0.0.1\n"
                        "PROJECT_LOGO            = (null)/logo.png\n"
                        "PROJECT_BRIEF           = name\n"
                        "OUTPUT_DIRECTORY        = (null)/documentation\n"
                        "CREATE_SUBDIRS          = NO\n"
                        "ALLOW_UNICODE_NAMES     = NO\n"
                        "OUTPUT_LANGUAGE         = English\n"
                        "MARKDOWN_SUPPORT        = YES\n"
                        "AUTOLINK_SUPPORT        = YES\n"
                        "USE_MDFILE_AS_MAINPAGE  = (null)\n"
                        "INPUT                   = (null)\n"
                        "INPUT_ENCODING          = UTF-8\n"
                        "FILE_PATTERNS           = *.h *.c *.js *.py *.sh\n"
                        "RECURSIVE               = YES\n"
                        "EXCLUDE_SYMLINKS        = YES\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        result = testrun_generate_doxygen("name", "path", NULL, NULL);
        expect =        "DOXYFILE_ENCODING       = UTF-8\n"
                        "PROJECT_NAME            = name\n"
                        "PROJECT_NUMBER          = 0.0.1\n"
                        "PROJECT_LOGO            = path/logo.png\n"
                        "PROJECT_BRIEF           = name\n"
                        "OUTPUT_DIRECTORY        = path/documentation\n"
                        "CREATE_SUBDIRS          = NO\n"
                        "ALLOW_UNICODE_NAMES     = NO\n"
                        "OUTPUT_LANGUAGE         = English\n"
                        "MARKDOWN_SUPPORT        = YES\n"
                        "AUTOLINK_SUPPORT        = YES\n"
                        "USE_MDFILE_AS_MAINPAGE  = (null)\n"
                        "INPUT                   = (null)\n"
                        "INPUT_ENCODING          = UTF-8\n"
                        "FILE_PATTERNS           = *.h *.c *.js *.py *.sh\n"
                        "RECURSIVE               = YES\n"
                        "EXCLUDE_SYMLINKS        = YES\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        result = testrun_generate_doxygen("name", "path", "main", NULL);
        expect =        "DOXYFILE_ENCODING       = UTF-8\n"
                        "PROJECT_NAME            = name\n"
                        "PROJECT_NUMBER          = 0.0.1\n"
                        "PROJECT_LOGO            = path/logo.png\n"
                        "PROJECT_BRIEF           = name\n"
                        "OUTPUT_DIRECTORY        = path/documentation\n"
                        "CREATE_SUBDIRS          = NO\n"
                        "ALLOW_UNICODE_NAMES     = NO\n"
                        "OUTPUT_LANGUAGE         = English\n"
                        "MARKDOWN_SUPPORT        = YES\n"
                        "AUTOLINK_SUPPORT        = YES\n"
                        "USE_MDFILE_AS_MAINPAGE  = main\n"
                        "INPUT                   = (null)\n"
                        "INPUT_ENCODING          = UTF-8\n"
                        "FILE_PATTERNS           = *.h *.c *.js *.py *.sh\n"
                        "RECURSIVE               = YES\n"
                        "EXCLUDE_SYMLINKS        = YES\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        result = testrun_generate_doxygen("name", "path", "main", "input");
        expect =        "DOXYFILE_ENCODING       = UTF-8\n"
                        "PROJECT_NAME            = name\n"
                        "PROJECT_NUMBER          = 0.0.1\n"
                        "PROJECT_LOGO            = path/logo.png\n"
                        "PROJECT_BRIEF           = name\n"
                        "OUTPUT_DIRECTORY        = path/documentation\n"
                        "CREATE_SUBDIRS          = NO\n"
                        "ALLOW_UNICODE_NAMES     = NO\n"
                        "OUTPUT_LANGUAGE         = English\n"
                        "MARKDOWN_SUPPORT        = YES\n"
                        "AUTOLINK_SUPPORT        = YES\n"
                        "USE_MDFILE_AS_MAINPAGE  = main\n"
                        "INPUT                   = input\n"
                        "INPUT_ENCODING          = UTF-8\n"
                        "FILE_PATTERNS           = *.h *.c *.js *.py *.sh\n"
                        "RECURSIVE               = YES\n"
                        "EXCLUDE_SYMLINKS        = YES\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_gitignore(){

        char *expect = NULL;
        char *result = NULL;

        result = testrun_generate_gitignore();
        expect = "# Prerequisites\n"
                "*.d\n"
                "\n"
                "# Object files\n"
                "*.o\n"
                "*.so\n"
                "*.ko\n"
                "*.obj\n"
                "*.elf\n"
                "\n"
                "# Linker output\n"
                "*.ilk\n"
                "*.map\n"
                "*.exp\n"
                "\n"
                "# Precompiled Headers\n"
                "*.gch\n"
                "*.pch\n"
                "\n"
                "# Libraries\n"
                "*.lib\n"
                "*.a\n"
                "*.la\n"
                "*.lo\n"
                "\n"
                "# Shared objects (inc. Windows DLLs)\n"
                "*.dll\n"
                "*.so\n"
                "*.so.*\n"
                "*.dylib\n"
                "\n"
                "# Executables\n"
                "*.exe\n"
                "*.out\n"
                "*.app\n"
                "*.i*86\n"
                "*.x86_64\n"
                "*.hex\n"
                "\n"
                "# Debug files\n"
                "*.dSYM/\n"
                "*.su\n"
                "*.idb\n"
                "*.pdb\n"
                "\n"
                "# Kernel Module Compile Results\n"
                "*.mod*\n"
                "*.cmd\n"
                ".tmp_versions/\n"
                "modules.order\n"
                "Module.symvers\n"
                "Mkfile.old\n"
                "dkms.conf\n"
                "\n"
                "# Local files\n"
                "**/local\n"
                "**/bin/\n"
                "**/gen/\n"
                "**/build/\n"
                "**/docs/doxygen/\n"
                "**/doxygen/documentation/\n"
                "\n"
                "# vagrant (if used)\n"
                ".vagrant\n"
                "\n"
                "# subprojects (if used)\n"
                "*.git\n"
                "\n"
                "# generated package config\n"
                "*.pc\n"
                "\n"
                "# ctags\n"
                ".tags\n"
                "tags\n"
                "functions\n"
                "\n"
                "# IDE\n"
                "\n"
                "## IntelliJ\n"
                ".idea\n"
                "\n"
                "## Sublime\n"
                "*.sublime-workspace\n"
                "*.sublime-project\n"
                "\n"
                "## VIM\n"
                "[._]*.s[a-w][a-z]\n"
                "[._]s[a-w][a-z]\n"
                "*.un~\n"
                "Session.vim\n"
                ".netrwhist\n"
                "*~\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_readme(){

        char *expect = NULL;
        char *result = NULL;

        result = testrun_generate_readme(NULL, NULL, NULL);
        expect = "# Project (null)\n"
                "\n"
                "This module is self supported and may be build, tested, installed and\n"
                "run independently.\n"
                "\n"
                "## Overview\n"
                "\n"
                "* [Description](#description)\n"
                "* [Usage](#usage)\n"
                "* [Installation](#installation)\n"
                "* [Requirements](#requirements)\n"
                "* [Structure](#structure)\n"
                "* [Tests](#tests)\n"
                "* [Tips](#tips)\n"
                "* [Copyright](#copyright)\n"
                "\n"
                "## Description\n"
                "\n"
                "(null)\n"
                "\n"
                "## Usage\n"
                "\n"
                "...\n"
                "\n"
                "## Installation\n"
                "\n"
                "...\n"
                """\n"
                "### build sources\n"
                "\n"
                "\\`\\`\\`bash\n"
                "make\n"
                "\\`\\`\\`\n"
                "\n"
                "### build documentation\n"
                "\n"
                "\\`\\`\\`bash\n"
                "make documentation\n"
                "\\`\\`\\`\n"
                "\n"
                "### test sources\n"
                "\n"
                "\\`\\`\\`bash\n"
                "make tested\n"
                "\\`\\`\\`\n"
                "\n"
                "### install binaries\n"
                "\n"
                "\\`\\`\\`bash\n"
                "sudo make install\n"
                "\\`\\`\\`\n"
                "\n"
                "### uninstall binaries\n"
                "\n"
                "\\`\\`\\`bash\n"
                "sudo make uninstall\n"
                "\\`\\`\\`\n"
                "\n"
                "## Requirements\n"
                "\n"
                "## Structure\n"
                "\n"
                "### Default structure of the folder:\n"
                "\n"
                "\\`\\`\\`\n"
                "<pre>\n"
                ".\n"
                "├── README.MD\n"
                "├── .gitignore\n"
                "├── makefile\n"
                "├── makefile_common.mk\n"
                "│\n"
                "├── copyright\n"
                "│   └── ... \n"
                "│\n"
                "├── doxygen\n"
                "│   ├── documentation\n"
                "│   └── doxygen.config\n"
                "│\n"
                "├── docs\n"
                "│   ├── CHANGELOG.MD\n"
                "│   └── ...\n"
                "│\n"
                "├── include\n"
                "│   ├── (null).h\n"
                "│   └── ...\n"
                "│\n"
                "├── src\n"
                "│   ├── (null).c\n"
                "│   └── ...\n"
                "│\n"
                "└── tests\n"
                "    ├── resources\n"
                "    ├── tools\n"
                "    │   ├── testrun.h\n"
                "    │   ├── testrun_runner.sh\n"
                "    │   ├── testrun_gcov.sh\n"
                "    │   ├── testrun_gprof.sh\n"
                "    │   ├── testrun_simple_coverage_tests.sh\n"
                "    │   ├── testrun_simple_unit_tests.sh\n"
                "    │   ├── testrun_simple_acceptance_tests.sh\n"
                "    │   └── testrun_simple_loc.sh\n"
                "    │\n"
                "    ├── acceptance\n"
                "    │   ├── ...\n"
                "    │   └── ...\n"
                "    │\n"
                "    └── unit\n"
                "        ├── (null)_test.c\n"
                "        └── ...\n"
                "\n"
                "</pre>\n"
                "\\`\\`\\`\n"
                "\n"
                "## Tests\n"
                "\n"
                "All test sources will be recompiled on each make run. That means,\n"
                "all module tests will be created new on any change in any source file.\n"
                "\n"
                "### Test a project (all files contained in tests/unit)\n"
                "\n"
                "Test compile and run\n"
                "~~~\n"
                "make tested\n"
                "~~~\n"
                "\n"
                "## Tips\n"
                "\n"
                "## Copyright\n"
                "\n"
                "(null)\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        result = testrun_generate_readme("(name)", NULL, NULL);
        expect = "# Project (name)\n"
                "\n"
                "This module is self supported and may be build, tested, installed and\n"
                "run independently.\n"
                "\n"
                "## Overview\n"
                "\n"
                "* [Description](#description)\n"
                "* [Usage](#usage)\n"
                "* [Installation](#installation)\n"
                "* [Requirements](#requirements)\n"
                "* [Structure](#structure)\n"
                "* [Tests](#tests)\n"
                "* [Tips](#tips)\n"
                "* [Copyright](#copyright)\n"
                "\n"
                "## Description\n"
                "\n"
                "(null)\n"
                "\n"
                "## Usage\n"
                "\n"
                "...\n"
                "\n"
                "## Installation\n"
                "\n"
                "...\n"
                """\n"
                "### build sources\n"
                "\n"
                "\\`\\`\\`bash\n"
                "make\n"
                "\\`\\`\\`\n"
                "\n"
                "### build documentation\n"
                "\n"
                "\\`\\`\\`bash\n"
                "make documentation\n"
                "\\`\\`\\`\n"
                "\n"
                "### test sources\n"
                "\n"
                "\\`\\`\\`bash\n"
                "make tested\n"
                "\\`\\`\\`\n"
                "\n"
                "### install binaries\n"
                "\n"
                "\\`\\`\\`bash\n"
                "sudo make install\n"
                "\\`\\`\\`\n"
                "\n"
                "### uninstall binaries\n"
                "\n"
                "\\`\\`\\`bash\n"
                "sudo make uninstall\n"
                "\\`\\`\\`\n"
                "\n"
                "## Requirements\n"
                "\n"
                "## Structure\n"
                "\n"
                "### Default structure of the folder:\n"
                "\n"
                "\\`\\`\\`\n"
                "<pre>\n"
                ".\n"
                "├── README.MD\n"
                "├── .gitignore\n"
                "├── makefile\n"
                "├── makefile_common.mk\n"
                "│\n"
                "├── copyright\n"
                "│   └── ... \n"
                "│\n"
                "├── doxygen\n"
                "│   ├── documentation\n"
                "│   └── doxygen.config\n"
                "│\n"
                "├── docs\n"
                "│   ├── CHANGELOG.MD\n"
                "│   └── ...\n"
                "│\n"
                "├── include\n"
                "│   ├── (name).h\n"
                "│   └── ...\n"
                "│\n"
                "├── src\n"
                "│   ├── (name).c\n"
                "│   └── ...\n"
                "│\n"
                "└── tests\n"
                "    ├── resources\n"
                "    ├── tools\n"
                "    │   ├── testrun.h\n"
                "    │   ├── testrun_runner.sh\n"
                "    │   ├── testrun_gcov.sh\n"
                "    │   ├── testrun_gprof.sh\n"
                "    │   ├── testrun_simple_coverage_tests.sh\n"
                "    │   ├── testrun_simple_unit_tests.sh\n"
                "    │   ├── testrun_simple_acceptance_tests.sh\n"
                "    │   └── testrun_simple_loc.sh\n"
                "    │\n"
                "    ├── acceptance\n"
                "    │   ├── ...\n"
                "    │   └── ...\n"
                "    │\n"
                "    └── unit\n"
                "        ├── (name)_test.c\n"
                "        └── ...\n"
                "\n"
                "</pre>\n"
                "\\`\\`\\`\n"
                "\n"
                "## Tests\n"
                "\n"
                "All test sources will be recompiled on each make run. That means,\n"
                "all module tests will be created new on any change in any source file.\n"
                "\n"
                "### Test a project (all files contained in tests/unit)\n"
                "\n"
                "Test compile and run\n"
                "~~~\n"
                "make tested\n"
                "~~~\n"
                "\n"
                "## Tips\n"
                "\n"
                "## Copyright\n"
                "\n"
                "(null)\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        result = testrun_generate_readme("(name)", "DESCRIPTION", "COPYRIGHT");
        expect = "# Project (name)\n"
                "\n"
                "This module is self supported and may be build, tested, installed and\n"
                "run independently.\n"
                "\n"
                "## Overview\n"
                "\n"
                "* [Description](#description)\n"
                "* [Usage](#usage)\n"
                "* [Installation](#installation)\n"
                "* [Requirements](#requirements)\n"
                "* [Structure](#structure)\n"
                "* [Tests](#tests)\n"
                "* [Tips](#tips)\n"
                "* [Copyright](#copyright)\n"
                "\n"
                "## Description\n"
                "\n"
                "DESCRIPTION\n"
                "\n"
                "## Usage\n"
                "\n"
                "...\n"
                "\n"
                "## Installation\n"
                "\n"
                "...\n"
                """\n"
                "### build sources\n"
                "\n"
                "\\`\\`\\`bash\n"
                "make\n"
                "\\`\\`\\`\n"
                "\n"
                "### build documentation\n"
                "\n"
                "\\`\\`\\`bash\n"
                "make documentation\n"
                "\\`\\`\\`\n"
                "\n"
                "### test sources\n"
                "\n"
                "\\`\\`\\`bash\n"
                "make tested\n"
                "\\`\\`\\`\n"
                "\n"
                "### install binaries\n"
                "\n"
                "\\`\\`\\`bash\n"
                "sudo make install\n"
                "\\`\\`\\`\n"
                "\n"
                "### uninstall binaries\n"
                "\n"
                "\\`\\`\\`bash\n"
                "sudo make uninstall\n"
                "\\`\\`\\`\n"
                "\n"
                "## Requirements\n"
                "\n"
                "## Structure\n"
                "\n"
                "### Default structure of the folder:\n"
                "\n"
                "\\`\\`\\`\n"
                "<pre>\n"
                ".\n"
                "├── README.MD\n"
                "├── .gitignore\n"
                "├── makefile\n"
                "├── makefile_common.mk\n"
                "│\n"
                "├── copyright\n"
                "│   └── ... \n"
                "│\n"
                "├── doxygen\n"
                "│   ├── documentation\n"
                "│   └── doxygen.config\n"
                "│\n"
                "├── docs\n"
                "│   ├── CHANGELOG.MD\n"
                "│   └── ...\n"
                "│\n"
                "├── include\n"
                "│   ├── (name).h\n"
                "│   └── ...\n"
                "│\n"
                "├── src\n"
                "│   ├── (name).c\n"
                "│   └── ...\n"
                "│\n"
                "└── tests\n"
                "    ├── resources\n"
                "    ├── tools\n"
                "    │   ├── testrun.h\n"
                "    │   ├── testrun_runner.sh\n"
                "    │   ├── testrun_gcov.sh\n"
                "    │   ├── testrun_gprof.sh\n"
                "    │   ├── testrun_simple_coverage_tests.sh\n"
                "    │   ├── testrun_simple_unit_tests.sh\n"
                "    │   ├── testrun_simple_acceptance_tests.sh\n"
                "    │   └── testrun_simple_loc.sh\n"
                "    │\n"
                "    ├── acceptance\n"
                "    │   ├── ...\n"
                "    │   └── ...\n"
                "    │\n"
                "    └── unit\n"
                "        ├── (name)_test.c\n"
                "        └── ...\n"
                "\n"
                "</pre>\n"
                "\\`\\`\\`\n"
                "\n"
                "## Tests\n"
                "\n"
                "All test sources will be recompiled on each make run. That means,\n"
                "all module tests will be created new on any change in any source file.\n"
                "\n"
                "### Test a project (all files contained in tests/unit)\n"
                "\n"
                "Test compile and run\n"
                "~~~\n"
                "make tested\n"
                "~~~\n"
                "\n"
                "## Tips\n"
                "\n"
                "## Copyright\n"
                "\n"
                "COPYRIGHT\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_service_file(){

        char *expect = NULL;
        char *result = NULL;

        result = testrun_generate_service_file(NULL, NULL);
        expect = "[Unit]\n"
                "Description= (null) service\n"
                "\n"
                "[Service]\n"
                "ExecStart=(null)\n"
                "NonBlocking=True\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        result = testrun_generate_service_file("name", "path");
        expect = "[Unit]\n"
                "Description= name service\n"
                "\n"
                "[Service]\n"
                "ExecStart=path\n"
                "NonBlocking=True\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_socket_file(){

        char *expect = NULL;
        char *result = NULL;

        result = testrun_generate_socket_file(NULL);
        expect = "[Unit]\n"
                "Description= (null) socket\n"
                "\n"
                "[Socket]\n"
                "\n"
                "# example interface bound\n"
                "# ListenStream=10.0.0.100:12345\n"
                "\n"
                "# example localhost\n"
                "# ListenStream=127.0.0.1:12345\n"
                "\n"
                "# example listen on all\n"
                "# ListenStream=0.0.0.0:12345\n"
                "\n"
                "# example listen on UDP\n"
                "# ListenDatagram=0.0.0.0:12345\n"
                "\n"
                "# Maximum parallel connections for the socket\n"
                "Backlog=2048\n"
                "\n"
                "# TCP Keepalive (1h)\n"
                "KeepAlive=false\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        result = testrun_generate_socket_file("name");
        expect = "[Unit]\n"
                "Description= name socket\n"
                "\n"
                "[Socket]\n"
                "\n"
                "# example interface bound\n"
                "# ListenStream=10.0.0.100:12345\n"
                "\n"
                "# example localhost\n"
                "# ListenStream=127.0.0.1:12345\n"
                "\n"
                "# example listen on all\n"
                "# ListenStream=0.0.0.0:12345\n"
                "\n"
                "# example listen on UDP\n"
                "# ListenDatagram=0.0.0.0:12345\n"
                "\n"
                "# Maximum parallel connections for the socket\n"
                "Backlog=2048\n"
                "\n"
                "# TCP Keepalive (1h)\n"
                "KeepAlive=false\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target\n";

        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_tools_validate(){

        testrun_tools tools = testrun_tools_default();

        testrun(!testrun_tools_validate(NULL));

        testrun(testrun_tools_validate(&tools));

        tools.testrun_header = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.testrun_header = testrun_generate_header;
        testrun(testrun_tools_validate(&tools));

        tools.testrun_header_openmp = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.testrun_header_openmp = testrun_generate_header_openmp;
        testrun(testrun_tools_validate(&tools));

        tools.testrun_simple_tests = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.testrun_simple_tests = testrun_generate_script_simple_tests;
        testrun(testrun_tools_validate(&tools));

        tools.testrun_runner = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.testrun_runner = testrun_generate_script_runner;
        testrun(testrun_tools_validate(&tools));

        tools.testrun_loc = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.testrun_loc = testrun_generate_script_loc;
        testrun(testrun_tools_validate(&tools));

        tools.testrun_simple_coverage = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.testrun_simple_coverage = testrun_generate_script_coverage;
        testrun(testrun_tools_validate(&tools));

        tools.testrun_gcov = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.testrun_gcov = testrun_generate_script_gcov;
        testrun(testrun_tools_validate(&tools));

        tools.testrun_gprof = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.testrun_gprof = testrun_generate_script_gprof;
        testrun(testrun_tools_validate(&tools));

        tools.makefile_configurable = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.makefile_configurable = testrun_generate_makefile;
        testrun(testrun_tools_validate(&tools));

        tools.makefile_common = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.makefile_common = testrun_generate_makefile_common;
        testrun(testrun_tools_validate(&tools));

        tools.gitignore = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.gitignore = testrun_generate_gitignore;
        testrun(testrun_tools_validate(&tools));

        tools.readme = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.readme = testrun_generate_readme;
        testrun(testrun_tools_validate(&tools));

        tools.doxygen = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.doxygen = testrun_generate_doxygen;
        testrun(testrun_tools_validate(&tools));

        tools.service_file = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.service_file = testrun_generate_service_file;
        testrun(testrun_tools_validate(&tools));

        tools.socket_file = NULL;
        testrun(!testrun_tools_validate(&tools));
        tools.socket_file = testrun_generate_socket_file;
        testrun(testrun_tools_validate(&tools));

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                    #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int all_tests() {

        testrun_init();
        testrun_test(test_testrun_generate_header);
        testrun_test(test_testrun_generate_header_openmp);

        testrun_test(test_testrun_generate_script_simple_tests);
        testrun_test(test_testrun_generate_script_runner);
        testrun_test(test_testrun_generate_script_loc);
        testrun_test(test_testrun_generate_script_coverage);
        testrun_test(test_testrun_generate_script_gcov);
        testrun_test(test_testrun_generate_script_gprof);

        testrun_test(test_testrun_generate_makefile_common);
        testrun_test(test_testrun_generate_makefile);

        testrun_test(test_testrun_tools_default);

        testrun_test(test_testrun_generate_doxygen);
        testrun_test(test_testrun_generate_gitignore);
        testrun_test(test_testrun_generate_readme);
        testrun_test(test_testrun_generate_service_file);
        testrun_test(test_testrun_generate_socket_file);

        testrun_test(test_testrun_tools_validate);

        return testrun_counter;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST EXECUTION                                                  #EXEC
 *
 *      ------------------------------------------------------------------------
 */

testrun_run(all_tests);

