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

        @file           testrun_path.h
        @author         Markus Toepfer
        @date           2017-11-25

        @ingroup        testrun_lib

        @brief          Defintion of path related functions.


        ------------------------------------------------------------------------
*/

#ifndef testrun_path_h
#define testrun_path_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include <ctype.h>

#include <dirent.h>
#include <unistd.h>
#include <limits.h>

#include "testrun_config.h"


/*----------------------------------------------------------------------------*/

/**
        Check if a path is the top level directory of an project.
        This test is done by checking the existence of
                intput/
                src/
                tests/
        if these 3 folders are found, a project root directory is identified.
        NOTE works only if these 3 folders are found at the same level. This
        function is not configurable.

        @param path             path to start search
        @returns                true if path is project root
*/
bool testrun_path_is_project_top_dir(
        char *path);

/*----------------------------------------------------------------------------*/

/**
        Search the project root path starting from an input path.

        @param path             path to start search
        @returns                path to project root or NULL on error
*/
char *testrun_path_search_project_path(
        char *path);

/*----------------------------------------------------------------------------*/

/**
        Get the path from source to include.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_add_element(
        char *buffer, size_t max,
        char const * const separator,
        char const * const element);

/*----------------------------------------------------------------------------*/

/**
        Get the path from source to include.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_source_to_include(
        char *buffer, size_t max,
        struct testrun_config const * const config,
        char * module_name);

/*----------------------------------------------------------------------------*/

/**
        Get the path from test to source.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_test_to_source(
        char *buffer, size_t max,
        struct testrun_config const * const config,
        char * module_name);

/*----------------------------------------------------------------------------*/

/**
        Get the path from test to testrun.h.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_test_to_testrun_header(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the path from project to tools.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_tools(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the path from project to src.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_source(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the path from project to include.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_include(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the path from project to docs.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_docs(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the path from project to config.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_config(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the path from project to copyright.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_copyright(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the path from project to unit tests.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_unit_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the path from project to acceptance tests.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_acceptance_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config);

#endif /* testrun_path_h */
