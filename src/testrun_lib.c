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
        "Run each test.test of a folder and log Ok or NOK\n"
        TESTRUN_TAG_OFFSET"for each executed testfile of the folder.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"EXAMPLE OUTPUT\n"
        "#\n"
        TESTRUN_TAG_OFFSET"[OK]  1/5 filename1.test\n"
        TESTRUN_TAG_OFFSET"[NOK] 2/5 filename2.test\n"
        "#\n"
        TESTRUN_TAG_OFFSET"MODES\n"
        "#\n"
        TESTRUN_TAG_OFFSET"(1) RUN ALL TESTS (log but ignore errors)\n"
        TESTRUN_TAG_OFFSET"    use script with 2 parameters\n"
        TESTRUN_TAG_OFFSET"    e.g. ./%s logfile /path\n"
        "#\n"
        TESTRUN_TAG_OFFSET"    This mode will not return a test failure and\n"
        TESTRUN_TAG_OFFSET"    may be used to run all tests and return success\n"
        TESTRUN_TAG_OFFSET"    if all tests was run. (test results are logged)\n"
        "#\n"
        TESTRUN_TAG_OFFSET"(2) FAIL ON ERROR (Fail on first error)\n"
        TESTRUN_TAG_OFFSET"    use script with 3 parameters\n"
        TESTRUN_TAG_OFFSET"    e.g. ./%s logfile /path 1\n"
        "#\n"
        TESTRUN_TAG_OFFSET"    This mode returns -1 on the first test failure.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"PARAMETER\n"
        "#\n"
        TESTRUN_TAG_OFFSET"(1) path to logfile destination\n"
        TESTRUN_TAG_OFFSET"(2) path to folder with test cases\n"
        "#"
        ,config.project.name, config.project.name);

        snprintf(usage, d_size,
                "./%s /path/to/logfile /path/to/test/dir",
                config.project.name);

        snprintf(dependencies, d_size, "bash, tail, ls, grep, wc");


        snprintf(content, c_size,
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
        "        FAIL_ON_ERROR=1  \n"
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
        "FILES=`ls  $FOLDER/ | grep \"\\%s\" | wc -l`\n"
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
        "for i in $FOLDER/*%s\n"
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
        ,config.format.extensions.testexec, config.format.extensions.testexec);

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
        "Run all test executables build/test/acceptance/*.test\n"
        TESTRUN_TAG_OFFSET"Run the whole folder, until an error occurs.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"MODE         FAIL ON ERROR (Fail on first test error)\n"
        "#\n"
        TESTRUN_TAG_OFFSET"LOGFILE      build/test/log/acceptancetest.<time>.log\n"
        "#"
        );

        snprintf(usage, d_size,
                "./%s /path/to/project",
                config.project.path.tests.tools.acceptance_script);

        snprintf(dependencies, d_size, "bash, touch, chmod, ls, wc, date");


        snprintf(content, c_size,
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE ACCEPTANCE TESTING\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"build/test/log/acceptance_\".$start_time.\"log\"\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "\n"
        "# SET THE FOLDER\n"
        "FOLDER=\"build/test/acceptance\"\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               REPORT ACCEPTANCE TESTING\"                >> $LOGFILE\n"
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
        "Run all test executables build/test/unit/*.test\n"
        TESTRUN_TAG_OFFSET"Run the whole folder, until an error occurs.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"MODE         FAIL ON ERROR (Fail on first test error)\n"
        "#\n"
        TESTRUN_TAG_OFFSET"LOGFILE      build/test/log/unittest.<time>.log\n"
        "#"
        );

        snprintf(usage, d_size,
                "./%s /path/to/project",
                config.project.path.tests.tools.unit_script);

        snprintf(dependencies, d_size, "bash, touch, chmod, ls, wc, date");


        snprintf(content, c_size,
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE UNIT TESTING\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"build/test/log/unit_\".$start_time.\"log\"\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "\n"
        "# SET THE FOLDER\n"
        "FOLDER=\"build/test/unit\"\n"
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
        "exit $RESULT\n"
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
        "Count functions of folder src against their counterparts\n"
        TESTRUN_TAG_OFFSET"in the unit test folder.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"CONVENTION\n"
        "#\n"
        TESTRUN_TAG_OFFSET"Each function in any file of the source folder located\n"
        TESTRUN_TAG_OFFSET"\"%s\"\n"
        TESTRUN_TAG_OFFSET"will have a corresponding test function using the same\n"
        TESTRUN_TAG_OFFSET"name in any other file of the unit test folder located\n"
        TESTRUN_TAG_OFFSET"\"%s\",\n"
        TESTRUN_TAG_OFFSET"with a function name prefix of\n"
        TESTRUN_TAG_OFFSET"\"%s\".\n"
        "#\n"
        TESTRUN_TAG_OFFSET"EXAMPLE      function | %sfunction\n"
        "#\n"
        TESTRUN_TAG_OFFSET"NOTE         This simple coverage test just covers the\n"
        TESTRUN_TAG_OFFSET"             observance of the given coding convention.\n"
        "#"
        ,path_src, path_unit, config.format.prefix.unit_test,config.format.prefix.unit_test);

        snprintf(usage, d_size,
                "./%s /path/to/project",
                config.project.path.tests.tools.coverage_script);

        snprintf(dependencies, d_size, "bash, ctags, awk, sed, grep");


        snprintf(content, c_size,
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S\")\n"
        "PREFIX=\"%s\"\n"
        "\n"
        "LIBDIR=$1\n"
        "SRCDIR=$1/%s\n"
        "TESTDIR=$1/%s\n"
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"$LIBDIR/build/test/log/coverage_\".$start_time.\"log\"\n"
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
        "Count the lines of src and unit | acceptance tests.\n"
        TESTRUN_TAG_OFFSET"This file uses no error checking.");

        snprintf(usage, d_size,
                "./%s /path/to/project",
                config.project.path.tests.tools.loc_script);

        snprintf(dependencies, d_size, "bash, find, xargs, wc");

        snprintf(content, c_size,
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE LOC COUNTER\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"(LOC) src\"\n"
        "find $1/%s -name '*.c' | xargs wc -l\n"
        "echo \"(LOC) tests/unit\"\n"
        "find $1/%s -name '*.c' | xargs wc -l\n"
        "echo \"(LOC) tests/acceptance\"\n"
        "find $1/%s -name '*.c' | xargs wc -l\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"\"\n"
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
        "This makefile defines project specific parameter.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"These parameter are:\n"
        TESTRUN_TAG_OFFSET"(1) used compiler and special flags\n"
        TESTRUN_TAG_OFFSET"(2) name and version\n"
        TESTRUN_TAG_OFFSET"(3) installation prefix\n"
        TESTRUN_TAG_OFFSET"(4) used libraries\n"
        TESTRUN_TAG_OFFSET"(5) general makefiles used"
        );

        snprintf(usage, d_size, "make");
        snprintf(dependencies, d_size, "make & compiler");

        snprintf(content, c_size,
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
        "#MODCFLAGS      += -fopenmp\n"
        "\n"
        "#GCC only flags (example)\n"
        "#MODCFLAGS      += -rdynamic\n"
        "\n"
        "#TMP FILE DEFINITION\n"
        "TESTS_TMP_FILES = $(wildcard /tmp/test_*)\n"
        "\n"
        "# INCLUDE BASE MAKEFILE\n"
        "include testrun_makefile.main\n"
        "include testrun_makefile.test\n"
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
        "Generic makefile for testrun based projects.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"Target of this makefile is an independent library\n"
        TESTRUN_TAG_OFFSET"or executable to be installed at either PREFIX/lib\n"
        TESTRUN_TAG_OFFSET"or PREFIX/bin."
        );

        snprintf(usage, d_size, "SHOULD be used included by parent makefile");
        snprintf(dependencies, d_size, "testrun (makefile & service scripts), doxygen (if used)");

        snprintf(content, c_size,
        "# Switch on colors\n"
        "GCC_COLORS ?= 'gcc colors available, use them!'\n"
        "export GCC_COLORS\n"
        "\n"
        "# ----- PARAMETER DEFINITION --------------------------------------------------\n"
        "\n"
        "# CFLAGS\n"
        "# -g            enable Debugging symbols\n"
        "# -Ox           code optimization\n"
        "# -Wall         enable Warnings\n"
        "# -Wextra       additional Warnings\n"
        "\n"
        "CFLAGS\t\t= -c -Wall -Wextra -fPIC\n"
        "CFLAGS\t\t+= $(EXTRAHEADER)\n"
        "CFLAGS\t\t+= $(MODCFLAGS)\n"
        "\n"
        "PROJECTPATH\t:= $(abspath $(dir $(PROJECTMK)))\n"
        "DIRNAME\t\t:= $(notdir $(patsubst %%/,%%,$(dir $(PROJECTMK))))\n"
        "\n"
        "LIBNAME\t\t:= lib$(DIRNAME)\n"
        "LIBNAMEPC\t:= $(LIBNAME).pc\n"
        "\n"
        "INSTALL\t\t:= install\n"
        "\n"
        "INCDIR\t\t:= $(PREFIX)/include/$(DIRNAME)\n"
        "LIBDIR\t\t:= $(PREFIX)/lib\n"
        "EXECDIR\t\t:= $(PREFIX)/bin\n"
        "PRODIR\t\t:= $(LIBDIR)/$(DIRNAME)\n"
        "\n"
        "EXECUTABLE\t= bin/$(DIRNAME)\n"
        "\n"
        "CFLAGS\t\t+= -Iinclude\n"
        "\n"
        "MODMAKE\t\t:= $(patsubst %%,%s/%%/mod.mk,$(MODULES))\n"
        "\n"
        "STATIC\t\t= build/lib$(DIRNAME).a\n"
        "SHARED\t\t= $(patsubst %%.a,%%.so,$(STATIC))\n"
        "\n"
        "# Source and object files to compile\n"
        "SOURCES\t\t= $(wildcard %s/**/*.c %s/*.c)\n"
        "OBJECTS\t\t= $(patsubst %%.c,%%.o,$(SOURCES))\n"
        "\n"
        "\n"
        "# ----- DEFAULT MAKE RULES ----------------------------------------------------\n"
        "\n"
        "%%.o : %%.c\n"
        "\t@echo \" (CC)    $@\"\n"
        "\t@$(CC) $(CFLAGS) -g -o $@ -c $< $(LIBS)\n"
        "\n"
        "all:\t\t%s\n"
        "install:\t%s\n"
        "uninstall:\t%s\n"
        "\n"
        "all_lib:\tstart lib tests pkgconfig done\n"
        "all_exec:\tstart lib tests $(EXECUTABLE) done\n"
        "all_service:\tall_exec\n"
        "\n"
        "lib:\t\tbuild sources\n"
        "sources:\tbuild $(STATIC) $(SHARED)\n"
        "\n"
        "$(STATIC):  $(OBJECTS)\n"
        "\t@echo \" (AR)    $@ $(OBJECTS)\"\n"
        "\t@ar rcs $@ $(OBJECTS)""\n"
        "\t@ranlib $@""\n"
        "\n"
        "$(SHARED): $(STATIC) $(OBJECTS)\n"
        "\t@echo \" (CC)    $@ $(OBJECTS)\"\n"
        "\t@$(CC) -shared -o $@ $(OBJECTS) $(LIBS)\n"
        "\n"
        "$(EXECUTABLE): $(OBJECTS)\n"
        "\t@echo \" (CC)    $@ $(OBJECTS)\"\n"
        "\t$(CC) -o $@ $(STATIC) $(LIBS)\n"
        "\n"
        "\n"
        "# ----- BUILD & CLEANUP -----------------------------------------------------\n"
        "\n"
        "build:\n"
        "\t@mkdir -p bin\n"
        "\t@mkdir -p build\n"
        "\t@mkdir -p build/test\n"
        "\t@mkdir -p build/test/omp\n"
        "\t@mkdir -p build/test/omp/unit\n"
        "\t@mkdir -p build/test/omp/acceptance\n"
        "\t@mkdir -p build/test/unit\n"
        "\t@mkdir -p build/test/acceptance\n"
        "\t@mkdir -p build/test/log\n"
        "\t@echo \" (MK)    directories for build\"\n"
        "\n"
        ".PHONY: clean\n"
        "clean:\n"
        "\t@echo \" (CLEAN) $(LIBNAME)\"\n"
        "\t@rm -rf build bin doxygen/documentation $(OBJECTS) $(TESTS_OBJECTS) \\\n"
        "\t\t$(LIBNAMEPC) $(TESTS_TMP_FILES)\n"
        "\n"
        "\n"
        "# ----- DOCUMENATION -------------------------------------------------------\n"
        "\n"
        "#NOTE requires doxygen"
        ".PHONY: documentation\n"
        "documentation:\n"
        "\tdoxygen %s\n"
        "\n"
        "\n"
        "# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------\n"
        "\n"
        ".PHONY: pkgconfig\n"
        "pkgconfig:\n"
        "\t@echo 'prefix='$(PREFIX)                     >  $(LIBNAMEPC)\n"
        "\t@echo 'exec_prefix=$${prefix}'               >> $(LIBNAMEPC)\n"
        "\t@echo 'libdir=$${prefix}/lib'                >> $(LIBNAMEPC)\n"
        "\t@echo 'includedir=$${prefix}/include'        >> $(LIBNAMEPC)\n"
        "\t@echo ''                                     >> $(LIBNAMEPC)\n"
        "\t@echo 'Name: '$(LIBNAME)                     >> $(LIBNAMEPC)\n"
        "\t@echo 'Description: '                        >> $(LIBNAMEPC)\n"
        "\t@echo 'Version: '$(VERSION)                  >> $(LIBNAMEPC)\n"
        "\t@echo 'URL: '$(PROJECT_URL)                  >> $(LIBNAMEPC)\n"
        "\t@echo 'Libs: -L$${libdir} -l'$(DIRNAME)      >> $(LIBNAMEPC)\n"
        "\t@echo 'Cflags: -I$${includedir}'             >> $(LIBNAMEPC)\n"
        "\n"
        "\n"
        "# ----- INSTALLATION -------------------------------------------------------\n"
        "\n"
        "# Installation as a library ------------------------------------------------\n"
        "\n"
        "install_lib: $(SHARED) $(STATIC)\n"
        "\t@echo \" (OK)    installed $(LIBNAME) to $(LIBDIR)\"\n"
        "\t@mkdir -p $(PRODIR)\n"
        "\t@mkdir -p $(LIBDIR)/pkgconfig\n"
        "\t@mkdir -p $(INCDIR)\n"
        "\t@$(INSTALL) -m 0644 -t $(INCDIR) $(shell find include -name \"*.h\")\n"
        "\t@$(INSTALL) -m 0755 $(SHARED) $(PRODIR)\n"
        "\t@$(INSTALL) -m 0755 $(STATIC) $(PRODIR)\n"
        "\t@$(INSTALL) -m 0644 $(LIBNAMEPC) $(LIBDIR)/pkgconfig\n"
        "\t@ldconfig $(PRODIR)\n"
        "\n"
        "uninstall_lib:\n"
        "\t@echo \" (OK)    uninstalled $(LIBNAME) from $(LIBDIR)\"\n"
        "\t@rm -rf $(INCDIR)\n"
        "\t@rm -rf $(PRODIR)\n"
        "\t@rm -rf $(PRODIR)/$(LIBNAME).a\n"
        "\t@rm -rf $(PRODIR)/$(LIBNAME).so\n"
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
        "\t%s\n"
        "\n"
        "uninstall_service:\n"
        "\t%s\n"
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
        "\t@echo \"\\n (HINT)    $(PROJECT) \\t\\t ==> running make\\n\"\n"
        "\n"
        ".PHONY: done\n"
        "done:\n"
        "\t@echo\n"
        "\t@echo \" (DONE)  make $(PROJECT)\"\n"
        "\t@echo \" (HINT)  with unit testing      ==> 'make tested'\"\n"
        "\t@echo \" (HINT)  perform installation   ==> 'sudo make install\\n\"\n"
        "\t@echo \" (HINT)  generate documentation ==> 'make documentation\\n\"\n"
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
        "Makefile extension for the testrun enabled projects.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"The following part contains all required functionality\n"
        TESTRUN_TAG_OFFSET"to use the testrun tools via a makefile. It may be\n"
        TESTRUN_TAG_OFFSET"seen as a makefile integrated testrunner framework.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"in particular:\n"
        "#\n"
        TESTRUN_TAG_OFFSET"     \"make clean && make tested\"\n"
        "#\n"
        TESTRUN_TAG_OFFSET"may be used to build all sources as well as tests from\n"
        TESTRUN_TAG_OFFSET"scratch and perform an integrated testrun over all after\n"
        TESTRUN_TAG_OFFSET"compilation.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"Following folder structure is required\n"
        "#\n"
        TESTRUN_TAG_OFFSET"     tests MUST be located at tests/\n"
        TESTRUN_TAG_OFFSET"     build MUST be located at build/\n"
        "#\n"
        TESTRUN_TAG_OFFSET"ALL TEST SCRIPTS MAY BE EXCHANGED WITH CUSTOM RUNNERS\n"
        "#");

        snprintf(usage, d_size, "SHOULD be used included by parent makefile");
        snprintf(dependencies, d_size, "testrun scripts, lib for OpenMP (if used for testing)");

        snprintf(content, c_size,
        "\n"
        "# (1) TESTRUN SOURCE DEFINITION --------------------------------------------\n"
        "\n"
        "TESTS_OMP_SRC\t= $(wildcard tests/**/*%s.c tests/*%s.c)\n"
        "TESTS_OMP_TARGET= $(patsubst %%.c,%%%s,$(TESTS_OMP_SRC))\n"
        "TESTS_SOURCES   = $(wildcard tests/**/*%s.c tests/*%s.c)\n"
        "TESTS_TARGET    = $(patsubst %%.c,%%%s,$(TESTS_SOURCES))\n"
        "\n"
        "# (2) TESTRUN MAKE RULES ---------------------------------------------------\n"
        "\n"
        "# ALL IN ONE CALL (compile source, test and run test)\n"
        "tested:\ttests_build all testrun done\n"
        "\n"
        "# ensure the build folder are available\n"
        "tests_build:\n"
        "\t@mkdir -p build/test\n"
        "\t@mkdir -p build/test/omp\n"
        "\t@mkdir -p build/test/omp/unit\n"
        "\t@mkdir -p build/test/omp/acceptance\n"
        "\t@mkdir -p build/test/unit\n"
        "\t@mkdir -p build/test/acceptance\n"
        "\t@mkdir -p build/test/log\n"
        "\t@echo \" (MK)    directories for test under build\"\n"
        "\n"
        "# compile tests only\n"
        "tests:\ttests-resources $(TESTS_TARGET)\n"
        "\n"
        "# compile omp_tests only\n"
        "tests_omp:\ttests-resources $(TESTS_OMP_TARGET)\n"
        "\n"
        "# copy test resources to build\n"
        "tests-resources:\n"
        "\t@echo \" (CP)    tests/resources\"\n"
        "\t@cp -r tests/resources build/test\n"
        "\n"
        "# build all executable tests under build/tests\n"
        "$(TESTS_TARGET): $(TESTS_SOURCES)\n"
        "\t@echo \" (CC)    $(@)\"\n"
        "\t@$(CC) $(MODCFLAGS) $(patsubst %%%s,%%.c,$(@)) \\\n"
        "\t\t-ldl $(STATIC) -Wl,-rpath=$(RPATH) \\\n"
        "\t\t-g -o $(subst tests/,build/test/,$(@)) $(LIBS)\n"
        "\n"
        "# build all parallel executable tests under build/tests\n"
        "$(TESTS_OMP_TARGET): $(TESTS_OMP_SRC)\n"
        "\t@echo \" (CC)    $(@)\"\n"
        "\t@$(CC) $(MODCFLAGS) -fopenmp $(patsubst %%%s,%%.c,$(@)) \\\n"
        "\t\t-ldl $(STATIC) -Wl,-rpath=$(RPATH) \\\n"
        "\t\t-g -o $(subst tests/,build/test/omp/,$(@)) $(LIBS)\n"
        "\n"
       "# build a specific executable test (testname) under build/tests\n"
        "# NOTE: May be usefull for module development in large projects\n"
        "test:\n"
        "\t@echo \" (CC)    $(testname)\"\n"
        "\t@$(CC) $(MODCFLAGS) $(patsubst build/test/%%%s, \\\n"
        "\t\ttests/%%.c,$(testname)) -ldl $(STATIC) -Wl,-rpath=$(RPATH) -g -o\\\n"
        "\t\t$(patsubst tests/%%.c,build/test/%%%s,$(testname)) $(LIBS)\n"
        "\n"
        "\n"
        "# (3) TESTRUN runners ------------------------------------------------------\n"
        "\n"
        "# ACCEPTANCE TEST script invocation\n"
        ".PHONY: testrun-acceptance\n"
        "testrun-acceptance:\n"
        "\tsh %s\n"
        "\n"
        "# UNIT TEST script invocation\n"
        ".PHONY: testrun-unit\n"
        "testrun-unit:\n"
        "\tsh %s\n"
        "\n"
         "# COVERAGE TEST script invocation\n"
        ".PHONY: testrun-coverage\n"
        "testrun-coverage:\n"
        "\tsh %s $(PROJECTPATH)\n"
        "\n"
        "# LOC TEST script invocation\n"
        ".PHONY: testrun-loc\n"
        "testrun-loc:\n"
        "\tsh %s $(PROJECTPATH)\n"
        "\n"
        "# TESTRUN all scripts\n"
        ".PHONY: testrun\n"
        "testrun: $(TESTS_EXEC)\n"
        "\t@echo \" (HINT)  $(PROJECT) \\t\\t\\t==> running tests\\n\"\n"
        "\tsh %s\n"
        "\tsh %s\n"
        "\tsh %s $(PROJECTPATH)\n"
        "\tsh %s $(PROJECTPATH)\n"
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
