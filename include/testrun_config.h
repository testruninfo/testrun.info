/***
        ------------------------------------------------------------------------

        Copyright 2018 Markus Toepfer

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
        @date           2018-07-12

        @ingroup        testrun_lib

        @brief          Definition of all configurable input items to the 
                        testrun project creation. 

        ------------------------------------------------------------------------
*/
#ifndef testrun_config_h
#define testrun_config_h

#define TEST_PREFIX                     "test_" 
#define TEST_SUFFIX_SOURCE              "_test" 
#define TEST_SUFFIX_EXEC                ".test" 
#define TESTRUN_SUFFIX_C_HEADER         ".h" 
#define TESTRUN_SUFFIX_C_SOURCE         ".c" 

#define TESTRUN_FOLDER_BIN              "bin"
#define TESTRUN_FOLDER_BUILD            "build"
#define TESTRUN_FOLDER_COPYRIGHT        "copyright"
#define TESTRUN_FOLDER_DOCS             "docs"
#define TESTRUN_FOLDER_INCLUDE          "include"
#define TESTRUN_FOLDER_SOURCE           "src"
#define TESTRUN_FOLDER_TESTS            "tests"
#define TESTRUN_FOLDER_TOOLS            "tests/tools"
#define TESTRUN_FOLDER_SERVICE          "config"
#define TESTRUN_FOLDER_DOXYGEN          "doxygen"
#define TESTRUN_FOLDER_RESOURCES        "resources"

#define TESTRUN_FOLDER_UNIT_TESTS       "unit"
#define TESTRUN_FOLDER_ACCEPTANCE_TESTS "acceptance"

#define TESTRUN_SCRIPT_LOC              "testrun_simple_loc.sh"
#define TESTRUN_SCRIPT_COVERAGE         "testrun_simple_coverage_tests.sh"      
#define TESTRUN_SCRIPT_UNIT             "testrun_simple_unit_tests.sh"      
#define TESTRUN_SCRIPT_ACCEPTANCE       "testrun_simple_acceptance_tests.sh"     
#define TESTRUN_SCRIPT_RUNNER           "testrun_runner.sh"   
#define TESTRUN_SCRIPT_GCOV             "testrun_gcov.sh"
#define TESTRUN_SCRIPT_GPORF            "testrun_gprof.sh" 

#define TESTRUN_MAKEFILE                "makefile"
#define TESTRUN_MAKEFILE_COMMON         "makefile_common.mk" 

#define TESTRUN_APP_DEFAULT             "testrun.info"
#define TESTRUN_WWW_DEFAULT             "www.testrun.info"
#define TESTRUN_NOTE_DEFAULT            "This file is part of the testrun project. http://testrun.info"

#define TESTRUN_TAG_NAME                "[NAME]"
#define TESTRUN_TAG_COPYRIGHT           "[COPYRIGHT]"
#define TESTRUN_TAG_AUTHOR              "[AUTHOR]"
#define TESTRUN_TAG_OWNER               "[OWNER]"
#define TESTRUN_TAG_GROUP               "[GROUP]"
#define TESTRUN_TAG_VERSION             "0.0.1"

#define TESTRUN_FILE_GIT_IGNORE         ".gitignore"
#define TESTRUN_FILE_README             "README.md"
#define TESTRUN_FILE_CHANGELOG          "CHANGELOG.md"
#define TESTRUN_FILE_DOXYGEN            "doxygen.config"
#define TESTRUN_FILE_COPYRIGHT          "copyright"
#define TESTRUN_FILE_COPYRIGHT_FULL     "license.txt"

#define TESTRUN_FILE_SERVICE_EXT        ".service"
#define TESTRUN_FILE_SOCKET_EXT         ".socket"

#define TESTRUN_DEFAULT_INSTALL_PATH    "/usr/local/bin"
#define TESTRUN_DEFAULT_TESTS_TO_ROOT   "../../"
#define TESTRUN_DEFAULT_TO_ROOT         "../"

#define TESTRUN_DEFAULT_INDENT          4
#define TESTRUN_DEFAULT_C_INDENT        8
#define TESTRUN_DEFAULT_SHELL_INDENT    8

