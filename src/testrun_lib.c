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

#include "../include/testrun_module.h"

/*----------------------------------------------------------------------------*/

char *testrun_lib_testrun_header(){

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



