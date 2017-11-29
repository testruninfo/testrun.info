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

        @file           testrun_copyright_tests.c
        @author         Markus Toepfer
        @date           2017-11-25

        @ingroup        testrun_lib

        @brief          Unit testing for testrun_copyright.


        ------------------------------------------------------------------------
*/


/*
 *      ------------------------------------------------------------------------
 *
 *      TEST SETUP                                                      #SETUP
 *
 *      ------------------------------------------------------------------------
 */

#include "../../include/testrun.h"
#include "../../src/testrun_copyright.c"

/*
 *      ------------------------------------------------------------------------
 *
 *      STRUCTURE RELATED FUNCTIONS                                     #SRF
 *
 *      Functions which manipulate or support the structure itself.
 *
 *      ------------------------------------------------------------------------
 */

int test_testrun_copyright_structure_is_valid(){

        testrun_copyright statement;

        bzero(&statement, sizeof(testrun_copyright));
        testrun(!testrun_copyright_structure_is_valid(&statement),
                "bzero initialized");

        statement = (testrun_copyright) {
                .intro          = NULL,
                .year           = NULL,
                .owner          = NULL,
                .note           = NULL,
                .text           = NULL,
                .to_string      = NULL
        };
        testrun(!testrun_copyright_structure_is_valid(&statement),
                "initialized to NULL");


        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "text",
                .owner          = "text",
                .note           = "text",
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        testrun(testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = NULL,
                .year           = "text",
                .owner          = "text",
                .note           = "text",
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        testrun(!testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = NULL,
                .owner          = "text",
                .note           = "text",
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        testrun(!testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "text",
                .owner          = NULL,
                .note           = "text",
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        testrun(!testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "text",
                .owner          = "text",
                .note           = "text",
                .text           = NULL,
                .to_string      = testrun_copyright_to_string
        };
        testrun(!testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "text",
                .owner          = "text",
                .note           = "text",
                .text           = "text",
                .to_string      = NULL
        };
        testrun(!testrun_copyright_structure_is_valid(&statement));

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "text",
                .owner          = "text",
                .note           = NULL,
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        testrun(testrun_copyright_structure_is_valid(&statement),
                "note is optional");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_calculate_length(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t expect  = 0;
        size_t result  = 0;
        size_t lineend = strlen(TESTRUN_LINEEND);

        testrun(0 == testrun_copyright_calculate_length(NULL));
        testrun(0 == testrun_copyright_calculate_length(&statement));

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "123",
                .to_string      = testrun_copyright_to_string
        };
        expect = 5 + lineend + 3 + lineend;
        result = testrun_copyright_calculate_length(&statement);
        testrun(expect == result);

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "2017",
                .owner          = "owner",
                .note           = NULL,
                .text           = "123",
                .to_string      = testrun_copyright_to_string
        };
        expect = 4 + 1 + 4 + 1 + 5 + lineend + 3 + lineend;
        result = testrun_copyright_calculate_length(&statement);
        testrun(expect == result);

        statement = (testrun_copyright) {
                .intro          = "text",
                .year           = "2017",
                .owner          = "owner",
                .note           = "note",
                .text           = "123",
                .to_string      = testrun_copyright_to_string
        };
        expect = 4 + 1 + 4 + 1 + 5 + lineend + 3 + lineend + 4 + (2 * lineend);
        result = testrun_copyright_calculate_length(&statement);
        testrun(expect == result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_write_to(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 100;
        size_t open = size;
        size_t min  = 0;

        char expect[size];
        char buf[size];
        char *result = NULL;

        statement = (testrun_copyright) {
                .intro          = "i",
                .year           = "y",
                .owner          = "o",
                .note           = NULL,
                .text           = "t",
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_write_to(NULL, NULL, NULL));
        testrun(NULL == testrun_copyright_write_to(
                buf, &open, NULL));
        testrun(NULL == testrun_copyright_write_to(
                buf, NULL,  &statement));
        testrun(NULL == testrun_copyright_write_to(
                NULL,&open, &statement));

        snprintf(expect, size,  "i y o%s"
                                "t%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_write_to(buf, &open, &statement);
        testrun(result);
        testrun(result[0]  == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == size - strlen(expect));


        // reset
        open = size;
        bzero(buf, size);
        statement = (testrun_copyright) {
                .intro          = "intro",
                .year           = "year",
                .owner          = "owner",
                .note           = "note",
                .text           = "text",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "intro year owner%s"
                                "text%s"
                                "%snote%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_write_to(buf, &open, &statement);
        testrun(result);
        testrun(result[0]  == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == size - strlen(expect));


        // reset
        open = size;
        bzero(buf, size);
        statement = (testrun_copyright) {
                .intro          = "intro",
                .year           = "year",
                .owner          = "owner",
                .note           = "note",
                .text           = "line1\nline2\nline3\n",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "intro year owner%s"
                                "line1\nline2\nline3\n%s"
                                "%snote%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_write_to(buf, &open, &statement);
        testrun(result);
        testrun(result[0]  == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == size - strlen(expect));

        // write behind (write to previous result)
        statement = (testrun_copyright) {
                .intro          = "2",
                .year           = "2",
                .owner          = "2",
                .note           = "2",
                .text           = "line1\nline2\nline3\n",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "intro year owner%s"
                                "line1\nline2\nline3\n%s"
                                "%snote%s"
                                "2 2 2%s"
                                "line1\nline2\nline3\n%s"
                                "%s2%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_write_to(result,&open, &statement);
        testrun(result);
        testrun(result[0]  == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == size - strlen(expect));

        // write behind (write to previous result)
        statement = (testrun_copyright) {
                .intro          = "3",
                .year           = "3",
                .owner          = "3",
                .note           = NULL,
                .text           = "3",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "intro year owner%s"
                                "line1\nline2\nline3\n%s"
                                "%snote%s"
                                "2 2 2%s"
                                "line1\nline2\nline3\n%s"
                                "%s2%s"
                                "3 3 3%s"
                                "3%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND,
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_write_to(result,&open, &statement);
        testrun(result);
        testrun(result[0]  == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == size - strlen(expect));

        // reset
        open = size;
        bzero(buf, size);
        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "1 2 3%s"
                                "4%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);

        // check write < min
        min    = 5 + strlen(TESTRUN_LINEEND) + 1 + strlen(TESTRUN_LINEEND) + 1;
        open   = min - 1;
        result = testrun_copyright_write_to(buf, &open, &statement);
        testrun(!result, "buffer to small, nothing written.");
        for (size_t i = 0; i < size; i++){
                testrun(buf[i] == 0, "expect nothinǵ written.");
        }
        testrun(open == min - 1, "open unchanged");

        // check write to min
        open   = min;
        result = testrun_copyright_write_to(buf, &open, &statement);
        testrun(result);
        testrun(result[0] == 0);
        testrun(strncmp(buf, expect, strlen(expect)) == 0);
        testrun(result  == buf  + strlen(expect));
        testrun(open    == 1);

        // check try to write behind, but buffer is to small
        open   = min - 1;
        result = testrun_copyright_write_to(result,&open, &statement);

        testrun(!result, "buffer to small, nothing written.");
        for (size_t i = 0; i < size; i++){
                if (i < (min - 1)){
                        testrun(buf[i] != 0, "written in run 1");
                } else {
                       testrun(buf[i] == 0, "expect nothinǵ written in run 2");
                }
        }
        testrun(open == min - 1, "open unchanged");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_to_string(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 200;
        char expect[size];
        char *result = NULL;

        statement = (testrun_copyright) {
                .intro          = "i",
                .year           = "y",
                .owner          = NULL,
                .note           = NULL,
                .text           = "t",
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_to_string(NULL));
        testrun(NULL == testrun_copyright_to_string(&statement),
                "statement invalid");

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "1 2 3" TESTRUN_LINEEND
                                "4"     TESTRUN_LINEEND);
        result = testrun_copyright_to_string(&statement);
        testrun(result);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);


        statement = (testrun_copyright) {
                .intro          = "intro",
                .year           = "year",
                .owner          = "owner",
                .note           = "note",
                .text           = "line1\nline2\nline3\n",
                .to_string      = testrun_copyright_to_string
        };
        snprintf(expect, size,  "intro year owner"      TESTRUN_LINEEND
                                "line1\nline2\nline3\n" TESTRUN_LINEEND
                                TESTRUN_LINEEND
                                "note" TESTRUN_LINEEND);
        result = testrun_copyright_to_string(&statement);
        testrun(result);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_gpl_statement_to_string(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 200;
        char expect[size];
        char *result = NULL;

        statement = (testrun_copyright) {
                .intro          = "i",
                .year           = "y",
                .owner          = NULL,
                .note           = NULL,
                .text           = "t",
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_gpl_statement_to_string(NULL));
        testrun(NULL == testrun_copyright_gpl_statement_to_string(&statement),
                "statement invalid");

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_gpl_statement_to_string(&statement),
                "statement gpl invalid");

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .program        = "5",
                .variant        = NULL,
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_gpl_statement_to_string(&statement),
                "statement gpl invalid");

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .program        = NULL,
                .variant        = "6",
                .to_string      = testrun_copyright_to_string
        };

        testrun(NULL == testrun_copyright_gpl_statement_to_string(&statement),
                "statement gpl invalid");

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .program        = "5",
                .variant        = "6",
                .to_string      = testrun_copyright_to_string
        };

        snprintf(expect, size,  "1 2 3%s"
                                "4%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_gpl_statement_to_string(&statement);
        testrun(result, "no TAG to replace contained.");
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);

        char *text = "4"
                TESTRUN_COPYRIGHT_TAG_PROGRAM
                TESTRUN_COPYRIGHT_TAG_PROGRAM
                TESTRUN_COPYRIGHT_TAG_PROGRAM
                TESTRUN_COPYRIGHT_TAG_PROGRAM;

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = text,
                .program        = "5",
                .variant        = "6",
                .to_string      = testrun_copyright_to_string
        };

        snprintf(expect, size,  "1 2 3%s"
                                "45555%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_gpl_statement_to_string(&statement);
        testrun(result, "replaced all tags program");
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);

        text = "4"
                TESTRUN_COPYRIGHT_TAG_VARIANT
                TESTRUN_COPYRIGHT_TAG_VARIANT
                TESTRUN_COPYRIGHT_TAG_VARIANT;

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = text,
                .program        = "5",
                .variant        = "6",
                .to_string      = testrun_copyright_to_string
        };

        snprintf(expect, size,  "1 2 3%s"
                                "4666%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_gpl_statement_to_string(&statement);
        testrun(result, "replaced all tags variant");
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);

        text = "4"
                TESTRUN_COPYRIGHT_TAG_PROGRAM
                TESTRUN_COPYRIGHT_TAG_VARIANT
                TESTRUN_COPYRIGHT_TAG_PROGRAM
                TESTRUN_COPYRIGHT_TAG_VARIANT;

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = text,
                .program        = "5",
                .variant        = "6",
                .to_string      = testrun_copyright_to_string
        };

        snprintf(expect, size,  "1 2 3%s"
                                "45656%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);
        result = testrun_copyright_gpl_statement_to_string(&statement);
        testrun(result, "replaced all tags variant and program");
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        free(result);

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      OUTPUT FORMAT RELATED FUNCTIONS                                 #OFRF
 *
 *      Special output formats, which allow preconfigured and standardized
 *      output of the copyright statements, as well as custom output defintion.
 *
 *      ------------------------------------------------------------------------
 */

int test_testrun_copyright_to_header(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 1500;
        char expect[size];
        char text[size];
        char *result = NULL;
        char *prefix = NULL;
        char *suffix = NULL;
        char *line_prefix = NULL;
        char *line_suffix = NULL;

        statement = (testrun_copyright) {
                .intro          = "1",
                .year           = "2",
                .owner          = "3",
                .note           = NULL,
                .text           = "4",
                .to_string      = testrun_copyright_to_string
        };

        testrun(!testrun_copyright_to_header(   NULL,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0));

        // -------------------------------------------------------------
        // Positive test - unframed
        // -------------------------------------------------------------

        snprintf(expect, size,  "1 2 3%s"
                                "4%s",
                                TESTRUN_LINEEND, TESTRUN_LINEEND);

        result = testrun_copyright_to_header(   &statement,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0);
        testrun(strncmp(result, expect, strlen(expect)) == 0, "unframed");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - header prefixed
        // -------------------------------------------------------------

        prefix      = "abc" TESTRUN_LINEEND;
        suffix      = NULL;
        line_prefix = NULL;
        line_suffix = NULL;

        snprintf(expect, size,  "%s1 2 3" TESTRUN_LINEEND
                                "4"       TESTRUN_LINEEND,
                                prefix);

        result = testrun_copyright_to_header(   &statement,
                                                prefix, strlen(prefix),
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0);

        // log("|%s|\n", result);

        testrun(strncmp(result, expect, strlen(expect)) == 0, "prefixed");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - header suffixed
        // -------------------------------------------------------------

        prefix      = NULL;
        suffix      = "abc" TESTRUN_LINEEND;
        line_prefix = NULL;
        line_suffix = NULL;

        snprintf(expect, size,  "1 2 3" TESTRUN_LINEEND
                                "4" TESTRUN_LINEEND
                                "%s",
                                suffix);

        result = testrun_copyright_to_header(   &statement,
                                                NULL, 0,
                                                suffix, strlen(suffix),
                                                NULL, 0,
                                                NULL, 0);

        //log("|%s|\n", result);

        testrun(strncmp(result, expect, strlen(expect)) == 0, "suffixed");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - line prefixed
        // -------------------------------------------------------------

        prefix      = NULL;
        suffix      = NULL;
        line_prefix = "#";
        line_suffix = NULL;

        snprintf(expect, size,  "%s""1 2 3" TESTRUN_LINEEND
                                "%s""4"     TESTRUN_LINEEND,
                                line_prefix, line_prefix);

        result = testrun_copyright_to_header(   &statement,
                                                NULL, 0,
                                                NULL, 0,
                                                line_prefix, strlen(line_prefix),
                                                NULL, 0);

        //log("|%s|\n", result);

        testrun(strncmp(result, expect, strlen(expect)) == 0, "line prefixed");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - line suffixed
        // -------------------------------------------------------------


        prefix      = NULL;
        suffix      = NULL;
        line_prefix = NULL;
        line_suffix = "#";

        snprintf(expect, size,  "1 2 3""%s" TESTRUN_LINEEND
                                "4""%s" TESTRUN_LINEEND,
                                line_suffix, line_suffix);

        result = testrun_copyright_to_header(   &statement,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                line_suffix, strlen(line_suffix));

        //log("|%s|\n", result);

        testrun(strncmp(result, expect, strlen(expect)) == 0, "line suffixed");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - real world
        // -------------------------------------------------------------

        snprintf(text, size,    "line1" TESTRUN_LINEEND
                                "line2" TESTRUN_LINEEND
                                "line3" TESTRUN_LINEEND
                                TESTRUN_LINEEND
                                "line4");

        statement.text = text;

        prefix      = "/**" TESTRUN_LINEEND;
        suffix      = " */" TESTRUN_LINEEND;
        line_prefix = " *  ";
        line_suffix = NULL;

        snprintf(expect, size,  "/**" TESTRUN_LINEEND
                                " *  1 2 3" TESTRUN_LINEEND
                                " *  line1" TESTRUN_LINEEND
                                " *  line2" TESTRUN_LINEEND
                                " *  line3" TESTRUN_LINEEND
                                " *"TESTRUN_LINEEND
                                " *  line4" TESTRUN_LINEEND
                                " */"TESTRUN_LINEEND);

        result = testrun_copyright_to_header(   &statement,
                                                prefix, strlen(prefix),
                                                suffix, strlen(suffix),
                                                line_prefix, strlen(line_prefix),
                                                NULL, 0);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));

        testrun(strncmp(result, expect, strlen(expect)) == 0, "reallife");
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - real world 2
        // -------------------------------------------------------------

        statement = testrun_copyright_apache_version_2(
                "2017", "MT" , "some additional note");

        prefix      = "/**" TESTRUN_LINEEND;
        suffix      = " */" TESTRUN_LINEEND;
        line_prefix = " *  ";
        line_suffix = NULL;

        snprintf(expect, size,
        "/**"
        TESTRUN_LINEEND
        " *  Copyright 2017 MT"
        TESTRUN_LINEEND
        " *"
        TESTRUN_LINEEND
        " *  Licensed under the Apache License, Version 2.0 (the \"License\");"
        TESTRUN_LINEEND
        " *  you may not use this file except in compliance with the License."
        TESTRUN_LINEEND
        " *  You may obtain a copy of the License at"
        TESTRUN_LINEEND
        " *"
        TESTRUN_LINEEND
        " *          http://www.apache.org/licenses/LICENSE-2.0"
        TESTRUN_LINEEND
        " *"
        TESTRUN_LINEEND
        " *  Unless required by applicable law or agreed to in writing, software"
        TESTRUN_LINEEND
        " *  distributed under the License is distributed on an \"AS IS\" BASIS,"
        TESTRUN_LINEEND
        " *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."
        TESTRUN_LINEEND
        " *  See the License for the specific language governing permissions and"
        TESTRUN_LINEEND
        " *  limitations under the License."
        TESTRUN_LINEEND
        " *"
        TESTRUN_LINEEND
        " *  some additional note"
        TESTRUN_LINEEND
        " */"
        TESTRUN_LINEEND);

        result = testrun_copyright_to_header(   &statement,
                                                prefix, strlen(prefix),
                                                suffix, strlen(suffix),
                                                line_prefix, strlen(line_prefix),
                                                NULL, 0);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));

        testrun(strncmp(result, expect, strlen(expect)) == 0, "reallife");
        result = testrun_string_free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_default_c_header(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 1500;
        char expect[size];
        char text[size];
        char *result = NULL;

        statement = testrun_copyright_default("2017", "NAME", "NOTE");

        snprintf(expect, size,
        "/***"  TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) 2017 NAME"       TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        NOTE" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        );

        testrun(!testrun_copyright_default_c_header(NULL, false));
        testrun(!testrun_copyright_default_c_header(NULL, true));

        result = testrun_copyright_default_c_header(&statement, false);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);


        statement = testrun_copyright_apache_version_2("YEAR", "NAME", NULL);

        snprintf(expect, size,
        "/***"  TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright YEAR NAME" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Licensed under the Apache License, Version 2.0 (the \"License\");" TESTRUN_LINEEND
        "        you may not use this file except in compliance with the License." TESTRUN_LINEEND
        "        You may obtain a copy of the License at" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "                http://www.apache.org/licenses/LICENSE-2.0" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Unless required by applicable law or agreed to in writing, software" TESTRUN_LINEEND
        "        distributed under the License is distributed on an \"AS IS\" BASIS," TESTRUN_LINEEND
        "        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied." TESTRUN_LINEEND
        "        See the License for the specific language governing permissions and" TESTRUN_LINEEND
        "        limitations under the License." TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        );

        testrun(!testrun_copyright_default_c_header(NULL, false));
        testrun(!testrun_copyright_default_c_header(NULL, true));

        result = testrun_copyright_default_c_header(&statement, false);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);


        statement = testrun_copyright_default("2017", "NAME", "NOTE");

        snprintf(expect, size,
        "/***"  TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) 2017 NAME"       TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        NOTE" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        );

        result = testrun_copyright_default_c_header(&statement, true);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_default_shell_header(){

        testrun_copyright statement;
        bzero(&statement, sizeof(testrun_copyright));

        size_t size = 1500;
        char expect[size];
        char text[size];
        char *result = NULL;

        statement = testrun_copyright_default("2017", "NAME", "NOTE");

        snprintf(expect, size,
        "#!/usr/bin/env bash"  TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       Copyright (c) 2017 NAME"       TESTRUN_LINEEND
        "#       All rights reserved."          TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       NOTE" TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        );


        testrun(!testrun_copyright_default_shell_header(NULL));

        result = testrun_copyright_default_shell_header(&statement);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);


        statement = testrun_copyright_apache_version_2("YEAR", "NAME", NULL);

        snprintf(expect, size,
        "#!/usr/bin/env bash"  TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       Copyright YEAR NAME" TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       Licensed under the Apache License, Version 2.0 (the \"License\");" TESTRUN_LINEEND
        "#       you may not use this file except in compliance with the License." TESTRUN_LINEEND
        "#       You may obtain a copy of the License at" TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#               http://www.apache.org/licenses/LICENSE-2.0" TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       Unless required by applicable law or agreed to in writing, software" TESTRUN_LINEEND
        "#       distributed under the License is distributed on an \"AS IS\" BASIS," TESTRUN_LINEEND
        "#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied." TESTRUN_LINEEND
        "#       See the License for the specific language governing permissions and" TESTRUN_LINEEND
        "#       limitations under the License." TESTRUN_LINEEND
        "#" TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        );

        result = testrun_copyright_default_shell_header(&statement);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      CONTENT RELATED FUNCTIONS                                       #CRF
 *
 *      Functions which create content copyright statements.
 *      NOTE: These statements are hardcoded with intention.
 *
 *      ------------------------------------------------------------------------
 */

int test_testrun_copyright_default(){

        testrun_copyright statement;

        char *intro = "Copyright (c)";
        char *text  = "All rights reserved.";

        statement = testrun_copyright_default(NULL, NULL, NULL);
        testrun(statement.intro != NULL, "check structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_default("2017", NULL, NULL);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_default("2017", "name", NULL);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);


        statement = testrun_copyright_default("2017", "name", "note");
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_default(NULL, "name", NULL);
        testrun(statement.intro != NULL, "check name only structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_default(NULL, NULL, "note");
        testrun(statement.intro != NULL, "check note only structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(strncmp(statement.note, "note",  strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_apache_version_2(){

        testrun_copyright statement;

        char *intro = "Copyright";
        char *text  =
        TESTRUN_LINEEND
        "Licensed under the Apache License, Version 2.0 (the \"License\");"
        TESTRUN_LINEEND
        "you may not use this file except in compliance with the License."
        TESTRUN_LINEEND
        "You may obtain a copy of the License at"
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        http://www.apache.org/licenses/LICENSE-2.0"
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "Unless required by applicable law or agreed to in writing, software"
        TESTRUN_LINEEND
        "distributed under the License is distributed on an \"AS IS\" BASIS,"
        TESTRUN_LINEEND
        "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."
        TESTRUN_LINEEND
        "See the License for the specific language governing permissions and"
        TESTRUN_LINEEND
        "limitations under the License.";

        statement = testrun_copyright_apache_version_2(
                NULL, NULL, NULL);
        testrun(statement.intro != NULL, "check structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);

        statement = testrun_copyright_apache_version_2(
                "2017", NULL, NULL);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_apache_version_2(
                "2017", "name", NULL);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);


        statement = testrun_copyright_apache_version_2(
                "2017", "name", "note");
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_GPL_v3(){

        testrun_copyright statement;

        char *intro   = "Copyright ";
        char *program = TESTRUN_COPYRIGHT_TAG_PROGRAM;
        char *variant = "General";
        char *string  = NULL;

        char text[1500];
        char text2[1500];
        bzero(text, 1500);
        snprintf(text, 1500,
        TESTRUN_LINEEND
        "This file is part of "
        TESTRUN_COPYRIGHT_TAG_PROGRAM
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_COPYRIGHT_TAG_PROGRAM
        " is free software: you can redistribute it and/or modify"
        TESTRUN_LINEEND
        "it under the terms of the GNU "
        TESTRUN_COPYRIGHT_TAG_VARIANT
        " Public License as published by"
        TESTRUN_LINEEND
        "the Free Software Foundation, either version 3 of the License, or"
        TESTRUN_LINEEND
        "(at your option) any later version. "
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_COPYRIGHT_TAG_PROGRAM
        " is distributed in the hope that it will be useful,"
        TESTRUN_LINEEND
        "but WITHOUT ANY WARRANTY; without even the implied warranty of"
        TESTRUN_LINEEND
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
        TESTRUN_LINEEND
        "GNU "TESTRUN_COPYRIGHT_TAG_VARIANT" Public License for more details."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "You should have received a copy of the GNU "
        TESTRUN_COPYRIGHT_TAG_VARIANT" Public License"
        TESTRUN_LINEEND
        "along with " TESTRUN_COPYRIGHT_TAG_PROGRAM
        ". If not, see <http://www.gnu.org/licenses/>.");

        statement = testrun_copyright_GPL_v3(
                NULL, NULL, NULL, NULL, 0);
        testrun(statement.intro != NULL, "check structure created.");


        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);

        statement = testrun_copyright_GPL_v3(
                "2017", NULL, NULL, NULL, 0);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);


        statement = testrun_copyright_GPL_v3(
                "2017", "name", NULL, NULL, 0);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);


        statement = testrun_copyright_GPL_v3(
                "2017", "name", "note", NULL, 0);
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);

        statement = testrun_copyright_GPL_v3(
                "2017", "name", "note", NULL, 0);
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);

        program = "TEST";
        variant = "General";
        bzero(text2, 1500);
        snprintf(text2, 1500,
        "%s %s %s"
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "This file is part of %s"
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "%s is free software: you can redistribute it and/or modify"
        TESTRUN_LINEEND
        "it under the terms of the GNU %s Public License as published by"
        TESTRUN_LINEEND
        "the Free Software Foundation, either version 3 of the License, or"
        TESTRUN_LINEEND
        "(at your option) any later version. "
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "%s is distributed in the hope that it will be useful,"
        TESTRUN_LINEEND
        "but WITHOUT ANY WARRANTY; without even the implied warranty of"
        TESTRUN_LINEEND
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
        TESTRUN_LINEEND
        "GNU %s Public License for more details."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "You should have received a copy of the GNU %s Public License"
        TESTRUN_LINEEND
        "along with %s. If not, see <http://www.gnu.org/licenses/>."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "note"
        TESTRUN_LINEEND,
        statement.intro, statement.year, statement.owner,
        program, program, variant, program, variant, variant, program);

        statement = testrun_copyright_GPL_v3(
                "2017", "name", "note", program, 0);
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);

        string = statement.to_string(&statement);
        testrun(strncmp(string,  text2,   strlen(text2))   == 0);
        free(string);

        program = "PROGRAM";
        variant = "Lesser General";
        bzero(text2, 1500);
        snprintf(text2, 1500,
        "%s %s %s"
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "This file is part of %s"
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "%s is free software: you can redistribute it and/or modify"
        TESTRUN_LINEEND
        "it under the terms of the GNU %s Public License as published by"
        TESTRUN_LINEEND
        "the Free Software Foundation, either version 3 of the License, or"
        TESTRUN_LINEEND
        "(at your option) any later version. "
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "%s is distributed in the hope that it will be useful,"
        TESTRUN_LINEEND
        "but WITHOUT ANY WARRANTY; without even the implied warranty of"
        TESTRUN_LINEEND
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
        TESTRUN_LINEEND
        "GNU %s Public License for more details."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "You should have received a copy of the GNU %s Public License"
        TESTRUN_LINEEND
        "along with %s. If not, see <http://www.gnu.org/licenses/>."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "note"
        TESTRUN_LINEEND,
        statement.intro, statement.year, statement.owner,
        program, program, variant, program, variant, variant, program);

        statement = testrun_copyright_GPL_v3(
                "2017", "name", "note", program, LESSER);
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_gpl_statement_to_string);

        string = statement.to_string(&statement);
        testrun(strncmp(string,  text2,   strlen(text2))   == 0);
        free(string);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_BSD_3Clause(){

        testrun_copyright statement;

        char *intro = "Copyright ";
        char *text =
        "All rights reserved."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "Redistribution and use in source and binary forms, with or without"
        TESTRUN_LINEEND
        "modification, are permitted provided that the following conditions are met:"
        TESTRUN_LINEEND
        "    * Redistributions of source code must retain the above copyright"
        TESTRUN_LINEEND
        "      notice, this list of conditions and the following disclaimer."
        TESTRUN_LINEEND
        "    * Redistributions in binary form must reproduce the above copyright"
        TESTRUN_LINEEND
        "      notice, this list of conditions and the following disclaimer in the"
        TESTRUN_LINEEND
        "      documentation and/or other materials provided with the distribution."
        TESTRUN_LINEEND
        "    * Neither the name of the copyright holder nor the"
        TESTRUN_LINEEND
        "      names of its contributors may be used to endorse or promote products"
        TESTRUN_LINEEND
        "      derived from this software without specific prior written permission."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND"
        TESTRUN_LINEEND
        "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED"
        TESTRUN_LINEEND
        "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE"
        TESTRUN_LINEEND
        "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY"
        TESTRUN_LINEEND
        "DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES"
        TESTRUN_LINEEND
        "(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;"
        TESTRUN_LINEEND
        "LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND"
        TESTRUN_LINEEND
        "ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT"
        TESTRUN_LINEEND
        "(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS"
        TESTRUN_LINEEND
        "SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.";

        statement = testrun_copyright_BSD_3Clause(
                NULL, NULL, NULL);
        testrun(statement.intro != NULL, "check structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);

        statement = testrun_copyright_BSD_3Clause(
                "2017", NULL, NULL);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_BSD_3Clause(
                "2017", "name", NULL);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);


        statement = testrun_copyright_BSD_3Clause(
                "2017", "name", "note");
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);

        // log("%s", statement.to_string(&statement));

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_BSD_2Clause(){

        testrun_copyright statement;

        char *intro = "Copyright";
        char *text =
        "All rights reserved."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "Redistribution and use in source and binary forms, with or without"
        TESTRUN_LINEEND
        "modification, are permitted provided that the following conditions are met:"
        TESTRUN_LINEEND
        "1. Redistributions of source code must retain the above copyright notice, this"
        TESTRUN_LINEEND
        "   list of conditions and the following disclaimer."
        TESTRUN_LINEEND
        "2. Redistributions in binary form must reproduce the above copyright notice,"
        TESTRUN_LINEEND
        "   this list of conditions and the following disclaimer in the documentation"
        TESTRUN_LINEEND
        "   and/or other materials provided with the distribution."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND"
        TESTRUN_LINEEND
        "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED"
        TESTRUN_LINEEND
        "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE"
        TESTRUN_LINEEND
        "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR"
        TESTRUN_LINEEND
        "ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES"
        TESTRUN_LINEEND
        "(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;"
        TESTRUN_LINEEND
        "LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND"
        TESTRUN_LINEEND
        "ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT"
        TESTRUN_LINEEND
        "(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS"
        TESTRUN_LINEEND
        "SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.";

        statement = testrun_copyright_BSD_2Clause(
                NULL, NULL, NULL);
        testrun(statement.intro != NULL, "check structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);

        statement = testrun_copyright_BSD_2Clause(
                "2017", NULL, NULL);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_BSD_2Clause(
                "2017", "name", NULL);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);


        statement = testrun_copyright_BSD_2Clause(
                "2017", "name", "note");
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);

        //log("%s", statement.to_string(&statement));

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_MIT(){

        testrun_copyright statement;

        char *intro = "Copyright";
        char *text =
        TESTRUN_LINEEND
        "Permission is hereby granted, free of charge, to any person obtaining a copy"
        TESTRUN_LINEEND
        "of this software and associated documentation files (the \"Software\"), to deal"
        TESTRUN_LINEEND
        "in the Software without restriction, including without limitation the rights"
        TESTRUN_LINEEND
        "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell"
        TESTRUN_LINEEND
        "copies of the Software, and to permit persons to whom the Software is"
        TESTRUN_LINEEND
        "furnished to do so, subject to the following conditions:"
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "The above copyright notice and this permission notice shall be included in all"
        TESTRUN_LINEEND
        "copies or substantial portions of the Software."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
        TESTRUN_LINEEND
        "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
        TESTRUN_LINEEND
        "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE"
        TESTRUN_LINEEND
        "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
        TESTRUN_LINEEND
        "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
        TESTRUN_LINEEND
        "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE"
        TESTRUN_LINEEND
        "SOFTWARE.";

        statement = testrun_copyright_MIT(
                NULL, NULL, NULL);
        testrun(statement.intro != NULL, "check structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,
                TESTRUN_COPYRIGHT_TAG_YEAR,
                strlen(TESTRUN_COPYRIGHT_TAG_YEAR)) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);

        statement = testrun_copyright_MIT(
                "2017", NULL, NULL);
        testrun(statement.intro != NULL, "check year structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner,
                TESTRUN_COPYRIGHT_TAG_OWNER,
                strlen(TESTRUN_COPYRIGHT_TAG_OWNER)) == 0);
        testrun(statement.note == NULL);
        testrun(statement.to_string == testrun_copyright_to_string);


        statement = testrun_copyright_MIT(
                "2017", "name", NULL);
        testrun(statement.intro != NULL, "check year, name structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(statement.note == NULL);


        statement = testrun_copyright_MIT(
                "2017", "name", "note");
        testrun(statement.intro != NULL, "check full structure created.");
        testrun(strncmp(statement.intro, intro,  strlen(intro))  == 0);
        testrun(strncmp(statement.text,  text,   strlen(text))   == 0);
        testrun(strncmp(statement.year,  "2017", strlen("2017")) == 0);
        testrun(strncmp(statement.owner, "name", strlen("name")) == 0);
        testrun(strncmp(statement.note,  "note", strlen("note")) == 0);
        testrun(statement.to_string == testrun_copyright_to_string);

        //log("%s", statement.to_string(&statement));

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                   #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int testcases_structure() {

        testrun_init();
        testrun_test(test_testrun_copyright_structure_is_valid);
        testrun_test(test_testrun_copyright_calculate_length);
        testrun_test(test_testrun_copyright_write_to);
        testrun_test(test_testrun_copyright_to_string);
        testrun_test(test_testrun_copyright_gpl_statement_to_string);

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int testcases_output_formating() {

        testrun_init();
        testrun_test(test_testrun_copyright_to_header);
        testrun_test(test_testrun_copyright_default_c_header);
        testrun_test(test_testrun_copyright_default_shell_header);

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int testcases_content() {

        testrun_init();
        testrun_test(test_testrun_copyright_default);
        testrun_test(test_testrun_copyright_apache_version_2);
        testrun_test(test_testrun_copyright_GPL_v3);
        testrun_test(test_testrun_copyright_BSD_3Clause);
        testrun_test(test_testrun_copyright_BSD_2Clause);
        testrun_test(test_testrun_copyright_MIT);

        return testrun_counter;
}

/*----------------------------------------------------------------------------*/

int all_tests() {

       testrun_init();

       int r = 0;

        r = testcases_structure();
        if (r < 0)
                return -1;

        testrun_counter += r;

        r = testcases_content();
        if (r < 0)
                return -1;

       testrun_counter += r;

       r = testcases_output_formating();
        if (r < 0)
                return -1;

       testrun_counter += r;

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
