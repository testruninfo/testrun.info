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
        char const * const path){

        if (!path)
                return false;

        bool include    = false;
        bool source     = false;
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
                                        TESTRUN_FOLDER_INCLUDE,
                                        strlen(TESTRUN_FOLDER_INCLUDE))
                                        == 0) {
                                        include = true;
                                        continue;
                                }
                        }

                        if (!source) {
                                if (strncmp(ep->d_name,
                                        TESTRUN_FOLDER_SOURCE,
                                        strlen(TESTRUN_FOLDER_SOURCE))
                                        == 0) {
                                        source = true;
                                        continue;
                                }
                        }

                        if (!tests) {
                                if (strncmp(ep->d_name,
                                        TESTRUN_FOLDER_TESTS,
                                        strlen(TESTRUN_FOLDER_TESTS))
                                        == 0) {
                                        tests = true;
                                        continue;
                                }
                        }
                }
        }
        (void) closedir (dp);

    return (include && source && tests);
}

/*----------------------------------------------------------------------------*/

char *testrun_path_search_project_path(
        char const * const input){

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

bool testrun_path_create(char *in, size_t max){

        if (!in)
                return false;

        if (max > PATH_MAX)
                return false;

        testrun_vector *vector = testrun_string_split(in, max, "/", 1, false);

        if (!vector)
                return false;

        bool created = false;

        DIR *dp;

        char *pointer = NULL;
        char *element = NULL;
        size_t open   = PATH_MAX;

        char path[open];
        bzero(path, open);

        pointer = path;
        for (size_t i = 0; i <= vector->last; i++){

                element = (char*) vector->items[i];

                if (i == 0) {

                        if (element == NULL) {

                                log_debug("create path starting at ROOT");

                                if (snprintf(pointer, open, "/") < 0)
                                        goto error;

                                pointer++;
                                open--;

                                continue;

                        } else {

                                log_debug("create relative path");

                       }

                } else if (element == NULL) {
                        log_error("NULL in path");
                        goto error;
                }

                if (snprintf(pointer, open, "%s/", element) < 0)
                        goto error;

                pointer += (strlen(element) + 1);
                open    -= (strlen(element) + 1);

                log("PATH %jd| %s", i, path);

                dp = opendir(path);
                if (dp){
                        // PATH exists access OK
                        (void) closedir (dp);

                } else if (mkdir(path,
                                S_IRUSR | S_IWUSR | S_IXUSR |
                                S_IRGRP | S_IXGRP |
                                S_IROTH | S_IXOTH) != 0){

                        log_error("Failed to create or access PATH %s", path);
                        goto error;
                } else {
                        created = true;
                }
        }

        log_debug("PATH OK %s (%s)", path, created ? "created" : "existing");

        vector = testrun_vector_terminate(vector);
        return true;
error:
        vector = testrun_vector_terminate(vector);
        return false;
}
