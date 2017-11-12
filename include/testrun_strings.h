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
 *      @file           testrun_strings.h
 *      @author         Markus Toepfer
 *      @date           2017-11-12
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Definition of string manipulation functions.
 *
 *
 *      ------------------------------------------------------------------------
 **/

#ifndef LINEBREAK
#define LINEBREAK "\n"
#endif

#ifndef testrun_strings_h
#define testrun_strings_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*----------------------------------------------------------------------------*/

/**
 *      Create a string allocation with optional suffix, prefix and lineend.
 *      This function will write concat all prefix, source, suffix and linebreak
 *      string up to their given length at a new allocated memory area pointed
 *      to with pointer.
 *
 *      @param  pointer       mandatory pointer for the result
 *      @param  length        mandatory length for the result
 *      @param  source        (optional) source to be used
 *      @param  source_length (optional) length of the source
 *      @param  prefix        (optional) prefix to be used
 *      @param  prefix_length (optional) length of the prefix
 *      @param  suffix        (optional) suffix to be used
 *      @param  suffix_length (optional) length of the suffix
 *      @param  lbreak        (optional) linebreak to be used
 *      @param  lbreak_length (optional) length of the line break
 *      @return               negative on error, 0 on success
 */
int testrun_create_string(
        char **pointer, size_t *length,
        const char * const source, const size_t source_length,
        const char * const prefix, const size_t prefix_length,
        const char * const suffix, const size_t suffix_length,
        const char * const lbreak, const size_t lbreak_length);

/*----------------------------------------------------------------------------*/

/**
 *      Write a source string with optional prefix, suffix and linebreak to a
 *      destination pointer.
 *
 *      @Note This is a very simple string concat function, which checks if the
 *      strings will fit to the buffer provided.
 *
 *      Example
 *
 *              source  |abcd|
 *              prefix  |xxx|
 *              suffix  |yyy|
 *              lbreak  |\n|
 *
 *              pointer |xxxabcdyyy\n|
 *
 *      @param  pointer       pointer to open result buffer
 *      @param  open          length open at pointer
 *      @param  source        (optional) source to be used
 *      @param  source_length (optional) length of the source
 *      @param  prefix        (optional) prefix to be used
 *      @param  prefix_length (optional) length of the prefix
 *      @param  suffix        (optional) suffix to be used
 *      @param  suffix_length (optional) length of the suffix
 *      @param  lbreak        (optional) linebreak to be used
 *      @param  lbreak_length (optional) length of the line break
 *
 *      @return               amount of written bytes at poiner
 */
size_t testrun_write_to(
              char * pointer,           const size_t open,
        const char * const source,      const size_t source_length,
        const char * const prefix,      const size_t prefix_length,
        const char * const suffix,      const size_t suffix_length,
        const char * const lbreak,      const size_t lbreak_length);

/*----------------------------------------------------------------------------*/

/**
 *      Embed all parts of source string in a new allocated string with
 *      prefix, suffix and part delimiter adaptations to each part between
 *      a delimiter.
 *
 *      Example:
 *
 *      (1) SPACE TO LINEBREAK
 *
 *              testrun_embed_string(&ptr, &size,
 *                      "a b c", strlen("a b c")        SOURCE
 *                      0,0,                            PREFIX
 *                      0,0,                            SUFFIX
 *                      "\n",1,                         LBREAK PATTERN
 *                      " ", 1,                         DELIMITER
 *                      false);                         UNDELIMIT LAST
 *
 *              RESULT at &ptr "a\nb\nc\n"
 *
 *      (2) COMMA TO LINEBREAK
 *
 *              testrun_embed_string(&ptr, &size,
 *                      "a,b,c", strlen("a,b,c")        SOURCE
 *                      0,0,                            PREFIX
 *                      0,0,                            SUFFIX
 *                      "\n",1,                         LBREAK PATTERN
 *                      ",", 1,                         DELIMITER
 *                      false);                         UNDELIMIT LAST
 *
 *              RESULT at &ptr "a\nb\nc\n" "a\nb\nc\n"
 *
 *      (3) COLON TO COLON with pre and suffix
 *
 *              testrun_embed_string(&ptr, &size,
 *                      "a,b,c", strlen("a,b,c")        SOURCE
 *                      " ",1,                          PREFIX
 *                      " ",1,                          SUFFIX
 *                      ":",1,                          LBREAK PATTERN
 *                      ":",1,                          DELIMITER
 *                      true );                         UNDELIMIT LAST
 *
 *              RESULT at &ptr " a : b : c "
 *
 *      (4) Whatever Whenever
 *
 *              Reuse of the LBREAK DELIMITER replacement as a simple
 *              string replacement.
 *
 *              testrun_embed_string(&ptr, &size,
 *                      "Whatever", strlen("Whatever")  SOURCE
 *                      0,0,                            PREFIX
 *                      0,0,                            SUFFIX
 *                      "HEN",3,                        LBREAK PATTERN
 *                      "hat",3,                        DELIMITER
 *                      true);                          UNDELIMIT LAST
 *
 *              RESULT at &ptr "WHENever"
 *
 *      (5) Comment lines
 *
 *              testrun_embed_string(&ptr, &size,
 *                      "a\nb\nc", strlen("a,b,c")      SOURCE
 *                      "#",1,                          PREFIX
 *                      0,0                             SUFFIX
 *                      "\n",1,                         LBREAK PATTERN
 *                      "\n",1,                         DELIMITER
 *                      false );                        UNDELIMIT LAST
 *
 *              RESULT at &ptr "#a\n#b\n#c\n"
 *
 *      @param  source        (mandatory) source to be used
 *      @param  source_length (mandatory) length of the source
 *      @param  prefix        (optional) prefix to be used
 *      @param  prefix_length (optional) length of the prefix
 *      @param  suffix        (optional) suffix to be used
 *      @param  suffix_length (optional) length of the suffix
 *      @param  lbreak        (optional) lbreak to be used
 *      @param  lbreak_length (optional) length of the lbreak
 *      @param  delim         (optional) delimiter to be used
 *      @param  delim_length  (optional) length of the delimiter
 *      @param  undelimit_last if true, the last item will not be delimited
 *
 *      @return               0 on success, -1 on error
 */
int testrun_embed_string(
        char **ptr, size_t *size,
              char * const source, const size_t source_length,
        const char * const prefix, const size_t prefix_length,
        const char * const suffix, const size_t suffix_length,
        const char * const lbreak, const size_t lbreak_length,
        const char * const delim,  const size_t delim_length,
        bool undelimit_last);




#endif /* testrun_strings_h */
