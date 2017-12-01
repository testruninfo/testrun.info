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

        @file           testrun_lib.c
        @author         Markus Toepfer
        @date           2017-11-29

        @ingroup        testrun_lib

        @brief          Generic lib related functions.


        ------------------------------------------------------------------------
*/

#include "../include/testrun_lib.h"

/*----------------------------------------------------------------------------*/

char *testrun_lib_testrun_header_content(){

        char    *result = NULL;
        size_t  re_size = 0;

        char    *step1  = NULL;
        char    *step2  = NULL;
        size_t  size1   = 0;
        size_t  size2   = 0;

        size_t size = 10000;
        char buffer[size];
        bzero(buffer, size);

        char *name   = "testrun";
        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author,
                "This file is part of the testrun project. http://testrun.info");

        testrun_config config   = testrun_config_default();
        config.project.name     = name;
        config.copyright        = copyright;
        config.author           = author;

        char *line = testrun_text_block_splitline(0, 81, true);

        char *head = testrun_copyright_default_c_header(
                        &copyright, true);

        char *docu = testrun_text_block_c_header_documentation(
                        name, TESTRUN_HEADER, &config, false,
                        "Simple serial test execution framework.");

        char *lineend = config.format.line_end;
        size_t le_len = strlen(lineend);

        /*
         *      SXLINE    = lineend
         *      STRIPLINE = SXLINE stripline SXLINE
         */
        if (snprintf(buffer, size,
                "%s%sSXLINE"
                "#ifndef testrun_hSXLINE"
                "#define testrun_hSXLINE"
                "SXLINE"
                "#include <unistd.h>     /* C89/C90 */SXLINE"
                "#include <stdlib.h>     /* C89/C90 */SXLINE"
                "#include <stdio.h>      /* C89/C90 */SXLINE"
                "#include <string.h>     /* C89/C90 */SXLINE"
                "#include <errno.h>      /* C89/C90 */SXLINE"
                "#include <time.h>       /* C89/C90 */SXLINE"
                "STRIPLINE"
                "#define testrun_errno() \\SXLINE"
                "        (errno == 0 ? \"NONE\" :  strerror(errno))SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Log a failure. Failure: Inability to perform a function as expected.SXLINE"
                "*/SXLINE"
                "#define testrun_log_failure(msg, ...) \\SXLINE"
                "        fprintf(stderr, \"\\t[FAIL]\\t%%s line:%%d errno:%%s message: \" msg \"\\n\",\\SXLINE"
                "               __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Log an error. Error: Difference between expected and actual result.SXLINE"
                "*/SXLINE"
                "#define testrun_log_error(msg, ...) \\SXLINE"
                "        fprintf(stderr, \"\\t[ERROR]\\t%%s line:%%d errno:%%s message: \" msg \"\\n\",\\SXLINE"
                "        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)SXLINE"
                "STRIPLINE"
                "#define testrun_log_success(msg, ...) \\SXLINE"
                "        fprintf(stdout, \"\\t[OK] \\t%%s \" msg \"\\n\", __FUNCTION__, ##__VA_ARGS__)SXLINE"
                "STRIPLINE"
                "#define testrun_log(msg, ...) \\SXLINE"
                "        fprintf(stdout, \"\\t\" msg \"\\n\", ##__VA_ARGS__)SXLINE"
                "STRIPLINE"
                "#define testrun_log_function_info(msg, ...) \\SXLINE"
                "        fprintf(stdout, \"\\t[INFO] \\t%%s line:%%d message: \" msg \"\\n\", \\SXLINE"
                "                __FUNCTION__, __LINE__, ##__VA_ARGS__)SXLINE"
                "STRIPLINE"
                "#define testrun_log_clock(start, end) \\SXLINE"
                "        fprintf(stdout, \"\\tClock ticks function: ( %%s ) | %%f s | %%.0f ms \\n\", \\SXLINE"
                "        __func__, \\SXLINE"
                "        ((double)(end - start)) / CLOCKS_PER_SEC, \\SXLINE"
                "        (((double)(end - start)) / CLOCKS_PER_SEC ) * 1000)SXLINE"
                "STRIPLINE"
                "#define testrun_init()  \\SXLINE"
                "        int result = 0; \\SXLINE"
                "        int testrun_counter = 0;SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Run a single atomar test. Return the surrounding block on error.SXLINE"
                "        This function will leave the context block running on error. TheSXLINE"
                "        Mindset is a defused assert. LEAVE THE FUNCTION NOT THE PROGRAM.SXLINE"
                "SXLINE"
                "        @param test     boolean decision input.SXLINE"
                "        @returns        the calling function on error with -1SXLINE"
                "*/SXLINE"
                "#define testrun_check(test, ... )\\SXLINE"
                "        if (!(test)) { testrun_log_error(__VA_ARGS__);  return -1;}SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Alias to @see testrun_check.SXLINE"
                "*/SXLINE"
                "#define testrun(test, ...)\\SXLINE"
                "        testrun_check(test, __VA_ARGS__ )SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Run a single test (execute a function pointer. Runs a test function.SXLINE"
                "        On non negative return value of the function run, a testrun_counterSXLINE"
                "        is increased, on negative result, the negative result will be returned.SXLINE"
                "SXLINE"
                "        @param test     function pointer of the test to runSXLINE"
                "        @NOTE           The surrounding block is left on negative result of theSXLINE"
                "                        function pointer execution.SXLINE"
                "*/SXLINE"
                "#define testrun_test(test)\\SXLINE"
                "        result = test(); testrun_counter++; if (result < 0) return result;SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Runs a function pointer, which SHALL contain the test function pointersSXLINE"
                "        to run. The function pointer is wrapped in a main procedure, which andSXLINE"
                "        allows indepentent testruns of the input testcluster over externalSXLINE"
                "        execution.SXLINE"
                "SXLINE"
                "        A clock will be started, as soon as the main is executed and the theSXLINE"
                "        time is stopped again, at the end of the execution. The differenceSXLINE"
                "        will be printed and is the runtime of the whole input testcluster.SXLINE"
                "SXLINE"
                "        A run will fail, as soon as one of the tests in the testcluster fails.SXLINE"
                "        (Fail on first) or will run all functions dependent on the testsetup.SXLINE"
                "SXLINE"
                "        @param testcluster      function pointer to be executed.SXLINE"
                "*/SXLINE"
                "#define testrun_run(testcluster) int main(int argc, char *argv[]) {\\SXLINE"
                "        argc = argc;\\SXLINE"
                "        clock_t start1_t, end1_t; \\SXLINE"
                "        start1_t = clock(); \\SXLINE"
                "        testrun_log(\"\\ntestrun\\t%%s\", argv[0]);\\SXLINE"
                "        int64_t result = testcluster();\\SXLINE"
                "        if (result > 0) \\SXLINE"
                "                testrun_log(\"ALL TESTS RUN (%%jd tests)\", result);\\SXLINE"
                "        end1_t = clock(); \\SXLINE"
                "        testrun_log_clock(start1_t, end1_t); \\SXLINE"
                "        testrun_log("");\\SXLINE"
                "        result >=0 ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE); \\SXLINE"
                "}SXLINE"
                "SXLINE"
                "/*--------------- For EXAMPLE code check http://testrun.info -----------------*/"
                "SXLINE"
                "SXLINE"
                "#endif /* testrun_h */"
                ,head, docu) < 0)
                        goto error;


        char old[1000];
        char new[1000];
        bzero(old, 1000);
        bzero(new, 1000);

        if (snprintf(old, 1000, "STRIPLINE") < 0)
                goto error;

        if (snprintf(new, 1000, "SXLINE%sSXLINESXLINE", line) < 0)
                goto error;

        // insert the stripline
        if (!testrun_string_replace_all(&step1, &size1,
                buffer, size,
                old, strlen(old),
                new, strlen(new),
                false))
                goto error;

        // insert the newlines
        if (snprintf(old, 1000, "SXLINE") < 0)
                goto error;

        if (snprintf(new, 1000, "%s", lineend) < 0)
                goto error;

        if (!testrun_string_replace_all(&step2, &size2,
                step1, size1,
                old, strlen(old),
                new, strlen(new),
                false))
                goto error;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                step2, size2,
                lineend, le_len))
                goto error;

        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        line  = testrun_string_free(line);
        head  = testrun_string_free(head);
        docu  = testrun_string_free(docu);

        return result;
