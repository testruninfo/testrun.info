/***
        -------------------------------------------------------------------------

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

        -------------------------------------------------------------------------
*//**

        @file           testrun_lib.h
        @author         Markus Toepfer
        @date           2017-11-13

        @ingroup        testrun_lib

        @brief          Main interface for the testrun_lib.

        This header includes all functionality of the library and a definition
        of the top level functions provided over this library.

        -------------------------------------------------------------------------
*/

#ifndef testrun_lib_h
#define testrun_lib_h

/*----------------------------------------------------------------------------*/

#include "testrun.h"
#include "testrun_config.h"
#include "testrun_copyright.h"
#include "testrun_log.h"
#include "testrun_path.h"
#include "testrun_string.h"
#include "testrun_text_block.h"
#include "testrun_time.h"
#include "testrun_vector.h"


/*
 *      ------------------------------------------------------------------------
 *
 *      General functions to create a testrun project or a new module within
 *      an existing testrun project.
 *
 *      ------------------------------------------------------------------------
 */

/**
        Create a project based on the config. This function will create all
        PATHS and FILES for the project, based on the input config.

        This function SHALL be used to create a new project structure.

        @param config           an input config, which is used as a base.
        @param readme           (optional) content for the project readme.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
bool testrun_lib_create_project(
        testrun_config const * const config,
        char const * const readme);

/*----------------------------------------------------------------------------*/

/**
        Generate the files name.h, name.c and name_test.c within their
        respective folders for header, source and unit test folder. All
        path and dynamic data will be generated out of the config.

        @param name             name for the new module file within the project
        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
bool testrun_lib_create_module_files(
        char const * const name,
        testrun_config const * const config);

/*
 *      ------------------------------------------------------------------------
 *
 *      Additonal functions, which may be used in an external program to
 *      advance an existing project to a testrun project.
 *
 *      ------------------------------------------------------------------------
 */

/**
        Generate all test tool files and write the tools to the PATH defined
        within the config.

        E.g. config.project.path.tests.to_tools = "/home/user/project/tools"
        will write all test scripts to "/home/user/project/tools"

        NOTE: This function will not create any type of makefile, just all test
        scripts as well as the general testrun.h header, and write the files to
        the given tools destination, which PATH MUST exist.

        This function MAY be used to add the test tools to an existing project.

        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
bool testrun_lib_create_test_tools(
        testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the central header testrun.h

        @returns                allocated string with file content or NULL
                                (MUST be freed)
*/
char *testrun_lib_testrun_header_content();


/*----------------------------------------------------------------------------*/

/**
        Generate the content of a c file for a project. The file content
        will be created based on the filetype, which may be any of
        TESTRUN_HEADER, TESTRUN_SOURCE or TESTRUN_TEST.

        @param name             name of the module
        @param filetype         file to be created
        @param config           an input config, which is used as a base.
        @returns                allocated string with file content or NULL
                                (MUST be freed)
*/
char * testrun_lib_c_file_content(
        char const * const name,
        testrun_extension_t filetype,
        struct testrun_config const * const config);

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script to run all test files of a folder.
        (Common name: testrun_runner.sh)
        This script will autogenerate the default testrun script and set
        Copyright, Author and Year accordingly.

        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
char *testrun_lib_script_folder_runner_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script for acceptance tests.
        (Common name: testrun_simple_acceptance_tests.sh)
        This script will autogenerate the default testrun script and set
        Copyright, Author and Year accordingly.

        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
char *testrun_lib_script_acceptance_tests_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script to run unit tests.
        (Common name: testrun_simple_unit_tests.sh)
        This script will autogenerate the default testrun script and set
        Copyright, Author and Year accordingly.

        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
char *testrun_lib_script_unit_tests_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script to run coverrage tests.
        (Common name: testrun_simple_coverage_tests.sh)
        This script will autogenerate the default testrun script and set
        Copyright, Author and Year accordingly.

        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
char *testrun_lib_script_coverage_tests_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script to count the lines of code.
        (Common name: testrun_simple_loc_tests.sh)
        This script will autogenerate the default testrun script and set
        Copyright, Author and Year accordingly.

        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
char *testrun_lib_script_loc_tests_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the central makefile of the project.

        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
char *testrun_lib_makefile_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the main makefile of the project.

        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
char *testrun_lib_makefile_main_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the test makefile of the project.

        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
char *testrun_lib_makefile_test_content(testrun_config config);


/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script for the service installation.
        This script will autogenerate the default testrun script and set
        Copyright, Author and Year accordingly.

        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
char *testrun_lib_script_service_install_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script for the service deinstallation.
        This script will autogenerate the default testrun script and set
        Copyright, Author and Year accordingly.

        @param config           an input config, which is used as a base.
        @returns                allocated string with script content or NULL
                                (MUST be freed)
*/
char *testrun_lib_script_service_uninstall_content(testrun_config config);

#endif /* testrun_lib_h */
