/***
 *      ------------------------------------------------------------------------
 *
 *      Copyright 2017 Markus Toepfer
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 *
 *      This file is part of the testrun project. http://testrun.info
 *
 *      ------------------------------------------------------------------------
 ***//**
 *
 *      @file           testrun_copyright.h
 *      @author         Markus Toepfer
 *      @date           2017-11-12
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Definition of all copyright related tools for the lib.
 *
 *
 *      ------------------------------------------------------------------------
 **/

#ifndef testrun_copyright_h
#define testrun_copyright_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#include "testrun_header.h"
#include "testrun_strings.h"

/*----------------------------------------------------------------------------*/

#define TESTRUN_FILENAME_COPYRIGHT      "copyright"

/*----------------------------------------------------------------------------*/

/**
 *      Create a copyright header with a line_intro as well as general intro
 *      and outro.
 *
 *      @param  result  pointer to result pointer
 *      @param  size    pointer to result size
 *      @param  source  source copyright string
 *      @param  s_len   length of the source
 *      @param  prefix  prefix to be added to each line
 *      @param  p_len   length of the prefix
 *      @param  intro   intro to be added to header
 *      @param  i_len   length of the intro
 *      @param  outro   outro to be added to header
 *      @param  o_len   length of the outro
 *      @return         0 on success -1 on error
 */
int testrun_copyright_create_header(
        char **result, size_t *size,
        const char * const source, const size_t const s_length,
        const char * const prefix, const size_t const p_length,
        const char * const intro,  const size_t const i_length,
        const char * const outro,  const size_t const o_length);

/*----------------------------------------------------------------------------*/
/**
 *      Read a copyright definition from a given path.
 *      This functions reads a main copyright source from a path
 *      and add the comment prefix of the testrun tools.
 *
 *      @param  path    source path to read
 *      @return         allocated string (MUST be freed) with copyright or NULL
 */
char *testrun_read_copyright_from(
        const char * const path);

/*----------------------------------------------------------------------------*/

/**
 *      Create a default fallback copyright in form of:
 *
 *              COPYRIGHT (c) YEAR  NAME
 *              All rights reserved.
 *
 *      @param  year    (optional) string for the year
 *      @param  name    (optional) name for the name
 *      @param  intro   if true, the standard header line intro will be used
 *      @return         allocated string (MUST be freed) with copyright or NULL
 */
char *testrun_copyright_default(
        char *year, char *name, bool intro);

/*----------------------------------------------------------------------------*/

/**
 *      Create a default fallback copyright in form of:
 *
 *              COPYRIGHT (c) YEAR  NAME
 *              All rights reserved.
 *
 *      @param  year    (optional) string for the year
 *      @param  name    (optional) name for the name
 *      @param  intro   if true, the standard header line intro will be used
 *      @return         allocated string (MUST be freed) with copyright or NULL
 */
char *testrun_copyright_configure(
        char *path, char *year, char *name, char *note);

#endif /* testrun_copyright_h */
