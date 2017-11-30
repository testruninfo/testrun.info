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


        -------------------------------------------------------------------------
*/

#ifndef testrun_lib_h
#define testrun_lib_h

/*----------------------------------------------------------------------------*/

#include "testrun.h"
#include "testrun_string.h"
#include "testrun_vector.h"
#include "testrun_copyright.h"
#include "testrun_header.h"

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the testrun.h files.
*/
char *testrun_lib_testrun_header_content();

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script to run test files of a folder.
        (Common name: testrun_runner.sh)
*/
char *testrun_lib_script_folder_runner_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script acceptance tests.
        (Common name: testrun_simple_acceptance_tests.sh)
        This script will autogenerate the default testrun script and set
        Copyright, Author and Year accordingly.

        @param config   an input config, which is used as a base.
        @returns        script content
*/
char *testrun_lib_script_acceptance_tests_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script unit tests.
        (Common name: testrun_simple_unit_tests.sh)
        This script will autogenerate the default testrun script and set
        Copyright, Author and Year accordingly.

        @param config   an input config, which is used as a base.
        @returns        script content
*/
char *testrun_lib_script_unit_tests_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script coverrage tests.
        (Common name: testrun_simple_coverage_tests.sh)
        This script will autogenerate the default testrun script and set
        Copyright, Author and Year accordingly.

        @param config   an input config, which is used as a base.
        @returns        script content
*/
char *testrun_lib_script_coverage_tests_content(testrun_config config);

/*----------------------------------------------------------------------------*/

/**
        Generate the content of the script loc tests.
        (Common name: testrun_simple_loc_tests.sh)
        This script will autogenerate the default testrun script and set
        Copyright, Author and Year accordingly.

        @param config   an input config, which is used as a base.
        @returns        script content
*/
char *testrun_lib_script_loc_tests_content(testrun_config config);


#endif /* testrun_lib_h */
