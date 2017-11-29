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

int test_testrun_lib_testrun_header(){

        size_t size = 10000;
        char expect[size];

        char *result = testrun_lib_testrun_header();
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

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                   #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int all_tests() {

       testrun_init();

       testrun_test(test_testrun_lib_testrun_header);

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
