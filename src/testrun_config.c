/***
        ------------------------------------------------------------------------

        Copyright 2018 [COPYRIGHT_OWNER]

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
        @author         [AUTHOR]
        @date           2018-07-12

        @ingroup        testrun_lib

        @brief


        ------------------------------------------------------------------------
*/
#include "../include/testrun_config.h"

bool testrun_config_validate(struct testrun_config *config){

        // check required default parameter
        if (    !config ||
                !config->test_prefix ||
                !config->test_suffix_source ||
                !config->test_suffix_exec)
                return false;

        // check required project parameter
        if (    !config->project.name ||
                !config->project.path ||
                !config->project.search_project_path)
                return false;

        // check required path parameter
        if (    !config->path.bin       ||
                !config->path.build     ||
                !config->path.include   ||
                !config->path.source    ||
                !config->path.tests     ||
                !config->path.tools     ||
                !config->path.service   ||
                !config->path.doxygen)
                return false;

        // check required path parameter
        if (    !config->script.lines_of_code   ||
                !config->script.test_coverage   ||
                !config->script.test_unit       ||
                !config->script.test_acceptance ||
                !config->script.test_runner     ||
                !config->script.test_gcov       ||
                !config->script.test_gprof)
                return false;

         // check required path parameter
        if (    !config->makefile.configurable ||
                !config->makefile.common)
                return false;

        return true;
}       

/*----------------------------------------------------------------------------*/

struct testrun_config testrun_config_default(){

        struct testrun_config config = {

                .test_prefix            = TEST_PREFIX,
                .test_suffix_source     = TEST_SUFFIX_SOURCE,
                .test_suffix_exec       = TEST_SUFFIX_EXEC,

                .project.path           = NULL,
                .project.name           = NULL,
                .project.url            = NULL,
                .project.description    = NULL,
                .project.search_project_path = testrun_utils_search_project_path,

                .path.bin               = TESTRUN_FOLDER_BIN,
                .path.build             = TESTRUN_FOLDER_BUILD,
                .path.include           = TESTRUN_FOLDER_INCLUDE,
                .path.source            = TESTRUN_FOLDER_SOURCE,
                .path.tests             = TESTRUN_FOLDER_TESTS,
                .path.tools             = TESTRUN_FOLDER_TOOLS,
                .path.service           = TESTRUN_FOLDER_SERVICE,
                .path.doxygen           = TESTRUN_FOLDER_DOXYGEN,

                .script.lines_of_code   = TESTRUN_SCRIPT_LOC,
                .script.test_coverage   = TESTRUN_SCRIPT_COVERAGE,
                .script.test_unit       = TESTRUN_SCRIPT_UNIT,
                .script.test_acceptance = TESTRUN_SCRIPT_ACCEPTANCE,
                .script.test_runner     = TESTRUN_SCRIPT_RUNNER,
                .script.test_gcov       = TESTRUN_SCRIPT_GCOV,
                .script.test_gprof      = TESTRUN_SCRIPT_GPORF,

                .makefile.configurable  = TESTRUN_MAKEFILE,
                .makefile.common        = TESTRUN_MAKEFILE_COMMON,
                .makefile.target        = LIB
        
        };

        return config;
}

