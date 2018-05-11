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

#define TESTRUN_URL                             "http://testrun.info"

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

#define TESTRUN_FILE_MAKEFILE                   "makefile"
#define TESTRUN_FILE_MAKEFILE_MAIN              "makefile_general.mk"

#define TESTRUN_FILE_TESTRUN_HEADER             "testrun.h"
#define TESTRUN_SCRIPT_RUNNER                   "testrun_runner.sh"
#define TESTRUN_SCRIPT_ACCEPTANCE               "testrun_simple_acceptance_tests.sh"
#define TESTRUN_SCRIPT_UNIT                     "testrun_simple_unit_tests.sh"
#define TESTRUN_SCRIPT_COVERAGE                 "testrun_simple_coverage_tests.sh"
#define TESTRUN_SCRIPT_LOC                      "testrun_simple_loc.sh"
#define TESTRUN_SCRIPT_GCOV                     "testrun_gcov.sh"
#define TESTRUN_SCRIPT_GPROF                    "testrun_gprof.sh"

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

// define key names for all struct items used in config -> to_json

#define TESTRUN_KEY_AUTHOR                      "author"
#define TESTRUN_KEY_PROJECT                     "project"
#define TESTRUN_KEY_NAME                        "name"
#define TESTRUN_KEY_URL                         "url"
#define TESTRUN_KEY_PATH                        "path"
#define TESTRUN_KEY_TYPE                        "type"
#define TESTRUN_KEY_FOLDER                      "folder"
#define TESTRUN_KEY_FILE                        "file"
#define TESTRUN_KEY_UNIT                        "unit"
#define TESTRUN_KEY_ACCEPTANCE                  "acceptance"
#define TESTRUN_KEY_TOOLS                       "tools"

#define TESTRUN_KEY_INCLUDE                     "include"
#define TESTRUN_KEY_SOURCE                      "source"
#define TESTRUN_KEY_DOCS                        "docs"
#define TESTRUN_KEY_CONFIG                      "config"
#define TESTRUN_KEY_COPYRIGHT                   "copyright"
#define TESTRUN_KEY_DOXYGEN                     "doxygen"
#define TESTRUN_KEY_TESTS                       "tests"
#define TESTRUN_KEY_TOOLS                       "tools"

#define TESTRUN_KEY_SERVICE                     "service"
#define TESTRUN_KEYE_EXEC                       "exec"
#define TESTRUN_KEY_LIB                         "lib"

#define TESTRUN_KEY_COPYRIGHT                   "copyright"
#define TESTRUN_KEY_INTRO                       "intro"
#define TESTRUN_KEY_YEAR                        "year"
#define TESTRUN_KEY_OWNER                       "owner"
#define TESTRUN_KEY_TEXT                        "text"
#define TESTRUN_KEY_NOTE                        "note"
#define TESTRUN_KEY_VARIANT                     "variant"
#define TESTRUN_KEY_PROGRAM                     "program"

#define TESTRUN_KEY_FORMAT                      "format"
#define TESTRUN_KEY_LINE_LENGTH                 "line_length"
#define TESTRUN_KEY_INDENT_C                    "indent_c"
#define TESTRUN_KEY_OFFSET_DOCU                 "offset_docu"
#define TESTRUN_KEY_PREFIX                      "prefix"
#define TESTRUN_KEY_SUFFIX                      "suffix"

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

/*----------------------------------------------------------------------------*/

struct testrun_config_format{

        size_t  line_length;            /* @param visible char length        */
        size_t  indent_c;               /* @param indent used in c files     */
        size_t  offset_docu;            /* @param offset of docu content     */

};

/*----------------------------------------------------------------------------*/

struct testrun_config_project{

        char *name;
        char *path;
        char *url;
        testrun_project_t type;
        bool doxygen;
};

/*----------------------------------------------------------------------------*/

typedef struct testrun_config testrun_config;

struct testrun_config{

        char *author;

        struct testrun_config_project project;
        struct testrun_copyright copyright;
        struct testrun_config_format format;

};

/*----------------------------------------------------------------------------*/

/**
        Create a default configuration.

        @returns        default config
*/
testrun_config testrun_config_default();

/*----------------------------------------------------------------------------*/

/**
        Dump the config to the stream.

        @param stream   dump target (e.g. stdout)
        @param config   config to be dumped
        @returns        true on dump
*/
bool testrun_config_dump(
        FILE *stream, testrun_config const * const config);

/*----------------------------------------------------------------------------*/

#endif /* testrun_config_h */
