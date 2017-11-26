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

        @file           testrun_config_tests.c
        @author         Markus Toepfer
        @date           2017-11-25

        @ingroup

        @brief


        ------------------------------------------------------------------------
*/

#include "../../include/testrun2.h"
#include "../../src/testrun_config.c"

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST HELPER                                                    #HELPER
 *
 *      ------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------*/

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------*/




/*
int test_testrun_config_paths_default() {


        testrun_config_paths path = testrun_config_paths_default();

        testrun(strncmp(
                path.path,
                TESTRUN_CONFIG_PATH_DUMMY,
                strlen(TESTRUN_CONFIG_PATH_DUMMY)) == 0);
        testrun(strncmp(
                path.include,
                TESTRUN_CONFIG_PATH_INCLUDE,
                strlen(TESTRUN_CONFIG_PATH_INCLUDE)) == 0);
        testrun(strncmp(
                path.src,
                TESTRUN_CONFIG_PATH_SRC,
                strlen(TESTRUN_CONFIG_PATH_SRC)) == 0);
        testrun(strncmp(
                path.tests,
                TESTRUN_CONFIG_PATH_TESTS,
                strlen(TESTRUN_CONFIG_PATH_TESTS)) == 0);
        testrun(strncmp(
                path.tests_unit,
                TESTRUN_CONFIG_PATH_UNIT_TESTS,
                strlen(TESTRUN_CONFIG_PATH_UNIT_TESTS)) == 0);
        testrun(strncmp(
                path.tests_acceptance,
                TESTRUN_CONFIG_PATH_ACCEPTANCE_TESTS,
                strlen(TESTRUN_CONFIG_PATH_ACCEPTANCE_TESTS)) == 0);
        testrun(strncmp(
                path.tests_tools,
                TESTRUN_CONFIG_PATH_TEST_TOOLS,
                strlen(TESTRUN_CONFIG_PATH_TEST_TOOLS)) == 0);
        testrun(strncmp(
                path.config,
                TESTRUN_CONFIG_PATH_CONFIG,
                strlen(TESTRUN_CONFIG_PATH_CONFIG)) == 0);
        testrun(strncmp(
                path.doxygen,
                TESTRUN_CONFIG_PATH_DOXYGEN,
                strlen(TESTRUN_CONFIG_PATH_DOXYGEN)) == 0);
        testrun(strncmp(
                path.docs,
                TESTRUN_CONFIG_PATH_DOCS,
                strlen(TESTRUN_CONFIG_PATH_DOCS)) == 0);

        return testrun_log_success();
}


int test_testrun_config_default() {

        testrun_config config = testrun_config_default();

        testrun(strncmp(
                config.name,
                "name",
                strlen("NAME")) == 0);
        testrun(strncmp(
                config.author,
                "author",
                strlen("AUTHOR")) == 0);
        testrun(strncmp(
                config.copyright,
                "All rights reserved.",
                strlen("All rights reserved.")) == 0);
        testrun(strncmp(
                config.copyright_src,
                "copyright_src",
                strlen("copyright_src")) == 0);
        testrun(strncmp(
                config.makefile_src,
                "makefile_src",
                strlen("makefile_src")) == 0);

        testrun(strncmp(
                config.path.include,
                TESTRUN_CONFIG_PATH_INCLUDE,
                strlen(TESTRUN_CONFIG_PATH_INCLUDE)) == 0);
        testrun(strncmp(
                config.path.src,
                TESTRUN_CONFIG_PATH_SRC,
                strlen(TESTRUN_CONFIG_PATH_SRC)) == 0);
        testrun(strncmp(
                config.path.tests,
                TESTRUN_CONFIG_PATH_TESTS,
                strlen(TESTRUN_CONFIG_PATH_TESTS)) == 0);
        testrun(strncmp(
                config.path.tests_unit,
                TESTRUN_CONFIG_PATH_UNIT_TESTS,
                strlen(TESTRUN_CONFIG_PATH_UNIT_TESTS)) == 0);
        testrun(strncmp(
                config.path.tests_acceptance,
                TESTRUN_CONFIG_PATH_ACCEPTANCE_TESTS,
                strlen(TESTRUN_CONFIG_PATH_ACCEPTANCE_TESTS)) == 0);
        testrun(strncmp(
                config.path.tests_tools,
                TESTRUN_CONFIG_PATH_TEST_TOOLS,
                strlen(TESTRUN_CONFIG_PATH_TEST_TOOLS)) == 0);
        testrun(strncmp(
                config.path.config,
                TESTRUN_CONFIG_PATH_CONFIG,
                strlen(TESTRUN_CONFIG_PATH_CONFIG)) == 0);
        testrun(strncmp(
                config.path.doxygen,
                TESTRUN_CONFIG_PATH_DOXYGEN,
                strlen(TESTRUN_CONFIG_PATH_DOXYGEN)) == 0);
        testrun(strncmp(
                config.path.docs,
                TESTRUN_CONFIG_PATH_DOCS,
                strlen(TESTRUN_CONFIG_PATH_DOCS)) == 0);


        return testrun_log_success();
}
*/
/*----------------------------------------------------------------------------*/

int test_testrun_config_to_string() {

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                   #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int all_tests() {

       testrun_init();
/*
       testrun_test(test_testrun_config_paths_default);
       testrun_test(test_testrun_config_default);

       testrun_test(test_testrun_config_to_string);
*/
       return testrun_counter;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST EXECUTION                                                  #EXEC
 *
 *      ------------------------------------------------------------------------
 */

testrun_run(all_tests);
