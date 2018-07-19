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

        ------------------------------------------------------------------------
*//**
        @file           testrun_app.h
        @author         Markus Toepfer
        @date           2018-07-19

        @ingroup        testrun_lib

        @brief          Example application using the testrun_lib.

        ------------------------------------------------------------------------
*/
#ifndef testrun_app_h
#define testrun_app_h

#include "testrun_lib.h"

/*----------------------------------------------------------------------------*/

/**
        APP running loop. The app will be run as a process, until this runner
        returns. This functionality is used, to make the whole runner testable.

        @param argc     amount of command line arguments
        @param argv     vector of command line arguments

        @returns        EXIT_SUCCESS or EXIT_ERROR
*/
int testrun_app_run(int argc, char *argv[]);

#endif /* testrun_app_h */

