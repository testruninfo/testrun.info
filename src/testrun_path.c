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

        @file           testrun_path.c
        @author         Markus Toepfer
        @date           2017-11-25

        @ingroup        testrun_lib

        @brief          Implementation of path related functionality


        ------------------------------------------------------------------------
*/

#include "../include/testrun_path.h"

/*----------------------------------------------------------------------------*/

bool testrun_path_is_project_top_dir(
        char *path){

        if (!path)
                return false;

        bool include    = false;
        bool src        = false;
        bool tests      = false;

        DIR *dp;
        struct dirent *ep;

        dp = opendir(path);
        if (!dp)
                return false;

        while((ep = readdir (dp))) {
                if (ep->d_type == DT_DIR) {

                        if (!include) {
                                if (strncmp(ep->d_name,
                                        TESTRUN_PATH_INCLUDE,
                                        strlen(TESTRUN_PATH_INCLUDE))
                                        == 0) {
                                        include = true;
                                        continue;
                                }
                        }

                        if (!src) {
                                if (strncmp(ep->d_name,
                                        TESTRUN_PATH_SRC,
                                        strlen(TESTRUN_PATH_SRC))
                                        == 0) {
                                        src = true;
                                        continue;
                                }
                        }

                        if (!tests) {
                                if (strncmp(ep->d_name,
                                        TESTRUN_PATH_TESTS,
                                        strlen(TESTRUN_PATH_TESTS))
                                        == 0) {
                                        tests = true;
                                        continue;
                                }
                        }
                }
        }
        (void) closedir (dp);

    return (include && src && tests);
}

/*----------------------------------------------------------------------------*/

char *testrun_path_search_project_path(
        char *input){

        char *parent  = NULL;
        char *current = NULL;

        if (!input)
                return NULL;

        char path[PATH_MAX];

        current = realpath(input, current);
        if (!current)
                goto error;

        if (!testrun_path_is_project_top_dir(current)){

                /* walk up one level */
                sprintf(path, "%s%s%s%s",
                        current,
                        TESTRUN_PATH_SPLIT,
                        "..",
                        TESTRUN_PATH_SPLIT);

                parent = realpath(path, parent);
                if (!parent)
                        goto error;

                if (strcmp(current, parent) == 0) {

                        goto error;

                } else {

                        free(current);
                        current = testrun_path_search_project_path(
                                parent);

                        free(parent);
                        parent = NULL;

                }
        }

        return current;

error:
        if (current)
                free(current);
        if (parent)
                free(parent);
        return NULL;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_source_to_include(
        char *buffer, size_t max,
        struct testrun_config const * const config,
        char * module_name){

        if (!buffer || !config || !module_name )
                return false;

        /**
         *      default LINUX   "../include/module_name.h
         *
         *      step1 - add config transition src_to_include
         *      step3 - add include foldername
         *      step4 - add filename for the module.
         */

        if (max < (     strlen(module_name) +
                        strlen(config->format.extensions.c_header) +
                        2 * strlen(TESTRUN_PATH_SPLIT) +
                        strlen(config->project.path.src_to_include) +
                        strlen(config->project.path.include) + 1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s%s%s%s",
                config->project.path.src_to_include,
                TESTRUN_PATH_SPLIT,
                config->project.path.include,
                TESTRUN_PATH_SPLIT,
                module_name,
                config->format.extensions.c_header) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_test_to_source(
        char *buffer, size_t max,
        struct testrun_config const * const config,
        char * module_name){

        if (!buffer || !config || !module_name )
                return false;

        /**
         *      default LINUX   "../../src/module_name.c
         *
         *      step1 - move 1 level up to be at tests
         *      step2 - add config transition tests_to_src
         *      step3 - add src foldername
         *      step4 - add filename for the module.
         */

        if (max < (     strlen(module_name) +
                        strlen(config->format.extensions.c_source) +
                        3 * strlen(TESTRUN_PATH_SPLIT) +
                        strlen(config->project.path.tests.tests_to_src) +
                        strlen( config->project.path.src) + 3 ))
                return false;

        if (snprintf(buffer, max, "..%s%s%s%s%s%s%s",
                TESTRUN_PATH_SPLIT,
                config->project.path.tests.tests_to_src,
                TESTRUN_PATH_SPLIT,
                config->project.path.src,
                TESTRUN_PATH_SPLIT,
                module_name,
                config->format.extensions.c_source) < 0)
                return false;

        return true;
}


/*----------------------------------------------------------------------------*/

bool testrun_path_testrun_header(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config)
                return false;

        if (max < (     strlen(TESTRUN_FILE_TESTRUN_HEADER) +
                        3 * strlen(TESTRUN_PATH_SPLIT) +
                        strlen(config->project.path.tests.tests_to_tools) +
                        strlen(config->project.path.tests.tools) + 1 ))
                return false;

        if (snprintf(buffer, max, "..%s%s%s%s%s%s",
                TESTRUN_PATH_SPLIT,
                config->project.path.tests.tests_to_tools,
                TESTRUN_PATH_SPLIT,
                config->project.path.tests.tools,
                TESTRUN_PATH_SPLIT,
                TESTRUN_FILE_TESTRUN_HEADER) < 0)
                return false;

        return true;
}
