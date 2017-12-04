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

        @file           testrun_path.c
        @author         Markus Toepfer
        @date           2017-11-25

        @ingroup        testrun_lib

        @brief          Implementation of path related functionality


        ------------------------------------------------------------------------
*/

#include "../include/testrun_path.h"

/*----------------------------------------------------------------------------*/

bool testrun_path_is_project_top_dir(
        char *path){

        if (!path)
                return false;

        bool include    = false;
        bool source     = false;
        bool tests      = false;

        DIR *dp;
        struct dirent *ep;

        dp = opendir(path);
        if (!dp)
                return false;

        while((ep = readdir (dp))) {
                if (ep->d_type == DT_DIR) {

                        if (!include) {
                                if (strncmp(ep->d_name,
                                        TESTRUN_FOLDER_INCLUDE,
                                        strlen(TESTRUN_FOLDER_INCLUDE))
                                        == 0) {
                                        include = true;
                                        continue;
                                }
                        }

                        if (!source) {
                                if (strncmp(ep->d_name,
                                        TESTRUN_FOLDER_SOURCE,
                                        strlen(TESTRUN_FOLDER_SOURCE))
                                        == 0) {
                                        source = true;
                                        continue;
                                }
                        }

                        if (!tests) {
                                if (strncmp(ep->d_name,
                                        TESTRUN_FOLDER_TESTS,
                                        strlen(TESTRUN_FOLDER_TESTS))
                                        == 0) {
                                        tests = true;
                                        continue;
                                }
                        }
                }
        }
        (void) closedir (dp);

    return (include && source && tests);
}

/*----------------------------------------------------------------------------*/