error:
        line  = testrun_string_free(line);
        head  = testrun_string_free(head);
        docu  = testrun_string_free(docu);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        return NULL;
}


/*----------------------------------------------------------------------------*/

char *testrun_lib_script_folder_runner_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = config.project.path.tests.tools.runner_script;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        snprintf(description, d_size,
        "Run each test.test of a folder and log Ok or NOK"                              TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"for each executed testfile of the folder."                   TESTRUN_TAG_END
        "#"                                                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"EXAMPLE OUTPUT"                                              TESTRUN_TAG_END
        "#"                                                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"[OK]  1/5 filename1.test"                                    TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"[NOK] 2/5 filename2.test"                                    TESTRUN_TAG_END
        "#"                                                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"MODES"                                                       TESTRUN_TAG_END
        "#"                                                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"(1) RUN ALL TESTS (log but ignore errors)"                   TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"    use script with 2 parameters "                           TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"    e.g. ./%s logfile /path"                                 TESTRUN_TAG_END
        "#"                                                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"    This mode will not return a test failure and"            TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"    may be used to run all tests and return success"         TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"    if all tests was run. (test results are logged)"         TESTRUN_TAG_END
        "#"                                                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"(2) FAIL ON ERROR (Fail on first error)"                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"    use script with 3 parameters"                            TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"    e.g. ./%s logfile /path 1"                               TESTRUN_TAG_END
        "#"                                                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"    This mode returns -1 on the first test failure."         TESTRUN_TAG_END
        "#"                                                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"PARAMETER"                                                   TESTRUN_TAG_END
        "#"                                                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"(1) path to logfile destination"                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"(2) path to folder with test cases"                          TESTRUN_TAG_END
        "#"
        ,config.project.name, config.project.name);

        snprintf(usage, d_size,
                "./%s /path/to/logfile /path/to/test/dir",
                config.project.name);

        snprintf(dependencies, d_size, "bash, tail, ls, grep, wc");


        snprintf(content, c_size,
        TESTRUN_TAG_END
        "#       ------------------------------------------------------------------------"     TESTRUN_TAG_END
        "#       CONFIGURE SCRIPT BASED ON PRESET VARIBALES OR ON INPUT"                      TESTRUN_TAG_END
        "#       ------------------------------------------------------------------------"     TESTRUN_TAG_END
        TESTRUN_TAG_END
        "if [ -z $1 ]; then"                                                                            TESTRUN_TAG_END
        "        echo \"ERROR ... NO LOGFILE INPUT TO SRCIPT\""                                         TESTRUN_TAG_END
        "        exit 1"                                                                                TESTRUN_TAG_END
        "fi"                                                                                            TESTRUN_TAG_END
        "LOGFILE=$1"                                                                                    TESTRUN_TAG_END
        TESTRUN_TAG_END
        "if [ -z $2 ]; then"                                                                            TESTRUN_TAG_END
        "        echo \"ERROR ... FOLDER INPUT TO SRCIPT\""                                             TESTRUN_TAG_END
        "        exit 1"                                                                                TESTRUN_TAG_END
        "fi"                                                                                            TESTRUN_TAG_END
        "FOLDER=$2"                                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_END
        "FAIL_ON_ERROR=0"                                                                               TESTRUN_TAG_END
        "if [ ! -z $3 ]; then"                                                                          TESTRUN_TAG_END
        "        FAIL_ON_ERROR=1  "                                                                     TESTRUN_TAG_END
        "fi"                                                                                            TESTRUN_TAG_END
        TESTRUN_TAG_END
        "if [ ! -w $LOGFILE ]; then"                                                                    TESTRUN_TAG_END
        "        echo \"ERROR ... LOGFILE NOT WRITABLE\""                                               TESTRUN_TAG_END
        "        exit 1"                                                                                TESTRUN_TAG_END
        "fi"                                                                                            TESTRUN_TAG_END
        TESTRUN_TAG_END
        "#       ------------------------------------------------------------------------"              TESTRUN_TAG_END
        "#       PERFORM TESTRUN"TESTRUN_TAG_END
        "#       ------------------------------------------------------------------------"              TESTRUN_TAG_END
        TESTRUN_TAG_END
        "FILES=`ls  $FOLDER/ | grep \"\\.test\" | wc -l`"                                               TESTRUN_TAG_END
        "if [ $? -ne 0 ]; then"                                                                         TESTRUN_TAG_END
        "        echo \"ERROR ... could not count files of $FOLDER\""                                   TESTRUN_TAG_END
        "        exit 1"                                                                                TESTRUN_TAG_END
        "fi"                                                                                            TESTRUN_TAG_END
        "c=0"                                                                                           TESTRUN_TAG_END
        TESTRUN_TAG_END
        "if [ $FILES -eq 0 ]; then"                                                                     TESTRUN_TAG_END
        "        exit 0"                                                                                TESTRUN_TAG_END
        "fi"                                                                                            TESTRUN_TAG_END
        TESTRUN_TAG_END
        "for i in $FOLDER/*%s"                                                                          TESTRUN_TAG_END
        "do"                                                                                            TESTRUN_TAG_END
        "        c=$((c+1))"                                                                            TESTRUN_TAG_END
        TESTRUN_TAG_END
        "        # RUN EXECUTABLE"                                                                      TESTRUN_TAG_END
        "        $i 2>&1 >> $LOGFILE"                                                                   TESTRUN_TAG_END
        TESTRUN_TAG_END
        "        # CHECK RETURN OF EXECUTABLE"                                                          TESTRUN_TAG_END
        "        if [ $? -ne 0 ]; then"                                                                 TESTRUN_TAG_END
        TESTRUN_TAG_END
        "                echo \"NOK\\t(\"$c\"/\"$FILES\")\\t\"$i"                                       TESTRUN_TAG_END
        TESTRUN_TAG_END
        "                if [ $FAIL_ON_ERROR -eq 1 ]; then"                                             TESTRUN_TAG_END
        "                        exit 1"                                                                TESTRUN_TAG_END
        "                fi"                                                                            TESTRUN_TAG_END
        "        else"                                                                                  TESTRUN_TAG_END
        "                echo \"OK\\t(\"$c\"/\"$FILES\")\\t\"$i"                                        TESTRUN_TAG_END
        "        fi"                                                                                    TESTRUN_TAG_END
        "done"                                                                                          TESTRUN_TAG_END
        "exit 0"                                                                                        TESTRUN_TAG_END
        ,config.format.extensions.testexec);

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_script_acceptance_tests_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = config.project.path.tests.tools.acceptance_script;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        char testrunner[PATH_MAX];
        char buffer[PATH_MAX];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(testrunner,       PATH_MAX);
        bzero(buffer,           PATH_MAX);

        if (!testrun_path_project_to_tools(buffer, PATH_MAX,
                &config))
                return NULL;

        if (snprintf(testrunner, PATH_MAX, "%s%s%s",
                buffer, config.format.path_split,
                config.project.path.tests.tools.runner_script) < 0)
                return NULL;

        snprintf(description, d_size,
        "Run all test executables build/test/acceptance/*.test"                         TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"Run the whole folder, until an error occurs."                TESTRUN_TAG_END
        "#"                                                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"MODE         FAIL ON ERROR (Fail on first test error)"       TESTRUN_TAG_END
        "#"                                                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"LOGFILE      build/test/log/acceptancetest.<time>.log"       TESTRUN_TAG_END
        "#"
        );

        snprintf(usage, d_size,
                "./%s /path/to/project",
                config.project.path.tests.tools.acceptance_script);

        snprintf(dependencies, d_size, "bash, touch, chmod, ls, wc, date");


        snprintf(content, c_size,
        "echo \"-------------------------------------------------------\""      TESTRUN_TAG_END
        "echo \"               SIMPLE ACCEPTANCE TESTING\""                     TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\""      TESTRUN_TAG_END
        TESTRUN_TAG_END
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")"                   TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# SET A LOGFILE"                                                       TESTRUN_TAG_END
        "LOGFILE=\"build/test/log/acceptance_\".$start_time.\"log\""            TESTRUN_TAG_END
        "echo \" (log)   $start_time\" > $LOGFILE"                              TESTRUN_TAG_END
        "touch $LOGFILE"                                                        TESTRUN_TAG_END
        "chmod a+w $LOGFILE"                                                    TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# SET THE FOLDER"                                                      TESTRUN_TAG_END
        "FOLDER=\"build/test/acceptance\""                                      TESTRUN_TAG_END
        TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\" >> $LOGFILE" TESTRUN_TAG_END
        "echo \"               REPORT ACCEPTANCE TESTING\"                >> $LOGFILE" TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\" >> $LOGFILE"  TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# RUN THE RUNNER"                                                      TESTRUN_TAG_END
        "sh %s  $LOGFILE $FOLDER FAIL_ON_ERROR"                                 TESTRUN_TAG_END
        "RESULT=$?"                                                             TESTRUN_TAG_END
        TESTRUN_TAG_END
        "end_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")"                     TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# FINISH THE REPORT"                                                   TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\">> $LOGFILE"  TESTRUN_TAG_END
        "echo \"DONE \\t ACCEPTANCE TEST RUN\"  >> $LOGFILE"                    TESTRUN_TAG_END
        "if [ $RESULT -eq 0 ]; then"                                            TESTRUN_TAG_END
        "        echo \"RESULT\\t SUCCESS\"  >> $LOGFILE"                       TESTRUN_TAG_END
        "else"                                                                  TESTRUN_TAG_END
        "        echo \"RESULT\\t FAILURE\"  >> $LOGFILE"                       TESTRUN_TAG_END
        "fi"                                                                    TESTRUN_TAG_END
        "echo \"START \\t $start_time\" >> $LOGFILE"                            TESTRUN_TAG_END
        "echo \"END   \\t $end_time\" >> $LOGFILE"                              TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\">> $LOGFILE"      TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# DUMP THE REPORT"                                                   TESTRUN_TAG_END
        "cat $LOGFILE"                                                          TESTRUN_TAG_END
        "echo \"\""                                                             TESTRUN_TAG_END
        "exit $RESULT"                                                          TESTRUN_TAG_END
        ,
        testrunner);

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_script_unit_tests_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = config.project.path.tests.tools.unit_script;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        char testrunner[PATH_MAX];
        char buffer[PATH_MAX];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(testrunner,       PATH_MAX);
        bzero(buffer,           PATH_MAX);

        if (!testrun_path_project_to_tools(buffer, PATH_MAX,
                &config))
                return NULL;

        if (snprintf(testrunner, PATH_MAX, "%s%s%s",
                buffer, config.format.path_split,
                config.project.path.tests.tools.runner_script) < 0)
                return NULL;

        snprintf(description, d_size,
        "Run all test executables build/test/unit/*.test"                         TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"Run the whole folder, until an error occurs."          TESTRUN_TAG_END
        "#"                                                                       TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"MODE         FAIL ON ERROR (Fail on first test error)" TESTRUN_TAG_END
        "#"                                                                       TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"LOGFILE      build/test/log/unittest.<time>.log"       TESTRUN_TAG_END
        "#"
        );

        snprintf(usage, d_size,
                "./%s /path/to/project",
                config.project.path.tests.tools.unit_script);

        snprintf(dependencies, d_size, "bash, touch, chmod, ls, wc, date");


        snprintf(content, c_size,
        "echo \"-------------------------------------------------------\""      TESTRUN_TAG_END
        "echo \"               SIMPLE UNIT TESTING\""                           TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\""      TESTRUN_TAG_END
        TESTRUN_TAG_END
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")"                   TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# SET A LOGFILE"                                                       TESTRUN_TAG_END
        "LOGFILE=\"build/test/log/unit_\".$start_time.\"log\""                  TESTRUN_TAG_END
        "echo \" (log)   $start_time\" > $LOGFILE"                              TESTRUN_TAG_END
        "touch $LOGFILE"                                                        TESTRUN_TAG_END
        "chmod a+w $LOGFILE"                                                    TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# SET THE FOLDER"                                                      TESTRUN_TAG_END
        "FOLDER=\"build/test/unit\""                                            TESTRUN_TAG_END
        TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\" >> $LOGFILE" TESTRUN_TAG_END
        "echo \"               REPORT UNIT TESTING\"                      >> $LOGFILE" TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\" >> $LOGFILE" TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# RUN THE RUNNER"                                                      TESTRUN_TAG_END
        "sh %s  $LOGFILE $FOLDER FAIL_ON_ERROR"                                 TESTRUN_TAG_END
        "RESULT=$?"                                                             TESTRUN_TAG_END
        TESTRUN_TAG_END
        "end_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")"                     TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# FINISH THE REPORT"                                                   TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\">> $LOGFILE"  TESTRUN_TAG_END
        "echo \"DONE \\t UNIT TEST RUN\"  >> $LOGFILE"                          TESTRUN_TAG_END
        "if [ $RESULT -eq 0 ]; then"                                            TESTRUN_TAG_END
        "        echo \"RESULT\\t SUCCESS\"  >> $LOGFILE"                       TESTRUN_TAG_END
        "else"                                                                  TESTRUN_TAG_END
        "        echo \"RESULT\\t FAILURE\"  >> $LOGFILE"                       TESTRUN_TAG_END
        "fi"                                                                    TESTRUN_TAG_END
        "echo \"START \\t $start_time\" >> $LOGFILE"                            TESTRUN_TAG_END
        "echo \"END   \\t $end_time\" >> $LOGFILE"                              TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\">> $LOGFILE" TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# DUMP THE REPORT"                                                     TESTRUN_TAG_END
        "cat $LOGFILE"                                                          TESTRUN_TAG_END
        "echo \"\""                                                             TESTRUN_TAG_END
        "exit $RESULT"                                                          TESTRUN_TAG_END
        ,
        testrunner);

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}