#define TESTRUN_CFLAGS                  "-std=c11 -D _DEFAULT_SOURCE -D _POSIX_C_SOURCE=200809"

#define TESTRUN_COPYRIGHT_PREFIX "/***\n" \
"        ------------------------------------------------------------------------\n" \
"\n"

#define TESTRUN_COPYRIGHT_SUFFIX "\n" \
"        ------------------------------------------------------------------------\n" \
"*/"

#define TESTRUN_DOCUMENTATION_PREFIX "/**\n"

#define TESTRUN_DOCUMENTATION_SUFFIX "\n" \
"        ------------------------------------------------------------------------\n" \
"*/"


typedef struct testrun_config testrun_config;

#include "testrun_copyright.h"
#include "testrun_utils.h"
#include "testrun_tools.h"

#include <getopt.h>
#include <unistd.h>
#include <libgen.h>

#define TESTRUN_APP_VERSION "1.0.0"

#define testrun_config_print_version() \
        fprintf(stdout, "version: %s\n", TESTRUN_APP_VERSION);

/*----------------------------------------------------------------------------*/

/**
        Format realated data.
*/
struct testrun_indent_config {

        size_t standard;
        size_t c;
        size_t script;

};

/*----------------------------------------------------------------------------*/

/**
        Project realated data.
*/
struct testrun_project_config {

        char *name;             // name of the project     
        char *path;             // path to the project
        char *url;              // project url
        char *description;      // short project description for pkgconfig

        char *(*search_project_path) (const char* start_path);

};

/*----------------------------------------------------------------------------*/

/**
        Project internal relative folder structure

*/
struct testrun_path_config {

        char *bin;              // relative path project to bin files
        char *build;            // relative path project to build files
        char *copyright;        // relative path project to copyright files
        char *docs;             // relative path project to doc files
        char *include;          // relative path project to header files 
        char *source;           // relative path project to source files
        char *tests;            // relative path project to test files
        char *tools;            // relative path project to test tool scripts
        char *service;          // relative path project to service files
        char *doxygen;          // relative path project to doxygen files

        char *source_to_root;           // relative up source to root
        char *include_to_root;          // relative up include to root
        char *unit_tests_to_root;       // relatvie up unit_tests to root

};

/*----------------------------------------------------------------------------*/

/**
        Name configuration for the testrun scripts.

*/
struct testrun_script_config {

        char *lines_of_code;   
        char *test_coverage;
        char *test_unit;
        char *test_acceptance;
        char *test_runner;
        char *test_gcov;
        char *test_gprof;

};

/*----------------------------------------------------------------------------*/

/**
        Name configuration for makefiles

*/
struct testrun_makefile_config {

        char *configurable;   
        char *common;
        testrun_makefile_target target;

};

/*----------------------------------------------------------------------------*/

/**
        Copyright parameter configuration

*/
struct testrun_copyright_config {

        char *author;   
        char *owner;
        char *note;

        struct testrun_copyright copyright;
        struct testrun_copyright_gpl_v3_parameter *gpl_parameter;

};



/*----------------------------------------------------------------------------*/

struct testrun_config {

        char *test_prefix;         // prefix function      <-> test_function
        char *test_suffix_source;  // suffix file.c        <-> file_tests.c
        char *test_suffix_exec;    // suffix file_tests.c  <-> file_tests.exec
        
        char *cflags;              // (optional standard cflags)
        char *version;             // (optional version to start)

        struct testrun_indent_config    indent;
        struct testrun_copyright_config copyright;
        struct testrun_project_config   project;
        struct testrun_path_config      path;
        struct testrun_script_config    script;
        struct testrun_makefile_config  makefile;

};

/*----------------------------------------------------------------------------*/

/**
        Validate all required parameter of a config are set.
*/
bool testrun_config_validate(const struct testrun_config *config);

/*----------------------------------------------------------------------------*/

/**
        Create the default config of libtestrun.
*/
struct testrun_config testrun_config_default();

/*----------------------------------------------------------------------------*/

/**
        Default standard config reader for user based configuration input.
*/
struct testrun_config testrun_config_read_user_input(
        const char *app_name, bool *success, bool *project, int argc, char *argv[]);


#endif /* testrun_config_h */

