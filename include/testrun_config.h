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
#define TESTRUN_PATH_INCLUDE                    "include"
#define TESTRUN_PATH_SRC                        "src"
#define TESTRUN_PATH_TESTS                      "tests"

#define TESTRUN_PATH_SRC_TO_INCLUDE             ".."
#define TESTRUN_PATH_TESTS_TO_SRC               ".."
#define TESTRUN_PATH_TESTS_TO_TOOLS             "."
#define TESTRUN_PATH_UNIT_TESTS                 "unit"
#define TESTRUN_PATH_ACCEPTANCE_TESTS           "acceptance"
#define TESTRUN_PATH_TEST_TOOLS                 "tools"

#define TESTRUN_PATH_CONFIG                     "config"
#define TESTRUN_PATH_DOXYGEN                    "doxygen"
#define TESTRUN_PATH_DOCS                       "docs"
#define TESTRUN_PATH_COPYRIGHT                  "copyright"

#define TESTRUN_FILE_DOXYGEN                    "doxygen.config"
#define TESTRUN_FILE_CHANGELOG                  "CHANGELOG.MD"
#define TESTRUN_FILE_README                     "README.MD"
#define TESTRUN_FILE_COPYRIGHT                  "copyright"

#define TESTRUN_FILE_MAKEFILE                   "makefile"
#define TESTRUN_FILE_MAKEFILE_SOURCE            "makefile.src"
#define TESTRUN_FILE_MAKEFILE_TESTRUN           "makefile.testrun"

#define TESTRUN_FILE_TESTRUN_HEADER             "testrun.h"

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

typedef enum testrun_extension {
        TESTRUN_HEADER,
        TESTRUN_SOURCE,
        TESTRUN_TEST,
        TESTRUN_SHELL,
        TESTRUN_MAKE,
        TESTRUN_MARKDOWN,
        TESTRUN_CONFIG } testrun_extension_t;
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
};

/*----------------------------------------------------------------------------*/

struct testrun_config_prefix{

        char *unit_test;
};

/*----------------------------------------------------------------------------*/

struct testrun_config_suffix{

        char *test_exec;
};

/*----------------------------------------------------------------------------*/

struct testrun_config_format{

        char    *line_end;              /* @param linebreak defintion        */
        size_t  line_length;            /* @param visible char length        */
        size_t  indent_c;               /* @param indent used in c files     */
        size_t  indent_sh;              /* @param indent used in shell files */
        size_t  offset_docu;            /* @param offset of docu content     */

        struct testrun_config_prefix prefix;
        struct testrun_config_suffix suffix;
        struct testrun_config_file_extensions extensions;

};

/*----------------------------------------------------------------------------*/

struct testrun_config_doxygen{

        char *path;
        char *file;

};

/*----------------------------------------------------------------------------*/

struct testrun_config_path_tests{

        char *root;
        char *unit;
        char *acceptance;
        char *tools;
        char *tests_to_src;
        char *tests_to_tools;

};

/*----------------------------------------------------------------------------*/

struct testrun_config_path{

        char *root;
        char *include;
        char *src;
        char *docs;
        char *copyright;
        char *config;
        char *src_to_include;

        struct testrun_config_path_tests tests;

};

/*----------------------------------------------------------------------------*/

struct testrun_config_project{

        char *name;

        struct testrun_config_path path;
        struct testrun_config_doxygen doxygen;

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
