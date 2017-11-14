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
 *      @file           tr_string.c
 *      @author         Markus Toepfer
 *      @date           2017-11-13
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Implementation of string manipulations of testrun_lib
 *
 *
 *      ------------------------------------------------------------------------
 **/

#include "../include/tr_string.h"

/*----------------------------------------------------------------------------*/

char *tr_string_free(char *string){

        if (string)
                free(string);

        return NULL;
}

/*----------------------------------------------------------------------------*/

int tr_string_prepare(
        char **result,
        size_t * const size,
        size_t * const open,
        size_t * const used){

        if ((NULL == result) || (NULL == size))
                return -1;

        if ((NULL == open) || (NULL == used))
                return -1;

        if (*result != NULL){

                // check if buffer contains a string
                *used = strnlen(*result, *size);

                // contained string not \0 terminated
                if (*used == *size)
                        return -1;

                *open = *size - *used;

        } else {

                // new data
                *result = calloc(TR_STRING_DEFAULT_SIZE, sizeof(char));
                if (!*result)
                        return -1;

                *size  = TR_STRING_DEFAULT_SIZE;
                *open  = TR_STRING_DEFAULT_SIZE;
                *used  = 0;
        }

        return 0;
}

/*----------------------------------------------------------------------------*/

int tr_string_append(
        char **dest, size_t * const size,
        char const * const source, size_t len){

        if ((NULL == dest) || (NULL == size) || (NULL == source) || (len < 1))
                return -1;


        int64_t done = 0;
        size_t  open = 0;
        size_t  used = 0;
        size_t  nuse = 0;

        char    *ptr = NULL;

        if (*dest){
                used = strnlen(*dest, *size);
                if (used == *size)
                        return -1;
        }
        open = *size - used;

        if (len > strlen(source))
                return -1;

        if (open > (len) ){

                // standard append
                if (strncat(*dest, source, open))
                        return 0;
                return -1;

        }

        // realloc append

        open = 0;
        ptr  = NULL;

        done = tr_string_write_embeded(&ptr, &open, &nuse, 1,
                *dest, used,
                source, len,
                0,0,0,0);

        if (done < 0)
                return -1;

        free(*dest);
        *dest = ptr;
        *size = open + nuse;
        return 0;
}

/*----------------------------------------------------------------------------*/

int64_t tr_string_write_embeded(
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

int tr_string_embed(
        char **result, size_t * const size,
        char const * const source, size_t sc_len,
        char const * const prefix, size_t px_len,
        char const * const suffix, size_t sx_len,
        char const * const delim1, size_t d1_len,
        char const * const delim2, size_t d2_len){

        if ((NULL == result) || (NULL == size))
                return -1;

        if ((NULL == source) || (sc_len < 1))
                return -1;

        if (delim1)
                if (strlen(delim1) > d1_len)
                        return -1;

        size_t open = 0;
        size_t used = 0;
        size_t line = 0;

        char *start = NULL;
        char *end   = NULL;

        if (tr_string_prepare(result, size, &open, &used) < 0)
                return -1;

        start = (char*) source;

        if (delim1)
                end = strstr(start, delim1);

        while (end) {

                /* Cut latest at given sc_len */
                if (end > source + sc_len)
                        break;

                line = end - start;
                if (tr_string_write_embeded(
                        result, &open, &used,
                        TR_STRING_DEFAULT_SIZE,
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
                if(tr_string_write_embeded(
                        result, &open, &used,
                        TR_STRING_DEFAULT_SIZE,
                        prefix, px_len,
                        start, line,
                        suffix, sx_len,
                        delim2, d2_len) < 0)
                        goto error;

        }

        *size   = used + open;

        return 0;

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

        return -1;
}

/*----------------------------------------------------------------------------*/

int tr_string_unset_end(
        char  * const result, size_t size,
        char const * const string, size_t len){

        if ((NULL == result) || (size < 1))
                return -1;

        if ((NULL == string) || (len < 1))
                return -1;

        if (size < len)
                return -1;

        size_t used = strnlen(result, size);

        char *end = result + used;
        if (!end)
                return -1;

        /* end points to last character of the string at result */
        if (len > (size_t) (end  - result + 1))
                return -1;

        end-= len;

        /* Match last string */

        for(size_t i = 0; i < len; i++){

                if (end[i] != string[i])
                        return -1;
        }

        /* Actual unset last string */

        bzero(end, len);
        return 0;

}

/*----------------------------------------------------------------------------*/

int tr_string_replace_first(
        char **result, size_t * const size,
        char const * const source, size_t sc_len,
        char const * const old,    size_t old_len,
        char const * const new,    size_t new_len){

        if ((NULL == result) || (NULL == size))
                return -1;

        if ((NULL == source) || (sc_len < 1))
                return -1;

        if ((NULL == old) || (old_len < 1))
                return -1;

        if (old_len < strlen(old))
                return -1;

        if (new)
               if (new_len < strlen(new))
                        return -1;


        size_t open = 0;
        size_t used = 0;
        size_t plen = 0;

        char *end   = NULL;
        char *part  = NULL;

        if (tr_string_prepare(result, size, &open, &used) < 0)
                return -1;

        end   = strstr(source, old);
        part  = end + old_len;
        plen  = sc_len - (part - source);

        if (end) {

                if(tr_string_write_embeded(
                        result, &open, &used,
                        TR_STRING_DEFAULT_SIZE,
                        NULL, 0,
                        source, end - source,
                        NULL, 0,
                        new, new_len) < 0)
                        goto error;

                // append second part

                if(tr_string_write_embeded(
                        result, &open, &used,
                        TR_STRING_DEFAULT_SIZE,
                        NULL, 0,
                        part, plen,
                        NULL, 0,
                        NULL, 0) < 0)
                        goto error;

        } else {

                // append an unchanged copy

                if(tr_string_write_embeded(
                        result, &open, &used,
                        TR_STRING_DEFAULT_SIZE,
                        NULL, 0,
                        source, sc_len,
                        NULL, 0,
                        NULL, 0) < 0)
                        goto error;

        }

        *size   = used + open;

        return 0;

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

        return -1;
}

/*----------------------------------------------------------------------------*/

int tr_string_replace_all(
        char **result, size_t *size,
        char const * const source, size_t sc_len,
        char const * const delim1, size_t d1_len,
        char const * const delim2, size_t d2_len,
        bool set_last){

        if (NULL == result || NULL == size )
                return -1;

        if (NULL == source || 0 == sc_len )
                return -1;

        if (NULL == delim1 || 0 == d1_len )
                return -1;

        if (tr_string_embed(result, size,
                source, sc_len, 0,0, 0,0,
                delim1, d1_len, delim2, d2_len) < 0 )
                return -1;

        if (set_last)
                return 0;

        return tr_string_unset_end(*result, *size, delim2, d2_len);
}
