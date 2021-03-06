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

        @file           testrun_time.c
        @author         Markus Toepfer
        @date           2017-11-28

        @ingroup        testrun_lib

        @brief          Testrun time calculations


        ------------------------------------------------------------------------
*/

#include "../include/testrun_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
/*
 *      ------------------------------------------------------------------------
 *
 *      INTERNAL HEADER (unexposed)
 *
 *      ------------------------------------------------------------------------
 */

typedef enum testrun_time_scope {
        TESTRUN_SCOPE_YEAR, TESTRUN_SCOPE_MONTH,  TESTRUN_SCOPE_DAY,
        TESTRUN_SCOPE_HOUR, TESTRUN_SCOPE_MINUTE, TESTRUN_SCOPE_SECOND,
        TESTRUN_SCOPE_MILLISECOND, TESTRUN_SCOPE_MICROSECOND} testrun_time_scope_t;

/*----------------------------------------------------------------------------*/

/**
        Create an ISO 8601 or an ISO 8601 enhanced timestamp including
        microseconds. The result will be either NULL on error or one of:

        2017-11-20T14:57:01Z
        2017-11-20T14:57:01.123456Z

        @param micro            if true use microseconds
        @returns                Timestring buffer of 30 allocated bytes
                                or NULL. result MUST be freed.
 */
char *testrun_timestamp(bool micro);

/*----------------------------------------------------------------------------*/

/**
        Create an ISO 8601 or an ISO 8601 enhanced timestamp including
        microseconds. This function will write one of:

        2017-11-20T14:57:01Z
        2017-11-20T14:57:01.123456Z

        to the pointer at buffer if the size is sufficiant.

        @param micro            if true use microseconds
        @param buffer           pointer to buffer to be filled with chars
        @param size             size of the buffer (incl. terminating zero)
        @returns                true on success, false on error
*/
bool testrun_timestamp_write_to(
        bool micro, char *buffer, size_t size);

/*----------------------------------------------------------------------------*/

/**
        Create an time string based on the scope provided. This functions
        will create string of the following form:

        SCOPE           RESULT
        -------------------------------------------
        YEAR            2017
        MONTH           2017-11
        DAY             2017-11-20
        HOUR            2017-11-20 14
        MINUTE          2017-11-20 14:57
        SECOND          2017-11-20 14:57:01
        MILLISECOND     2017-11-20 14:57:01.123
        MICROSECOND     2017-11-20 14:57:01.123456

        @param scope    SCOPE to be used for the string
        @returns        allocated timestring or NULL (result MUST be freed)
*/
char *testrun_time_string(testrun_time_scope_t scope);


/*----------------------------------------------------------------------------*/

/**
        Write a scoped timestring to a buffer.

        @param scope    SCOPE to be used for the string
        @param buffer   pointer to buffer to be filled with chars
        @param size     size of the buffer (incl. terminating zero)
        @returns        true on success, false on error
*/
bool testrun_time_write_to(
        testrun_time_scope_t scope, char *buffer, size_t size);

/*
 *      ------------------------------------------------------------------------
 *
 *      IMPLEMENTATION
 *
 *      ------------------------------------------------------------------------
 */

char *testrun_timestamp(bool micro) {

        time_t t;
        time(&t);

        struct timeval tv;

        /*
         *      This is a threadsave local implementation of the header
         *      Timestamp creation in @see testrun_log.h of the function
         *      testrun_log_create_timestamp(bool micro); Based in local
         *      variable use, this function is threadsave.
         */

        size_t size = 30;
        char time_buf[30] = { 0 };
        char *time_utc = calloc(size, sizeof(char));

        if (0 != gettimeofday(&tv, NULL))
                goto error;

        if (!micro){
                if (!strftime(time_utc, size, "%FT%TZ", gmtime(&tv.tv_sec)))
                        goto error;
                return time_utc;
        }

        if (!strftime(time_buf, size, "%FT%T", gmtime(&tv.tv_sec)))
                        goto error;

        if (!snprintf(time_utc, size, "%s.%.6jdZ",
                time_buf, (int64_t) (tv.tv_usec)))
                goto error;

        return time_utc;
error:
        free(time_utc);
        return NULL;
}

/*----------------------------------------------------------------------------*/

