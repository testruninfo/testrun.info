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
        @file           testrun_utils.h
        @author         [AUTHOR]
        @date           2018-07-09

        @ingroup        testrun_lib

        @brief


        ------------------------------------------------------------------------
*/
#ifndef testrun_utils_h
#define testrun_utils_h

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include <ctype.h>

#include <dirent.h>
#include <unistd.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "testrun_config.h"
#include "testrun_log.h"

/**
        Create an allocated string with the content of text, 
        where each line of text is prefixed and suffixed with
        optional strings e.g. indentitation.

        @NOTE   if the content of the line is 0, 
                prefix and suffix will not be added.
                e.g. \n\n will become \n\n
        
        @param text     source text
        @param intro    (optional) intro to be added
        @param outro    (optional) outro to be added

        @returns        allocated string with new content or NULL
*/
char *testrun_utils_insert_at_each_line(
        const char* text,
        const char* intro,
        const char* outro);

/*----------------------------------------------------------------------------*/

/**
        Default function to search a project top level path, 
        using the default config. 
*/
char* testrun_utils_search_project_path(const char *start_path);

/*----------------------------------------------------------------------------*/

/**
        Default function to search a project top level path, 
        using the default config. 
*/
bool testrun_utils_create_paths(struct testrun_config *config);



#endif /* testrun_utils_h */

