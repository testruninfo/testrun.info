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

        @file           testrun_string.c
        @author         Markus Toepfer
        @date           2017-11-13

        @ingroup        testrun_lib

        @brief          Implementation of string manipulations of testrun_lib


        ------------------------------------------------------------------------
*/

#include "../include/testrun_string.h"

/*----------------------------------------------------------------------------*/

char *testrun_string_free(char *string){

        if (string)
                free(string);

        return NULL;
}

/*----------------------------------------------------------------------------*/

bool testrun_string_prepare(
        char **result,
        size_t * const size,
        size_t * const open,
        size_t * const used){

        if ((NULL == result) || (NULL == size))
                return false;

        if ((NULL == open) || (NULL == used))
                return false;

        if (*result != NULL){

                // check if buffer contains a string
                *used = strnlen(*result, *size);

                // contained string not \0 terminated
                if (*used == *size)
                        return false;

                *open = *size - *used;

        } else {

                // new data
                *result = calloc(testrun_STRING_DEFAULT_SIZE, sizeof(char));
                if (!*result)
                        return false;

                *size  = testrun_STRING_DEFAULT_SIZE;
                *open  = testrun_STRING_DEFAULT_SIZE;
                *used  = 0;
        }

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_string_append(
        char **dest, size_t * const size,
        char const * const source, size_t len){

        if ((NULL == dest) || (NULL == size) || (NULL == source) || (len < 1))
                return false;


        int64_t done = 0;
        size_t  open = 0;
        size_t  used = 0;
        size_t  nuse = 0;

        char    *ptr = NULL;

        if (*dest){
                used = strnlen(*dest, *size);
                if (used == *size)
                        return false;
        }
        open = *size - used;

        if (len > strlen(source))
                return false;

        if (open > (len) ){

                // standard append
                if (strncat(*dest, source, open))
                        return true;
                return -1;

        }

        // realloc append

        open = 0;
        ptr  = NULL;

        done = testrun_string_write_embeded(&ptr, &open, &nuse, 1,
                *dest, used,
                source, len,
                0,0,0,0);

        if (done < 0)
                return false;

        free(*dest);
        *dest = ptr;
        *size = open + nuse;
        return true;
}

/*----------------------------------------------------------------------------*/

int64_t testrun_string_write_embeded(
        char  **buffer,
        size_t * const open,
        size_t * const used,
        size_t reallocate_rate,
        char const * const prefix, size_t px_len,
        char const * const source, size_t sc_len,
        char const * const suffix, size_t sx_len,
        char const * const lbreak, size_t lb_len){

        if ((NULL == buffer) || (NULL == open) || (NULL == used))
                return -1;

        if (0 == reallocate_rate)
                return -1;

        size_t required = px_len + sc_len + sx_len + lb_len + 1;
        size_t written  = 0;

        if (*open <= (required)) {

                if ( (*open + reallocate_rate) > required)
                        *open += reallocate_rate;
                else
                        *open += (required - *open);

                *buffer = realloc(*buffer, (*used + *open));
        }

        written = snprintf(*buffer + *used, *open, "%.*s%.*s%.*s%.*s",
                        (int) px_len, prefix,
                        (int) sc_len, source,
                        (int) sx_len, suffix,
                        (int) lb_len, lbreak);

        if (written > 0) {
                *used += written;
                *open -= written;
                return written;
        }
        return -1;
}

/*----------------------------------------------------------------------------*/

bool testrun_string_embed(
        char **result, size_t * const size,
        char const * const source, size_t sc_len,
        char const * const prefix, size_t px_len,
        char const * const suffix, size_t sx_len,
        char const * const delim1, size_t d1_len,
        char const * const delim2, size_t d2_len){

        if ((NULL == result) || (NULL == size))
                return false;

        if ((NULL == source) || (sc_len < 1))
                return false;

        if (delim1)
                if (strlen(delim1) > d1_len)
                        return false;

        size_t open = 0;
        size_t used = 0;
        size_t line = 0;

        char *start = NULL;
        char *end   = NULL;

        if (!testrun_string_prepare(result, size, &open, &used))
                return false;

        start = (char*) source;

        if (delim1)
                end = strstr(start, delim1);

        while (end) {

                /* Cut latest at given sc_len */
                if (end > source + sc_len)
                        break;

                line = end - start;
                if (testrun_string_write_embeded(
                        result, &open, &used,
                        testrun_STRING_DEFAULT_SIZE,
                        prefix, px_len,
                        start, line,
                        suffix, sx_len,
                        delim2, d2_len) < 0)
                        goto error;

                start = end + d1_len;
                end   = strstr(start, delim1);
        }

        if (start == source) {

                /* no delim1 found at all */
                end = (char*) source + sc_len;

        } else {

                /* last item without final delimiter delim1 */
                if (end != source + sc_len)
                        end = (char*) source + sc_len;
        }

        if (end) {

                line = end - start;
                if(testrun_string_write_embeded(
                        result, &open, &used,
                        testrun_STRING_DEFAULT_SIZE,
                        prefix, px_len,
                        start, line,
                        suffix, sx_len,
                        delim2, d2_len) < 0)
                        goto error;

        }

        *size   = used + open;

        return true;

error:

        if (result)
                if(*result){
                        free(*result);
                        *result = NULL;
                }

        if (size){
                if(*size)
                        *size = 0;
        }

        return false;
}

/*----------------------------------------------------------------------------*/

bool testrun_string_unset_end(
        char  * const result, size_t size,
        char const * const string, size_t len){

        if ((NULL == result) || (size < 1))
                return false;

        if ((NULL == string) || (len < 1))
                return false;

        if (size < len)
                return false;

        size_t used = strnlen(result, size);

        char *end = result + used;
        if (!end)
                return false;

        /* end points to last character of the string at result */
        if (len > (size_t) (end  - result + 1))
                return false;

        end-= len;

        /* Match last string */

        for(size_t i = 0; i < len; i++){

                if (end[i] != string[i])
                        return false;
        }

        /* Actual unset last string */

        bzero(end, len);
        return true;

}

/*----------------------------------------------------------------------------*/

bool testrun_string_replace_first(
        char **result, size_t * const size,
        char const * const source, size_t sc_len,
        char const * const old,    size_t old_len,
        char const * const new,    size_t new_len){

        if ((NULL == result) || (NULL == size))
                return false;

        if ((NULL == source) || (sc_len < 1))
                return false;

        if ((NULL == old) || (old_len < 1))
                return false;

        if (old_len < strlen(old))
                return false;

        if (new)
               if (new_len < strlen(new))
                        return false;


        size_t open = 0;
        size_t used = 0;
        size_t plen = 0;

        char *end   = NULL;
        char *part  = NULL;

        if (!testrun_string_prepare(result, size, &open, &used))
                return false;

        end   = strstr(source, old);
        part  = end + old_len;
        plen  = sc_len - (part - source);

        if (end) {

                if(!testrun_string_write_embeded(
                        result, &open, &used,
                        testrun_STRING_DEFAULT_SIZE,
                        NULL, 0,
                        source, end - source,
                        NULL, 0,
                        new, new_len))
                        goto error;

                // append second part

                if(!testrun_string_write_embeded(
                        result, &open, &used,
                        testrun_STRING_DEFAULT_SIZE,
                        NULL, 0,
                        part, plen,
                        NULL, 0,
                        NULL, 0))
                        goto error;

        } else {

                // append an unchanged copy

                if(!testrun_string_write_embeded(
                        result, &open, &used,
                        testrun_STRING_DEFAULT_SIZE,
                        NULL, 0,
                        source, sc_len,
                        NULL, 0,
                        NULL, 0))
                        goto error;

        }

        *size   = used + open;

        return true;

error:

        if (result)
                if(*result){
                        free(*result);
                        *result = NULL;
                }

        if (size){
                if(*size)
                        *size = 0;
        }

        return false;
}

/*----------------------------------------------------------------------------*/

bool testrun_string_replace_all(
        char **result, size_t *size,
        char const * const source, size_t sc_len,
        char const * const delim1, size_t d1_len,
        char const * const delim2, size_t d2_len,
        bool set_last){

        if (NULL == result || NULL == size )
                return false;

        if (NULL == source || 0 == sc_len )
                return false;

        if (NULL == delim1 || 0 == d1_len )
                return false;

        if (!testrun_string_embed(result, size,
                source, sc_len, 0,0, 0,0,
                delim1, d1_len, delim2, d2_len))
                return false;

        if (set_last)
                return true;

        return testrun_string_unset_end(*result, *size, delim2, d2_len);
}

/*----------------------------------------------------------------------------*/

testrun_vector *testrun_string_pointer(
        char * const source, size_t sc_len,
        char const * const delim,  size_t dm_len){

        if (!source || !delim)
                return NULL;

        testrun_vector *vector = testrun_vector_create(0, NULL, NULL);
        if (!vector)
                return NULL;

        if ( (sc_len < 1) || (dm_len < 1))
                goto error;

        if (dm_len > sc_len)
                goto error;

        char *start   = source;
        char *pointer = strstr(source, delim);

        if (!testrun_vector_add(vector, start))
                goto error;

        while (pointer) {

                start = pointer + dm_len;

                if ( (start - source) >= (int64_t) sc_len - 1)
                        break;

                if (start[0] == 0)
                        break;

                if (!testrun_vector_add(vector, start))
                        goto error;

                pointer = strstr(start, delim);
        }

        return vector;

error:
        vector = testrun_vector_terminate(vector);
        return NULL;
}

/*----------------------------------------------------------------------------*/

testrun_vector *testrun_string_split(
        char * const source, size_t sc_len,
        char const * const delim,  size_t dm_len,
        bool copy_delimiter){

        if (!source || !delim)
                return NULL;

        if ((sc_len < 1) || (dm_len < 1))
                return NULL;

        void    *ptr1   = NULL;
        void    *ptr2   = NULL;
        char    *string = NULL;
        int64_t length  = 0;


        testrun_vector *vector_pointer = NULL;
        testrun_vector *vector_copies  = NULL;

        vector_pointer = testrun_string_pointer(source, sc_len, delim, dm_len);
        if (!vector_pointer)
                goto error;

        vector_copies = testrun_vector_create(vector_pointer->last + 1,
                testrun_vector_item_free, NULL);
        if (!vector_copies)
                return NULL;

        vector_copies->last = vector_pointer->last;

        for (size_t i = 0; i <= vector_pointer->last; i++){

                ptr1 = vector_pointer->items[i];
                if (!ptr1)
                        goto error;

                if (i < vector_pointer->last){

                        ptr2 = vector_pointer->items[i + 1];
                        if (!ptr2)
                                goto error;

                        length = ptr2 - ptr1;

                        if (!copy_delimiter)
                                length -= dm_len;

                } else {

                        ptr2 = source + strlen(source);
                        length = ptr2 - ptr1;

                        if (strstr(ptr1, delim))
                                if (!copy_delimiter)
                                        length -= dm_len;
                }

                if (length < 0)
                        goto error;

                if (length == 0)
                        continue;

                string = calloc(length + 1, sizeof(char));
                if (!string)
                        goto error;

                if (!strncpy(string, ptr1, length)){
                        free(string);
                        goto error;
                }
                vector_copies->items[i] = string;

        }

        vector_pointer = testrun_vector_terminate(vector_pointer);
        return vector_copies;

error:
        vector_copies  = testrun_vector_terminate(vector_copies);
        vector_pointer = testrun_vector_terminate(vector_pointer);
        return NULL;
}

/*----------------------------------------------------------------------------*/

bool testrun_string_clear_whitespace_before_lineend(
        char **result,  size_t * const size,
        char * const source, size_t sc_len,
        char const * const lineend,  size_t le_len){

        if (NULL == result || NULL == size )
                return false;

        if (NULL == source || 0 == sc_len )
                return false;

        if (NULL == lineend || 0 == le_len )
                return false;

        // (1) SPLIT to lines
        testrun_vector *lines = testrun_string_split(
                source, sc_len, lineend, le_len, true);

        if (!lines)
                return false;

        size_t  length      = 0;
        size_t  line_length = 0;
        size_t  used        = 0;
        char    *string     = NULL;
        char    *pointer    = NULL;

        // (2) Adapt the result size

        for (size_t i = 0; i <= lines->last; i++) {

                string = testrun_vector_get(lines, i);
                if (!string){
                        // empty line
                        length += le_len;
                        continue;
                }

                line_length = strlen(string);

                pointer = string + line_length - 1;
                while(isspace(pointer[0])){

                        if ((pointer - string) == 0){
                                // whitespace only line
                                pointer--;
                                break;
                        }

                        pointer--;
                }

                *(pointer+1) = '\0';
                line_length  = (pointer - string) + 1 + le_len;
                length += line_length;

        }

        // (3) Adapt the result size
        if (*result == NULL) {

                *result = calloc(length + 1, sizeof(char));
                if (!*result)
                        goto error;

                used  = 0;
                *size = length;

        } else {

                used = strlen(*result);

                if (*size < (length + used)){

                        *result = realloc(*result,
                                ((length + used) + 1) * sizeof(char));

                        if (!*result)
                                goto error;

                        *size  = (length + used);
                }
        }

        // (4) Write in append mode to result
        pointer = *result + used;

        for (size_t i = 0; i <= lines->last; i++) {

                string = testrun_vector_get(lines, i);

                if (string) {

                        line_length = strlen(string);

                        if (!strncpy(pointer, string, line_length))
                                goto error;

                        pointer += line_length;
                }

                if (!strncpy(pointer, lineend, le_len))
                        goto error;

                pointer += le_len;

        }

        // (5) remove helper structure
        lines = testrun_vector_terminate(lines);
        return true;
error:
        lines = testrun_vector_terminate(lines);
        return false;
}

/*----------------------------------------------------------------------------*/

bool testrun_string_remove_whitespace(
        char **string, size_t * length, bool front, bool end){

        if (!string || !length)
                return false;

        if (!front)
                if (!end)
                        return false;

        char *first  = *string;
        char *last   = NULL;
        char *result = NULL;
        size_t len   = 0;

        // Limit length to input as max
        len = strnlen(*string, *length);

        // Allow only NULL terminated strings
        if (len == *length)
                return false;

        last = *string + len;

        if (front) {

                while(isspace(first[0])){

                        if ( (first - *string) == (int64_t) length){
                                // all whitespace
                                *string = testrun_string_free(*string);
                                *length = 0;
                                return true;
                        }

                        first++;
                }
        }

        if (end) {

                if (last[0] == '\0')
                        last--;

                while(isspace(last[0])){

                        if ( (last - *string) == 0){
                                // all whitespace
                                *string = testrun_string_free(*string);
                                *length = 0;
                                return true;
                        }

                        last--;
                }
        }

        //log("string |%s|%s|%s|%d", *string, first, last, last-first);

        // check first last consistency
        if (first == last) {

                if (isspace(first[0]) || (first[0] == '\0')){
                        // all whitespace
                        *string = testrun_string_free(*string);
                        *length = 0;
                        return true;
                }

        } else {

                if (first > last)
                        return false;
        }

        if (first != *string){

                // remove whitespace in front of string

                result = calloc(last - first + 2, sizeof(char));
                if (!result)
                        return false;

                if (!strncpy(result, first, (last-first) + 1 )){
                        free(result);
                        return false;
                }

                *string = testrun_string_free(*string);
                *string = result;
                *length = strlen(result) + 1;

        } else {

                // remove whitespace behind string

                if (last < *string + (*length -1)){

                        // cut string behind last
                        last[1] = '\0';
                        *length = strlen(*string) + 1;

                } else {

                        // cut length to strlen + 1
                        *length = strlen(*string) + 1;
                }
        }

        return true;
}

/*----------------------------------------------------------------------------*/

int64_t testrun_string_vector_remove_whitespace(
        testrun_vector *vector, bool front, bool end){

        if (!vector)
                return -1;

        if (!front)
                if (!end)
                        return -1;

        // check for pointer vector
        if (!vector->item_free)
                return -1;

        char    *pointer = NULL;
        size_t    length = 0;
        int64_t   result = 0;

        for (size_t i = 0; i <= vector->last; +i++){

                /*
                 *      Use of remove to be save if pointer is deleted,
                 *      or reallocated.
                 */

                pointer = testrun_vector_remove(vector, i);
                if (!pointer)
                        continue;

                //log("%d|%s|", i, pointer);
                length = strlen(pointer) + 1;

                if (!testrun_string_remove_whitespace(
                        &pointer, &length, front, end))
                        return -1;

                if (pointer){
                        testrun_vector_set(vector, i, pointer);
                        result += length - 1;
                }
        }

        //log("result %jd", result);
        return result + 1;
}
