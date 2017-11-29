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

        @file           testrun_module.h
        @author         Markus Toepfer
        @date           2017-11-26

        @ingroup        testrun_lib

        @brief          Definition of module realated functionality.


        ------------------------------------------------------------------------
*/

#ifndef testrun_module_h
#define testrun_module_h

#include "testrun_copyright.h"
#include "testrun_text_block.h"
#include "testrun_config.h"
#include "testrun_path.h"

char * testrun_module_c_file_content(
        char * module_name,
        testrun_extension_t filetype,
        struct testrun_config const * const config);


char * testrun_module_testrun_header_content();

#endif /* testrun_module_h */
