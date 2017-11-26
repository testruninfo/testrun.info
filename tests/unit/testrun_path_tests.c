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

        @file           testrun_path_tests.c
        @author         Markus Toepfer
        @date           2017-11-25

        @ingroup

        @brief


        ------------------------------------------------------------------------
*/


#include "../../include/testrun.h"
#include "../../src/testrun_path.c"
#include <sys/stat.h>

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */

int test_testrun_path_is_project_top_dir() {

        char *bases[] = {
                "./build/test/resources/test_top/ok",
                "./build/test/resources/test_top",
                "./build/test/resources/test_top/no_include",
                "./build/test/resources/test_top/no_tests",
                "./build/test/resources/test_top/files_not_dir"
        };

        char *folder = "level1";
        char path[PATH_MAX];

        for (int i = 0; i < 5; i++){

                if (i == 0){
                        testrun(testrun_path_is_project_top_dir(bases[i]));
                } else {
                        testrun(!testrun_path_is_project_top_dir(bases[i]));
                }

                /* No access to folder */
                testrun(0 == chmod(bases[i], 000));
                testrun(!testrun_path_is_project_top_dir(bases[i]));
                testrun(0 == chmod(bases[i],
                        S_IRUSR | S_IWUSR | S_IXUSR |
                        S_IRGRP | S_IXGRP |
                        S_IROTH | S_IXOTH));

                /* recheck with access reset */
                if (i == 0) {
                        testrun(testrun_path_is_project_top_dir(bases[i]));
                } else {
                        testrun(!testrun_path_is_project_top_dir(bases[i]));
                }

                /* check with no access to a path within the folder */
                testrun(sprintf(path, "%s/%s", bases[i], folder) > 0);
                testrun(0 == chmod(path, 000));

                if (i == 0) {
                        testrun(testrun_path_is_project_top_dir(bases[i]));
                } else {
                        testrun(!testrun_path_is_project_top_dir(bases[i]));
                }

                testrun(0 == chmod(path,
                        S_IRUSR | S_IWUSR | S_IXUSR |
                        S_IRGRP | S_IXGRP |
                        S_IROTH | S_IXOTH));


        }

        testrun(testrun_path_is_project_top_dir("."));

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_search_project_path() {

        char *bases[] = {
                "./build/test/resources/test_top/ok",
                "./build/test/resources/test_top",
                "./build/test/resources/test_top/no_include",
                "./build/test/resources/test_top/no_tests",
                "./build/test/resources/test_top/files_not_dir"
        };

        char path[PATH_MAX];

        char *level1    = "level1";
        char *level2    = "level2";
        char *level3    = "level3";

        char *result    = NULL;
        char *real      = NULL;

        for (int i = 0; i < 5; i++){


                testrun(sprintf(path, "%s", bases[i]) > 0);
                if (i == 0){
                        result = testrun_path_search_project_path(path);
                        testrun(result);
                        real = realpath(bases[i], real);
                        testrun(strncmp(result,real, strlen(result)) == 0);
                        free(real);
                        real = NULL;
                        free(result);
                        result = NULL;

                } else {

                        /* @NOTE: this test will be run within the valid project
                         * **testrun_create_module**, so it is actually testing
                         * finding a top project from different folders */

                        real   = realpath(".", real);
                        result = testrun_path_search_project_path(path);
                        testrun(strncmp(result,real, strlen(result)) == 0);
                        free(real);
                        real = NULL;
                        free(result);
                        result = NULL;
                }

                /* TEST AGAIN DEEPER NESTED */

                testrun(sprintf(path, "%s/%s/%s/%s",
                        bases[i], level1, level2, level3) > 0);

                if (i == 0){
                        result = testrun_path_search_project_path(path);
                        testrun(result);
                        real = realpath(bases[i], real);
                        testrun(strncmp(result,real, strlen(result)) == 0);
                        free(real);
                        real = NULL;
                        free(result);
                        result = NULL;

                } else {

                        /* @NOTE: this test will be run within the valid project
                         * **testrun_create_module**, so it is actually testing
                         * finding a top project from different folders */

                        real   = realpath(".", real);
                        result = testrun_path_search_project_path(path);
                        testrun(strncmp(result,real, strlen(result)) == 0);
                        free(real);
                        real = NULL;
                        free(result);
                        result = NULL;
                }

        }

        /* Check a folder outside of the testrun_project structure */
        testrun(!testrun_path_search_project_path("/tmp"));
        testrun(!testrun_path_search_project_path("/etc"));
        testrun(!testrun_path_search_project_path("/home"));

        /* Check NULL input */
        testrun(!testrun_path_search_project_path(NULL));

        /* Check project base input */

        result = testrun_path_search_project_path(".");
        testrun(result);
        free(result);

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

       testrun_test(test_testrun_path_is_project_top_dir);
       testrun_test(test_testrun_path_search_project_path);

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
