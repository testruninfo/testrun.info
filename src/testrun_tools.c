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
        @file           testrun_tools.c
        @author         [AUTHOR]
        @date           2018-07-10

        @ingroup        testrun_lib

        @brief


        ------------------------------------------------------------------------
*/
#include "../include/testrun_tools.h"

#include "testrun_header_constant.c"
#include "testrun_header_openmp_constant.c"

const char *bash_header = 
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
"#       ------------------------------------------------------------------------\n";

/*----------------------------------------------------------------------------*/

char *testrun_generate_header(){

        return strdup(testrun_header);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_header_openmp(){

        return strdup(testrun_header_openmp);
}

/*----------------------------------------------------------------------------*/

char *testrun_generate_script_simple_unit_tests(
        const char *runner_script,
        const char *path_logfile,
        const char *path_tests){

        if (!runner_script || !path_logfile || !path_tests)
                return NULL;

        size_t size = 5000;
        char buffer[size];
        memset(buffer, 0, size);

        if (0 > snprintf(buffer, size, 
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
                runner_script
                ))
                return NULL;

        return strdup(buffer);
}