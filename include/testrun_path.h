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

#include <sys/types.h>
#include <sys/stat.h>

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
        char const * const path);

/*----------------------------------------------------------------------------*/

/**
        Search the project root path starting from an input path.

        @param path             path to start search
        @returns                path to project root or NULL on error
*/
char *testrun_path_search_project_path(
        char const * const path);

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
        char const * const module_name);

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
        char const * const module_name);

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
        Get the path from project to doxygen.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_doxygen(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the path from project to tests.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_tests(
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
        Get the path from project to config.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_config_data(
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
        Get the path from project to acceptance tests

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_acceptance_tests(
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
bool testrun_path_project_to_test_resources(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the full path for the unit test script

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_script_unit_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the full path for the acceptance test script

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_script_acceptance_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the full path for the coverage test script

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_script_coverage_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the full path for the loc test script

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_script_loc_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the full path for the service config folder

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_service_config(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the full path for the service install script

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_project_to_service_install(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the full path for the service.service configuration file.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_script_service_service_definition(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the full path for the service.socket configuration file.

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_script_service_socket_definition(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the full path for the service install script

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_script_service_install(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the full path for the service uninstall script

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_script_service_uninstall(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Get the full path for the doxygen config

        @param buffer           buffer to write to
        @param max              max open buffer
        @param config           current config
        @returns                true if path was written to buffer
*/
bool testrun_path_doxygen_config(
        char *buffer, size_t max,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Create a path.

        @param path             path to create
        @param max              max open buffer
        @returns                true if path was written to buffer
*/
bool testrun_path_create(char *path, size_t max);


#endif /* testrun_path_h */
