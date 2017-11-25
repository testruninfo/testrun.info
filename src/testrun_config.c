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
 *      @file           testrun_config.c
 *      @author         Markus Toepfer
 *      @date           2017-11-25
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Implementation of the testrun_config structure
 *
 *
 *      ------------------------------------------------------------------------
 **/

#include "../include/testrun_config.h"

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
