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
        @file           testrun_utils.c
        @author         [AUTHOR]
        @date           2018-07-09

        @ingroup        testrun_lib

        @brief


        ------------------------------------------------------------------------
*/
#include "../include/testrun_utils.h"
#include "../include/testrun_log.h"

char *testrun_utils_insert_at_each_line(
        const char* text,
        const char* intro,
        const char* outro){

        if (!text)
                return NULL;

        char *result = NULL;
        char *ptr    = NULL;
        char *nxt    = NULL;
        char *cur    = NULL;

        size_t len   = 0;
        size_t tlen  = 0;
        size_t ilen  = 0;
        size_t olen  = 0;
        size_t lines = 0;
        size_t size  = 0;

        if (text)
                tlen = strlen(text);

        if (intro)
                ilen = strlen(intro);

        if (outro)
                olen = strlen(outro);

        cur = (char*) text;
        nxt = memchr(cur, '\n', tlen);
        while (nxt) {

                lines++;
                cur = nxt + 1;
                nxt = memchr(cur, '\n', tlen - (cur - text)); 
        }

        if (0 == lines)
                goto error;

        size   = (lines * ilen) + (lines * olen) + tlen + 1;
        result = calloc(size, sizeof(char));
        if (!result)
                goto error;

        ptr = result;
        cur = (char*) text;
        nxt = memchr(cur, '\n', tlen);

        while (nxt) {

                len = nxt - cur;

                if (len > 0){

                        if (!strncat(ptr, intro, ilen))
                                goto error;
        
                        ptr += ilen;
        
                        if (!strncat(ptr, cur, len))
                                goto error;
        
                        ptr += len;
        
                        if (!strncat(ptr, outro, olen))
                                goto error;
        
                        ptr += olen;

                }

                if (!strcat(ptr, "\n"))
                        goto error;

                ptr ++;

                cur = nxt + 1;
                nxt = memchr(cur, '\n', tlen - (cur - text));
        }

        // copy rest after last linebreak
        len = tlen - (cur - text);
        if (len > 0)
                if (!strncat(ptr, cur, len))
                        goto error;

        return result;

error:
        if (result)
                free(result);
        return NULL;
}