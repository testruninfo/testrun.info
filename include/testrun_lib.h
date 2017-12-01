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
        char * name,
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




#endif /* testrun_lib_h */
