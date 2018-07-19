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
        @file           testrun_lib.c
        @author         Markus Toepfer
        @date           2018-07-06

        @ingroup        testrun_lib

        @brief          Top level implementation of the library.


        ------------------------------------------------------------------------
*/
#include "../include/testrun_lib.h"

bool testrun_lib_create_project_paths(const testrun_lib *lib);
bool testrun_lib_create_project_files(const testrun_lib *lib);

struct testrun_lib testrun_lib_default(){

        struct testrun_lib lib = {

                .config = testrun_config_default(),
                .tools  = testrun_tools_default(),

                .create_project_paths   = testrun_lib_create_project_paths,
                .create_project_files   = testrun_lib_create_project_files

        };

        return lib;
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_validate(const testrun_lib *self){

        if (!self)
                return false;

        // check config
        if (!testrun_config_validate(&self->config))
                return false;

        // check create functions
        if (    !self->create_project_paths ||
                !self->create_project_files)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_project_paths(const testrun_lib *lib){

        if (!lib)
                return false;

        return testrun_utils_create_project_paths(&lib->config);
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_project_files(const testrun_lib *lib){

        if (!lib)
                return false;

        return testrun_utils_create_project_paths(&lib->config);
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_project(const testrun_lib *lib){

        if (!testrun_lib_validate(lib))
                goto error;

        fprintf(stdout, "START creating a new project %s\n",
                lib->config.project.name);

        if (!lib->create_project_paths(lib))
                return false;

        if (!lib->create_project_files(lib))
                return false;

        return true;
error:
        log_error("Failed to create a new project.");
        return false;
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_module(const testrun_lib *lib){

        if (!testrun_lib_validate(lib))
                return false;

        return false;
}