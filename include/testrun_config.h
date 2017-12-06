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

        @file           testrun_config.h
        @author         Markus Toepfer
        @date           2017-11-25

        @ingroup        testrun_lib

        @brief          Definition of a testrun_config


        ------------------------------------------------------------------------
*/

#ifndef testrun_config_h
#define testrun_config_h

#include "testrun_string.h"
#include "testrun_copyright.h"

#define TESTRUN_PATH_DUMMY                      "/tmp/test_dummy_path"

#define TESTRUN_PATH_LEVEL_UP                   ".."
#define TESTRUN_PATH_LEVEL_DOWN                 "."

#define TESTRUN_FOLDER_CONFIG                   "config"
#define TESTRUN_FOLDER_DOXYGEN                  "doxygen"
#define TESTRUN_FOLDER_DOCS                     "docs"
#define TESTRUN_FOLDER_COPYRIGHT                "copyright"
#define TESTRUN_FOLDER_INCLUDE                  "include"
#define TESTRUN_FOLDER_SOURCE                   "src"
#define TESTRUN_FOLDER_INSTALL                  "install"
#define TESTRUN_FOLDER_TESTS                    "tests"
#define TESTRUN_FOLDER_TESTS_UNIT               "unit"
#define TESTRUN_FOLDER_TESTS_ACCEPTANCE         "acceptance"
#define TESTRUN_FOLDER_TESTS_TOOLS              "tools"
#define TESTRUN_FOLDER_RESOURCES                "resources"
#define TESTRUN_FOLDER_SERVICE_DATA             "data"

#define TESTRUN_FILE_DOXYGEN                    "doxygen.config"
#define TESTRUN_FILE_CHANGELOG                  "CHANGELOG.MD"
#define TESTRUN_FILE_README                     "README.MD"
#define TESTRUN_FILE_COPYRIGHT                  "copyright"

#define TESTRUN_FILE_SERVICE_INSTALL            "install.sh"
#define TESTRUN_FILE_SERVICE_UNINSTALL          "uninstall.sh"

#define TESTRUN_SERVICE_EXTENSION               ".service"
#define TESTRUN_SOCKET_EXTENSION                ".socket"

#define TESTRUN_FILE_MAKEFILE                   "makefile"
#define TESTRUN_FILE_MAKEFILE_MAIN              "makefile.main"
#define TESTRUN_FILE_MAKEFILE_TEST              "makefile.test"

#define TESTRUN_FILE_TESTRUN_HEADER             "testrun.h"
#define TESTRUN_SCRIPT_RUNNER                   "testrun_runner.sh"
#define TESTRUN_SCRIPT_ACCEPTANCE               "testrun_simple_acceptance_tests.sh"
#define TESTRUN_SCRIPT_UNIT                     "testrun_simple_unit_tests.sh"
#define TESTRUN_SCRIPT_COVERAGE                 "testrun_simple_coverage_tests.sh"
#define TESTRUN_SCRIPT_LOC                      "testrun_simple_loc.sh"

#define TESTRUN_TAG_FILE                        "@file"
#define TESTRUN_TAG_AUTHOR                      "@author"
#define TESTRUN_TAG_DATE                        "@date"
#define TESTRUN_TAG_GROUP                       "@ingroup"
#define TESTRUN_TAG_BRIEF                       "@brief"

#define TESTRUN_TAG_DEFAULT_NAME                "[NAME]"
#define TESTRUN_TAG_DEFAULT_TAG                 "[TAG]"
#define TESTRUN_TAG_DEFAULT_MODULE              "[MODULE]"
#define TESTRUN_TAG_DEFAULT_AUTHOR              "[AUTHOR]"
#define TESTRUN_TAG_DEFAULT_DATE                "[DATE]"
#define TESTRUN_TAG_DEFAULT_PROJECT             "[PROJECT]"
#define TESTRUN_TAG_DEFAULT_YEAR                "[YEAR]"
#define TESTRUN_TAG_DEFAULT_OWNER               "[OWNER]"

#define TESTRUN_TAG_OFFSET                      "[OFFSET]"
#define TESTRUN_PATH_SPLIT                      "/"

typedef enum testrun_extension {
        TESTRUN_HEADER,
        TESTRUN_SOURCE,
        TESTRUN_TEST,
        TESTRUN_SHELL,
        TESTRUN_MAKE,
        TESTRUN_MARKDOWN,
        TESTRUN_CONFIG } testrun_extension_t;

typedef enum testrun_project_type {
        TESTRUN_LIB,
        TESTRUN_EXEC,
        TESTRUN_SERVICE} testrun_project_t;

/*
 *      ------------------------------------------------------------------------
 *
 *      STRUCTURE DEFINITION
 *
 *      ...
 *
 *      ------------------------------------------------------------------------
 */

struct testrun_config_file_extensions{

        char *c_header;
        char *c_source;
        char *c_test;
        char *shell;
        char *make;
        char *markdown;
        char *config;
        char *testexec;
        char *service;
        char *socket;
};


/*----------------------------------------------------------------------------*/

struct testrun_config_prefix{

        char *unit_test;
};

/*----------------------------------------------------------------------------*/

struct testrun_config_suffix{

        char *tests_source;
        char *tests_source_omp;
};

/*----------------------------------------------------------------------------*/

struct testrun_config_format{

        char    *line_end;              /* @param linebreak defintion        */
        size_t  line_length;            /* @param visible char length        */
        char    *path_split;            /* @param format for path split      */
        size_t  indent_c;               /* @param indent used in c files     */
        size_t  indent_sh;              /* @param indent used in shell files */
        size_t  offset_docu;            /* @param offset of docu content     */

        struct testrun_config_prefix prefix;
        struct testrun_config_suffix suffix;
        struct testrun_config_file_extensions extensions;

};

/*----------------------------------------------------------------------------*/

struct testrun_config_doxygen{

        char *folder;
        char *file;

};

/*----------------------------------------------------------------------------*/

struct testrun_config_test_tools{

        char *folder;
        char *to_tests;

        char *header;
        char *runner_script;
        char *acceptance_script;
        char *unit_script;
        char *coverage_script;
        char *loc_script;
};

/*----------------------------------------------------------------------------*/

struct testrun_config_path_tests{

        char *folder;
        char *unit;
        char *acceptance;

        char *to_project;
        char *to_tools;

        struct testrun_config_test_tools tools;

};

/*----------------------------------------------------------------------------*/

struct testrun_config_path{

        char *name;

        char *include;
        char *source;
        char *docs;
        char *copyright;
        char *config;

        char *to_include;
        char *to_source;
        char *to_docs;
        char *to_copyright;
        char *to_config;

        char *to_doxygen;
        char *to_tests;

        char *from_include;
        char *from_source;
        char *from_docs;
        char *from_copyright;
        char *from_config;
        char *from_doxygen;
        char *from_tests;

        struct testrun_config_path_tests tests;

};

struct testrun_config_service{

        char *folder;
        char *install_script;
        char *uninstall_script;
        char *config_data;

        char *listenStream;
        char *listenData;
};

/*----------------------------------------------------------------------------*/

struct testrun_config_project{

        char *name;
        testrun_project_t type;

        struct testrun_config_path path;
        struct testrun_config_doxygen doxygen;
        struct testrun_config_service service;

};

/*----------------------------------------------------------------------------*/

typedef struct testrun_config testrun_config;

struct testrun_config{

        char *author;

        struct testrun_config_project project;
        struct testrun_copyright copyright;
        struct testrun_config_format format;

};

testrun_config testrun_config_default();

#endif /* testrun_config_h */