/*----------------------------------------------------------------------------*/

char *testrun_lib_script_coverage_tests_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = config.project.path.tests.tools.coverage_script;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        char path_src[PATH_MAX];
        char path_unit[PATH_MAX];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(path_src,         PATH_MAX);
        bzero(path_unit,        PATH_MAX);

        if (!testrun_path_project_to_unit_tests(path_unit, PATH_MAX,
                &config))
                return NULL;

        if (!testrun_path_project_to_source(path_src, PATH_MAX,
                &config))
                return NULL;

        snprintf(description, d_size,
        "Count functions of folder src against their counterparts"              TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"in the unit test folder."                            TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"CONVENTION"                                          TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"Each function in any file of the source folder located"TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"\"%s\""                                              TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"will have a corresponding test function using the same "TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"name in any other file of the unit test folder located"TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"\"%s\","                                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"with a function name prefix of"                      TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"\"%s\"."                                             TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"EXAMPLE      function | %sfunction"                  TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"NOTE         This simple coverage test just covers the"TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"             observance of the given coding convention."   TESTRUN_TAG_END
        "#"
        ,path_src, path_unit, config.format.prefix.unit_test,config.format.prefix.unit_test);

        snprintf(usage, d_size,
                "./%s /path/to/project",
                config.project.path.tests.tools.coverage_script);

        snprintf(dependencies, d_size, "bash, ctags, awk, sed, grep");


        snprintf(content, c_size,
        TESTRUN_TAG_END
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S\")"                       TESTRUN_TAG_END
        "PREFIX=\"%s\""                                                         TESTRUN_TAG_END
        TESTRUN_TAG_END
        "LIBDIR=$1"                                                             TESTRUN_TAG_END
        "SRCDIR=$1/%s"                                                          TESTRUN_TAG_END
        "TESTDIR=$1/%s"                                                         TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# SET A LOGFILE"                                                       TESTRUN_TAG_END
        "LOGFILE=\"$LIBDIR/build/test/log/coverage_\".$start_time.\"log\""      TESTRUN_TAG_END
        "touch $LOGFILE"                                                        TESTRUN_TAG_END
        "chmod a+w $LOGFILE"                                                    TESTRUN_TAG_END
        TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\" >> $LOGFILE" TESTRUN_TAG_END
        "echo \"               REPORT COVERAGE TESTING\"                  >> $LOGFILE" TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\" >> $LOGFILE" TESTRUN_TAG_END
        "echo \"   TIME \\t $start_time\" >> $LOGFILE"                          TESTRUN_TAG_END
        "echo \"\" >> $LOGFILE"                                                 TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# GENERATE CTAGS SOURCE"                                               TESTRUN_TAG_END
        "cd $SRCDIR"                                                            TESTRUN_TAG_END
        "if [ $? -ne 0 ]; then"                                                 TESTRUN_TAG_END
        "        exit 1"                                                        TESTRUN_TAG_END
        "fi"                                                                    TESTRUN_TAG_END
        "ctags --c-types=f -R"                                                  TESTRUN_TAG_END
        "# remove the ctags stuff, to leave just the function lines"            TESTRUN_TAG_END
        "sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions"                        TESTRUN_TAG_END
        "# remove anything but the function names"                              TESTRUN_TAG_END
        "awk '{print $1 }' $SRCDIR/functions > \\"                              TESTRUN_TAG_END
        "        $SRCDIR/functionNames"                                         TESTRUN_TAG_END
        "# count the lines"                                                     TESTRUN_TAG_END
        "sourceFkt=\"$(cat functions | wc -l)\""                                TESTRUN_TAG_END
        "echo \"   count source\\t\" $sourceFkt >> $LOGFILE"                     TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# GENERATE CTAGS TESTS"                                                TESTRUN_TAG_END
        "cd $TESTDIR"                                                           TESTRUN_TAG_END
        "if [ $? -ne 0 ]; then"                                                 TESTRUN_TAG_END
        "        exit 1"                                                        TESTRUN_TAG_END
        "fi"                                                                    TESTRUN_TAG_END
        "ctags --c-types=f -R"                                                  TESTRUN_TAG_END
        "# remove the ctags stuff, to leave just the function lines"            TESTRUN_TAG_END
        "sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions"                        TESTRUN_TAG_END
        "# remove anything but the function names"                              TESTRUN_TAG_END
        "awk '{print $1 }' $TESTDIR/functions > \\"                             TESTRUN_TAG_END
        "        $TESTDIR/functionNames"                                        TESTRUN_TAG_END
        "# count the lines"                                                     TESTRUN_TAG_END
        "testFkt=\"$(cat functions | grep -i ^$PREFIX | wc -l)\""               TESTRUN_TAG_END
        "echo \"   count tests\\t\" $testFkt >> $LOGFILE"                       TESTRUN_TAG_END
        TESTRUN_TAG_END
        "echo \"\\nUNTESTED: \" >> $LOGFILE"                                    TESTRUN_TAG_END
        "# Found functions:"                                                    TESTRUN_TAG_END
        "while read line;"                                                      TESTRUN_TAG_END
        "do"                                                                    TESTRUN_TAG_END
        "        grep -n '^test_'$line'$' $TESTDIR/functionNames > \\"          TESTRUN_TAG_END
        "        /dev/null || echo $line >> $LOGFILE"                           TESTRUN_TAG_END
        "done < $SRCDIR/functionNames"                                          TESTRUN_TAG_END
        TESTRUN_TAG_END
        "if [ $sourceFkt != 0 ]; then"                                          TESTRUN_TAG_END
        "        echo \"............................................\"  >> $LOGFILE"TESTRUN_TAG_END
        "        echo \"COVERAGE: $sourceFkt $testFkt\" | \\"                   TESTRUN_TAG_END
        "        awk '{ printf $1 \" %%.2f %%%% \\n\", $3/$2*100}' >> $LOGFILE" TESTRUN_TAG_END
        "fi"                                                                    TESTRUN_TAG_END
        TESTRUN_TAG_END
        "cat $LOGFILE"                                                          TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\""      TESTRUN_TAG_END
        "echo \"\""TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# cleanup remove the files we created"TESTRUN_TAG_END
        "rm $SRCDIR/tags"TESTRUN_TAG_END
        "rm $SRCDIR/functions"TESTRUN_TAG_END
        "rm $SRCDIR/functionNames"TESTRUN_TAG_END
        "rm $TESTDIR/tags"TESTRUN_TAG_END
        "rm $TESTDIR/functions"TESTRUN_TAG_END
        "rm $TESTDIR/functionNames"TESTRUN_TAG_END
        ,config.format.prefix.unit_test, path_src, path_unit);

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_script_loc_tests_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = config.project.path.tests.tools.loc_script;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        char path_src[PATH_MAX];
        char path_unit[PATH_MAX];
        char path_acceptance[PATH_MAX];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(path_src,         PATH_MAX);
        bzero(path_unit,        PATH_MAX);
        bzero(path_acceptance,  PATH_MAX);

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(path_src,         PATH_MAX);
        bzero(path_unit,        PATH_MAX);

        if (!testrun_path_project_to_unit_tests(path_unit, PATH_MAX,
                &config))
                return NULL;

        if (!testrun_path_project_to_acceptance_tests(path_acceptance, PATH_MAX,
                &config))
                return NULL;

        if (!testrun_path_project_to_source(path_src, PATH_MAX,
                &config))
                return NULL;

        snprintf(description, d_size,
        "Count the lines of src and unit | acceptance tests."                   TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"This file uses no error checking.");

        snprintf(usage, d_size,
                "./%s /path/to/project",
                config.project.path.tests.tools.loc_script);

        snprintf(dependencies, d_size, "bash, find, xargs, wc");

        snprintf(content, c_size,
        "echo \"-------------------------------------------------------\""      TESTRUN_TAG_END
        "echo \"               SIMPLE LOC COUNTER\""                            TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\""      TESTRUN_TAG_END
        "echo \"(LOC) src\""                                                    TESTRUN_TAG_END
        "find $1/%s -name '*.c' | xargs wc -l"                                  TESTRUN_TAG_END
        "echo \"(LOC) tests/unit\""                                             TESTRUN_TAG_END
        "find $1/%s -name '*.c' | xargs wc -l"                                  TESTRUN_TAG_END
        "echo \"(LOC) tests/acceptance\""                                       TESTRUN_TAG_END
        "find $1/%s -name '*.c' | xargs wc -l"                                  TESTRUN_TAG_END
        "echo \"-------------------------------------------------------\""      TESTRUN_TAG_END
        "echo \"\""                                                             TESTRUN_TAG_END
        , path_src, path_unit, path_acceptance);

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}

