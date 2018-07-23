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

        -------------------------------------------------------------------------
*//**
        @file           testrun_lib.h
        @author         Markus Toepfer
        @date           2018-07-06

        @ingroup        testrun_lib

        @brief          Definition of an abstract, configurable and modular library 
                        for C projects with a shell script based, makefile integrated 
                        testrunner infrastructure. 

        ------------------------------------------------------------------------
*/
#ifndef testrun_lib_h
#define testrun_lib_h

#include "testrun_log.h"
#include "testrun_time.h"
#include "testrun_config.h"
#include "testrun_tools.h"
#include "testrun_utils.h"

typedef struct testrun_lib testrun_lib;

struct testrun_lib {

        struct testrun_config           config;
        struct testrun_tools            tools;

        bool (*create_project_paths)    (const testrun_lib *self); // generate the folder structure of a project
        bool (*create_project_files)    (const testrun_lib *self); // generate and write all files for a project
        char *(*search_project_path)    (const char *start_path);  // search the top level project path from any given start path
        bool (*create_module_files)     (const testrun_lib *self, const char *module_name);

};

/*----------------------------------------------------------------------------*/

/**
        Create the default library including the default testrun tools. 
        @returns the default testrun_lib implementation.
*/
struct testrun_lib testrun_lib_default();

/*----------------------------------------------------------------------------*/

/**
        Validate that all function pointers, 
        as well as all required config data
        is set within any lib implementation.

        @param lib     pointer to lib implementation
*/
bool testrun_lib_validate(const testrun_lib *lib);

/*----------------------------------------------------------------------------*/

/**
        Create a new project, 
        using a provided testrun_lib implementation.

        @param lib     pointer to lib implementation
*/
bool testrun_lib_create_project(const testrun_lib *lib);

/*----------------------------------------------------------------------------*/

/**
        Create a new module, 
        using a provided testrun_lib implementation.
        This function will set the project path based on the provided 
        search function, starting with the project path set in lib.

        @param lib     pointer to lib implementation
*/
bool testrun_lib_create_module(const testrun_lib *lib, const char *module_name);

/*----------------------------------------------------------------------------*/

/**
        Create all tool scripts and testrun.h under the path configured for tools.

        @param lib     pointer to lib implementation
*/
bool testrun_lib_create_testrun_scripts(const testrun_lib *lib);

#endif /* testrun_lib_h */

