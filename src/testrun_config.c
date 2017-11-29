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
                        .path =
                        {
                                .root           = TESTRUN_PATH_DUMMY,
                                .include        = TESTRUN_PATH_INCLUDE,
                                .src            = TESTRUN_PATH_SRC,
                                .docs           = TESTRUN_PATH_DOCS,
                                .copyright      = TESTRUN_PATH_COPYRIGHT,
                                .config         = TESTRUN_PATH_CONFIG,
                                .src_to_include = TESTRUN_PATH_SRC_TO_INCLUDE,
                                .tests          =
                                {
                                        .root           = TESTRUN_PATH_TESTS,
                                        .unit           = TESTRUN_PATH_UNIT_TESTS,
                                        .acceptance     = TESTRUN_PATH_ACCEPTANCE_TESTS,
                                        .tools          = TESTRUN_PATH_TEST_TOOLS,
                                        .tests_to_src   = TESTRUN_PATH_TESTS_TO_SRC,
                                        .tests_to_tools = TESTRUN_PATH_TESTS_TO_TOOLS
                                }
                        },
                        .doxygen =
                        {
                                .path = TESTRUN_PATH_DOXYGEN,
                                .file = TESTRUN_FILE_DOXYGEN
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
                        .indent_c       = 8,
                        .offset_docu    = 3 * 8,
                        .indent_sh      = 4,
                        .prefix         =
                        {
                                .unit_test = "test_"
                        },
                        .suffix         =
                        {
                                .test_exec = "_test"
                        },
                        .extensions =
                        {
                                .c_header = ".h",
                                .c_source = ".c",
                                .c_test   = "_test.c",
                                .shell    = ".sh",
                                .make     = ".mk",
                                .markdown = ".md",
                                .config   = ".config"
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
                .src                    = TESTRUN_CONFIG_PATH_SRC,
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
                .copyright_src          = NULL,
                .makefile_src           = NULL,
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