char *testrun_path_search_project_path(
        char *input){

        char *parent  = NULL;
        char *current = NULL;

        if (!input)
                return NULL;

        char path[PATH_MAX];

        current = realpath(input, current);
        if (!current)
                goto error;

        if (!testrun_path_is_project_top_dir(current)){

                /* walk up one level */
                sprintf(path, "%s%s%s%s",
                        current,
                        TESTRUN_PATH_SPLIT,
                        "..",
                        TESTRUN_PATH_SPLIT);

                parent = realpath(path, parent);
                if (!parent)
                        goto error;

                if (strcmp(current, parent) == 0) {

                        goto error;

                } else {

                        free(current);
                        current = testrun_path_search_project_path(
                                parent);

                        free(parent);
                        parent = NULL;

                }
        }

        return current;

error:
        if (current)
                free(current);
        if (parent)
                free(parent);
        return NULL;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_source_to_include(
        char *buffer, size_t max,
        struct testrun_config const * const config,
        char * module_name){

        if (!buffer || !config || !module_name )
                return false;

        /**
         *      default LINUX   ".././include/module_name.h
         *
         *      step1 - move up to project
         *      step2 - move down to include
         *      step3 - add include foldername
         *      step4 - add filename for the module.
         */

        if (max < (     strlen(module_name) +
                        strlen(config->format.extensions.c_header) +
                        strlen(config->project.path.from_source) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.to_include) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.include) +
                        strlen(config->format.path_split) +
                        1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s%s%s%s%s%s",
                config->project.path.from_source,
                config->format.path_split,
                config->project.path.to_include,
                config->format.path_split,
                config->project.path.include,
                config->format.path_split,
                module_name,
                config->format.extensions.c_header) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_test_to_source(
        char *buffer, size_t max,
        struct testrun_config const * const config,
        char * module_name){

        if (!buffer || !config || !module_name )
                return false;

        /**
         *      default LINUX   "../../source/module_name.h
         *
         *      step1 - move up to tests
         *      step2 - move up to project
         *      step3 - move to source
         *      step5 - add source foldername
         *      step6 - add filename for the module.
         */

        if (max < (     strlen(module_name) +
                        strlen(config->format.extensions.c_source) +
                        2 +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.from_tests) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.to_source) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.source) +
                        strlen(config->format.path_split) +
                        1 ))
                return false;

        if (snprintf(buffer, max, "..%s%s%s%s%s%s%s%s%s",
                config->format.path_split,
                config->project.path.from_tests,
                config->format.path_split,
                config->project.path.to_source,
                config->format.path_split,
                config->project.path.source,
                config->format.path_split,
                module_name,
                config->format.extensions.c_source) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_test_to_testrun_header(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config)
                return false;

        if (max < (     strlen(config->project.path.tests.tools.header) +
                        2 +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.to_tools) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.tools.name) +
                        strlen(config->format.path_split) +
                        1 ))
                return false;

        if (snprintf(buffer, max, "..%s%s%s%s%s%s",
                config->format.path_split,
                config->project.path.tests.to_tools,
                config->format.path_split,
                config->project.path.tests.tools.name,
                config->format.path_split,
                config->project.path.tests.tools.header) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_project_to_tools(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        /**
         *      default LINUX   "../../tests/tools
         *
         *      step1 - move up to project
         *      step2 - move down to include
         *      step3 - add include foldername
         *      step4 - add filename for the module.
         */

        if (max < (     strlen(config->project.path.to_tests) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.name) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.to_tools) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.tools.name) +
                        1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s%s%s%s%s",
                config->project.path.to_tests,
                config->format.path_split,
                config->project.path.tests.name,
                config->format.path_split,
                config->project.path.tests.to_tools,
                config->format.path_split,
                config->project.path.tests.tools.name) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_project_to_source(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        /**
         *      default LINUX   "./src"
         */

        if (max < (     strlen(config->project.path.to_source) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.source)  +
                        1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                config->project.path.to_source,
                config->format.path_split,
                config->project.path.source) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_project_to_include(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        /**
         *      default LINUX   "./include"
         */

        if (max < (     strlen(config->project.path.to_include) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.include)  +
                        1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                config->project.path.to_include,
                config->format.path_split,
                config->project.path.include) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_project_to_docs(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        /**
         *      default LINUX   "./docs"
         */

        if (max < (     strlen(config->project.path.to_docs) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.docs)  +
                        1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                config->project.path.to_docs,
                config->format.path_split,
                config->project.path.docs) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_project_to_doxygen(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        /**
         *      default LINUX   "./docs"
         */

        if (max < (     strlen(config->project.path.to_doxygen) +
                        strlen(config->format.path_split) +
                        strlen(config->project.doxygen.foldername)  +
                        1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                config->project.path.to_doxygen,
                config->format.path_split,
                config->project.doxygen.foldername) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_project_to_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        /**
         *      default LINUX   "./tests"
         */

        if (max < (     strlen(config->project.path.to_tests) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.name)  +
                        1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                config->project.path.to_tests,
                config->format.path_split,
                config->project.path.tests.name) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_project_to_copyright(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        /**
         *      default LINUX   "./copyright"
         */

        if (max < (     strlen(config->project.path.to_copyright) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.copyright)  +
                        1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                config->project.path.to_copyright,
                config->format.path_split,
                config->project.path.copyright) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_project_to_config(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        /**
         *      default LINUX   "./config"
         */

        if (max < (     strlen(config->project.path.to_config) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.config)  +
                        1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                config->project.path.to_config,
                config->format.path_split,
                config->project.path.config) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_project_to_unit_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        /**
         *      default LINUX   "./tests/unit"
         */

        if (max < (     strlen(config->project.path.to_tests) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.name)  +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.unit)  +
                        1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s%s%s",
                config->project.path.to_tests,
                config->format.path_split,
                config->project.path.tests.name,
                config->format.path_split,
                config->project.path.tests.unit) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_project_to_acceptance_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        /**
         *      default LINUX   "./tests/acceptance"
         */

        if (max < (     strlen(config->project.path.to_tests) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.name)  +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.acceptance)  +
                        1 ))
                return false;

        if (snprintf(buffer, max, "%s%s%s%s%s",
                config->project.path.to_tests,
                config->format.path_split,
                config->project.path.tests.name,
                config->format.path_split,
                config->project.path.tests.acceptance) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_script_unit_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        char temp[max];
        bzero(temp, max);

        if (!testrun_path_project_to_tools(temp, max, config))
                return false;

        if (max <  1 +  strlen(temp) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.tools.unit_script))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                temp,
                config->format.path_split,
                config->project.path.tests.tools.unit_script) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_script_acceptance_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        char temp[max];
        bzero(temp, max);

        if (!testrun_path_project_to_tools(temp, max, config))
                return false;

        if (max <  1 +  strlen(temp) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.tools.acceptance_script))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                temp,
                config->format.path_split,
                config->project.path.tests.tools.acceptance_script) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_script_coverage_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        char temp[max];
        bzero(temp, max);

        if (!testrun_path_project_to_tools(temp, max, config))
                return false;

        if (max <  1 +  strlen(temp) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.tools.coverage_script))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                temp,
                config->format.path_split,
                config->project.path.tests.tools.coverage_script) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_script_loc_tests(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        char temp[max];
        bzero(temp, max);

        if (!testrun_path_project_to_tools(temp, max, config))
                return false;

        if (max <  1 +  strlen(temp) +
                        strlen(config->format.path_split) +
                        strlen(config->project.path.tests.tools.loc_script))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                temp,
                config->format.path_split,
                config->project.path.tests.tools.loc_script) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_script_service_install(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        char temp[max];
        bzero(temp, max);

        if (!testrun_path_project_to_config(temp, max, config))
                return false;

        if (max <  1 +  strlen(temp) +
                        strlen(config->format.path_split) +
                        strlen(config->project.service.folder) +
                        strlen(config->format.path_split) +
                        strlen(config->project.service.install_script))
                return false;

        if (snprintf(buffer, max, "%s%s%s%s%s",
                temp,
                config->format.path_split,
                config->project.service.folder,
                config->format.path_split,
                config->project.service.install_script) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_script_service_service_definition(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        char temp[max];
        bzero(temp, max);

        if (!testrun_path_project_to_config(temp, max, config))
                return false;

        if (max <  1 +  strlen(temp) +
                        strlen(config->format.path_split) +
                        strlen(config->project.service.folder) +
                        strlen(config->format.path_split) +
                        strlen(config->project.name) +
                        strlen(".service"))
                return false;

        if (snprintf(buffer, max, "%s%s%s%s%s%s",
                temp,
                config->format.path_split,
                config->project.service.folder,
                config->format.path_split,
                config->project.name,
                ".service") < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_script_service_socket_definition(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        char temp[max];
        bzero(temp, max);

        if (!testrun_path_project_to_config(temp, max, config))
                return false;

        if (max <  1 +  strlen(temp) +
                        strlen(config->format.path_split) +
                        strlen(config->project.service.folder) +
                        strlen(config->format.path_split) +
                        strlen(config->project.name) +
                        strlen(".socket"))
                return false;

        if (snprintf(buffer, max, "%s%s%s%s%s%s",
                temp,
                config->format.path_split,
                config->project.service.folder,
                config->format.path_split,
                config->project.name,
                ".socket") < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_script_service_uninstall(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        char temp[max];
        bzero(temp, max);

        if (!testrun_path_project_to_config(temp, max, config))
                return false;

        if (max <  1 +  strlen(temp) +
                        strlen(config->format.path_split) +
                        strlen(config->project.service.folder) +
                        strlen(config->format.path_split) +
                        strlen(config->project.service.uninstall_script))
                return false;

        if (snprintf(buffer, max, "%s%s%s%s%s",
                temp,
                config->format.path_split,
                config->project.service.folder,
                config->format.path_split,
                config->project.service.uninstall_script) < 0)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_path_doxygen_config(
        char *buffer, size_t max,
        struct testrun_config const * const config){

        if (!buffer || !config )
                return false;

        char temp[max];
        bzero(temp, max);

        if (!testrun_path_project_to_doxygen(temp, max, config))
                return false;

        if (max <  1 +  strlen(temp) +
                        strlen(config->format.path_split) +
                        strlen(config->project.doxygen.filename))
                return false;

        if (snprintf(buffer, max, "%s%s%s",
                temp,
                config->format.path_split,
                config->project.doxygen.filename) < 0)
                return false;

        return true;
}
