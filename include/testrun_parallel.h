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
 *      @file           testrun_parallel.h
 *      @author         Markus Toepfer
 *      @date           2017-11-17
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Parallel test execution.
 *
 *
 *      ------------------------------------------------------------------------
 **/

#ifndef testrun_parallel_h
#define testrun_parallel_h


#include <omp.h>                /* OpenMP parallel */
#include <stdbool.h>
#include "testrun.h"            /* Standard testrun header */


/**
        Run a number of functions in parallel.
        @param functions        array of function pointers
        @param amount           number of functions enabled in functions

        @returns
*/
bool testrun_parallel(
        void (*functions[])(),
        size_t amount,
        size_t const * successful,
        size_t const * failed);


#endif /* testrun_parallel_h */
