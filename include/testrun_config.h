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

#define TESTRUN_CONFIG_PATH_DUMMY               "/tmp/test_dummy_path"
#define TESTRUN_CONFIG_PATH_INCLUDE             "include"
#define TESTRUN_CONFIG_PATH_SRC                 "src"
#define TESTRUN_CONFIG_PATH_TESTS               "tests"

#define TESTRUN_CONFIG_PATH_UNIT_TESTS          "unit"
#define TESTRUN_CONFIG_PATH_ACCEPTANCE_TESTS    "acceptance"
#define TESTRUN_CONFIG_PATH_TEST_TOOLS          "tools"

#define TESTRUN_CONFIG_PATH_CONFIG              "config"
#define TESTRUN_CONFIG_PATH_DOXYGEN             "doxygen"
#define TESTRUN_CONFIG_PATH_DOCS                "docs"

#define TESTRUN_CONFIG_FILE_DOXYGEN             "doxygen.config"
#define TESTRUN_CONFIG_FILE_CHANGELOG           "CHANGELOG.MD"
#define TESTRUN_CONFIG_FILE_README              "README.MD"
#define TESTRUN_CONFIG_FILE_COPYRIGHT           "copyright"

#define TESTRUN_CONFIG_FILE_MAKEFILE            "makefile"
#define TESTRUN_CONFIG_FILE_MAKEFILE_SOURCE     "makefile.src"
#define TESTRUN_CONFIG_FILE_MAKEFILE_TESTRUN    "makefile.testrun"

/*
 *      ------------------------------------------------------------------------
 *
 *      STRUCTURE DEFINITION
 *
 *      ...
 *
 *      ------------------------------------------------------------------------
 */

struct testrun_config_copyright{

        char *path;
        char *owner;
        char *year;
        char *note;
        char *copyright_src;
        char *copyright;

};

struct testrun_config_doxygen{

        char *path;
        char *config;
        char *logo;

};

struct testrun_config_tests{

        char *path;
        char *unit;
        char *acceptance;
        char *tools;

};



struct testrun_config_project{

        char *name;
        char *path;

        char *include;
        char *src;
        char *docs;

        struct testrun_config_doxygen doxygen;
        struct testrun_config_tests tests;

};

struct testrun_config_makefile{

        char *src;
        char *testrun;
        char *combined;

};

struct testrun_config{

        char *author;

        struct testrun_config_project project;
        struct testrun_config_makefile makefile;
        struct testrun_config_copyright copyright;

};




#endif /* testrun_config_h */
