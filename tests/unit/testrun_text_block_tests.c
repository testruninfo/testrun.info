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

        @file           testrun_text_block_tests.c
        @author         Markus Toepfer
        @date           2017-11-26

        @ingroup        testrun_lib

        @brief          Test of standard text block elements.


       ------------------------------------------------------------------------
*/



#include "../../include/testrun.h"
#include "../../src/testrun_text_block.c"


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

int test_testrun_text_block_splitline(){

        size_t size = 1500;
        char expect[size];

        char *result = NULL;

        testrun(!testrun_text_block_splitline(0, 0, true));

        testrun(!testrun_text_block_splitline(9, 10, false),
                "line <= indent +1");

        testrun(!testrun_text_block_splitline(9, 14, true),
                "line <= indent + 4 + 1, commented");

        testrun(!testrun_text_block_splitline(0, 2, true),
                "line <= indent + 4 + 1, commented");

        result = testrun_text_block_splitline(0, 2, false);
        testrun(result);
        testrun(strlen(result) == 1);
        testrun(result[0] == '-');
        result = testrun_string_free(result);

        snprintf(expect, size, "--");
        result = testrun_text_block_splitline(0, 3, false);
        testrun(result);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        snprintf(expect, size, "---");
        result = testrun_text_block_splitline(0, 4, false);
        testrun(result);
        testrun(strlen(result) == 3);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        snprintf(expect, size, "----");
        result = testrun_text_block_splitline(0, 5, false);
        testrun(result);
        testrun(strlen(result) == 4);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        testrun(!testrun_text_block_splitline(0, 5, true));

        snprintf(expect, size, "/*-*/");
        result = testrun_text_block_splitline(0, 6, true);
        testrun(result);
        testrun(strlen(result) == 5);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        snprintf(expect, size, "-----");
        result = testrun_text_block_splitline(0, 6, false);
        testrun(result);
        testrun(strlen(result) == 5);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        testrun(!testrun_text_block_splitline(1, 6, true));
        snprintf(expect, size, " ----");
        result = testrun_text_block_splitline(1, 6, false);
        testrun(result);
        testrun(strlen(result) == 5);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        snprintf(expect, size, " -----");
        result = testrun_text_block_splitline(1, 7, false);
        testrun(result);
        testrun(strlen(result) == 6);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        snprintf(expect, size, "/* -*/");
        result = testrun_text_block_splitline(1, 7, true);
        testrun(result);
        testrun(strlen(result) == 6);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        snprintf(expect, size, "  -----");
        result = testrun_text_block_splitline(2, 8, false);
        testrun(result);
        testrun(strlen(result) == 7);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        snprintf(expect, size, "/*  -*/");
        result = testrun_text_block_splitline(2, 8, true);
        testrun(result);
        testrun(strlen(result) == 7);
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_comment_header(){

        size_t size = 2000;
        char expect[size];
        bzero(expect, size);

        char *result = NULL;

        snprintf(expect, size,
                "/*"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      [NAME]                                                          #[TAG]"   TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " */"   TESTRUN_LINEEND);
        result = testrun_text_block_comment_header(NULL, NULL);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        snprintf(expect, size,
                "/*"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      abc                                                             #1234567"   TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " */"   TESTRUN_LINEEND);
        result = testrun_text_block_comment_header("abc", "1234567");
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);


        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int test_testrun_text_block_header_body() {

        char *expect = NULL;
        char *result = NULL;

        testrun(!testrun_text_block_header_body(NULL));

        expect =        "#ifndef name_h\n"
                        "#define name_h\n"
                        "\n"
                        "#endif /* name_h */\n";

        result = testrun_text_block_header_body("name");
        testrun(strncmp(expect, result, strlen(result)) == 0);
        free(result);

        expect =        "#ifndef test_h\n"
                        "#define test_h\n"
                        "\n"
                        "#endif /* test_h */\n";

        result = testrun_text_block_header_body("test");
        testrun(strncmp(expect, result, strlen(result)) == 0);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_source_body() {

        char *expect = NULL;
        char *result = NULL;
        char *name   = NULL;

        testrun(!testrun_text_block_source_body(NULL));

        name   = "../include/name.h";
        expect = "#include \"../include/name.h\"" TESTRUN_LINEEND;
        result = testrun_text_block_source_body(name);
        testrun(strncmp(expect, result, strlen(result)) == 0);
        free(result);

        name   = "../include/test.h";
        expect = "#include \"../include/test.h\"" TESTRUN_LINEEND;
        result = testrun_text_block_source_body(name);
        testrun(strncmp(expect, result, strlen(result)) == 0);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_test_body(){

        size_t size = 2000;
        char expect[size];
        bzero(expect, size);

        char *path_src     = "../../src/XXXXXXX.c";
        char *path_testrun = "../tools/testrun.h";

        testrun(!testrun_text_block_test_body(NULL,     NULL));
        testrun(!testrun_text_block_test_body(path_src, NULL));
        testrun(!testrun_text_block_test_body(NULL,     path_testrun));

        char *result = NULL;

        snprintf(expect, size,
                TESTRUN_LINEEND
                "#include \"../tools/testrun.h\""               TESTRUN_LINEEND
                "#include \"../../src/XXXXXXX.c\""              TESTRUN_LINEEND
                TESTRUN_LINEEND
                "/*"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      TEST HELPER                                                     #HELPER"   TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " */"   TESTRUN_LINEEND
                TESTRUN_LINEEND
                "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
                TESTRUN_LINEEND
                "/*"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      TEST CASES                                                      #CASES"    TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " */"   TESTRUN_LINEEND
                TESTRUN_LINEEND
                "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
                TESTRUN_LINEEND
                "/**"                                           TESTRUN_LINEEND
                "int test_ ... (){"                             TESTRUN_LINEEND
                "        testrun(true);"                        TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        return testrun_log_success();"         TESTRUN_LINEEND
                "}"                                             TESTRUN_LINEEND
                "*/"                                            TESTRUN_LINEEND
                TESTRUN_LINEEND
                "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
                TESTRUN_LINEEND
                "/*"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      TEST CLUSTER                                                    #CLUSTER" TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " */"   TESTRUN_LINEEND
                TESTRUN_LINEEND
                "int all_tests() {"                             TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        testrun_init();"                       TESTRUN_LINEEND
                "        //testrun_test(test_);"                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        return testrun_counter;"               TESTRUN_LINEEND
                "}"TESTRUN_LINEEND
                TESTRUN_LINEEND
                "/*"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      TEST EXECUTION                                                  #EXEC"    TESTRUN_LINEEND
                " *"    TESTRUN_LINEEND
                " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
                " */"   TESTRUN_LINEEND
                TESTRUN_LINEEND
                "testrun_run(all_tests);"                       TESTRUN_LINEEND);

        result = testrun_text_block_test_body(path_src, path_testrun);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        path_src     = "../../src/XXXXXXX";
        path_testrun = "../tools/testrun.h";
        testrun(!testrun_text_block_test_body(path_src, path_testrun),
                "source extension not .c");

        path_src     = "../../src/XXXXXXX.c";
        path_testrun = "../tools/testrun";
        testrun(!testrun_text_block_test_body(path_src, path_testrun),
                "testrun extension not .h");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_write_space(){

        size_t size = 2000;
        char result[size];
        bzero(result, size);

        testrun(!testrun_text_block_write_space(NULL,   NULL, 0));
        testrun(!testrun_text_block_write_space(result, NULL, 10));
        testrun(!testrun_text_block_write_space(NULL,   &size, 10));
        testrun(!testrun_text_block_write_space(result, &size, size+1));
        testrun(testrun_text_block_write_space(result,  &size, 0));

        testrun(strlen(result) == 0);
        testrun(testrun_text_block_write_space(result,  &size, 1));
        testrun(strlen(result) == 1);
        testrun(size == 1999);
        testrun(result[0] == ' ');

        testrun(testrun_text_block_write_space(result,  &size, 123));
        testrun(strlen(result) == 123);
        testrun(size == 1999 - 123);
        for (size_t i = 0; i < 123; i++){
                testrun(result[i] == ' ');
        }

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_write_docu_line(){

        size_t size = 2000;
        size_t open = size;
        char buffer[size];
        bzero(buffer, size);
        char expect[size];
        bzero(expect, size);

        size_t indent = 0;
        size_t offset = 0;

        char *tag     = "tag";
        char *content = "content";
        char *lineend = "\n";

        char *pointer = buffer;

        testrun(!testrun_text_block_write_docu_line(
                NULL,   NULL,
                0,0,
                NULL, 0,
                NULL, 0,
                NULL, 0));

        testrun(!testrun_text_block_write_docu_line(
                NULL,   &open,
                0,0,
                tag,     strlen(tag),
                content, strlen(content),
                lineend, strlen(lineend)), "NO BUFFER");

        testrun(!testrun_text_block_write_docu_line(
                &pointer,   NULL,
                0,0,
                tag,     strlen(tag),
                content, strlen(content),
                lineend, strlen(lineend)), "NO SIZE");

        testrun(!testrun_text_block_write_docu_line(
                &pointer,   &open,
                0,0,
                NULL,     strlen(tag),
                content, strlen(content),
                lineend, strlen(lineend)), "NO TAG");

        // -------------------------------------------------------------
        // Check space (indent / offset)
        // -------------------------------------------------------------

        snprintf(expect, size, "tag content\n");
        testrun(testrun_text_block_write_docu_line(
                &pointer,   &open,
                0,0,
                tag,     strlen(tag),
                content, strlen(content),
                lineend, strlen(lineend)));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  buffer, strlen(buffer));
        testrun(pointer[0] == '\0', "point to stringend");
        testrun(pointer == buffer + strlen(buffer));
        testrun(strlen(buffer) == strlen(expect));
        testrun(strncmp(buffer, expect, strlen(expect)) == 0);
        testrun(open == size - strlen(buffer));


        open = size;
        bzero(buffer, open);
        pointer = buffer;
        snprintf(expect, size, " tag content\n");
        testrun(testrun_text_block_write_docu_line(
                &pointer,   &open,
                1,0,
                tag,     strlen(tag),
                content, strlen(content),
                lineend, strlen(lineend)));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  buffer, strlen(buffer));
        testrun(pointer[0] == '\0', "point to stringend");
        testrun(pointer == buffer + strlen(buffer));
        testrun(strlen(buffer) == strlen(expect));
        testrun(strncmp(buffer, expect, strlen(expect)) == 0);
        testrun(open == size - strlen(buffer));
        open = size;
        bzero(buffer, open);

        open = size;
        bzero(buffer, open);
        pointer = buffer;
        snprintf(expect, size, "tag content\n");
        testrun(testrun_text_block_write_docu_line(
                &pointer,   &open,
                0,1,
                tag,     strlen(tag),
                content, strlen(content),
                lineend, strlen(lineend)));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  buffer, strlen(buffer));
        testrun(pointer[0] == '\0', "point to stringend");
        testrun(pointer == buffer + strlen(buffer));
        testrun(strlen(buffer) == strlen(expect));
        testrun(strncmp(buffer, expect, strlen(expect)) == 0);
        testrun(open == size - strlen(buffer));

        open = size;
        bzero(buffer, open);
        pointer = buffer;
        snprintf(expect, size, "tag  content\n");
        testrun(testrun_text_block_write_docu_line(
                &pointer,   &open,
                0,5,
                tag,     strlen(tag),
                content, strlen(content),
                lineend, strlen(lineend)));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  buffer, strlen(buffer));
        testrun(pointer[0] == '\0', "point to stringend");
        testrun(pointer == buffer + strlen(buffer));
        testrun(strlen(buffer) == strlen(expect));
        testrun(strncmp(buffer, expect, strlen(expect)) == 0);
        testrun(open == size - strlen(buffer));
        open = size;
        bzero(buffer, open);

        // -------------------------------------------------------------
        // Default use case
        // -------------------------------------------------------------

        open = size;
        bzero(buffer, open);
        pointer = buffer;
        snprintf(expect, size, "        tag                     content\n");
        testrun(testrun_text_block_write_docu_line(
                &pointer,   &open,
                8,4*8,
                tag,     strlen(tag),
                content, strlen(content),
                lineend, strlen(lineend)));
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  buffer, strlen(buffer));
        testrun(pointer[0] == '\0', "point to stringend");
        testrun(pointer == buffer + strlen(buffer));
        testrun(strlen(buffer) == strlen(expect));
        testrun(strncmp(buffer, expect, strlen(expect)) == 0);
        testrun(open == size - strlen(buffer));
        open = size;
        bzero(buffer, open);

        // -------------------------------------------------------------
        // Check length pattern
        // -------------------------------------------------------------

        open = size;
        bzero(buffer, open);
        pointer = buffer;

        indent  = 0;
        offset  = 0;
        tag     = "123";
        content = "4567";
        lineend = "89";

        // required size is strlen(tag) + strlen(content) + strlen(lineend)
        // + 1 (minimum offset) + 1 point to next byte == 11 bytes

        snprintf(expect, size, "123 456789");

        open    = 9;
        testrun(!testrun_text_block_write_docu_line(
                &pointer,   &open,
                indent,  offset,
                tag,     strlen(tag),
                content, strlen(content),
                lineend, strlen(lineend)), "size == linecontent");

        // will write nothing
        testrun(strlen(buffer) == 0);

        open    = 10;
        testrun(testrun_text_block_write_docu_line(
                &pointer,   &open,
                indent,  offset,
                tag,     strlen(tag),
                content, strlen(content),
                lineend, strlen(lineend)), "size == linecontent + 1 (offset 0)");
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  buffer, strlen(buffer));
        testrun(pointer[0] == '\0', "point to stringend");
        testrun(pointer == buffer + strlen(buffer));
        testrun(strlen(buffer) == strlen(expect));
        testrun(strncmp(buffer, expect, strlen(expect)) == 0);
        testrun(open == 0);
        open = size;
        bzero(buffer, open);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_c_header_documentation(){

        size_t size = 2000;
        char expect[size];
        bzero(expect, size);

        char *result = NULL;
        char *module = "test";
        testrun_config config = testrun_config_default();

        char *date =  testrun_time_string(TESTRUN_SCOPE_DAY);

        // -------------------------------------------------------------
        // Positive
        // -------------------------------------------------------------

        snprintf(expect, size,
                "/**" TESTRUN_LINEEND
                "        @file           %s.h"                  TESTRUN_LINEEND
                "        @author         [AUTHOR]"              TESTRUN_LINEEND
                "        @date           %s"                    TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        @ingroup        [PROJECT]"             TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        @brief"                                TESTRUN_LINEEND
                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        ------------------------------------------------------------------------" TESTRUN_LINEEND
                "*/" TESTRUN_LINEEND,
                module, date);

        result = testrun_text_block_c_header_documentation(
                module, TESTRUN_HEADER, &config, true, NULL);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Input error
        // -------------------------------------------------------------

        testrun(!testrun_text_block_c_header_documentation(
                NULL, TESTRUN_HEADER, &config, true, NULL));

        testrun(!testrun_text_block_c_header_documentation(
                module, TESTRUN_HEADER, NULL, true, NULL));

        // -------------------------------------------------------------
        // Possitive no comment open
        // -------------------------------------------------------------

        snprintf(expect, size,
                "        @file           %s.h"                  TESTRUN_LINEEND
                "        @author         [AUTHOR]"              TESTRUN_LINEEND
                "        @date           %s"                    TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        @ingroup        [PROJECT]"             TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        @brief"                                TESTRUN_LINEEND
                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        ------------------------------------------------------------------------" TESTRUN_LINEEND
                "*/" TESTRUN_LINEEND,
                module, date);

        result = testrun_text_block_c_header_documentation(
                module, TESTRUN_HEADER, &config, false, NULL);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);


        // -------------------------------------------------------------
        // Format config
        // -------------------------------------------------------------

        config.format.extensions.c_header = ".header";

        snprintf(expect, size,
                "        @file           %s.header"             TESTRUN_LINEEND
                "        @author         [AUTHOR]"              TESTRUN_LINEEND
                "        @date           %s"                    TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        @ingroup        [PROJECT]"             TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        @brief"                                TESTRUN_LINEEND
                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        ------------------------------------------------------------------------" TESTRUN_LINEEND
                "*/" TESTRUN_LINEEND,
                module, date);

        result = testrun_text_block_c_header_documentation(
                module, TESTRUN_HEADER, &config, false, NULL);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);
        // reset
        config.format.extensions.c_header = "h";


        config.format.extensions.c_header = NULL;
        snprintf(expect, size,
                "        @file           %s(null)"              TESTRUN_LINEEND
                "        @author         [AUTHOR]"              TESTRUN_LINEEND
                "        @date           %s"                    TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        @ingroup        [PROJECT]"             TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        @brief"                                TESTRUN_LINEEND
                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        ------------------------------------------------------------------------" TESTRUN_LINEEND
                "*/" TESTRUN_LINEEND,
                module, date);

        result = testrun_text_block_c_header_documentation(
                module, TESTRUN_HEADER, &config, false, NULL);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);
        // reset
        config.format.extensions.c_header = ".h";

        config.format.line_end = "\r\n";
        snprintf(expect, size,
                "        @file           %s.h\r\n"
                "        @author         [AUTHOR]\r\n"
                "        @date           %s\r\n"
                "\r\n"
                "        @ingroup        [PROJECT]\r\n"
                "\r\n"
                "        @brief\r\n"
                "\r\n"
                "\r\n"
                "        ------------------------------------------------------------------------\r\n"
                "*/\r\n" ,
                module, date);

        result = testrun_text_block_c_header_documentation(
                module, TESTRUN_HEADER, &config, false, NULL);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);
        // reset
        config.format.line_end = "\n";


        config.format.line_length = 5;
        testrun(!testrun_text_block_c_header_documentation(
                module, TESTRUN_HEADER, &config, false, NULL), "linelength too small");
        config.format.line_length = 80;

        config.format.indent_c = 2;
        snprintf(expect, size,
                "  @file                 %s.h"              TESTRUN_LINEEND
                "  @author               [AUTHOR]"              TESTRUN_LINEEND
                "  @date                 %s"                    TESTRUN_LINEEND
                TESTRUN_LINEEND
                "  @ingroup              [PROJECT]"             TESTRUN_LINEEND
                TESTRUN_LINEEND
                "  @brief                text"                 TESTRUN_LINEEND
                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "  ------------------------------------------------------------------------------" TESTRUN_LINEEND
                "*/" TESTRUN_LINEEND,
                module, date);

        result = testrun_text_block_c_header_documentation(
                module, TESTRUN_HEADER, &config, false, "text");

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);
        // reset
        config.format.indent_c = 8;

        config.format.indent_c = 2;
        config.format.offset_docu = 10;
        snprintf(expect, size,
                "  @file   %s.h"             TESTRUN_LINEEND
                "  @author [AUTHOR]"         TESTRUN_LINEEND
                "  @date   %s"               TESTRUN_LINEEND
                TESTRUN_LINEEND
                "  @ingroup [PROJECT]"       TESTRUN_LINEEND
                TESTRUN_LINEEND
                "  @brief  text"             TESTRUN_LINEEND
                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "  ------------------------------------------------------------------------------" TESTRUN_LINEEND
                "*/" TESTRUN_LINEEND,
                module, date);

        result = testrun_text_block_c_header_documentation(
                module, TESTRUN_HEADER, &config, false, "text");

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);
        // reset
        config.format.offset_docu = 3*8;
        config.format.indent_c = 8;

        date = testrun_string_free(date);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_write_shell_commented(){

        size_t size = 2000;
        char *result = calloc(size, sizeof(char));
        char *pointer = result;

        char *string = "test";

        testrun(!testrun_text_block_write_shell_commented(
                NULL, NULL, NULL));
        testrun(!testrun_text_block_write_shell_commented(
                &pointer, NULL, string));
        testrun(!testrun_text_block_write_shell_commented(
                NULL, &size, string));


        testrun(strlen(result) == 0);
        testrun(testrun_text_block_write_shell_commented(
                &pointer,  &size, NULL));
        testrun(strlen(result) == 1);
        testrun(size == 1999);
        testrun(result[0] == '#');

        pointer = result;
        size = 2000;
        testrun(testrun_text_block_write_shell_commented(
                &pointer,  &size, "test"));
        testrun(strlen(result) == 5);
        testrun(size == 1995);
        testrun(strncmp(result, "#test", 5) == 0);

        free(result);
        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int test_testrun_text_block_sh_header_documentation(){

        size_t size = 2000;
        char expect[size];
        bzero(expect, size);

        char *result = NULL;
        char *module = "test";
        testrun_config config = testrun_config_default();

        char *date =  testrun_time_string(TESTRUN_SCOPE_DAY);

        // -------------------------------------------------------------
        // Positive
        // -------------------------------------------------------------

        snprintf(expect, size,
        "#"                                            TESTRUN_LINEEND
        "#       File            %s"                   TESTRUN_LINEEND
        "#       Authors         [AUTHOR]"             TESTRUN_LINEEND
        "#       Date            %s"                   TESTRUN_LINEEND
        "#"                                            TESTRUN_LINEEND
        "#       Project         [PROJECT]"            TESTRUN_LINEEND
        "#"                                            TESTRUN_LINEEND
        "#       Description"                          TESTRUN_LINEEND
        "#"                                            TESTRUN_LINEEND
        "#       Usage"                                TESTRUN_LINEEND
        "#"                                            TESTRUN_LINEEND
        "#       Dependencies"                         TESTRUN_LINEEND
        "#"                                            TESTRUN_LINEEND
        "#       Last changed    %s"                   TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        module, date, date);

        result = testrun_text_block_sh_header_documentation(
                module, &config, NULL, NULL, NULL);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        testrun(!testrun_text_block_sh_header_documentation(
                NULL, NULL, NULL, NULL, NULL));
        testrun(!testrun_text_block_sh_header_documentation(
                module, NULL, NULL, NULL, NULL));
        testrun(!testrun_text_block_sh_header_documentation(
                NULL, &config, NULL, NULL, NULL));

        // -------------------------------------------------------------
        // Config changes
        // -------------------------------------------------------------

        config.author = "test";
        config.project.name = "some name";

        snprintf(expect, size,
        "#"                                            TESTRUN_LINEEND
        "#       File            %s"                    TESTRUN_LINEEND
        "#       Authors         test"                 TESTRUN_LINEEND
        "#       Date            %s"                   TESTRUN_LINEEND
        "#"                                            TESTRUN_LINEEND
        "#       Project         some name"            TESTRUN_LINEEND
        "#"                                            TESTRUN_LINEEND
        "#       Description"                          TESTRUN_LINEEND
        "#"                                            TESTRUN_LINEEND
        "#       Usage"                                TESTRUN_LINEEND
        "#"                                            TESTRUN_LINEEND
        "#       Dependencies"                         TESTRUN_LINEEND
        "#"                                            TESTRUN_LINEEND
        "#       Last changed    %s"                   TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        module, date, date);

        result = testrun_text_block_sh_header_documentation(
                module, &config, NULL, NULL, NULL);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Check with paramenter (unformated)
        // -------------------------------------------------------------

        config.author = "test";
        config.project.name = "some name";

        snprintf(expect, size,
        "#"                                             TESTRUN_LINEEND
        "#       File            %s"                    TESTRUN_LINEEND
        "#       Authors         test"                  TESTRUN_LINEEND
        "#       Date            %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Project         some name"             TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Description     description text"      TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Usage           usage text"            TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Dependencies    some dependencies"     TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Last changed    %s"                    TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        module, date, date);

        result = testrun_text_block_sh_header_documentation(
                module, &config, "description text", "usage text", "some dependencies");

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        date = testrun_string_free(date);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_script(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        char *date =  testrun_time_string(TESTRUN_SCOPE_DAY);

        char *result = NULL;
        char *module = "test";
        testrun_config config = testrun_config_default();

        // -------------------------------------------------------------
        // Check default
        // -------------------------------------------------------------

        result = testrun_text_block_script(&config, NULL, NULL, NULL, NULL);
        snprintf(expect, size,
        "#!/usr/bin/env bash"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]"TESTRUN_LINEEND
        "#       All rights reserved."                  TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       File            %s"                    TESTRUN_LINEEND
        "#       Authors         %s"                    TESTRUN_LINEEND
        "#       Date            %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Project         %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Description"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Usage"                                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Dependencies"                          TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Last changed    %s"                    TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        "[PROJECT]","[AUTHOR]",date, "[PROJECT]", date);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Check configured
        // -------------------------------------------------------------

        config.project.name = "shellscript";
        config.author       = "author name";
        config.copyright.year  = "1888";
        config.copyright.owner = "owner";

        result = testrun_text_block_script(&config, NULL, NULL, NULL, NULL);
        snprintf(expect, size,
        "#!/usr/bin/env bash"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Copyright (c) 1888 owner"              TESTRUN_LINEEND
        "#       All rights reserved."                  TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       File            %s"                    TESTRUN_LINEEND
        "#       Authors         %s"                    TESTRUN_LINEEND
        "#       Date            %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Project         %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Description"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Usage"                                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Dependencies"                          TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Last changed    %s"                    TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        "shellscript","author name",date, "shellscript", date);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Description
        // -------------------------------------------------------------

        config.project.name = "shellscript";
        config.author       = "author";
        config.copyright.year  = "1888";
        config.copyright.owner = "owner";

        char *description = "This is a one line description.";

        result = testrun_text_block_script(&config, description, NULL, NULL, NULL);
        snprintf(expect, size,
        "#!/usr/bin/env bash"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Copyright (c) 1888 owner"              TESTRUN_LINEEND
        "#       All rights reserved."                  TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       File            shellscript"           TESTRUN_LINEEND
        "#       Authors         author"                TESTRUN_LINEEND
        "#       Date            %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Project         shellscript"           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Description     %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Usage"                                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Dependencies"                          TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Last changed    %s"                    TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        date, description, date);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // NOTE keep in mind, that we have automated whitespace cleaning enabled
        // so something like " test \n" will fail the test, based on the space
        description = "This\n is\n a\n multi\n line\n description.\n";

        result = testrun_text_block_script(&config, description, NULL, NULL, NULL);
        snprintf(expect, size,
        "#!/usr/bin/env bash"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Copyright (c) 1888 owner"              TESTRUN_LINEEND
        "#       All rights reserved."                  TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       File            shellscript"           TESTRUN_LINEEND
        "#       Authors         author"                TESTRUN_LINEEND
        "#       Date            %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Project         shellscript"           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Description     %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Usage"                                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Dependencies"                          TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Last changed    %s"                    TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        date, description, date);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // Fail non whitespace clean expectation
        description = "This    \n is  \n a  \n wrong \n expectation .   \n";

        result = testrun_text_block_script(&config, description, NULL, NULL, NULL);
        snprintf(expect, size,
        "#!/usr/bin/env bash"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Copyright (c) 1888 owner"              TESTRUN_LINEEND
        "#       All rights reserved."                  TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       File            shellscript"           TESTRUN_LINEEND
        "#       Authors         author"                TESTRUN_LINEEND
        "#       Date            %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Project         shellscript"           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Description     %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Usage"                                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Dependencies"                          TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Last changed    %s"                    TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        date, description, date);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) != strlen(result));
        result = testrun_string_free(result);


        // -------------------------------------------------------------
        // Usage
        // -------------------------------------------------------------

        config.project.name = "shellscript";
        config.author       = "author";
        config.copyright.year  = "1888";
        config.copyright.owner = "owner";

        char *usage = "This is a one line usage.";

        result = testrun_text_block_script(&config, NULL, usage, NULL, NULL);
        snprintf(expect, size,
        "#!/usr/bin/env bash"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Copyright (c) 1888 owner"              TESTRUN_LINEEND
        "#       All rights reserved."                  TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       File            shellscript"           TESTRUN_LINEEND
        "#       Authors         author"                TESTRUN_LINEEND
        "#       Date            %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Project         shellscript"           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Description"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Usage           %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Dependencies"                          TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Last changed    %s"                    TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        date, usage, date);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Dependencies
        // -------------------------------------------------------------

        config.project.name = "shellscript";
        config.author       = "author";
        config.copyright.year  = "1888";
        config.copyright.owner = "owner";

        char *dependencies = "This are dependencies";

        result = testrun_text_block_script(&config, NULL, NULL, dependencies, NULL);
        snprintf(expect, size,
        "#!/usr/bin/env bash"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Copyright (c) 1888 owner"              TESTRUN_LINEEND
        "#       All rights reserved."                  TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       File            shellscript"           TESTRUN_LINEEND
        "#       Authors         author"                TESTRUN_LINEEND
        "#       Date            %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Project         shellscript"           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Description"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Usage"                                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Dependencies    %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Last changed    %s"                    TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        date, dependencies, date);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Content
        // -------------------------------------------------------------

        config.project.name = "shellscript";
        config.author       = "author";
        config.copyright.year  = "1888";
        config.copyright.owner = "owner";

        char *content = "This is some content.";

        result = testrun_text_block_script(&config, NULL, NULL, NULL, content);
        snprintf(expect, size,
        "#!/usr/bin/env bash"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Copyright (c) 1888 owner"              TESTRUN_LINEEND
        "#       All rights reserved."                  TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       File            shellscript"           TESTRUN_LINEEND
        "#       Authors         author"                TESTRUN_LINEEND
        "#       Date            %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Project         shellscript"           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Description"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Usage"                                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Dependencies"                          TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Last changed    %s"                    TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "%s"
        TESTRUN_LINEEND,
        date, date, content);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);


        // -------------------------------------------------------------
        // Check offset replacement
        // -------------------------------------------------------------

        description     = "Line1\n"TESTRUN_TAG_OFFSET"Line2\n"TESTRUN_TAG_OFFSET"Line3";
        usage           = "Line1\n"TESTRUN_TAG_OFFSET"Line2\n"TESTRUN_TAG_OFFSET"Line3";
        dependencies    = "Line1\n"TESTRUN_TAG_OFFSET"Line2\n"TESTRUN_TAG_OFFSET"Line3";
        content         = "Line1\n"TESTRUN_TAG_OFFSET"Line2\n"TESTRUN_TAG_OFFSET"Line3";

        result = testrun_text_block_script(&config, description, usage, dependencies, content);
        snprintf(expect, size,
        "#!/usr/bin/env bash"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Copyright (c) 1888 owner"              TESTRUN_LINEEND
        "#       All rights reserved."                  TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       File            shellscript"           TESTRUN_LINEEND
        "#       Authors         author"                TESTRUN_LINEEND
        "#       Date            %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Project         shellscript"           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Description     Line1"                 TESTRUN_LINEEND
        "#                       Line2"                 TESTRUN_LINEEND
        "#                       Line3"                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Usage           Line1"                 TESTRUN_LINEEND
        "#                       Line2"                 TESTRUN_LINEEND
        "#                       Line3"                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Dependencies    Line1"                 TESTRUN_LINEEND
        "#                       Line2"                 TESTRUN_LINEEND
        "#                       Line3"                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Last changed    %s"                    TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "Line1"                                         TESTRUN_LINEEND
        "#                       Line2"                 TESTRUN_LINEEND
        "#                       Line3"                 TESTRUN_LINEEND,
        date, date);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Check END replacement
        // -------------------------------------------------------------

        description     =       "Line1"TESTRUN_TAG_END TESTRUN_TAG_OFFSET
                                "Line2"TESTRUN_TAG_END TESTRUN_TAG_OFFSET
                                "Line3";

        usage           =       "Line1"TESTRUN_TAG_END TESTRUN_TAG_OFFSET
                                "Line2"TESTRUN_TAG_END TESTRUN_TAG_OFFSET
                                "Line3";

        dependencies    =       "Line1"TESTRUN_TAG_END TESTRUN_TAG_OFFSET
                                "Line2"TESTRUN_TAG_END TESTRUN_TAG_OFFSET
                                "Line3";

        content         =       "Line1"TESTRUN_TAG_END TESTRUN_TAG_OFFSET
                                "Line2"TESTRUN_TAG_END TESTRUN_TAG_OFFSET
                                "Line3";

        result = testrun_text_block_script(&config, description, usage, dependencies, content);
        snprintf(expect, size,
        "#!/usr/bin/env bash"                           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Copyright (c) 1888 owner"              TESTRUN_LINEEND
        "#       All rights reserved."                  TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       File            shellscript"           TESTRUN_LINEEND
        "#       Authors         author"                TESTRUN_LINEEND
        "#       Date            %s"                    TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Project         shellscript"           TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Description     Line1"                 TESTRUN_LINEEND
        "#                       Line2"                 TESTRUN_LINEEND
        "#                       Line3"                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Usage           Line1"                 TESTRUN_LINEEND
        "#                       Line2"                 TESTRUN_LINEEND
        "#                       Line3"                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Dependencies    Line1"                 TESTRUN_LINEEND
        "#                       Line2"                 TESTRUN_LINEEND
        "#                       Line3"                 TESTRUN_LINEEND
        "#"                                             TESTRUN_LINEEND
        "#       Last changed    %s"                    TESTRUN_LINEEND
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "Line1"                                         TESTRUN_LINEEND
        "#                       Line2"                 TESTRUN_LINEEND
        "#                       Line3"                 TESTRUN_LINEEND,
        date, date);

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));
        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        date = testrun_string_free(date);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_readme(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        char *result = NULL;
        char *module = "test";
        testrun_config config = testrun_config_default();

        testrun(!testrun_text_block_readme(NULL, NULL, NULL, NULL));

        result = testrun_text_block_readme(&config, NULL, NULL, NULL);
        testrun(result);

        char *description = "123";
        char *usage       = "456";
        char *install     = "789";
        char *copyright   = config.copyright.to_string(&config.copyright);

snprintf(expect, size,
"# Project %s"TESTRUN_LINEEND
""TESTRUN_LINEEND
"This module is self supported and may be build, tested, installed and"TESTRUN_LINEEND
"run independently."TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Overview"TESTRUN_LINEEND
""TESTRUN_LINEEND
"* [Description](#description)"TESTRUN_LINEEND
"* [Usage](#usage)"TESTRUN_LINEEND
"* [Installation](#installation)"TESTRUN_LINEEND
"* [Requirements](#requirements)"TESTRUN_LINEEND
"* [Structure](#structure)"TESTRUN_LINEEND
"* [Tests](#tests)"TESTRUN_LINEEND
"* [Tips](#tips)"TESTRUN_LINEEND
"* [Copyright](#copyright)"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Description"TESTRUN_LINEEND
""TESTRUN_LINEEND
"[TAG_DESCRIPTION]"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Usage"TESTRUN_LINEEND
""TESTRUN_LINEEND
"[TAG_USAGE]"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Installation"TESTRUN_LINEEND
""TESTRUN_LINEEND
"[TAG_INSTALL]"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### build sources"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`bash"TESTRUN_LINEEND
"make"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### build documentation"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`bash"TESTRUN_LINEEND
"make documentation"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### test sources"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`bash"TESTRUN_LINEEND
"make tested"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### install binaries"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`bash"TESTRUN_LINEEND
"sudo make install"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### uninstall binaries"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`bash"TESTRUN_LINEEND
"sudo make uninstall"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Requirements"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Structure"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### Default structure of the folder:"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
"<pre>"TESTRUN_LINEEND
"."TESTRUN_LINEEND
"├── COPYRIGHT"TESTRUN_LINEEND
"├── README.MD"TESTRUN_LINEEND
"├── makefile"TESTRUN_LINEEND
"├── testrun_makefile.main"TESTRUN_LINEEND
"├── testrun_makefile.test"TESTRUN_LINEEND
"│"TESTRUN_LINEEND
"├── doxygen"TESTRUN_LINEEND
"│   ├── documentation"TESTRUN_LINEEND
"│   └── doxygen.config"TESTRUN_LINEEND
"│"TESTRUN_LINEEND
"├── docs"TESTRUN_LINEEND
"│   ├── CHANGELOG.MD"TESTRUN_LINEEND
"│   └── ..."TESTRUN_LINEEND
"│"TESTRUN_LINEEND
"├── include"TESTRUN_LINEEND
"│   ├── %s.h"TESTRUN_LINEEND
"│   └── ..."TESTRUN_LINEEND
"│"TESTRUN_LINEEND
"├── src"TESTRUN_LINEEND
"│   ├── %s.c"TESTRUN_LINEEND
"│   └── ..."TESTRUN_LINEEND
"│"TESTRUN_LINEEND
"└── tests"TESTRUN_LINEEND
"    ├── resources"TESTRUN_LINEEND
"    ├── tools"TESTRUN_LINEEND
"    │   ├── testrun_runner.h"TESTRUN_LINEEND
"    │   ├── testrun_simple_coverage_tests.sh"TESTRUN_LINEEND
"    │   ├── testrun_simple_unit_tests.sh"TESTRUN_LINEEND
"    │   ├── testrun_simple_acceptance_tests.sh"TESTRUN_LINEEND
"    │   └── testrun_simple_loc.sh"TESTRUN_LINEEND
"    ├── acceptance"TESTRUN_LINEEND
"    │   ├── ..."TESTRUN_LINEEND
"    │   └── ..."TESTRUN_LINEEND
"    └── unit"TESTRUN_LINEEND
"        ├── %s%s.c"TESTRUN_LINEEND
"        └── ..."TESTRUN_LINEEND
""TESTRUN_LINEEND
"</pre>"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Tests"TESTRUN_LINEEND
""TESTRUN_LINEEND
"All test sources of will be recompiled on each make run. That means,"TESTRUN_LINEEND
"all module tests will be created new on any change in any source file."TESTRUN_LINEEND
""TESTRUN_LINEEND
"### Test a project (all files contained in tests/unit)"TESTRUN_LINEEND
""TESTRUN_LINEEND
"Test compile and run"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
"make tested"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### Test a dedicated source file (single file of tests/unit)"TESTRUN_LINEEND
""TESTRUN_LINEEND
"To develop a test for a specific source file,"TESTRUN_LINEEND
"it may be helpful to use a separated test run."TESTRUN_LINEEND
"This may be done via an integrated make functionality."TESTRUN_LINEEND
""TESTRUN_LINEEND
"Test compile example"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
"make test testname=tests/unit/filename_tests.c"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
""TESTRUN_LINEEND
"Test compile and run in valgrind"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
"make test testname=tests/unit/filename_tests.c && valgrind ./build/test/unit/filename_tests.test"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Tips"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Copyright"TESTRUN_LINEEND
""TESTRUN_LINEEND
"%s"TESTRUN_LINEEND
,config.project.name,
config.project.name,
config.project.name,
config.project.name,
config.format.suffix.tests_source,
copyright);


        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);

        copyright = testrun_string_free(copyright);

        // -------------------------------------------------------------
        // Check parameter change
        // -------------------------------------------------------------


        config.copyright = testrun_copyright_MIT(
                "2017", "owner", NULL);
        config.project.name = "name";
        config.format.suffix.tests_source = "_unit_test";
        copyright = config.copyright.to_string(&config.copyright);
        snprintf(expect, size,
"# Project name"TESTRUN_LINEEND
""TESTRUN_LINEEND
"This module is self supported and may be build, tested, installed and"TESTRUN_LINEEND
"run independently."TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Overview"TESTRUN_LINEEND
""TESTRUN_LINEEND
"* [Description](#description)"TESTRUN_LINEEND
"* [Usage](#usage)"TESTRUN_LINEEND
"* [Installation](#installation)"TESTRUN_LINEEND
"* [Requirements](#requirements)"TESTRUN_LINEEND
"* [Structure](#structure)"TESTRUN_LINEEND
"* [Tests](#tests)"TESTRUN_LINEEND
"* [Tips](#tips)"TESTRUN_LINEEND
"* [Copyright](#copyright)"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Description"TESTRUN_LINEEND
""TESTRUN_LINEEND
"123"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Usage"TESTRUN_LINEEND
""TESTRUN_LINEEND
"456"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Installation"TESTRUN_LINEEND
""TESTRUN_LINEEND
"789"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### build sources"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`bash"TESTRUN_LINEEND
"make"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### build documentation"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`bash"TESTRUN_LINEEND
"make documentation"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### test sources"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`bash"TESTRUN_LINEEND
"make tested"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### install binaries"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`bash"TESTRUN_LINEEND
"sudo make install"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### uninstall binaries"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`bash"TESTRUN_LINEEND
"sudo make uninstall"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Requirements"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Structure"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### Default structure of the folder:"TESTRUN_LINEEND
""TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
"<pre>"TESTRUN_LINEEND
"."TESTRUN_LINEEND
"├── COPYRIGHT"TESTRUN_LINEEND
"├── README.MD"TESTRUN_LINEEND
"├── makefile"TESTRUN_LINEEND
"├── testrun_makefile.main"TESTRUN_LINEEND
"├── testrun_makefile.test"TESTRUN_LINEEND
"│"TESTRUN_LINEEND
"├── doxygen"TESTRUN_LINEEND
"│   ├── documentation"TESTRUN_LINEEND
"│   └── doxygen.config"TESTRUN_LINEEND
"│"TESTRUN_LINEEND
"├── docs"TESTRUN_LINEEND
"│   ├── CHANGELOG.MD"TESTRUN_LINEEND
"│   └── ..."TESTRUN_LINEEND
"│"TESTRUN_LINEEND
"├── include"TESTRUN_LINEEND
"│   ├── name.h"TESTRUN_LINEEND
"│   └── ..."TESTRUN_LINEEND
"│"TESTRUN_LINEEND
"├── src"TESTRUN_LINEEND
"│   ├── name.c"TESTRUN_LINEEND
"│   └── ..."TESTRUN_LINEEND
"│"TESTRUN_LINEEND
"└── tests"TESTRUN_LINEEND
"    ├── resources"TESTRUN_LINEEND
"    ├── tools"TESTRUN_LINEEND
"    │   ├── testrun_runner.h"TESTRUN_LINEEND
"    │   ├── testrun_simple_coverage_tests.sh"TESTRUN_LINEEND
"    │   ├── testrun_simple_unit_tests.sh"TESTRUN_LINEEND
"    │   ├── testrun_simple_acceptance_tests.sh"TESTRUN_LINEEND
"    │   └── testrun_simple_loc.sh"TESTRUN_LINEEND
"    ├── acceptance"TESTRUN_LINEEND
"    │   ├── ..."TESTRUN_LINEEND
"    │   └── ..."TESTRUN_LINEEND
"    └── unit"TESTRUN_LINEEND
"        ├── name_unit_test.c"TESTRUN_LINEEND
"        └── ..."TESTRUN_LINEEND
""TESTRUN_LINEEND
"</pre>"TESTRUN_LINEEND
"\\`\\`\\`"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Tests"TESTRUN_LINEEND
""TESTRUN_LINEEND
"All test sources of will be recompiled on each make run. That means,"TESTRUN_LINEEND
"all module tests will be created new on any change in any source file."TESTRUN_LINEEND
""TESTRUN_LINEEND
"### Test a project (all files contained in tests/unit)"TESTRUN_LINEEND
""TESTRUN_LINEEND
"Test compile and run"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
"make tested"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
""TESTRUN_LINEEND
"### Test a dedicated source file (single file of tests/unit)"TESTRUN_LINEEND
""TESTRUN_LINEEND
"To develop a test for a specific source file,"TESTRUN_LINEEND
"it may be helpful to use a separated test run."TESTRUN_LINEEND
"This may be done via an integrated make functionality."TESTRUN_LINEEND
""TESTRUN_LINEEND
"Test compile example"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
"make test testname=tests/unit/filename_tests.c"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
""TESTRUN_LINEEND
"Test compile and run in valgrind"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
"make test testname=tests/unit/filename_tests.c && valgrind ./build/test/unit/filename_tests.test"TESTRUN_LINEEND
"~~~"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Tips"TESTRUN_LINEEND
""TESTRUN_LINEEND
"## Copyright"TESTRUN_LINEEND
""TESTRUN_LINEEND
"%s"TESTRUN_LINEEND
,copyright);


        testrun(strlen(expect) != strlen(result));
        result = testrun_string_free(result);
        result = testrun_text_block_readme(
                &config, description, usage, install);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);

        copyright = testrun_string_free(copyright);
        result = testrun_string_free(result);

        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int test_testrun_text_block_doxygen_config(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        char *result = NULL;
        char *module = "test";
        testrun_config config = testrun_config_default();

        result = testrun_text_block_doxygen_config(&config);
        snprintf(expect, size,
        "DOXYFILE_ENCODING       = UTF-8"TESTRUN_LINEEND
        "PROJECT_NAME            = %s"TESTRUN_LINEEND
        "PROJECT_NUMBER          = 0.0.1"TESTRUN_LINEEND
        "PROJECT_LOGO            = %s/%s/logo_doxygen.png"TESTRUN_LINEEND
        "PROJECT_BRIEF           = \"%s brief\""TESTRUN_LINEEND
        "OUTPUT_DIRECTORY        = %s/%s/documentation"TESTRUN_LINEEND
        "CREATE_SUBDIRS          = NO"TESTRUN_LINEEND
        "ALLOW_UNICODE_NAMES     = NO"TESTRUN_LINEEND
        "OUTPUT_LANGUAGE         = English"TESTRUN_LINEEND
        "MARKDOWN_SUPPORT        = YES"TESTRUN_LINEEND
        "AUTOLINK_SUPPORT        = YES"TESTRUN_LINEEND
        "INPUT                   = README.MD %s/%s %s/%s %s/%s"TESTRUN_LINEEND
        "INPUT_ENCODING          = UTF-8"TESTRUN_LINEEND
        "FILE_PATTERNS           = *.h *.c *.js *.py *.sh"TESTRUN_LINEEND
        "RECURSIVE               = YES"TESTRUN_LINEEND
        "EXCLUDE_SYMLINKS        = YES"TESTRUN_LINEEND
        ,config.project.name,
        config.project.path.to_doxygen, config.project.doxygen.foldername,
        config.project.name,
        config.project.path.to_doxygen, config.project.doxygen.foldername,
        config.project.path.to_include, config.project.path.include,
        config.project.path.to_source,  config.project.path.source,
        config.project.path.to_tests,   config.project.path.tests.name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);

        result = testrun_string_free(result);

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                   #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int all_tests() {

        testrun_init();

        testrun_test(test_testrun_text_block_splitline);
        testrun_test(test_testrun_text_block_comment_header);

        testrun_test(test_testrun_text_block_header_body);
        testrun_test(test_testrun_text_block_source_body);
        testrun_test(test_testrun_text_block_test_body);

        testrun_test(test_testrun_text_block_write_space);
        testrun_test(test_testrun_text_block_write_docu_line);
        testrun_test(test_testrun_text_block_c_header_documentation);

        testrun_test(test_testrun_text_block_write_shell_commented);
        testrun_test(test_testrun_text_block_sh_header_documentation);

        testrun_test(test_testrun_text_block_script);
        testrun_test(test_testrun_text_block_readme);
        testrun_test(test_testrun_text_block_doxygen_config);


        return 1;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST EXECUTION                                                  #EXEC
 *
 *      ------------------------------------------------------------------------
 */


testrun_run(all_tests);
