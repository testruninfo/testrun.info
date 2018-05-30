/***
 *      ------------------------------------------------------------------------
 *
 *      Copyright 2017 Markus Toepfer
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 *
 *      This file is part of the testrun project. http://testrun.info
 *
 *      ------------------------------------------------------------------------
 ***//**
 *
 *      @file           testrun_project_app.h
 *      @author         Markus Toepfer
 *      @date           2018-05-11
 *
 *      @ingroup        testrun_project
 *
 *      @brief          Defintition of an testrun_project_app.
 *
 *      ------------------------------------------------------------------------
 **/

#ifndef testrun_project_app_h
#define testrun_project_app_h

#include <getopt.h>
#include <unistd.h>
#include <libgen.h>

#include "testrun_lib.h"

#define PROJECT_APP_VERSION "1.0.0"

#define testrun_project_app_print_version() \
        fprintf(stdout, "version: %s\n", PROJECT_APP_VERSION);


/*----------------------------------------------------------------------------*/

/**
        Try to set an author based on the environment parameter.
        @param  buffer to store the string
        @param  size   size of buffer
        @return        true on success.
*/
bool testrun_project_app_set_author_dynamic(
        char *buffer, size_t size);

/*----------------------------------------------------------------------------*/

/**
        Create a module based on a config.

        @param  config for of the APP
        @return        0 on success.
 */
int testrun_project_app_create_new_module(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Create a new project based on a config.

        @param  config for of the APP
        @return        0 on success.
 */
int testrun_project_app_create_new_project(testrun_config config);

/*----------------------------------------------------------------------------*/
/**
        APP running loop. The app will be run as a process, until this runner
        returns. This functionality is used, to make the whole runner testable.

        @param argc     amount of command line arguments
        @param argv     vector of command line arguments

        @returns        negative value on error, 0 on success
*/
int testrun_project_app_run(int argc, char *argv[]);

#endif /* testrun_project_app_h */
