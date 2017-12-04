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

int test_testrun_path_source_to_include() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        char *name = "module";

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_source_to_include(NULL,   0, NULL,  NULL));
        testrun(!testrun_path_source_to_include(NULL,   PATH_MAX, &config, name));
        testrun(!testrun_path_source_to_include(buffer, PATH_MAX, NULL,    name));
        testrun(!testrun_path_source_to_include(buffer, PATH_MAX, &config, NULL));

        testrun(!testrun_path_source_to_include(buffer, 4,        &config, name),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "..%s.%sinclude%smodule.h",
                TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT);
        testrun(testrun_path_source_to_include(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_include = "../home";
        snprintf(expect, PATH_MAX, "..%s../home%sinclude%smodule.h",
                TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT);
        testrun(testrun_path_source_to_include(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.format.extensions.c_header = ".header";
        snprintf(expect, PATH_MAX, "..%s../home%sinclude%smodule.header",
                TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT);
        testrun(testrun_path_source_to_include(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.from_source = "xyz";
        snprintf(expect, PATH_MAX, "xyz%s../home%sinclude%smodule.header",
                TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT);
        testrun(testrun_path_source_to_include(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.from_source   = "1";
        config.project.path.to_include    = "2";
        config.project.path.include       = "3";
        name                              = "4";
        config.format.extensions.c_header = "5";

        snprintf(expect, PATH_MAX, "1%s2%s3%s45",
                TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT);
        testrun(testrun_path_source_to_include(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        bzero(expect, PATH_MAX);
        snprintf(expect, PATH_MAX, "1%s2%s3%s45",
                TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT,TESTRUN_PATH_SPLIT);
        testrun(testrun_path_source_to_include(buffer, 9, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 8);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_source_to_include(buffer, 8, &config, name));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
         snprintf(expect, PATH_MAX, "1---2---3---45");
        testrun(testrun_path_source_to_include(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_test_to_source() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        char *name = "module";

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_test_to_source(NULL,   0, NULL,  NULL));
        testrun(!testrun_path_test_to_source(NULL,   PATH_MAX, &config, name));
        testrun(!testrun_path_test_to_source(buffer, PATH_MAX, NULL,    name));
        testrun(!testrun_path_test_to_source(buffer, PATH_MAX, &config, NULL));

        testrun(!testrun_path_test_to_source(buffer, 4,        &config, name),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "../.././src/module.c");
        testrun(testrun_path_test_to_source(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_source = "../home";
        snprintf(expect, PATH_MAX, "../../../home/src/module.c");
        testrun(testrun_path_test_to_source(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.format.extensions.c_source = ".header";
        snprintf(expect, PATH_MAX, "../../../home/src/module.header");
        testrun(testrun_path_test_to_source(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);
        config.format.extensions.c_source = ".c";

        config.project.path.from_tests = "xyz";
        snprintf(expect, PATH_MAX, "../xyz/../home/src/module.c");
        testrun(testrun_path_test_to_source(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.from_tests    = "1";
        config.project.path.to_source     = "2";
        config.project.path.source        = "3";
        name                              = "4";
        config.format.extensions.c_source = "5";

        snprintf(expect, PATH_MAX, "../1/2/3/45");
        testrun(testrun_path_test_to_source(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_test_to_source(buffer, 12, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 11);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_test_to_source(buffer, 11, &config, name));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "..---1---2---3---45");
        testrun(testrun_path_test_to_source(buffer, PATH_MAX, &config, name));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_test_to_testrun_header() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_test_to_testrun_header(NULL,   0, NULL));
        testrun(!testrun_path_test_to_testrun_header(NULL,   PATH_MAX, &config));
        testrun(!testrun_path_test_to_testrun_header(buffer, PATH_MAX, NULL));

        testrun(!testrun_path_test_to_testrun_header(buffer, 4, &config),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, ".././tools/testrun.h");
        testrun(testrun_path_test_to_testrun_header(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.tests.to_tools = "../home";
        snprintf(expect, PATH_MAX, "../../home/tools/testrun.h");
        testrun(testrun_path_test_to_testrun_header(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.tools.header = "name";
        snprintf(expect, PATH_MAX, "../../home/tools/name");
        testrun(testrun_path_test_to_testrun_header(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.tools.name = "othertools";
        snprintf(expect, PATH_MAX, "../../home/othertools/name");
        testrun(testrun_path_test_to_testrun_header(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.tests.to_tools    = "1";
        config.project.path.tests.tools.name  = "2";
        config.project.path.tests.tools.header= "3";

        snprintf(expect, PATH_MAX, "../1/2/3");
        testrun(testrun_path_test_to_testrun_header(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_test_to_testrun_header(buffer, 9, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 8);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_test_to_testrun_header(buffer, 8, &config));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "..---1---2---3");
        testrun(testrun_path_test_to_testrun_header(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_project_to_tools() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_project_to_tools(NULL,   0, NULL));
        testrun(!testrun_path_project_to_tools(NULL,   PATH_MAX, &config));
        testrun(!testrun_path_project_to_tools(buffer, PATH_MAX, NULL));

        testrun(!testrun_path_project_to_tools(buffer, 4, &config),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "./tests/./tools");
        testrun(testrun_path_project_to_tools(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_tests = "../home";
        snprintf(expect, PATH_MAX, "../home/tests/./tools");
        testrun(testrun_path_project_to_tools(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.name = "other";
        snprintf(expect, PATH_MAX, "../home/other/./tools");
        testrun(testrun_path_project_to_tools(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.to_tools = "../no";
        snprintf(expect, PATH_MAX, "../home/other/../no/tools");
        testrun(testrun_path_project_to_tools(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.tools.name = "thing";
        snprintf(expect, PATH_MAX, "../home/other/../no/thing");
        testrun(testrun_path_project_to_tools(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.to_tests          = "1";
        config.project.path.tests.name        = "2";
        config.project.path.tests.to_tools    = "3";
        config.project.path.tests.tools.name  = "4";

        snprintf(expect, PATH_MAX, "1/2/3/4");
        testrun(testrun_path_project_to_tools(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_project_to_tools(buffer, 8, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 7);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_project_to_tools(buffer, 7, &config));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "1---2---3---4");
        testrun(testrun_path_project_to_tools(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_project_to_include() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_project_to_include(NULL,   0, NULL));
        testrun(!testrun_path_project_to_include(NULL,   PATH_MAX, &config));
        testrun(!testrun_path_project_to_include(buffer, PATH_MAX, NULL));

        testrun(!testrun_path_project_to_include(buffer, 4, &config),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "./include");
        testrun(testrun_path_project_to_include(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_include = "../home";
        snprintf(expect, PATH_MAX, "../home/include");
        testrun(testrun_path_project_to_include(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.include = "other";
        snprintf(expect, PATH_MAX, "../home/other");
        testrun(testrun_path_project_to_include(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.to_include          = "1";
        config.project.path.include             = "2";

        snprintf(expect, PATH_MAX, "1/2");
        testrun(testrun_path_project_to_include(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_project_to_include(buffer, 4, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 3);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_project_to_include(buffer, 3, &config));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "1---2");
        testrun(testrun_path_project_to_include(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_project_to_source() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_project_to_source(NULL,   0, NULL));
        testrun(!testrun_path_project_to_source(NULL,   PATH_MAX, &config));
        testrun(!testrun_path_project_to_source(buffer, PATH_MAX, NULL));

        testrun(!testrun_path_project_to_source(buffer, 4, &config),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "./src");
        testrun(testrun_path_project_to_source(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_source = "../home";
        snprintf(expect, PATH_MAX, "../home/src");
        testrun(testrun_path_project_to_source(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.source = "source";
        snprintf(expect, PATH_MAX, "../home/source");
        testrun(testrun_path_project_to_source(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.to_source          = "1";
        config.project.path.source             = "2";

        snprintf(expect, PATH_MAX, "1/2");
        testrun(testrun_path_project_to_source(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_project_to_source(buffer, 4, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 3);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_project_to_source(buffer, 3, &config));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "1---2");
        testrun(testrun_path_project_to_source(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_project_to_docs() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_project_to_docs(NULL,   0, NULL));
        testrun(!testrun_path_project_to_docs(NULL,   PATH_MAX, &config));
        testrun(!testrun_path_project_to_docs(buffer, PATH_MAX, NULL));

        testrun(!testrun_path_project_to_docs(buffer, 4, &config),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "./docs");
        testrun(testrun_path_project_to_docs(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_docs = "../home";
        snprintf(expect, PATH_MAX, "../home/docs");
        testrun(testrun_path_project_to_docs(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.docs = "documents";
        snprintf(expect, PATH_MAX, "../home/documents");
        testrun(testrun_path_project_to_docs(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.to_docs          = "1";
        config.project.path.docs             = "2";

        snprintf(expect, PATH_MAX, "1/2");
        testrun(testrun_path_project_to_docs(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_project_to_docs(buffer, 4, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 3);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_project_to_docs(buffer, 3, &config));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "1---2");
        testrun(testrun_path_project_to_docs(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_project_to_doxygen() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_project_to_doxygen(NULL,   0, NULL));
        testrun(!testrun_path_project_to_doxygen(NULL,   PATH_MAX, &config));
        testrun(!testrun_path_project_to_doxygen(buffer, PATH_MAX, NULL));

        testrun(!testrun_path_project_to_doxygen(buffer, 4, &config),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "./doxygen");
        testrun(testrun_path_project_to_doxygen(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_doxygen = "../home";
        snprintf(expect, PATH_MAX, "../home/doxygen");
        testrun(testrun_path_project_to_doxygen(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.doxygen.foldername = "documents";
        snprintf(expect, PATH_MAX, "../home/documents");
        testrun(testrun_path_project_to_doxygen(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.to_doxygen          = "1";
        config.project.doxygen.foldername       = "2";

        snprintf(expect, PATH_MAX, "1/2");
        testrun(testrun_path_project_to_doxygen(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_project_to_doxygen(buffer, 4, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 3);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_project_to_doxygen(buffer, 3, &config));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "1---2");
        testrun(testrun_path_project_to_doxygen(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_project_to_tests() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_project_to_tests(NULL,   0, NULL));
        testrun(!testrun_path_project_to_tests(NULL,   PATH_MAX, &config));
        testrun(!testrun_path_project_to_tests(buffer, PATH_MAX, NULL));

        testrun(!testrun_path_project_to_tests(buffer, 4, &config),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "./tests");
        testrun(testrun_path_project_to_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_tests = "../home";
        snprintf(expect, PATH_MAX, "../home/tests");
        testrun(testrun_path_project_to_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.name = "documents";
        snprintf(expect, PATH_MAX, "../home/documents");
        testrun(testrun_path_project_to_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.to_tests         = "1";
        config.project.path.tests.name       = "2";

        snprintf(expect, PATH_MAX, "1/2");
        testrun(testrun_path_project_to_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_project_to_tests(buffer, 4, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 3);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_project_to_tests(buffer, 3, &config));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "1---2");
        testrun(testrun_path_project_to_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_project_to_copyright() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_project_to_copyright(NULL,   0, NULL));
        testrun(!testrun_path_project_to_copyright(NULL,   PATH_MAX, &config));
        testrun(!testrun_path_project_to_copyright(buffer, PATH_MAX, NULL));

        testrun(!testrun_path_project_to_copyright(buffer, 4, &config),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "./copyright");
        testrun(testrun_path_project_to_copyright(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_copyright = "../home";
        snprintf(expect, PATH_MAX, "../home/copyright");
        testrun(testrun_path_project_to_copyright(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.copyright = "copy";
        snprintf(expect, PATH_MAX, "../home/copy");
        testrun(testrun_path_project_to_copyright(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.to_copyright          = "1";
        config.project.path.copyright             = "2";

        snprintf(expect, PATH_MAX, "1/2");
        testrun(testrun_path_project_to_copyright(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_project_to_copyright(buffer, 4, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 3);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_project_to_copyright(buffer, 3, &config));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "1---2");
        testrun(testrun_path_project_to_copyright(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_project_to_config() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_project_to_config(NULL,   0, NULL));
        testrun(!testrun_path_project_to_config(NULL,   PATH_MAX, &config));
        testrun(!testrun_path_project_to_config(buffer, PATH_MAX, NULL));

        testrun(!testrun_path_project_to_config(buffer, 4, &config),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "./config");
        testrun(testrun_path_project_to_config(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_config = "../home";
        snprintf(expect, PATH_MAX, "../home/config");
        testrun(testrun_path_project_to_config(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.config = "copy";
        snprintf(expect, PATH_MAX, "../home/copy");
        testrun(testrun_path_project_to_config(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.to_config          = "1";
        config.project.path.config             = "2";

        snprintf(expect, PATH_MAX, "1/2");
        testrun(testrun_path_project_to_config(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_project_to_config(buffer, 4, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 3);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_project_to_config(buffer, 3, &config));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "1---2");
        testrun(testrun_path_project_to_config(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_project_to_unit_tests() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_project_to_unit_tests(NULL,   0, NULL));
        testrun(!testrun_path_project_to_unit_tests(NULL,   PATH_MAX, &config));
        testrun(!testrun_path_project_to_unit_tests(buffer, PATH_MAX, NULL));

        testrun(!testrun_path_project_to_unit_tests(buffer, 4, &config),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "./tests/unit");
        testrun(testrun_path_project_to_unit_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_tests = "../home";
        snprintf(expect, PATH_MAX, "../home/tests/unit");
        testrun(testrun_path_project_to_unit_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.name = "other";
        snprintf(expect, PATH_MAX, "../home/other/unit");
        testrun(testrun_path_project_to_unit_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.unit = "thing";
        snprintf(expect, PATH_MAX, "../home/other/thing");
        testrun(testrun_path_project_to_unit_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.to_tests          = "1";
        config.project.path.tests.name        = "2";
        config.project.path.tests.unit        = "3";

        snprintf(expect, PATH_MAX, "1/2/3");
        testrun(testrun_path_project_to_unit_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_project_to_unit_tests(buffer, 6, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 5);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_project_to_unit_tests(buffer, 5, &config));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "1---2---3");
        testrun(testrun_path_project_to_unit_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}
/*----------------------------------------------------------------------------*/

int test_testrun_path_project_to_acceptance_tests() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_project_to_acceptance_tests(NULL,   0, NULL));
        testrun(!testrun_path_project_to_acceptance_tests(NULL,   PATH_MAX, &config));
        testrun(!testrun_path_project_to_acceptance_tests(buffer, PATH_MAX, NULL));

        testrun(!testrun_path_project_to_acceptance_tests(buffer, 4, &config),
                "MAX to small");

        testrun(buffer[0] == 0);

        snprintf(expect, PATH_MAX, "./tests/acceptance");
        testrun(testrun_path_project_to_acceptance_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check configurability
        // -------------------------------------------------------------

        config.project.path.to_tests = "../home";
        snprintf(expect, PATH_MAX, "../home/tests/acceptance");
        testrun(testrun_path_project_to_acceptance_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.name = "other";
        snprintf(expect, PATH_MAX, "../home/other/acceptance");
        testrun(testrun_path_project_to_acceptance_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.acceptance = "thing";
        snprintf(expect, PATH_MAX, "../home/other/thing");
        testrun(testrun_path_project_to_acceptance_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check max
        // -------------------------------------------------------------

        config.project.path.to_tests          = "1";
        config.project.path.tests.name        = "2";
        config.project.path.tests.acceptance  = "3";

        snprintf(expect, PATH_MAX, "1/2/3");
        testrun(testrun_path_project_to_acceptance_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() + 1
        testrun(testrun_path_project_to_acceptance_tests(buffer, 6, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strlen(expect) == 5);
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        // max = strlen() // no space for \0
        testrun(!testrun_path_project_to_acceptance_tests(buffer, 5, &config));

        // -------------------------------------------------------------
        // Check path split configuration
        // -------------------------------------------------------------

        config.format.path_split = "---";
        snprintf(expect, PATH_MAX, "1---2---3");
        testrun(testrun_path_project_to_acceptance_tests(buffer, PATH_MAX, &config));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_script_acceptance_tests() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_script_acceptance_tests(
                NULL, 0, NULL));
        testrun(!testrun_path_script_acceptance_tests(
                buffer, PATH_MAX, NULL));
        testrun(!testrun_path_script_acceptance_tests(
                buffer, 0, &config));
        testrun(!testrun_path_script_acceptance_tests(
                NULL, PATH_MAX, &config));

        testrun(testrun_path_script_acceptance_tests(
                buffer, PATH_MAX, &config));

        snprintf(expect, PATH_MAX,
                "./tests/./tools/testrun_simple_acceptance_tests.sh");
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.tools.acceptance_script = "1.sh";
        config.project.path.tests.tools.name = "B";
        config.project.path.tests.name = "A";
        config.project.path.to_tests = "../..";

        snprintf(expect, PATH_MAX,
                "../../A/./B/1.sh");

        testrun(testrun_path_script_acceptance_tests(
                buffer, PATH_MAX, &config));

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_script_coverage_tests() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_script_coverage_tests(
                NULL, 0, NULL));
        testrun(!testrun_path_script_coverage_tests(
                buffer, PATH_MAX, NULL));
        testrun(!testrun_path_script_coverage_tests(
                buffer, 0, &config));
        testrun(!testrun_path_script_coverage_tests(
                NULL, PATH_MAX, &config));

        testrun(testrun_path_script_coverage_tests(
                buffer, PATH_MAX, &config));

        snprintf(expect, PATH_MAX,
                "./tests/./tools/testrun_simple_coverage_tests.sh");
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.tools.coverage_script = "1.sh";
        config.project.path.tests.tools.name = "B";
        config.project.path.tests.name = "A";
        config.project.path.to_tests = "../..";

        snprintf(expect, PATH_MAX,
                "../../A/./B/1.sh");

        testrun(testrun_path_script_coverage_tests(
                buffer, PATH_MAX, &config));

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_script_unit_tests() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_script_unit_tests(
                NULL, 0, NULL));
        testrun(!testrun_path_script_unit_tests(
                buffer, PATH_MAX, NULL));
        testrun(!testrun_path_script_unit_tests(
                buffer, 0, &config));
        testrun(!testrun_path_script_unit_tests(
                NULL, PATH_MAX, &config));

        testrun(testrun_path_script_unit_tests(
                buffer, PATH_MAX, &config));

        snprintf(expect, PATH_MAX,
                "./tests/./tools/testrun_simple_unit_tests.sh");
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.tools.unit_script = "1.sh";
        config.project.path.tests.tools.name = "B";
        config.project.path.tests.name = "A";
        config.project.path.to_tests = "../..";

        snprintf(expect, PATH_MAX,
                "../../A/./B/1.sh");

        testrun(testrun_path_script_unit_tests(
                buffer, PATH_MAX, &config));

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_script_loc_tests() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_script_loc_tests(
                NULL, 0, NULL));
        testrun(!testrun_path_script_loc_tests(
                buffer, PATH_MAX, NULL));
        testrun(!testrun_path_script_loc_tests(
                buffer, 0, &config));
        testrun(!testrun_path_script_loc_tests(
                NULL, PATH_MAX, &config));

        testrun(testrun_path_script_loc_tests(
                buffer, PATH_MAX, &config));

        snprintf(expect, PATH_MAX,
                "./tests/./tools/testrun_simple_loc.sh");
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.path.tests.tools.loc_script = "1.sh";
        config.project.path.tests.tools.name = "B";
        config.project.path.tests.name = "A";
        config.project.path.to_tests = "../..";

        snprintf(expect, PATH_MAX,
                "../../A/./B/1.sh");

        testrun(testrun_path_script_loc_tests(
                buffer, PATH_MAX, &config));

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_script_service_install() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_script_service_install(
                NULL, 0, NULL));
        testrun(!testrun_path_script_service_install(
                buffer, PATH_MAX, NULL));
        testrun(!testrun_path_script_service_install(
                buffer, 0, &config));
        testrun(!testrun_path_script_service_install(
                NULL, PATH_MAX, &config));

        testrun(testrun_path_script_service_install(
                buffer, PATH_MAX, &config));

        snprintf(expect, PATH_MAX,
                "./config/install/install.sh");
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.service.install_script = "1.sh";
        config.project.service.folder = "B";
        config.project.path.config = "A";
        config.project.path.to_config = "../..";

        snprintf(expect, PATH_MAX,
                "../../A/B/1.sh");

        testrun(testrun_path_script_service_install(
                buffer, PATH_MAX, &config));

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_script_service_uninstall() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_script_service_uninstall(
                NULL, 0, NULL));
        testrun(!testrun_path_script_service_uninstall(
                buffer, PATH_MAX, NULL));
        testrun(!testrun_path_script_service_uninstall(
                buffer, 0, &config));
        testrun(!testrun_path_script_service_uninstall(
                NULL, PATH_MAX, &config));

        testrun(testrun_path_script_service_uninstall(
                buffer, PATH_MAX, &config));

        snprintf(expect, PATH_MAX,
                "./config/install/uninstall.sh");
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.service.uninstall_script = "1.sh";
        config.project.service.folder = "B";
        config.project.path.config = "A";
        config.project.path.to_config = "../..";

        snprintf(expect, PATH_MAX,
                "../../A/B/1.sh");

        testrun(testrun_path_script_service_uninstall(
                buffer, PATH_MAX, &config));

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_script_service_service_definition() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_script_service_service_definition(
                NULL, 0, NULL));
        testrun(!testrun_path_script_service_service_definition(
                buffer, PATH_MAX, NULL));
        testrun(!testrun_path_script_service_service_definition(
                buffer, 0, &config));
        testrun(!testrun_path_script_service_service_definition(
                NULL, PATH_MAX, &config));

        testrun(testrun_path_script_service_service_definition(
                buffer, PATH_MAX, &config));

        snprintf(expect, PATH_MAX,
                "./config/install/[PROJECT].service");
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.name = "1";
        config.project.service.folder = "B";
        config.project.path.config = "A";
        config.project.path.to_config = "../..";

        snprintf(expect, PATH_MAX,
                "../../A/B/1.service");

        testrun(testrun_path_script_service_service_definition(
                buffer, PATH_MAX, &config));

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_script_service_socket_definition() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_script_service_socket_definition(
                NULL, 0, NULL));
        testrun(!testrun_path_script_service_socket_definition(
                buffer, PATH_MAX, NULL));
        testrun(!testrun_path_script_service_socket_definition(
                buffer, 0, &config));
        testrun(!testrun_path_script_service_socket_definition(
                NULL, PATH_MAX, &config));

        testrun(testrun_path_script_service_socket_definition(
                buffer, PATH_MAX, &config));

        snprintf(expect, PATH_MAX,
                "./config/install/[PROJECT].socket");
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.name = "1";
        config.project.service.folder = "B";
        config.project.path.config = "A";
        config.project.path.to_config = "../..";

        snprintf(expect, PATH_MAX,
                "../../A/B/1.socket");

        testrun(testrun_path_script_service_socket_definition(
                buffer, PATH_MAX, &config));

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_path_doxygen_config() {

        char buffer[PATH_MAX];
        bzero(buffer, PATH_MAX);

        char expect[PATH_MAX];
        bzero(expect, PATH_MAX);

        testrun_config config = testrun_config_default();

        testrun(!testrun_path_doxygen_config(
                NULL, 0, NULL));
        testrun(!testrun_path_doxygen_config(
                buffer, PATH_MAX, NULL));
        testrun(!testrun_path_doxygen_config(
                buffer, 0, &config));
        testrun(!testrun_path_doxygen_config(
                NULL, PATH_MAX, &config));

        testrun(testrun_path_doxygen_config(
                buffer, PATH_MAX, &config));

        snprintf(expect, PATH_MAX,
                "./doxygen/doxygen.config");
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);

        config.project.doxygen.filename = "1.sh";
        config.project.doxygen.foldername = "B";
        config.project.path.to_doxygen = "../..";

        snprintf(expect, PATH_MAX,
                "../../B/1.sh");

        testrun(testrun_path_doxygen_config(
                buffer, PATH_MAX, &config));

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", buffer, strlen(buffer));
        testrun(strlen(expect) == strlen(buffer));
        testrun(strncmp(expect, buffer, strlen(expect)) == 0);


        return testrun_log_success();
}


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
        testrun_test(test_testrun_path_source_to_include);
        testrun_test(test_testrun_path_test_to_source);
        testrun_test(test_testrun_path_test_to_testrun_header);
        testrun_test(test_testrun_path_project_to_tools);

        testrun_test(test_testrun_path_project_to_include);
        testrun_test(test_testrun_path_project_to_source);
        testrun_test(test_testrun_path_project_to_docs);
        testrun_test(test_testrun_path_project_to_doxygen);
        testrun_test(test_testrun_path_project_to_tests);
        testrun_test(test_testrun_path_project_to_copyright);
        testrun_test(test_testrun_path_project_to_config);
        testrun_test(test_testrun_path_project_to_unit_tests);
        testrun_test(test_testrun_path_project_to_acceptance_tests);

        testrun_test(test_testrun_path_script_acceptance_tests);
        testrun_test(test_testrun_path_script_coverage_tests);
        testrun_test(test_testrun_path_script_unit_tests);
        testrun_test(test_testrun_path_script_loc_tests);
        testrun_test(test_testrun_path_script_service_install);
        testrun_test(test_testrun_path_script_service_uninstall);
        testrun_test(test_testrun_path_script_service_service_definition);
        testrun_test(test_testrun_path_script_service_socket_definition);
        testrun_test(test_testrun_path_doxygen_config);

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
