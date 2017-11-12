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
 *      @file           testrun_strings.c
 *      @author         Markus Toepfer
 *      @date           2017-11-12
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Implementation of basic string functions used in
 *                      the testrun tools.
 *
 *
 *      ------------------------------------------------------------------------
 **/

#include "../include/testrun_strings.h"

/*----------------------------------------------------------------------------*/

int testrun_create_string(
        char **pointer,     size_t *length,
        const char *source, const size_t source_length,
        const char *prefix, const size_t prefix_length,
        const char *suffix, const size_t suffix_length,
        const char *lbreak, const size_t lbreak_length){

        if (!pointer || !length)
                return -1;

        size_t count =  0;
        size_t size  =  source_length +
                        prefix_length +
                        suffix_length +
                        lbreak_length + 1;

        if (size < 2)
                return -1;

        char *result = calloc(size, sizeof(char));
        if (!result)
                goto error;

        count = testrun_write_to(result, size,
                source, source_length,
                prefix, prefix_length,
                suffix, suffix_length,
                lbreak, lbreak_length);

        if (count != size - 1)
                goto error;

        *length  = count;
        *pointer = result;

        return 0;

error:
        if (pointer)
                *pointer = NULL;

        if (length)
                *length = 0;

        return -1;
}

/*----------------------------------------------------------------------------*/

size_t testrun_write_to(
        char *pointer,          const size_t open,
        const char *source,     const size_t source_length,
        const char *prefix,     const size_t prefix_length,
        const char *suffix,     const size_t suffix_length,
        const char *lbreak,     const size_t lbreak_length){

        if (!pointer || (open < 1))
                return 0;

        if (open < (    source_length +
                        prefix_length +
                        suffix_length +
                        lbreak_length + 1))

                return 0;

        return snprintf(pointer, open, "%.*s%.*s%.*s%.*s",
                (int) prefix_length, prefix,
                (int) source_length, source,
                (int) suffix_length, suffix,
                (int) lbreak_length, lbreak);
}

/*----------------------------------------------------------------------------*/

int testrun_embed_string(
        char **out, size_t *size,
              char * const source, const size_t source_length,
        const char * const prefix, const size_t prefix_length,
        const char * const suffix, const size_t suffix_length,
        const char * const lbreak, const size_t lbreak_length,
        const char * const delim,  const size_t delim_length,
        bool undelimit_last){

        if (!source || source_length < 1)
                return -1;

        size_t line_length   = 0;
        size_t written       = 0;
        size_t offset        = 0;

        size_t open          = 0;
        size_t used          = 0;

        char *result = calloc(open, sizeof(char));
        if (!result)
                return -1;

        char *start = source;
        char *end   = NULL;
        char *ptr   = result;

        if (delim){
                end = strstr(start, delim);
        }

        while (end){

                line_length = end - start;

                /* check space, if not enough reallocated */

                if (open <= (   line_length +
                                prefix_length +
                                suffix_length +
                                lbreak_length)){

                        open  = line_length +
                                prefix_length +
                                suffix_length +
                                lbreak_length + 1;

                        offset = ptr - result;
                        result = realloc(result, open + used);
                        if (!result)
                                goto error;
                        ptr = result + offset;

                }

                written = testrun_write_to(ptr, open,
                        start, line_length,
                        prefix, prefix_length,
                        suffix, suffix_length,
                        lbreak, lbreak_length);

                if (written < 1)
                        goto error;

                used += written;
                ptr  += written;
                open -= written;

                start = end + delim_length;
                end   = strstr(start,delim);

        }

        /* allow source without delimiter */

        if (start != source){

                /* check if last pattern is not delimited */
                if (start != source + source_length)
                        end = source + source_length;

        } else {

                /* first pattern is not delimited */
                end = source + source_length;
        }

        if (end){

                line_length = end - start;

                if (open < (   line_length +
                                prefix_length +
                                suffix_length +
                                lbreak_length )){

                        open  = line_length +
                                prefix_length +
                                suffix_length +
                                lbreak_length + 1;

                        offset = ptr - result;
                        result = realloc(result, open + used);
                        if (!result)
                                goto error;

                        ptr = result + offset;

                }

                written = testrun_write_to(ptr, open,
                        start, line_length,
                        prefix, prefix_length,
                        suffix, suffix_length,
                        lbreak, lbreak_length);

                if (written < 1)
                        goto error;

                used += written;
                ptr  += written;
                open -= written;

        }

        if (undelimit_last){

                ptr = ptr - lbreak_length;
                if (strncmp(lbreak, ptr, lbreak_length) == 0){
                        for (size_t i = 0; i < lbreak_length; i++)
                                ptr[i] = 0;
                        used -= lbreak_length;
                }

        }

        *out  = result;
        *size = used;

        return 0;

error:
        if (result)
                free(result);

        if (out)
                *out = NULL;

        if (size)
                *size = 0;

        return -1;
}
