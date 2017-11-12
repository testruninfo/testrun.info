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
 *      @file           testrun_tests.h
 *      @author         Markus Toepfer
 *      @date           2017-11-12
 *
 *      @ingroup
 *
 *      @brief
 *
 *
 *      ------------------------------------------------------------------------
 **/

#ifndef testrun_tests_h
#define testrun_tests_h

/*----------------------------------------------------------------------------
 *
 *      TEST BLOCK DEFINITIONS
 *
 *      These definitions are used to separate different areas within a
 *      testfile. Goal is to make a highly visual block separation.
 *
 *----------------------------------------------------------------------------*/


#define TESTRUN_TEST_SETUP  \
"/*******************************************************************************\n\
 *\n\
 *      TEST SETUP\n\
 *\n\
 ******************************************************************************/\n"

#define TESTRUN_TEST_CASES  \
"/*******************************************************************************\n\
 *\n\
 *      TEST CASES\n\
 *\n\
 ******************************************************************************/\n"

#define TESTRUN_TEST_CLUSTER  \
"/*******************************************************************************\n\
 *\n\
 *      TEST CLUSTER\n\
 *\n\
 ******************************************************************************/\n"

#define TESTRUN_TEST_ACTIVATED  \
"/*******************************************************************************\n\
 *\n\
 *      ACTIVATED TESTS\n\
 *\n\
 ******************************************************************************/\n"

#endif /* testrun_tests_h */
