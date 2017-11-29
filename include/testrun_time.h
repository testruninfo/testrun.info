/***
        -------------------------------------------------------------------------

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

        -------------------------------------------------------------------------
*//**

        @file           testrun_time.h
        @author         Markus Toepfer
        @date           2017-11-28

        @ingroup        testrun_lib

        @brief          Testrun time calculations.


        -------------------------------------------------------------------------
*/

#ifndef testrun_time_h
#define testrun_time_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#include "testrun_log.h"

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


#endif /* testrun_time_h */
