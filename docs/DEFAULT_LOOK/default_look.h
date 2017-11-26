/***
        -------------------------------------------------------------------------

        Copyright [COPYRIGHT_YEAR] [COPYRIGHT_NAME]

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

        @file           [MODULE_NAME].h
        @author         [AUTHOR_NAME]
        @date           [CREATION_DATE]

        @ingroup        [PROJECT]

        @brief


        -------------------------------------------------------------------------
*/

#ifndef [MODULE_NAME]_h
#define [MODULE_NAME]_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 *      ------------------------------------------------------------------------
 *
 *      FUNCTION BLOCK                                                 #BLOCK
 *
 *      a function block may be used to group module internal
 *      semantic and/or functional similar functions together.
 *      It is recommended to define a #HASHTAG for the block
 *      within the header and reuse (copy) the same definition
 *      to source as well as test files.
 *
 *      Function blocks (and other comments) are prefixed with
 *      a continous star, to improve differentiation between
 *      source code only and documentation related comments.
 *
 *      ------------------------------------------------------------------------
 */

/**
        Documentation is included in <slash><star><star> <star><slash> tags
        and has no continious <star> prefix in front of each line.

        @param parameter        style is Javadoc / Doxygen compatible
        @returns                nothing
*/
int function1(void parameter);

/*----------------------------------------------------------------------------*/

/**
        Functions with more than 2 parameters should use a linebreaked
        parameter definition to improve readability.

        @param parameter1       style is Javadoc / Doxygen compatible
        @param parameter2       style is Javadoc / Doxygen compatible
        @param parameter3       style is Javadoc / Doxygen compatible
        @returns                nothing
*/
int function2(
        int parameter1,
        int parameter2,
        int parameter3);

/*----------------------------------------------------------------------------*/

/**
        Functions are separated by a horizontal line, also related to
        try improve readabiliy.

        Another way to improve readybility is to use namings, which do
        not require a description, which is shown wtih the next functions.

        @returns sum of a and b
*/
int function2(int a, int b);

/*----------------------------------------------------------------------------*/

int do_summation_of(int a, int b);      // no intro comment needed

#endif /* [MODULE_NAME]_h */
