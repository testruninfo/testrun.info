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

        @file           testrun_config.c
        @author         Markus Toepfer
        @date           2017-11-25

        @ingroup        testrun_lib

        @brief          Implementation of the testrun_config structure


        ------------------------------------------------------------------------
*/

#include "../include/testrun_config.h"


testrun_config testrun_config_default(){

        testrun_config config = {

                .author  = TESTRUN_TAG_DEFAULT_AUTHOR,
                .project =
                {
                        .name = TESTRUN_TAG_DEFAULT_PROJECT,
                        .type = TESTRUN_LIB,
                        .path =
                        {
                                .name           = TESTRUN_PATH_DUMMY,

                                .include        = TESTRUN_FOLDER_INCLUDE,
                                .source         = TESTRUN_FOLDER_SOURCE,
                                .docs           = TESTRUN_FOLDER_DOCS,
                                .config         = TESTRUN_FOLDER_CONFIG,
                                .copyright      = TESTRUN_FOLDER_COPYRIGHT,

                                .to_include     = TESTRUN_PATH_LEVEL_DOWN,
                                .to_source      = TESTRUN_PATH_LEVEL_DOWN,
                                .to_docs        = TESTRUN_PATH_LEVEL_DOWN,
                                .to_copyright   = TESTRUN_PATH_LEVEL_DOWN,
                                .to_config      = TESTRUN_PATH_LEVEL_DOWN,
                                .to_doxygen     = TESTRUN_PATH_LEVEL_DOWN,
                                .to_tests       = TESTRUN_PATH_LEVEL_DOWN,
                                .from_include   = TESTRUN_PATH_LEVEL_UP,
                                .from_source    = TESTRUN_PATH_LEVEL_UP,
                                .from_docs      = TESTRUN_PATH_LEVEL_UP,
                                .from_copyright = TESTRUN_PATH_LEVEL_UP,
                                .from_config    = TESTRUN_PATH_LEVEL_UP,
                                .from_doxygen   = TESTRUN_PATH_LEVEL_UP,
                                .from_tests     = TESTRUN_PATH_LEVEL_UP,
                                .tests          =
                                {
                                        .name       = TESTRUN_FOLDER_TESTS,
                                        .unit       = TESTRUN_FOLDER_TESTS_UNIT,
                                        .acceptance = TESTRUN_FOLDER_TESTS_ACCEPTANCE,

                                        .to_project = TESTRUN_PATH_LEVEL_UP,
                                        .to_tools   = TESTRUN_PATH_LEVEL_DOWN,

                                        .tools =
                                        {
                                                .name     = TESTRUN_FOLDER_TESTS_TOOLS,
                                                .to_tests = TESTRUN_PATH_LEVEL_UP,

                                                .header            = TESTRUN_FILE_TESTRUN_HEADER,
                                                .runner_script     = TESTRUN_SCRIPT_RUNNER,
                                                .acceptance_script = TESTRUN_SCRIPT_ACCEPTANCE,
                                                .unit_script       = TESTRUN_SCRIPT_UNIT,
                                                .coverage_script   = TESTRUN_SCRIPT_COVERAGE,
                                                .loc_script        = TESTRUN_SCRIPT_LOC
                                        }
                                }
                        },
                        .service = {
                                .folder           = TESTRUN_FOLDER_INSTALL,
                                .install_script   = TESTRUN_FILE_SERVICE_INSTALL,
                                .uninstall_script = TESTRUN_FILE_SERVICE_UNINSTALL,
                                .config_data      = NULL,
                                .listenStream     = NULL,
                                .listenData       = NULL
                        },
                        .doxygen =
                        {
                                .foldername = TESTRUN_FOLDER_DOXYGEN,
                                .filename   = TESTRUN_FILE_DOXYGEN
                        }
                },
                .copyright =
                {
                        .intro          = TESTRUN_COPYRIGHT_INTRO_DEFAULT,
                        .year           = TESTRUN_COPYRIGHT_TAG_YEAR,
                        .owner          = TESTRUN_COPYRIGHT_TAG_OWNER,
                        .text           = TESTRUN_COPYRIGHT_TEXT_DEFAULT,

                        .note           = NULL,
                        .variant        = NULL,
                        .program        = NULL,

                        .to_string      = testrun_copyright_to_string
                },
                .format =
                {
                        .line_end       = "\n",
                        .line_length    = 80,
                        .path_split     = TESTRUN_PATH_SPLIT,
                        .indent_c       = 8,
                        .offset_docu    = 3 * 8,
                        .indent_sh      = 4,
                        .prefix         =
                        {
                                .unit_test = "test_"
                        },
                        .suffix         =
                        {
                                .tests_source      = "_tests",
                                .tests_source_omp  = "_tests_omp"
                        },
                        .extensions =
                        {
                                .c_header = ".h",
                                .c_source = ".c",
                                .c_test   = "_test.c",
                                .shell    = ".sh",
                                .make     = ".mk",
                                .markdown = ".md",
                                .config   = ".config",
                                .testexec = ".test",
                                .service  = ".service",
                                .socket   = ".socket"
                        }
                }
        };

        return config;
}


/*
testrun_config_paths testrun_config_paths_default(){

        testrun_config_paths paths = {
                .path                   = TESTRUN_CONFIG_PATH_DUMMY,
                .include                = TESTRUN_CONFIG_PATH_INCLUDE,
                .source                    = TESTRUN_CONFIG_PATH_source,
                .tests                  = TESTRUN_CONFIG_PATH_TESTS,
                .tests_unit             = TESTRUN_CONFIG_PATH_UNIT_TESTS,
                .tests_acceptance       = TESTRUN_CONFIG_PATH_ACCEPTANCE_TESTS,
                .tests_tools            = TESTRUN_CONFIG_PATH_TEST_TOOLS,
                .config                 = TESTRUN_CONFIG_PATH_CONFIG,
                .doxygen                = TESTRUN_CONFIG_PATH_DOXYGEN,
                .docs                   = TESTRUN_CONFIG_PATH_DOCS
        };

        return paths;
}



testrun_config testrun_config_default(){

        testrun_config_paths path = testrun_config_paths_default();

        testrun_config config = {
                .name                   = "NAME",
                .author                 = "AUTHOR",
                .copyright              = "All rights reserved.",
                .copyright_source          = NULL,
                .makefile_source           = NULL,
                .path                   = path
        };

        return config;
}
*/
/*----------------------------------------------------------------------------*/

char *testrun_config_to_string(
        struct testrun_config *config){

        if (!config)
                return NULL;

        char *string = "test";

        return string;
}
