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
        @file           testrun_app_test.c
        @author         [AUTHOR]
        @date           2018-07-19

        @ingroup        testrun_lib

        @brief


        ------------------------------------------------------------------------
*/

#include "../tools/testrun.h"
#include "../../src/testrun_app.c"

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

int test_testrun_app_run(){

        char *root_path = "/tmp/test_folder/not_existing";

        char project_root[PATH_MAX];
        memset(project_root, 0, PATH_MAX);

        DIR *dp;
        FILE *file = NULL;

        size_t size = 0;

        char path[PATH_MAX];
        memset(path, 0, PATH_MAX);

        char *args[100] = { 0 };

        args[0] = "programName";
        args[1] = "--project";
        args[2] = "test";
        args[3] = "-d";
        args[4] = "/tmp/test_folder/not_existing";

        // check root not existing
        dp = opendir(root_path);
        testrun(!dp);

        testrun(-1 == testrun_app_run(0, NULL));
        testrun(-1 == testrun_app_run(2, NULL));
        testrun(-1 == testrun_app_run(1, args));

        testrun(0  == testrun_app_run(5, args));

        // check root existing
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        snprintf(project_root, PATH_MAX, "%s/%s", root_path, "test");

        // check project path existing
        dp = opendir(project_root);
        testrun(dp);
        (void) closedir (dp);

        file = fopen("/tmp/test_folder/not_existing/test/include/test.h", "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        file = fopen("/tmp/test_folder/not_existing/test/src/test.c", "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        file = fopen("/tmp/test_folder/not_existing/test/tests/unit/test_test.c", "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        // TBD how to check the input loop ? 

        testrun(system("rm -rf /tmp/test_folder") == 0);
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
        testrun_test(test_testrun_app_run);

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

