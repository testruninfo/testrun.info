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

int test_testrun_config_default() {

        testrun_config c = testrun_config_default();

        // -------------------------------------------------------------
        // AUTHOR
        // -------------------------------------------------------------

        testrun(strncmp(c.author, TESTRUN_TAG_DEFAULT_AUTHOR,
                strlen(TESTRUN_TAG_DEFAULT_AUTHOR)) == 0);

        // -------------------------------------------------------------
        // PROJECT
        // -------------------------------------------------------------

        testrun(strncmp(c.project.name, TESTRUN_TAG_DEFAULT_PROJECT,
                strlen(TESTRUN_TAG_DEFAULT_PROJECT)) == 0);

        testrun(strncmp(c.project.url, TESTRUN_URL,
                strlen(TESTRUN_URL)) == 0);

        testrun(c.project.type == TESTRUN_LIB);
        testrun(c.project.doxygen == true);

        testrun(strncmp(c.project.path, TESTRUN_PATH_DUMMY,
                strlen(TESTRUN_PATH_DUMMY)) == 0);

        // -------------------------------------------------------------
        // COPYRIGHT
        // -------------------------------------------------------------

        testrun(strncmp(c.copyright.intro, TESTRUN_COPYRIGHT_INTRO_DEFAULT,
                strlen(TESTRUN_COPYRIGHT_INTRO_DEFAULT)) == 0);

        testrun(strncmp(c.copyright.text, TESTRUN_COPYRIGHT_TEXT_DEFAULT,
                strlen(TESTRUN_COPYRIGHT_TEXT_DEFAULT)) == 0);

        testrun(strncmp(c.copyright.year, TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);

        testrun(strncmp(c.copyright.owner, TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);

        testrun(c.copyright.note        == NULL);
        testrun(c.copyright.variant     == NULL);
        testrun(c.copyright.program     == NULL);
        testrun(c.copyright.to_string   == testrun_copyright_to_string);

        // -------------------------------------------------------------
        // FORMAT
        // -------------------------------------------------------------

        testrun(c.format.line_length    == 80);
        testrun(c.format.indent_c       == 8);
        testrun(c.format.offset_docu    == 8 * 3);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_config_dump() {

        testrun_config config = testrun_config_default();

        testrun(!testrun_config_dump(NULL, NULL));
        testrun(!testrun_config_dump(stdout, NULL));
        testrun(!testrun_config_dump(NULL, &config));

        testrun(testrun_config_dump(stdout, &config));

        // check dump to stdout manually

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

       testrun_test(test_testrun_config_default);
       testrun_test(test_testrun_config_dump);

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