bool testrun_timestamp_write_to(
        bool micro, char *buffer, size_t size) {

        if ((NULL == buffer) || (size < 21))
                return false;

        if (micro)
                if (size < 28)
                        return false;

        time_t t;
        time(&t);

        struct timeval tv;
        char time_utc[25];

        if (0 != gettimeofday(&tv, NULL))
                return false;

        if (!micro){

                if (!strftime(buffer, size, "%FT%TZ", gmtime(&tv.tv_sec)))
                        return false;
                return true;
        }

        if (!strftime(time_utc, 25, "%FT%T", gmtime(&tv.tv_sec)))
                        return false;

        if (!snprintf(buffer, size, "%s.%.6jdZ",
                time_utc, (int64_t) (tv.tv_usec)))
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

char *testrun_time_string(testrun_time_scope_t scope){

        time_t t;
        time(&t);

        struct timeval tv;

        size_t size = 30;
        char time_buf[30] = {0};
        char *time_utc = calloc(size, sizeof(char));

        if (0 != gettimeofday(&tv, NULL))
                return NULL;


        switch (scope) {

                case TESTRUN_SCOPE_YEAR:
                        if (!strftime(time_utc, size, "%Y",
                                gmtime(&tv.tv_sec)))
                                goto error;
                        break;
                case TESTRUN_SCOPE_MONTH:
                        if (!strftime(time_utc, size, "%Y-%m",
                                gmtime(&tv.tv_sec)))
                                goto error;
                        break;
                case TESTRUN_SCOPE_DAY:
                        if (!strftime(time_utc, size, "%F",
                                gmtime(&tv.tv_sec)))
                                goto error;
                        break;
                case TESTRUN_SCOPE_HOUR:
                        if (!strftime(time_utc, size, "%F %H",
                                gmtime(&tv.tv_sec)))
                                goto error;
                        break;
                case TESTRUN_SCOPE_MINUTE:
                        if (!strftime(time_utc, size, "%F %R",
                                gmtime(&tv.tv_sec)))
                                goto error;
                        break;
                case TESTRUN_SCOPE_SECOND:
                        if (!strftime(time_utc, size, "%F %T",
                                gmtime(&tv.tv_sec)))
                                goto error;
                        break;
                case TESTRUN_SCOPE_MILLISECOND:
                        if (!strftime(time_buf, size, "%F %T",
                                gmtime(&tv.tv_sec)))
                                goto error;
                        if (!snprintf(time_utc, size, "%s.%.3jd",
                                time_buf, (int64_t) (tv.tv_usec)/1000))
                                goto error;
                        break;
                case TESTRUN_SCOPE_MICROSECOND:
                        if (!strftime(time_buf, size, "%F %T",
                                gmtime(&tv.tv_sec)))
                                goto error;
                        if (!snprintf(time_utc, size, "%s.%.6jd",
                                time_buf, (int64_t) (tv.tv_usec)))
                                goto error;
                        break;

                default:
                        goto error;
        }

        return time_utc;
error:
        free(time_utc);
        return NULL;
}

/*----------------------------------------------------------------------------*/

bool testrun_time_write_to(
        testrun_time_scope_t scope, char *buffer, size_t size){

        if (!buffer || size < 5)
                return false;

        time_t t;
        time(&t);

        struct timeval tv;

        if (0 != gettimeofday(&tv, NULL))
                return false;


        switch (scope) {

                case TESTRUN_SCOPE_YEAR:

                        if (size < 5)
                                return false;

                        if (!strftime(buffer, size, "%Y",
                                gmtime(&tv.tv_sec)))
                                return false;

                        break;

                case TESTRUN_SCOPE_MONTH:

                        if (size < 8)
                                return false;

                        if (!strftime(buffer, size, "%Y-%m",
                                gmtime(&tv.tv_sec)))
                                return false;
                        break;

                case TESTRUN_SCOPE_DAY:

                        if (size < 11)
                                return false;

                        if (!strftime(buffer, size, "%F",
                                gmtime(&tv.tv_sec)))
                                return false;
                        break;

                case TESTRUN_SCOPE_HOUR:

                        if (size < 14)
                                return false;

                        if (!strftime(buffer, size, "%F %H",
                                gmtime(&tv.tv_sec)))
                                return false;
                        break;

                case TESTRUN_SCOPE_MINUTE:

                        if (size < 17)
                                return false;

                        if (!strftime(buffer, size, "%F %R",
                                gmtime(&tv.tv_sec)))
                                return false;
                        break;

                case TESTRUN_SCOPE_SECOND:

                        if (size < 20)
                                return false;

                        if (!strftime(buffer, size, "%F %T",
                                gmtime(&tv.tv_sec)))
                                return false;
                        break;

                case TESTRUN_SCOPE_MILLISECOND:

                        if (size < 24)
                                return false;

                        if (!strftime(buffer, size, "%F %T",
                                gmtime(&tv.tv_sec)))
                                return false;

                        if (!snprintf(buffer + 19, size, ".%.3jd",
                                (int64_t) (tv.tv_usec)/1000))
                                return false;

                        break;

                case TESTRUN_SCOPE_MICROSECOND:

                        if (size < 27)
                                return false;

                        if (!strftime(buffer, size, "%F %T",
                                gmtime(&tv.tv_sec)))
                                return false;

                        if (!snprintf(buffer + 19, size, ".%.6jd",
                                (int64_t) (tv.tv_usec)))
                                return false;

                        break;

                default:
                        return false;
        }

        return true;
}
