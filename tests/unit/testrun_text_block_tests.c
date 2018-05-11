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
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      [NAME]                                                          #[TAG]\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n");
        result = testrun_text_block_comment_header(NULL, NULL);
        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        snprintf(expect, size,
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      abc                                                             #1234567\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n");
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
        expect = "#include \"../include/name.h\"\n";
        result = testrun_text_block_source_body(name);
        testrun(strncmp(expect, result, strlen(result)) == 0);
        free(result);

        name   = "../include/test.h";
        expect = "#include \"../include/test.h\"\n";
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
                "\n"
                "#include \"../tools/testrun.h\"\n"
                "#include \"../../src/XXXXXXX.c\"\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      TEST HELPER                                                     #HELPER\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "/*----------------------------------------------------------------------------*/\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      TEST CASES                                                      #CASES\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "/*----------------------------------------------------------------------------*/\n"
                "\n"
                "/**\n"
                "int test_ ... (){\n"
                "        testrun(true);\n"
                "\n"
                "        return testrun_log_success();\n"
                "}\n"
                "*/\n"
                "\n"
                "/*----------------------------------------------------------------------------*/\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      TEST CLUSTER                                                    #CLUSTER\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "int all_tests() {\n"
                "\n"
                "        testrun_init();\n"
                "        //testrun_test(test_);\n"
                "\n"
                "        return testrun_counter;\n"
                "}\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      TEST EXECUTION                                                  #EXEC\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "testrun_run(all_tests);\n");

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
                "/**\n"
                "        @file           %s.h\n"
                "        @author         [AUTHOR]\n"
                "        @date           %s\n"
                "\n"
                "        @ingroup        [PROJECT]\n"
                "\n"
                "        @brief\n"
                "\n"
                "\n"
                "        ------------------------------------------------------------------------\n"
                "*/\n",
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
                "        @file           %s.h\n"
                "        @author         [AUTHOR]\n"
                "        @date           %s\n"
                "\n"
                "        @ingroup        [PROJECT]\n"
                "\n"
                "        @brief\n"
                "\n"
                "\n"
                "        ------------------------------------------------------------------------\n"
                "*/\n" ,
                module, date);

        result = testrun_text_block_c_header_documentation(
                module, TESTRUN_HEADER, &config, false, NULL);
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
        "#\n"
        "#       File            %s\n"
        "#       Authors         [AUTHOR]\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         [PROJECT]\n"
        "#\n"
        "#       Description\n"
        "#\n"
        "#       Usage\n"
        "#\n"
        "#       Dependencies\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n",
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
        "#\n"
        "#       File            %s\n"
        "#       Authors         test\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         some name\n"
        "#\n"
        "#       Description\n"
        "#\n"
        "#       Usage\n"
        "#\n"
        "#       Dependencies\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n",
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
        "#\n"
        "#       File            %s\n"
        "#       Authors         test\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         some name\n"
        "#\n"
        "#       Description     description text\n"
        "#\n"
        "#       Usage           usage text\n"
        "#\n"
        "#       Dependencies    some dependencies\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n",
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
        testrun_config config = testrun_config_default();

        // -------------------------------------------------------------
        // Check default
        // -------------------------------------------------------------

        result = testrun_text_block_script(&config, NULL, NULL, NULL, NULL);
        snprintf(expect, size,
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]\n"
        "#       All rights reserved.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            %s\n"
        "#       Authors         %s\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description\n"
        "#\n"
        "#       Usage\n"
        "#\n"
        "#       Dependencies\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n",
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
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright (c) 1888 owner\n"
        "#       All rights reserved.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            %s\n"
        "#       Authors         %s\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description\n"
        "#\n"
        "#       Usage\n"
        "#\n"
        "#       Dependencies\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n",
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
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright (c) 1888 owner\n"
        "#       All rights reserved.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            shellscript\n"
        "#       Authors         author\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         shellscript\n"
        "#\n"
        "#       Description     %s\n"
        "#\n"
        "#       Usage\n"
        "#\n"
        "#       Dependencies\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n",
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
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright (c) 1888 owner\n"
        "#       All rights reserved.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            shellscript\n"
        "#       Authors         author\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         shellscript\n"
        "#\n"
        "#       Description     %s\n"
        "#\n"
        "#       Usage\n"
        "#\n"
        "#       Dependencies\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n",
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
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright (c) 1888 owner\n"
        "#       All rights reserved.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            shellscript\n"
        "#       Authors         author\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         shellscript\n"
        "#\n"
        "#       Description     %s\n"
        "#\n"
        "#       Usage\n"
        "#\n"
        "#       Dependencies\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n",
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
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright (c) 1888 owner\n"
        "#       All rights reserved.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            shellscript\n"
        "#       Authors         author\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         shellscript\n"
        "#\n"
        "#       Description\n"
        "#\n"
        "#       Usage           %s\n"
        "#\n"
        "#       Dependencies\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n",
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
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright (c) 1888 owner\n"
        "#       All rights reserved.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            shellscript\n"
        "#       Authors         author\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         shellscript\n"
        "#\n"
        "#       Description\n"
        "#\n"
        "#       Usage\n"
        "#\n"
        "#       Dependencies    %s\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n",
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
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright (c) 1888 owner\n"
        "#       All rights reserved.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            shellscript\n"
        "#       Authors         author\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         shellscript\n"
        "#\n"
        "#       Description\n"
        "#\n"
        "#       Usage\n"
        "#\n"
        "#       Dependencies\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "%s"
        "\n",
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
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright (c) 1888 owner\n"
        "#       All rights reserved.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            shellscript\n"
        "#       Authors         author\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         shellscript\n"
        "#\n"
        "#       Description     Line1\n"
        "#                       Line2\n"
        "#                       Line3\n"
        "#\n"
        "#       Usage           Line1\n"
        "#                       Line2\n"
        "#                       Line3\n"
        "#\n"
        "#       Dependencies    Line1\n"
        "#                       Line2\n"
        "#                       Line3\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------" TESTRUN_LINEEND
        "\n"
        "Line1\n"
        "#                       Line2\n"
        "#                       Line3\n",
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

        description     =       "Line1\n" TESTRUN_TAG_OFFSET
                                "Line2\n" TESTRUN_TAG_OFFSET
                                "Line3";

        usage           =       "Line1\n" TESTRUN_TAG_OFFSET
                                "Line2\n" TESTRUN_TAG_OFFSET
                                "Line3";

        dependencies    =       "Line1\n" TESTRUN_TAG_OFFSET
                                "Line2\n" TESTRUN_TAG_OFFSET
                                "Line3";

        content         =       "Line1\n" TESTRUN_TAG_OFFSET
                                "Line2\n" TESTRUN_TAG_OFFSET
                                "Line3";

        result = testrun_text_block_script(&config, description, usage, dependencies, content);
        snprintf(expect, size,
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright (c) 1888 owner\n"
        "#       All rights reserved.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            shellscript\n"
        "#       Authors         author\n"
        "#       Date            %s\n"
        "#\n"
        "#       Project         shellscript\n"
        "#\n"
        "#       Description     Line1\n"
        "#                       Line2\n"
        "#                       Line3\n"
        "#\n"
        "#       Usage           Line1\n"
        "#                       Line2\n"
        "#                       Line3\n"
        "#\n"
        "#       Dependencies    Line1\n"
        "#                       Line2\n"
        "#                       Line3\n"
        "#\n"
        "#       Last changed    %s\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "Line1\n"
        "#                       Line2\n"
        "#                       Line3\n",
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
        testrun_config config = testrun_config_default();

        testrun(!testrun_text_block_readme(NULL, NULL, NULL, NULL));

        result = testrun_text_block_readme(&config, NULL, NULL, NULL);
        testrun(result);

        char *copyright   = config.copyright.to_string(&config.copyright);

        snprintf(expect, size,
        "# Project %s\n"
        "\n"
        "This module is self supported and may be build, tested, installed and\n"
        "run independently.\n"
        "\n"
        "## Overview\n"
        "\n"
        "* [Description](#description)\n"
        "* [Usage](#usage)\n"
        "* [Installation](#installation)\n"
        "* [Requirements](#requirements)\n"
        "* [Structure](#structure)\n"
        "* [Tests](#tests)\n"
        "* [Tips](#tips)\n"
        "* [Copyright](#copyright)\n"
        "\n"
        "## Description\n"
        "\n"
        "[TAG_DESCRIPTION]\n"
        "\n"
        "## Usage\n"
        "\n"
        "[TAG_USAGE]\n"
        "\n"
        "## Installation\n"
        "\n"
        "[TAG_INSTALL]\n"
        "\n"
        "### build sources\n"
        "\n"
        "\\`\\`\\`bash\n"
        "make\n"
        "\\`\\`\\`\n"
        "\n"
        "### build documentation\n"
        "\n"
        "\\`\\`\\`bash\n"
        "make documentation\n"
        "\\`\\`\\`\n"
        "\n"
        "### test sources\n"
        "\n"
        "\\`\\`\\`bash\n"
        "make tested\n"
        "\\`\\`\\`\n"
        "\n"
        "### install binaries\n"
        "\n"
        "\\`\\`\\`bash\n"
        "sudo make install\n"
        "\\`\\`\\`\n"
        "\n"
        "### uninstall binaries\n"
        "\n"
        "\\`\\`\\`bash\n"
        "sudo make uninstall\n"
        "\\`\\`\\`\n"
        "\n"
        "## Requirements\n"
        "\n"
        "## Structure\n"
        "\n"
        "### Default structure of the folder:\n"
        "\n"
        "\\`\\`\\`\n"
        "<pre>\n"
        ".\n"
        "├── COPYRIGHT\n"
        "├── README.MD\n"
        "├── makefile\n"
        "├── makefile_general.mk\n"
        "│\n"
        "├── doxygen\n"
        "│   ├── documentation\n"
        "│   └── doxygen.config\n"
        "│\n"
        "├── docs\n"
        "│   ├── CHANGELOG.MD\n"
        "│   └── ...\n"
        "│\n"
        "├── include\n"
        "│   ├── %s.h\n"
        "│   └── ...\n"
        "│\n"
        "├── src\n"
        "│   ├── %s.c\n"
        "│   └── ...\n"
        "│\n"
        "└── tests\n"
        "    ├── resources\n"
        "    ├── tools\n"
        "    │   ├── testrun_runner.h\n"
        "    │   ├── testrun_gcov.sh\n"
        "    │   ├── testrun_gprof.sh\n"
        "    │   ├── testrun_simple_coverage_tests.sh\n"
        "    │   ├── testrun_simple_unit_tests.sh\n"
        "    │   ├── testrun_simple_acceptance_tests.sh\n"
        "    │   └── testrun_simple_loc.sh\n"
        "    ├── acceptance\n"
        "    │   ├── ...\n"
        "    │   └── ...\n"
        "    └── unit\n"
        "        ├── %s_test.c\n"
        "        └── ...\n"
        "\n"
        "</pre>\n"
        "\\`\\`\\`\n"
        "\n"
        "## Tests\n"
        "\n"
        "All test sources will be recompiled on each make run. That means,\n"
        "all module tests will be created new on any change in any source file.\n"
        "\n"
        "### Test a project (all files contained in tests/unit)\n"
        "\n"
        "Test compile and run\n"
        "~~~\n"
        "make tested\n"
        "~~~\n"
        "\n"
        "### Test a dedicated source file (single file of tests/unit)\n"
        "\n"
        "To develop a test for a specific source file,\n"
        "it may be helpful to use a separated test run.\n"
        "This may be done via an integrated make functionality.\n"
        "\n"
        "Test compile example\n"
        "~~~\n"
        "make test testname=tests/unit/filename_tests.c\n"
        "~~~\n"
        "\n"
        "Test compile and run in valgrind\n"
        "~~~\n"
        "make test testname=tests/unit/filename_tests.c && valgrind ./build/test/unit/filename_tests.test\n"
        "~~~\n"
        "\n"
        "## Tips\n"
        "\n"
        "## Copyright\n"
        "\n"
        "%s\n"
        ,config.project.name,
        config.project.name,
        config.project.name,
        config.project.name,
        copyright);


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
        testrun_config config = testrun_config_default();

        result = testrun_text_block_doxygen_config(&config);
        snprintf(expect, size,
        "DOXYFILE_ENCODING       = UTF-8\n"
        "PROJECT_NAME            = %s\n"
        "PROJECT_NUMBER          = 0.0.1\n"
        "PROJECT_LOGO            = doxygen/logo_doxygen.png\n"
        "PROJECT_BRIEF           = \"%s brief\"\n"
        "OUTPUT_DIRECTORY        = doxygen/documentation\n"
        "CREATE_SUBDIRS          = NO\n"
        "ALLOW_UNICODE_NAMES     = NO\n"
        "OUTPUT_LANGUAGE         = English\n"
        "MARKDOWN_SUPPORT        = YES\n"
        "AUTOLINK_SUPPORT        = YES\n"
        "USE_MDFILE_AS_MAINPAGE  = README.MD\n"
        "INPUT                   = README.MD include src tests\n"
        "INPUT_ENCODING          = UTF-8\n"
        "FILE_PATTERNS           = *.h *.c *.js *.py *.sh\n"
        "RECURSIVE               = YES\n"
        "EXCLUDE_SYMLINKS        = YES\n"
        ,config.project.name,
        config.project.name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);

        result = testrun_string_free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_service_file(){


        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        char *result = NULL;
        testrun_config config = testrun_config_default();

        char socket_TCP[200];
        char socket_UDP[200];

        bzero(socket_TCP, 200);
        bzero(socket_UDP, 200);

        result = testrun_text_block_service_file(&config);
        snprintf(expect, size,
                "[Unit]\n"
                "Description= %s service\n"
                "\n"
                "[Service]\n"
                "ExecStart=/usr/local/bin/%s\n"
                "NonBlocking=True\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target\n"
                ,config.project.name, config.project.name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);

        // -------------------------------------------------------------
        // Check parameter change
        // -------------------------------------------------------------

        config.project.name = "test";

        snprintf(expect, size,
                "[Unit]\n"
                "Description= test service\n"
                "\n"
                "[Service]\n"
                "ExecStart=/usr/local/bin/test\n"
                "NonBlocking=True\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target\n"
                );

        testrun(strlen(expect) != strlen(result));
        result = testrun_string_free(result);
        result = testrun_text_block_service_file(&config);
        testrun(result);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);

        result = testrun_string_free(result);

        return testrun_log_success();



        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_socket_file(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        char *result = NULL;
        testrun_config config = testrun_config_default();

        result = testrun_text_block_socket_file(&config);
        snprintf(expect, size,
        "[Unit]\n"
        "Description= %s socket\n"
        "\n"
        "[Socket]\n"
        "\n"
        "# example interface bound\n"
        "# ListenStream=10.0.0.100:12345\n"
        "\n"
        "# example localhost\n"
        "# ListenStream=127.0.0.1:12345\n"
        "\n"
        "# example listen on all\n"
        "# ListenStream=0.0.0.0:12345\n"
        "\n"
        "# example listen on UDP\n"
        "# ListenDatagram=0.0.0.0:12345\n"
        "\n"
        "# Maximum parallel connections for the socket\n"
        "Backlog=2048\n"
        "\n"
        "# TCP Keepalive (1h)\n"
        "KeepAlive=false\n"
        "\n"
        "[Install]\n"
        "WantedBy=multi-user.target\n"
        , config.project.name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);

        result = testrun_string_free(result);

        return testrun_log_success();
}


/*----------------------------------------------------------------------------*/

int test_testrun_text_block_script_install(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        char *result = NULL;
        testrun_config config = testrun_config_default();

        char socket_TCP[200];
        char socket_UDP[200];

        bzero(socket_TCP, 200);
        bzero(socket_UDP, 200);

        result = testrun_text_block_script_install(&config);
        snprintf(expect, size,
"MODNAME=%s\n"
"CONFIGDIR=\"/etc/$MODNAME\"\n"
"\n"
"# Make sure only root can run our script\n"
"if [[ \\$EUID -ne 0 ]]; then\n"
"        echo \"\"\n"
"        echo \"This script must be run as root\" 1>&2\n"
"        echo \"\"\n"
"        exit 1\n"
"fi\n"
"\n"
"DIR=\"\\$( cd \"\\$( dirname \"\\${BASH_SOURCE[0]}\" )\" && pwd )\"\n"
"cd \\$DIR\n"
"\n"
"# ----------------------------------------------------------------------------\n"
"#       CHECK SOCKET CONFIG DONE\n"
"# ----------------------------------------------------------------------------\n"
"\n"
"grep '^ListenStream' $MODNAME.socket\n"
"if [[ \\$? -eq 1 ]]; then\n"
"        echo \"\"\n"
"        echo \"YOU MUST CONFIGURE $MODNAME.socket before running install.\"\n"
"        echo \"ListenStream not configured. Socket activated run will not work.\"\n"
"        echo \"\"\n"
"        echo \"EDIT config/install/$MODNAME.socket and enable a socket port\"\n"
"        exit 1\n"
"fi\n"
"\n"
"# ----------------------------------------------------------------------------\n"
"#       COPY REQUIRED FILES\n"
"# ----------------------------------------------------------------------------\n"
"\n"
"echo \"... copy executable to /usr/local/bin\"\n"
"cp \\${DIR}/../../bin/$MODNAME /usr/local/bin/$MODNAME\n"
"if [ \\$? -eq 0 ]; then\n"
"        echo \"copied $MODNAME to /usr/local/bin/\"\n"
"else\n"
"        echo \"FAILURE ... stopping, please check manually.\"\n"
"        exit 1\n"
"fi\n"
"\n"
"echo \"... copying systemd service descriptions.\"\n"
"cp \\${DIR}/../install/$MODNAME* /etc/systemd/system/\n"
"if [ \\$? -eq 0 ]; then\n"
"        echo \"copied systemd descriptions to /etc/systemd/system/\"\n"
"else\n"
"        echo \"FAILURE ... stopping, please check manually.\"\n"
"        exit 1\n"
"fi\n"
"\n"
"# ----------------------------------------------------------------------------\n"
"#       ENABLE THE SOCKET\n"
"# ----------------------------------------------------------------------------\n"
"\n"
"echo \"... enabling socket service\"\n"
"\n"
"systemctl enable $MODNAME.socket\n"
"if [ \\$? -eq 0 ]; then\n"
"        echo \"$MODNAME.socket enabled\"\n"
"        systemctl start $MODNAME.socket\n"
"        if [ \\$? -eq 0 ]; then\n"
"                echo \"$MODNAME.socket started\"\n"
"        else\n"
"                echo \"$MODNAME socket not started ... stopping.\"\n"
"                exit 1\n"
"        fi\n"
"else\n"
"        echo \"$MODNAME socket not enabled ... stopping\"\n"
"        exit 1\n"
"fi\n"
"\n"
"echo \"... enabling service service\"\n"
"\n"
"# ----------------------------------------------------------------------------\n"
"#       ENABLE THE SERVICE\n"
"# ----------------------------------------------------------------------------\n"
"\n"
"systemctl enable $MODNAME.service\n"
"if [ \\$? -eq 0 ]; then\n"
"        echo \"$MODNAME.service enabled\"\n"
"        systemctl start $MODNAME.socket\n"
"        if [ $? -eq 0 ]; then\n"
"                echo \"$MODNAME.service started\"\n"
"                echo \"check with 'systemctl status $MODNAME.service'\"\n"
"        else\n"
"                echo \"$MODNAME serivce not started ... stopping.\"\n"
"                exit 1\n"
"        fi\n"
"else\n"
"        echo \"$MODNAME serivce not enabled ... stopping.\"\n"
"        exit 1\n"
"fi\n"
"\n"
"# ----------------------------------------------------------------------------\n"
"#       COPY ADDITIONAL SERVICE CONFIGURATION DATA (EXAMPLE)\n"
"# ----------------------------------------------------------------------------\n"
"\n"
"# copy additional configuration data\n"
"mkdir -p \\$CONFIGDIR\n"
"mkdir -p \\$CONFIGDIR\"/%s\"\n"
"cp -r \\${DIR}/../config/%s/*      \\$CONFIGDIR\"/%s\" 2>/dev/null || :\n"
"cp -r \\${DIR}/../config/*.config  \\$CONFIGDIR 2>/dev/null || :\n"
"cp -r \\${DIR}/../config/*.conf    \\$CONFIGDIR 2>/dev/null || :\n"
"cp -r \\${DIR}/../config/*.ini     \\$CONFIGDIR 2>/dev/null || :\n"
"cp -r \\${DIR}/../config/*.cfg     \\$CONFIGDIR 2>/dev/null || :\n"
, config.project.name,
TESTRUN_FOLDER_SERVICE_DATA,
TESTRUN_FOLDER_SERVICE_DATA,
TESTRUN_FOLDER_SERVICE_DATA);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);

        result = testrun_string_free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_script_uninstall(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        char *result = NULL;
        testrun_config config = testrun_config_default();

        char socket_TCP[200];
        char socket_UDP[200];

        bzero(socket_TCP, 200);
        bzero(socket_UDP, 200);

        result = testrun_text_block_script_uninstall(&config);
        snprintf(expect, size,
"MODNAME=%s\n"
"CONFIGDIR=\"/etc/$MODNAME\"\n"
"\n"
"# Make sure only root can run our script\n"
"if [[ \\$EUID -ne 0 ]]; then\n"
"        echo \"This script must be run as root\" 1>&2\n"
"        exit 1\n"
"fi\n"
"\n"
"# ----------------------------------------------------------------------------\n"
"#       SERVICE SHUTDOWN\n"
"# ----------------------------------------------------------------------------\n"
"\n"
"echo \"Stopping $MODNAME.service\"\n"
"systemctl stop $MODNAME.service\n"
"if [ $? -eq 0 ]; then\n"
"        echo \"... service stopped.\";\n"
"else\n"
"        echo \"... service was not running.\"\n"
"fi\n"
"\n"
"echo \"Disabling $MODNAME.service\"\n"
"systemctl disable $MODNAME.service\n"
"if [ $? -eq 0 ]; then\n"
"        echo \"... service disabled.\";\n"
"else\n"
"        echo \"... service was not enabled.\"\n"
"fi\n"
"\n"
"# ----------------------------------------------------------------------------\n"
"#       SOCKET SHUTDOWN\n"
"# ----------------------------------------------------------------------------\n"
"\n"
"echo \"Stopping $MODNAME.socket\"\n"
"systemctl stop $MODNAME.socket\n"
"if [ $? -eq 0 ]; then\n"
"        echo \"... socket stopped.\";\n"
"else\n"
"        echo \"... socket was not running.\"\n"
"fi\n"
"\n"
"echo \"Disabling $MODNAME.socket\"\n"
"systemctl disable $MODNAME.socket\n"
"if [ $? -eq 0 ]; then\n"
"        echo \"... socket disabled.\";\n"
"else\n"
"        echo \"... socket was not enabled.\"\n"
"fi\n"
"\n"
"# ----------------------------------------------------------------------------\n"
"#       REMOVE CONFIG\n"
"# ----------------------------------------------------------------------------\n"
"\n"
"echo \"Removing systemd files\"\n"
"rm -rf /etc/systemd/system/$MODNAME*\n"
"if [ $? -eq 0 ]; then\n"
"        echo \"... removed /etc/systemd/system/$MODNAME*\";\n"
"else\n"
"        echo \"... WARN check and remove /etc/systemd/system/$MODNAME*\";\n"
"fi\n"
"echo \"uninstall done.\"\n"
"\n"
"echo \"Removing executable files\"\n"
"rm -rf /usr/local/bin/$MODNAME\n"
"if [ $? -eq 0 ]; then\n"
"        echo \"... removed /usr/local/bin/$MODNAME\";\n"
"else\n"
"        echo \"... WARN check and remove /usr/local/bin/$MODNAME\";\n"
"fi\n"
"echo \"uninstall done.\"\n"
"\n"
"echo \"Removing config files\"\n"
"rm -rf /etc/$MODNAME*\n"
"if [ $? -eq 0 ]; then\n"
"        echo \"... removed /etc/$MODNAME*\";\n"
"else\n"
"        echo \"... WARN check and remove /etc/$MODNAME*\";\n"
"fi\n"
"echo \"uninstall done.\"\n"
"\n"
"# ----------------------------------------------------------------------------\n"
"#       RELOAD SYSTEMCTL DAEMON\n"
"# ----------------------------------------------------------------------------\n"
"\n"
"systemctl daemon-reload\n"
"echo \"Systemd daemon reload performed.\"\n"
, config.project.name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n",  result, strlen(result));

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_changelog_file(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        char *date =  testrun_time_string(TESTRUN_SCOPE_DAY);

        char *result = NULL;
        testrun_config config = testrun_config_default();

        testrun(!testrun_text_block_changelog_file(NULL));

        result = testrun_text_block_changelog_file(&config);
        snprintf(expect, size,
                "Changelog for project %s created %s\n",
                config.project.name, date);

        testrun(strlen(expect) == strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);
        date = testrun_string_free(date);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_gitignore_file(){

        size_t size = 10000;
        char expect[size];
        bzero(expect, size);

        char *result = NULL;
        testrun_config config = testrun_config_default();

        testrun(!testrun_text_block_gitignore_file(NULL));

        result = testrun_text_block_gitignore_file(&config);
        snprintf(expect, size,
                "# C template\n"
                "*.o\n"
                "*.so\n"
                "*.ko\n"
                "*.obj\n"
                "*.elf\n"
                "\n"
                "# generated files\n"
                "**/bin/\n"
                "**/gen/\n"
                "**/build/\n"
                "\n"
                "# generated documentation\n"
                "doxygen/documentation\n"
                "\n"
                "# generated packages\n"
                "*.pc\n"
                "\n"
                "# generated ctags\n"
                ".tags\n"
                "tags\n"
                "functions\n"
                "\n"
                "# local files\n"
                "/local\n"
                ".log\n"
                ".Trash-*\n"
                "*.swp\n"
                ".~\n"
                "\n"
                "# tools\n"
                ".vagrant\n"
                "*.git\n"
                "\n"
                "# ide\n"
                "\n"
                "## Intellij\n"
                ".idea\n"
                "\n"
                "## Sublime Text\n"
                "*.sublime-workspace\n"
                "*.sublime-project\n"
                "\n"
                "## VIM\n"
                "[._]*.s[a-w][a-z]\n"
                "[._]s[a-w][a-z]\n"
                "*.un~\n"
                "Session.vim\n"
                ".netrwhist\n"
                "*~\n"
                "\n"
                "## EMACS\n"
                "*~\n"
                "\\#*\\#\n"
                "/.emacs.desktop\n"
                "/.emacs.desktop.lock\n"
                "*.elc\n"
                "auto-save-list\n"
                "tramp\n"
                ".\\#*\n"
                "# Org-mode\n"
                ".org-id-locations\n"
                "*_archive\n"
                "# flymake-mode\n"
                "*_flymake.*\n"
                "# Flycheck\n"
                "flycheck_*.el\n"
                "# cedet\n"
                "ede-projects.el\n"
                "# cask packages\n"
                ".cask/\n"
                "dist/\n"
                "# eshell files\n"
                "/eshell/history\n"
                "/eshell/lastdir\n"
                "# elpa packages\n"
                "/elpa/\n");

        testrun(result);
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

        testrun_test(test_testrun_text_block_service_file);
        testrun_test(test_testrun_text_block_socket_file);

        testrun_test(test_testrun_text_block_script_install);
        testrun_test(test_testrun_text_block_script_uninstall);

        testrun_test(test_testrun_text_block_changelog_file);
        testrun_test(test_testrun_text_block_gitignore_file);


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
