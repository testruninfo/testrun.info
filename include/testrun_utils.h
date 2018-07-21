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
        @file           testrun_utils.h
        @author         Markus Toepfer
        @date           2018-07-09

        @ingroup        testrun_lib

        @brief          Definition of supporting utilities for testrun_lib 
                        development. 

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
        Generate the project root path.
*/
bool testrun_utils_generate_project_root_path(
        const struct testrun_config *config,
        char *buffer,
        size_t size);

/*----------------------------------------------------------------------------*/

/**
        Default function to search a project top level path, 
        using the default config. 
*/
char* testrun_utils_search_project_path(const char *start_path);

/*----------------------------------------------------------------------------*/

/**
        Default function to create all configured paths for the project.
*/
bool testrun_utils_create_project_paths(const struct testrun_config *config);


/*----------------------------------------------------------------------------*/

/**
        Try to read the git author to buffer.
        @param buffer   buffer to write the author name
        @param size     size of the buffer.
*/ 
bool testrun_utils_get_git_author(
        char *buffer, size_t size);

/*----------------------------------------------------------------------------*/

/**
        Write some content to a file. 
        The file at the resulting path MUST not exist.

        Will create "root_path/realtive_path/filename" with content as file content.

        @param filename         name of the file to be created
        @param content          content to be written to the file
        @param root_path        (MANDATORY) root_path to write to 
                                (may be relative to working path of executable)  
        @param relative_path    (OPTIONAL) relative_path to root_path (e.g. subpath)
*/
bool testrun_utils_create_file(
        const char *filename,
        const char *content,
        const char *root_path,
        const char *relative_path);

/*----------------------------------------------------------------------------*/

/**
        Chmod wrapper with same inputs as used for file creation.
        @see testrun_utils_create_file

*/
bool testrun_utils_chmod(
        const char *filename,
        const char *absolute_path,
        const char *relative_path,
        int flags);

#endif /* testrun_utils_h */

