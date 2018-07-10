/***
        ------------------------------------------------------------------------

        Copyright 2018 [COPYRIGHT_OWNER]

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
        @author         [AUTHOR]
        @date           2018-07-10

        @ingroup        testrun_lib

        @brief


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

int test_testrun_header(){
        
        char *result = testrun_generate_header();
        testrun(result);
        testrun(0 == strncmp(result, 
                helper_testrun_header, 
                strlen(helper_testrun_header)));
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_header_openmp(){
        
        char *result = testrun_generate_header_openmp();
        testrun(result);
        testrun(0 == strncmp(result, 
                helper_testrun_header_openmp, 
                strlen(helper_testrun_header_openmp)));
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_generate_script_simple_unit_tests(){

        char *runner       = "runner";
        char *path_logfile = "some/path";
        char *path_tests   = "tests/path";
        char *result       = NULL;

        size_t size = 5000;
        char buffer[size];

        testrun(!testrun_generate_script_simple_unit_tests(NULL,   NULL,         NULL));
        testrun(!testrun_generate_script_simple_unit_tests(runner, path_logfile, NULL));
        testrun(!testrun_generate_script_simple_unit_tests(NULL,   path_logfile, path_tests));
        testrun(!testrun_generate_script_simple_unit_tests(runner, NULL,         path_tests));

        memset(buffer, 0, size);
        testrun(0 < snprintf(buffer, 
        "%s"
        "#\n"
        "#       File            testrun_simpe_unit_tests.sh\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2017-11-30\n"
        "#\n"
        "#       Project         testrun_simpe_unit_tests.sh\n"
        "#\n"
        "#       Description     Run all test executables build/test/unit/*.test\n"
        "#                       Run the whole folder, until an error occurs.\n"
        "#\n"
        "#                       MODE         FAIL ON ERROR (Fail on first test error)\n"
        "#\n"
        "#                       LOGFILE      %s/unittest.<time>.log\n"
        "#\n"
        "#\n"
        "#       Usage           ./testrun_simpe_unit_tests.sh /path/to/project\n"
        "#\n"
        "#       Dependencies    bash, touch, chmod, ls, wc, date\n"
        "#\n"
        "#       Last changed    2017-11-30\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE UNIT TESTING\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"%s/unittest\".$start_time.\"log\"\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "\n"
        "# SET THE FOLDER\n"
        "FOLDER=\"%s\"\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               REPORT UNIT TESTING\"                      >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "# RUN THE RUNNER\n"
        "sh %s  $LOGFILE $FOLDER FAIL_ON_ERROR\n"
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
        "exit $RESULT\n",
                bash_header,
                path_logfile,
                path_logfile,
                path_tests,
                runner
                ));
        result = testrun_generate_script_simple_unit_tests(runner, path_logfile, path_tests);
        testrun(result);
        testrun(0 == strncmp(result, buffer, strlen(buffer)));
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                    #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int all_tests() {

        testrun_init();
        testrun_test(test_testrun_header);
        testrun_test(test_testrun_header_openmp);

        testrun_test(test_testrun_generate_script_simple_unit_tests);

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

