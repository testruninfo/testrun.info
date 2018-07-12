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

        ------------------------------------------------------------------------
*//**
        @file           testrun_config.h
        @author         [AUTHOR]
        @date           2018-07-12

        @ingroup        testrun_lib

        @brief


        ------------------------------------------------------------------------
*/
#ifndef testrun_config_h
#define testrun_config_h



#define TEST_PREFIX                     "test_" 
#define TEST_SUFFIX_SOURCE              "_test" 
#define TEST_SUFFIX_EXEC                ".test" 

#define TESTRUN_FOLDER_BIN              "bin"
#define TESTRUN_FOLDER_BUILD            "build"
#define TESTRUN_FOLDER_INCLUDE          "include"
#define TESTRUN_FOLDER_SOURCE           "src"
#define TESTRUN_FOLDER_TESTS            "tests"
#define TESTRUN_FOLDER_TOOLS            "tests/tools"
#define TESTRUN_FOLDER_SERVICE          "config"
#define TESTRUN_FOLDER_DOXYGEN          "doxygen"

#define TESTRUN_SCRIPT_LOC              "testrun_simple_loc.sh"
#define TESTRUN_SCRIPT_COVERAGE         "testrun_simple_coverage_tests.sh"      
#define TESTRUN_SCRIPT_UNIT             "testrun_simple_unit_tests.sh"      
#define TESTRUN_SCRIPT_ACCEPTANCE       "testrun_simple_acceptance_tests.sh"     
#define TESTRUN_SCRIPT_RUNNER           "testrun_runner.sh"   
#define TESTRUN_SCRIPT_GCOV             "testrun_gcov.sh"
#define TESTRUN_SCRIPT_GPORF            "testrun_gprof.sh" 

#define TESTRUN_MAKEFILE                "makefile"
#define TESTRUN_MAKEFILE_COMMON         "makefile_common.mk" 

typedef struct testrun_config testrun_config;

#include "testrun_utils.h"
#include "testrun_tools.h"

/*----------------------------------------------------------------------------*/

/**
        Project realted data.
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
        char *include;          // relative path project to header files 
        char *source;           // relative path project to source files
        char *tests;            // relative path project to test files
        char *tools;            // relative path project to test tool scripts
        char *service;          // relative path project to service files
        char *doxygen;          // relative path project to doxygen files

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

struct testrun_config {

        char *test_prefix;         // prefix function      <-> test_function
        char *test_suffix_source;  // suffix file.c        <-> file_tests.c
        char *test_suffix_exec;    // suffix file_tests.c  <-> file_tests.exec
        
        char *cflags;              // (optional standard cflags)
        char *version;             // (optional version to start)

        struct testrun_project_config   project;
        struct testrun_path_config      path;
        struct testrun_script_config    script;
        struct testrun_makefile_config  makefile;

};

/*----------------------------------------------------------------------------*/

/**
        Validate all required parameter of a config are set.
*/
bool testrun_config_validate(struct testrun_config *config);

/*----------------------------------------------------------------------------*/

/**
        Create the default config of libtestrun.
*/
struct testrun_config testrun_config_default();


#endif /* testrun_config_h */