/*----------------------------------------------------------------------------*/

char * testrun_lib_c_file_content(
        char * module_name,
        testrun_extension_t filetype,
        struct testrun_config const * const config){

        if (!config)
                return false;

        if (!module_name)
                return false;

        char *body = NULL;

        char *head = testrun_copyright_default_c_header(
                        &config->copyright, true);

        char *docu = testrun_text_block_c_header_documentation(
                        module_name,
                        filetype,
                        config, false, NULL);

        char source[PATH_MAX];
        bzero(source, PATH_MAX);

        char header[PATH_MAX];
        bzero(header, PATH_MAX);

        if (!head || !docu)
                return NULL;

        switch(filetype) {

                case TESTRUN_HEADER:

                        body = testrun_text_block_header_body(
                                module_name);
                        break;

                case TESTRUN_SOURCE:

                        if (!testrun_path_source_to_include(source, PATH_MAX,
                                config, module_name))
                                goto error;

                        body = testrun_text_block_source_body(
                                source);

                        break;

                case TESTRUN_TEST:


                        if (!testrun_path_test_to_source(source, PATH_MAX,
                                config, module_name))
                                goto error;

                        if (!testrun_path_test_to_testrun_header(header, PATH_MAX,
                                config))
                                goto error;

                        body = testrun_text_block_test_body(
                                source, header);

                        break;

                default:
                        goto error;
        }

        if (!body)
                goto error;

        char    *result = NULL;
        size_t  size    = strlen(head) + strlen(docu) + strlen(body) +
                          strlen(config->format.line_end) + 1;

        result = calloc(size, sizeof(char));
        if (!result)
                goto error;

        if (!snprintf(result, size, "%s%s%s%s", head, docu, body,
                config->format.line_end))
                goto error;

        head = testrun_string_free(head);
        docu = testrun_string_free(docu);
        body = testrun_string_free(body);
        return result;
error:
        head = testrun_string_free(head);
        docu = testrun_string_free(docu);
        body = testrun_string_free(body);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_makefile_content(testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.copyright       = copyright;
        config.author          = author;

        char *step1 = NULL;
        char *step2 = NULL;
        char *step3 = NULL;
        char *wrong = "#!/usr/bin/env bash";
        char *line  = testrun_text_block_splitline(8, 81, false);
        line[0]     = '#';

        size_t size1 = 0;
        size_t size2 = 0;
        size_t size3 = 0;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);


        snprintf(description, d_size,
        "This makefile defines project specific parameter."                     TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"These parameter are:"                                TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"(1) used compiler and special flags"                 TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"(2) name and version"                                TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"(3) installation prefix"                             TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"(4) used libraries"                                  TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"(5) general makefiles used"
        );

        snprintf(usage, d_size, "make");
        snprintf(dependencies, d_size, "make & compiler");

        snprintf(content, c_size,
        "CC = gcc"                                                              TESTRUN_TAG_END
        TESTRUN_TAG_END
        "PROJECT         := %s"                                                 TESTRUN_TAG_END
        "VERSION         := 0.0.1"                                              TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# project path recalculation (if used included from parent make)"      TESTRUN_TAG_END
        "PROJECTMK       := $(abspath $(lastword $(MAKEFILE_LIST)))"            TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# base directory for installation"                                     TESTRUN_TAG_END
        "PREFIX          := /usr/local"                                         TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# LIBS USED (uncommented example includes)"                            TESTRUN_TAG_END
        "#LIBS           += `pkg-config --libs libsystemd`"                     TESTRUN_TAG_END
        "#LIBS           += `pkg-config --libs uuid`"                           TESTRUN_TAG_END
        "#LIBS           += `pkg-config --libs openssl`"                        TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# EXTRA CFLAGS (example)"                                              TESTRUN_TAG_END
        "MODCFLAGS       += -std=gnu11"                                         TESTRUN_TAG_END
        "#MODCFLAGS      += -fopenmp"                                           TESTRUN_TAG_END
        TESTRUN_TAG_END
        "#GCC only flags (example)"                                             TESTRUN_TAG_END
        "#MODCFLAGS      += -rdynamic"                                          TESTRUN_TAG_END
        TESTRUN_TAG_END
        "#TMP FILE DEFINITION"                                                  TESTRUN_TAG_END
        "TESTS_TMP_FILES = $(wildcard /tmp/test_*)"                             TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# INCLUDE BASE MAKEFILE"                                               TESTRUN_TAG_END
        "include testrun_makefile.main"                                         TESTRUN_TAG_END
        "include testrun_makefile.test"                                         TESTRUN_TAG_END
        ,config.project.name);

        step1 = testrun_text_block_script(&config,
                description, usage, dependencies, content);

        if (!step1)
                goto error;

        size1 = strlen(step1);

        if (!testrun_string_replace_first(
                &step2, &size2,
                step1,  size1,
                wrong, strlen(wrong),
                line, strlen(line)))
                goto error;

        /*
         *      NOTE The next part replaces File [PROJECT], which is set
         *      based on the configuraion with File makefile.
         *      This will only work, as long as the indent used in
         *      shell headers is fixed 8 spaces.
         *      (20171201: Currently not configuable and fixed to 8 spaces)
         *
         *      Using this strategy we generate something like:
         *
         *      #       File            makefile
         *      #       Authors         Markus Toepfer
         *      #       Date            2017-12-01
         *      #
         *      #       Project         [PROJECT]
         *
         *      Project is set correct everywhere in the makefile,
         *      based on the configuration. File is overriden with
         *      correct filename.
         */

        snprintf(description, d_size, "%s%s#       Authors",
                config.project.name, config.format.line_end);

        snprintf(usage, d_size, "makefile%s#       Authors",
                config.format.line_end);

        if (!testrun_string_replace_first(
                &step3, &size3,
                step2,  size2,
                description, strlen(description),
                usage, strlen(usage)))
                goto error;

        line  = testrun_string_free(line);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        return step3;
error:
        line  = testrun_string_free(line);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        step3 = testrun_string_free(step3);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_makefile_main_content(testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.copyright       = copyright;
        config.author          = author;

        char *step1 = NULL;
        char *step2 = NULL;
        char *step3 = NULL;
        char *wrong = "#!/usr/bin/env bash";
        char *line  = testrun_text_block_splitline(8, 81, false);
        line[0]     = '#';

        size_t size1 = 0;
        size_t size2 = 0;
        size_t size3 = 0;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

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
                        goto error;
        }

        char path_src[PATH_MAX];
        char script_install[PATH_MAX];
        char script_uninstall[PATH_MAX];
        char doxygen_config[PATH_MAX];

        bzero(path_src, PATH_MAX);
        bzero(script_install, PATH_MAX);
        bzero(script_uninstall, PATH_MAX);
        bzero(doxygen_config, PATH_MAX);

        if (!testrun_path_project_to_source(
                path_src, PATH_MAX, &config))
                goto error;

        if (!testrun_path_script_service_install(
                script_install, PATH_MAX, &config))
                goto error;

        if (!testrun_path_script_service_uninstall(
                script_uninstall, PATH_MAX, &config))
                goto error;

        if (!testrun_path_doxygen_config(
                doxygen_config, PATH_MAX, &config))
                goto error;


        snprintf(description, d_size,
        "Generic makefile for testrun based projects."                          TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"Target of this makefile is an independent library"   TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"or executable to be installed at either PREFIX/lib " TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"or PREFIX/bin."
        );

        snprintf(usage, d_size, "SHOULD be used included by parent makefile");
        snprintf(dependencies, d_size, "testrun (makefile & service scripts), doxygen (if used)");

        snprintf(content, c_size,
        "# Switch on colors"                                                    TESTRUN_TAG_END
        "GCC_COLORS ?= 'gcc colors available, use them!'"                       TESTRUN_TAG_END
        "export GCC_COLORS"                                                     TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# ----- PARAMETER DEFINITION --------------------------------------------------" TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# CFLAGS"                                                              TESTRUN_TAG_END
        "# -g            enable Debugging symbols"                              TESTRUN_TAG_END
        "# -Ox           code optimization"                                     TESTRUN_TAG_END
        "# -Wall         enable Warnings"                                       TESTRUN_TAG_END
        "# -Wextra       additional Warnings"                                   TESTRUN_TAG_END
        TESTRUN_TAG_END
        "CFLAGS\t\t= -c -Wall -Wextra -fPIC"                                    TESTRUN_TAG_END
        "CFLAGS\t\t+= $(EXTRAHEADER)"                                           TESTRUN_TAG_END
        "CFLAGS\t\t+= $(MODCFLAGS)"                                             TESTRUN_TAG_END
        TESTRUN_TAG_END
        "PROJECTPATH\t:= $(abspath $(dir $(PROJECTMK)))"                        TESTRUN_TAG_END
        "DIRNAME\t\t:= $(notdir $(patsubst %%/,%%,$(dir $(PROJECTMK))))"        TESTRUN_TAG_END
        TESTRUN_TAG_END
        "LIBNAME\t\t:= lib$(DIRNAME)"                                           TESTRUN_TAG_END
        "LIBNAMEPC\t:= $(LIBNAME).pc"                                           TESTRUN_TAG_END
        TESTRUN_TAG_END
        "INSTALL\t\t:= install"                                                 TESTRUN_TAG_END
        TESTRUN_TAG_END
        "INCDIR\t\t:= $(PREFIX)/include/$(DIRNAME)"                             TESTRUN_TAG_END
        "LIBDIR\t\t:= $(PREFIX)/lib"                                            TESTRUN_TAG_END
        "EXECDIR\t\t:= $(PREFIX)/bin"                                           TESTRUN_TAG_END
        "PRODIR\t\t:= $(LIBDIR)/$(DIRNAME)"                                     TESTRUN_TAG_END
        TESTRUN_TAG_END
        "EXECUTABLE\t= bin/$(DIRNAME)"                                          TESTRUN_TAG_END
        TESTRUN_TAG_END
        "CFLAGS\t\t+= -Iinclude"                                                TESTRUN_TAG_END
        TESTRUN_TAG_END
        "MODMAKE\t\t:= $(patsubst %%,%s/%%/mod.mk,$(MODULES))"                  TESTRUN_TAG_END
        TESTRUN_TAG_END
        "STATIC\t\t= build/lib$(DIRNAME).a"                                     TESTRUN_TAG_END
        "SHARED\t\t= $(patsubst %%.a,%%.so,$(STATIC))"                          TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# Source and object files to compile"                                  TESTRUN_TAG_END
        "SOURCES\t\t= $(wildcard %s/**/*.c %s/*.c)"                             TESTRUN_TAG_END
        "OBJECTS\t\t= $(patsubst %%.c,%%.o,$(SOURCES))"                         TESTRUN_TAG_END
        TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# ----- DEFAULT MAKE RULES ----------------------------------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "%%.o : %%.c"                                                           TESTRUN_TAG_END
        "\t@echo \" (CC)    $@\""                                               TESTRUN_TAG_END
        "\t@$(CC) $(CFLAGS) -g -o $@ -c $< $(LIBS)"                             TESTRUN_TAG_END
        TESTRUN_TAG_END
        "all:\t\t%s"                                                            TESTRUN_TAG_END
        "install:\t%s"                                                          TESTRUN_TAG_END
        "uninstall:\t%s"                                                        TESTRUN_TAG_END
        TESTRUN_TAG_END
        "all_lib:\tstart lib tests pkgconfig done"                              TESTRUN_TAG_END
        "all_exec:\tstart lib tests $(EXECUTABLE) done"                         TESTRUN_TAG_END
        "all_service:\tall_exec"                                                TESTRUN_TAG_END
        TESTRUN_TAG_END
        "lib:\t\tbuild sources"                                                 TESTRUN_TAG_END
        "sources:\tbuild $(STATIC) $(SHARED)"                                   TESTRUN_TAG_END
        TESTRUN_TAG_END
        "$(STATIC):  $(OBJECTS)"                                                TESTRUN_TAG_END
        "\t@echo \" (AR)    $@ $(OBJECTS)\""                                    TESTRUN_TAG_END
        "\t@ar rcs $@ $(OBJECTS)"TESTRUN_TAG_END
        "\t@ranlib $@"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "$(SHARED): $(STATIC) $(OBJECTS)"                                       TESTRUN_TAG_END
        "\t@echo \" (CC)    $@ $(OBJECTS)\""                                    TESTRUN_TAG_END
        "\t@$(CC) -shared -o $@ $(OBJECTS) $(LIBS)"                             TESTRUN_TAG_END
        TESTRUN_TAG_END
        "$(EXECUTABLE): $(OBJECTS)"                                             TESTRUN_TAG_END
        "\t@echo \" (CC)    $@ $(OBJECTS)\""                                    TESTRUN_TAG_END
        "\t$(CC) -o $@ $(STATIC) $(LIBS)"                                       TESTRUN_TAG_END
        TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# ----- BUILD & CLEANUP -----------------------------------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "build:"                                                                TESTRUN_TAG_END
        "\t@mkdir -p bin"                                                       TESTRUN_TAG_END
        "\t@mkdir -p build"                                                     TESTRUN_TAG_END
        "\t@mkdir -p build/test"                                                TESTRUN_TAG_END
        "\t@mkdir -p build/test/omp"                                            TESTRUN_TAG_END
        "\t@mkdir -p build/test/omp/unit"                                       TESTRUN_TAG_END
        "\t@mkdir -p build/test/omp/acceptance"                                 TESTRUN_TAG_END
        "\t@mkdir -p build/test/unit"                                           TESTRUN_TAG_END
        "\t@mkdir -p build/test/acceptance"                                     TESTRUN_TAG_END
        "\t@mkdir -p build/test/log"                                            TESTRUN_TAG_END
        "\t@echo \" (MK)    directories for build\""                            TESTRUN_TAG_END
        TESTRUN_TAG_END
        ".PHONY: clean"                                                         TESTRUN_TAG_END
        "clean:"TESTRUN_TAG_END
        "\t@echo \" (CLEAN) $(LIBNAME)\""                                       TESTRUN_TAG_END
        "\t@rm -rf build bin doxygen/documentation $(OBJECTS) $(TESTS_OBJECTS) \\"TESTRUN_TAG_END
        "\t\t$(LIBNAMEPC) $(TESTS_TMP_FILES)"                                   TESTRUN_TAG_END
        TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# ----- DOCUMENATION -------------------------------------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "#NOTE requires doxygen"
        ".PHONY: documentation"                                                 TESTRUN_TAG_END
        "documentation:"                                                        TESTRUN_TAG_END
        "\tdoxygen %s"                                                          TESTRUN_TAG_END
        TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        ".PHONY: pkgconfig"                                                     TESTRUN_TAG_END
        "pkgconfig:"                                                            TESTRUN_TAG_END
        "\t@echo 'prefix='$(PREFIX)                     >  $(LIBNAMEPC)"        TESTRUN_TAG_END
        "\t@echo 'exec_prefix=$${prefix}'               >> $(LIBNAMEPC)"        TESTRUN_TAG_END
        "\t@echo 'libdir=$${prefix}/lib'                >> $(LIBNAMEPC)"        TESTRUN_TAG_END
        "\t@echo 'includedir=$${prefix}/include'        >> $(LIBNAMEPC)"        TESTRUN_TAG_END
        "\t@echo ''                                     >> $(LIBNAMEPC)"        TESTRUN_TAG_END
        "\t@echo 'Name: '$(LIBNAME)                     >> $(LIBNAMEPC)"        TESTRUN_TAG_END
        "\t@echo 'Description: '                        >> $(LIBNAMEPC)"        TESTRUN_TAG_END
        "\t@echo 'Version: '$(VERSION)                  >> $(LIBNAMEPC)"        TESTRUN_TAG_END
        "\t@echo 'URL: '$(PROJECT_URL)                  >> $(LIBNAMEPC)"        TESTRUN_TAG_END
        "\t@echo 'Libs: -L$${libdir} -l'$(DIRNAME)      >> $(LIBNAMEPC)"        TESTRUN_TAG_END
        "\t@echo 'Cflags: -I$${includedir}'             >> $(LIBNAMEPC)"        TESTRUN_TAG_END
        TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# ----- INSTALLATION -------------------------------------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# Installation as a library ------------------------------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "install_lib: $(SHARED) $(STATIC)"                                      TESTRUN_TAG_END
        "\t@echo \" (OK)    installed $(LIBNAME) to $(LIBDIR)\""                TESTRUN_TAG_END
        "\t@mkdir -p $(PRODIR)"                                                 TESTRUN_TAG_END
        "\t@mkdir -p $(LIBDIR)/pkgconfig"                                       TESTRUN_TAG_END
        "\t@mkdir -p $(INCDIR)"                                                 TESTRUN_TAG_END
        "\t@$(INSTALL) -m 0644 -t $(INCDIR) $(shell find include -name \"*.h\")"TESTRUN_TAG_END
        "\t@$(INSTALL) -m 0755 $(SHARED) $(PRODIR)"                             TESTRUN_TAG_END
        "\t@$(INSTALL) -m 0755 $(STATIC) $(PRODIR)"                             TESTRUN_TAG_END
        "\t@$(INSTALL) -m 0644 $(LIBNAMEPC) $(LIBDIR)/pkgconfig"                TESTRUN_TAG_END
        "\t@ldconfig $(PRODIR)"                                                 TESTRUN_TAG_END
        TESTRUN_TAG_END
        "uninstall_lib:"                                                        TESTRUN_TAG_END
        "\t@echo \" (OK)    uninstalled $(LIBNAME) from $(LIBDIR)\""            TESTRUN_TAG_END
        "\t@rm -rf $(INCDIR)"                                                   TESTRUN_TAG_END
        "\t@rm -rf $(PRODIR)"                                                   TESTRUN_TAG_END
        "\t@rm -rf $(PRODIR)/$(LIBNAME).a"                                      TESTRUN_TAG_END
        "\t@rm -rf $(PRODIR)/$(LIBNAME).so"                                     TESTRUN_TAG_END
        "\t@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)"                            TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# Installation as an executable --------------------------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "install_exec: $(SHARED) $(STATIC)"                                     TESTRUN_TAG_END
        "\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\""               TESTRUN_TAG_END
        "\t@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)"                       TESTRUN_TAG_END
        TESTRUN_TAG_END
        "uninstall_exec:"                                                       TESTRUN_TAG_END
        "\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\""           TESTRUN_TAG_END
        "\t@rm -rf $(EXECDIR)/$(DIRNAME)"                                       TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# Installation as a service (outsourced to script)--------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "install_service: $(EXECUTABLE)"                                        TESTRUN_TAG_END
        "\t%s"                                                                  TESTRUN_TAG_END
        TESTRUN_TAG_END
        "uninstall_service:"                                                    TESTRUN_TAG_END
        "\t%s"                                                                  TESTRUN_TAG_END
        TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# ----- INFORMATION PRINTING -----------------------------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# print out a variable of the make file (e.g. \"make print-PROJECTPATH\")"TESTRUN_TAG_END
        ".PHONY: print"                                                         TESTRUN_TAG_END
        "print-%%  : ; @echo $* = $($*)"                                        TESTRUN_TAG_END
        TESTRUN_TAG_END
        ".PHONY: start"                                                         TESTRUN_TAG_END
        "start:"                                                                TESTRUN_TAG_END
        "\t@echo \"\\n (HINT)    $(PROJECT) \\t\\t ==> running make\\n\""       TESTRUN_TAG_END
        TESTRUN_TAG_END
        ".PHONY: done"                                                          TESTRUN_TAG_END
        "done:"                                                                 TESTRUN_TAG_END
        "\t@echo"                                                               TESTRUN_TAG_END
        "\t@echo \" (DONE)  make $(PROJECT)\""                                  TESTRUN_TAG_END
        "\t@echo \" (HINT)  with unit testing      ==> 'make tested'\""         TESTRUN_TAG_END
        "\t@echo \" (HINT)  perform installation   ==> 'sudo make install\\n\"" TESTRUN_TAG_END
        "\t@echo \" (HINT)  generate documentation ==> 'make documentation\\n\""TESTRUN_TAG_END
        ,path_src, path_src, path_src, all_target, install, uninstall, doxygen_config,
        script_install, script_uninstall );

        step1 = testrun_text_block_script(&config,
                description, usage, dependencies, content);

        if (!step1)
                goto error;

        size1 = strlen(step1);

        if (!testrun_string_replace_first(
                &step2, &size2,
                step1,  size1,
                wrong, strlen(wrong),
                line, strlen(line)))
                goto error;

        /*
         *      NOTE The next part replaces File [PROJECT], which is set
         *      based on the configuraion with File makefile.
         *      This will only work, as long as the indent used in
         *      shell headers is fixed 8 spaces.
         *      (20171201: Currently not configuable and fixed to 8 spaces)
         *
         *      Using this strategy we generate something like:
         *
         *      #       File            makefile
         *      #       Authors         Markus Toepfer
         *      #       Date            2017-12-01
         *      #
         *      #       Project         [PROJECT]
         *
         *      Project is set correct everywhere in the makefile,
         *      based on the configuration. File is overriden with
         *      correct filename.
         */

        snprintf(description, d_size, "%s%s#       Authors",
                config.project.name, config.format.line_end);

        snprintf(usage, d_size, "testrun_makefile.main%s#       Authors",
                config.format.line_end);

        if (!testrun_string_replace_first(
                &step3, &size3,
                step2,  size2,
                description, strlen(description),
                usage, strlen(usage)))
                goto error;

        line  = testrun_string_free(line);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        return step3;
error:
        line  = testrun_string_free(line);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        step3 = testrun_string_free(step3);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_makefile_test_content(testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.copyright       = copyright;
        config.author          = author;

        char *step1 = NULL;
        char *step2 = NULL;
        char *step3 = NULL;
        char *wrong = "#!/usr/bin/env bash";
        char *line  = testrun_text_block_splitline(8, 81, false);
        line[0]     = '#';

        size_t size1 = 0;
        size_t size2 = 0;
        size_t size3 = 0;

        size_t c_size = 10000;
        size_t d_size = 6000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        char script_unit[PATH_MAX];
        char script_acceptance[PATH_MAX];
        char script_coverage[PATH_MAX];
        char script_loc[PATH_MAX];

        bzero(script_unit,      PATH_MAX);
        bzero(script_acceptance,PATH_MAX);
        bzero(script_coverage,  PATH_MAX);
        bzero(script_loc,       PATH_MAX);

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        // load script pathes out of config
        if (!testrun_path_script_unit_tests(
                script_unit, PATH_MAX, &config))
                goto error;

        if (!testrun_path_script_acceptance_tests(
                script_acceptance, PATH_MAX, &config))
                goto error;

        if (!testrun_path_script_coverage_tests(
                script_coverage, PATH_MAX, &config))
                goto error;

        if (!testrun_path_script_loc_tests(
                script_loc, PATH_MAX, &config))
                goto error;


        snprintf(description, d_size,
        "Makefile extension for the testrun enabled projects."                  TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"The following part contains all required functionality"TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"to use the testrun tools via a makefile. It may be " TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"seen as a makefile integrated testrunner framework." TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"in particular:"                                      TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"     \"make clean && make tested\""                  TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"may be used to build all sources as well as tests from " TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"scratch and perform an integrated testrun over all after" TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"compilation."                                        TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"Following folder structure is required"              TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"     tests MUST be located at tests/"                 TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"     build MUST be located at build/"                 TESTRUN_TAG_END
        "#"                                                                     TESTRUN_TAG_END
        TESTRUN_TAG_OFFSET"ALL TEST SCRIPTS MAY BE EXCHANGED WITH CUSTOM RUNNERS"TESTRUN_TAG_END
        "#");

        snprintf(usage, d_size, "SHOULD be used included by parent makefile");
        snprintf(dependencies, d_size, "testrun scripts, lib for OpenMP (if used for testing)");

        snprintf(content, c_size,
        TESTRUN_TAG_END
        "# (1) TESTRUN SOURCE DEFINITION --------------------------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "TESTS_OMP_SRC\t= $(wildcard tests/**/*%s.c tests/*%s.c)"               TESTRUN_TAG_END
        "TESTS_OMP_TARGET= $(patsubst %%.c,%%%s,$(TESTS_OMP_SRC))"              TESTRUN_TAG_END
        "TESTS_SOURCES   = $(wildcard tests/**/*%s.c tests/*%s.c)"              TESTRUN_TAG_END
        "TESTS_TARGET    = $(patsubst %%.c,%%%s,$(TESTS_SOURCES))"              TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# (2) TESTRUN MAKE RULES ---------------------------------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# ALL IN ONE CALL (compile source, test and run test)"                 TESTRUN_TAG_END
        "tested:\ttests_build all testrun done"                                 TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# ensure the build folder are available"                               TESTRUN_TAG_END
        "tests_build:"                                                          TESTRUN_TAG_END
        "\t@mkdir -p build/test"                                                TESTRUN_TAG_END
        "\t@mkdir -p build/test/omp"                                            TESTRUN_TAG_END
        "\t@mkdir -p build/test/omp/unit"                                       TESTRUN_TAG_END
        "\t@mkdir -p build/test/omp/acceptance"                                 TESTRUN_TAG_END
        "\t@mkdir -p build/test/unit"                                           TESTRUN_TAG_END
        "\t@mkdir -p build/test/acceptance"                                     TESTRUN_TAG_END
        "\t@mkdir -p build/test/log"                                            TESTRUN_TAG_END
        "\t@echo \" (MK)    directories for test under build\""                 TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# compile tests only"                                                  TESTRUN_TAG_END
        "tests:\ttests-resources $(TESTS_TARGET)"                               TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# compile omp_tests only"                                              TESTRUN_TAG_END
        "tests_omp:\ttests-resources $(TESTS_OMP_TARGET)"                       TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# copy test resources to build"                                        TESTRUN_TAG_END
        "tests-resources:"                                                      TESTRUN_TAG_END
        "\t@echo \" (CP)    tests/resources\""                                  TESTRUN_TAG_END
        "\t@cp -r tests/resources build/test"                                   TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# build all executable tests under build/tests"                        TESTRUN_TAG_END
        "$(TESTS_TARGET): $(TESTS_SOURCES)"                                     TESTRUN_TAG_END
        "\t@echo \" (CC)    $(@)\""                                             TESTRUN_TAG_END
        "\t@$(CC) $(MODCFLAGS) $(patsubst %%%s,%%.c,$(@)) \\"                   TESTRUN_TAG_END
        "\t\t-ldl $(STATIC) -Wl,-rpath=$(RPATH) \\"                             TESTRUN_TAG_END
        "\t\t-g -o $(subst tests/,build/test/,$(@)) $(LIBS)"                    TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# build all parallel executable tests under build/tests"               TESTRUN_TAG_END
        "$(TESTS_OMP_TARGET): $(TESTS_OMP_SRC)"                                 TESTRUN_TAG_END
        "\t@echo \" (CC)    $(@)\""                                             TESTRUN_TAG_END
        "\t@$(CC) $(MODCFLAGS) -fopenmp $(patsubst %%%s,%%.c,$(@)) \\"          TESTRUN_TAG_END
        "\t\t-ldl $(STATIC) -Wl,-rpath=$(RPATH) \\"                             TESTRUN_TAG_END
        "\t\t-g -o $(subst tests/,build/test/omp/,$(@)) $(LIBS)"                TESTRUN_TAG_END
        TESTRUN_TAG_END
       "# build a specific executable test (testname) under build/tests"        TESTRUN_TAG_END
        "# NOTE: May be usefull for module development in large projects"       TESTRUN_TAG_END
        "test:"                                                                 TESTRUN_TAG_END
        "\t@echo \" (CC)    $(testname)\""                                      TESTRUN_TAG_END
        "\t@$(CC) $(MODCFLAGS) $(patsubst build/test/%%%s, \\"                  TESTRUN_TAG_END
        "\t\ttests/%%.c,$(testname)) -ldl $(STATIC) -Wl,-rpath=$(RPATH) -g -o\\"TESTRUN_TAG_END
        "\t\t$(patsubst tests/%%.c,build/test/%%%s,$(testname)) $(LIBS)"        TESTRUN_TAG_END
        TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# (3) TESTRUN runners ------------------------------------------------------"TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# ACCEPTANCE TEST script invocation"                                   TESTRUN_TAG_END
        ".PHONY: testrun-acceptance"                                            TESTRUN_TAG_END
        "testrun-acceptance:"                                                   TESTRUN_TAG_END
        "\tsh %s"                                                               TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# UNIT TEST script invocation"                                         TESTRUN_TAG_END
        ".PHONY: testrun-unit"                                                  TESTRUN_TAG_END
        "testrun-unit:"                                                         TESTRUN_TAG_END
        "\tsh %s"                                                               TESTRUN_TAG_END
        TESTRUN_TAG_END
         "# COVERAGE TEST script invocation"                                    TESTRUN_TAG_END
        ".PHONY: testrun-coverage"                                              TESTRUN_TAG_END
        "testrun-coverage:"                                                     TESTRUN_TAG_END
        "\tsh %s $(PROJECTPATH)"                                                TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# LOC TEST script invocation"                                          TESTRUN_TAG_END
        ".PHONY: testrun-loc"                                                   TESTRUN_TAG_END
        "testrun-loc:"TESTRUN_TAG_END
        "\tsh %s $(PROJECTPATH)"                                                TESTRUN_TAG_END
        TESTRUN_TAG_END
        "# TESTRUN all scripts"                                                 TESTRUN_TAG_END
        ".PHONY: testrun"TESTRUN_TAG_END
        "testrun: $(TESTS_EXEC)"TESTRUN_TAG_END
        "\t@echo \" (HINT)  $(PROJECT) \\t\\t\\t==> running tests\\n\""         TESTRUN_TAG_END
        "\tsh %s"                                                               TESTRUN_TAG_END
        "\tsh %s"                                                               TESTRUN_TAG_END
        "\tsh %s $(PROJECTPATH)"                                                TESTRUN_TAG_END
        "\tsh %s $(PROJECTPATH)"                                                TESTRUN_TAG_END
        ,config.format.suffix.tests_source_omp,
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

        step1 = testrun_text_block_script(&config,
                description, usage, dependencies, content);

        if (!step1)
                goto error;

        size1 = strlen(step1);

        if (!testrun_string_replace_first(
                &step2, &size2,
                step1,  size1,
                wrong, strlen(wrong),
                line, strlen(line)))
                goto error;

        /*
         *      NOTE The next part replaces File [PROJECT], which is set
         *      based on the configuraion with File makefile.
         *      This will only work, as long as the indent used in
         *      shell headers is fixed 8 spaces.
         *      (20171201: Currently not configuable and fixed to 8 spaces)
         *
         *      Using this strategy we generate something like:
         *
         *      #       File            makefile
         *      #       Authors         Markus Toepfer
         *      #       Date            2017-12-01
         *      #
         *      #       Project         [PROJECT]
         *
         *      Project is set correct everywhere in the makefile,
         *      based on the configuration. File is overriden with
         *      correct filename.
         */

        snprintf(description, d_size, "%s%s#       Authors",
                config.project.name, config.format.line_end);

        snprintf(usage, d_size, "testrun_makefile.test%s#       Authors",
                config.format.line_end);

        if (!testrun_string_replace_first(
                &step3, &size3,
                step2,  size2,
                description, strlen(description),
                usage, strlen(usage)))
                goto error;

        line  = testrun_string_free(line);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        return step3;
error:
        line  = testrun_string_free(line);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        step3 = testrun_string_free(step3);
        return NULL;
}
