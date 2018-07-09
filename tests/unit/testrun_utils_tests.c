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
        @file           testrun_utils_test.c
        @author         [AUTHOR]
        @date           2018-07-09

        @ingroup        testrun_lib

        @brief


        ------------------------------------------------------------------------
*/

#include "../tools/testrun.h"
#include "../../src/testrun_utils.c"

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST HELPER                                                     #HELPER
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

int test_testrun_utils_insert_at_each_line(){
        
        char *result = NULL;
        char *expect = NULL;
        char *text   = NULL;
        char *intro  = NULL;
        char *outro  = NULL;

        testrun(!testrun_utils_insert_at_each_line(NULL, NULL, NULL));

        // no line
        text   = "x";
        testrun(!testrun_utils_insert_at_each_line(NULL, NULL, NULL));

        // one line
        text   = "x\n";
        result = testrun_utils_insert_at_each_line(text, NULL, NULL);
        expect = "x\n";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line
        text   = "\nx";
        result = testrun_utils_insert_at_each_line(text, NULL, NULL);
        expect = "\nx";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (prefix)
        text   = "x\n";
        result = testrun_utils_insert_at_each_line(text, "pre", NULL);
        expect = "prex\n";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (prefix textlen0)
        text   = "\nx";
        result = testrun_utils_insert_at_each_line(text, "pre", NULL);
        expect = "\nx";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (suffix)
        text   = "x\n";
        result = testrun_utils_insert_at_each_line(text, NULL, "suffix");
        expect = "xsuffix\n";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (suffix (textlen0))
        text   = "\nx";
        result = testrun_utils_insert_at_each_line(text, NULL, "suffix");
        expect = "\nx";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (preffix, suffix)
        text   = "x\n";
        result = testrun_utils_insert_at_each_line(text, "preffix", "suffix");
        expect = "preffixxsuffix\n";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (preffix,  suffix)
        text   = "\nx";
        result = testrun_utils_insert_at_each_line(text, "preffix", "suffix");
        expect = "\nx";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // multiple line (preffix)
        text   = "x\ny\nz\na";
        result = testrun_utils_insert_at_each_line(text, "_", NULL);
        expect = "_x\n_y\n_z\na";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

         // multiple line (suffix)
        text   = "x\ny\nz\na";
        result = testrun_utils_insert_at_each_line(text, NULL, "_");
        expect = "x_\ny_\nz_\na";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
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
        testrun_test(test_testrun_utils_insert_at_each_line);

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

