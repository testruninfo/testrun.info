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

        @ingroup

        @brief


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

int test_testrun_text_block_c_header_documentation(){

        size_t size = 1500;
        char expect[size];
        char text[size];
        char *result = NULL;

        result = testrun_text_block_c_header_documentation(
                NULL, NULL, NULL, NULL, NULL, true);

        snprintf(expect, size,
        "/**"  TESTRUN_LINEEND
        "        @file           [MODULE].c"    TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           [DATE]"        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"  TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        );

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        result = testrun_text_block_c_header_documentation(
                "file", NULL, NULL, NULL, NULL, true);

        snprintf(expect, size,
        "/**"  TESTRUN_LINEEND
        "        @file           file.c"        TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           [DATE]"        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"  TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        );

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        result = testrun_text_block_c_header_documentation(
                NULL, "ext", NULL, NULL, NULL, true);

        snprintf(expect, size,
        "/**"  TESTRUN_LINEEND
        "        @file           [MODULE].ext"  TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           [DATE]"        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"  TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        );

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        result = testrun_text_block_c_header_documentation(
                NULL, NULL, "author", NULL, NULL, true);

        snprintf(expect, size,
        "/**"  TESTRUN_LINEEND
        "        @file           [MODULE].c"    TESTRUN_LINEEND
        "        @author         author"        TESTRUN_LINEEND
        "        @date           [DATE]"        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"  TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        );

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        result = testrun_text_block_c_header_documentation(
                NULL, NULL, NULL, "date", NULL, true);

        snprintf(expect, size,
        "/**"  TESTRUN_LINEEND
        "        @file           [MODULE].c"    TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           date"          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"  TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        );

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        result = testrun_text_block_c_header_documentation(
                NULL, NULL, NULL, NULL, "project", true);

        snprintf(expect, size,
        "/**"  TESTRUN_LINEEND
        "        @file           [MODULE].c"    TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           [DATE]"        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        project"       TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"  TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        );

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        result = testrun_text_block_c_header_documentation(
                NULL, NULL, NULL, NULL, NULL, false);

        snprintf(expect, size,
        "        @file           [MODULE].c"    TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           [DATE]"        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"  TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        );

        testrun(result);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        return testrun_log_success();
}

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

        testrun(!testrun_text_block_source_body(NULL));

        expect = "#include \"../include/name.h\"\n";
        result = testrun_text_block_source_body("name");
        testrun(strncmp(expect, result, strlen(result)) == 0);
        free(result);

        expect = "#include \"../include/test.h\"\n";
        result = testrun_text_block_source_body("test");
        testrun(strncmp(expect, result, strlen(result)) == 0);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_text_block_test_body(){

        size_t size = 2000;
        char expect[size];
        bzero(expect, size);

        testrun(!testrun_text_block_test_body(NULL));

        char *result = NULL;

        snprintf(expect, size,
                TESTRUN_LINEEND
                "#include \"../tools/testrun.h\""       TESTRUN_LINEEND
                "#include \"../../src/XXXXXXX.c\""      TESTRUN_LINEEND
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
                "/**"                                   TESTRUN_LINEEND
                "int test_ ... (){"                     TESTRUN_LINEEND
                "        testrun(true);"                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        return testrun_log_success();"  TESTRUN_LINEEND
                "}"                                     TESTRUN_LINEEND
                "*/"                                    TESTRUN_LINEEND
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
                "int all_tests() {"                     TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        testrun_init();"                TESTRUN_LINEEND
                "        //testrun_test(test_);"         TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        return testrun_counter;"        TESTRUN_LINEEND
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
                "testrun_run(all_tests);"               TESTRUN_LINEEND);

        result = testrun_text_block_test_body("XXXXXXX");
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
 *      TEST CLUSTER                                                   #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int all_tests() {

        testrun_init();
        testrun_test(test_testrun_text_block_c_header_documentation);
        testrun_test(test_testrun_text_block_splitline);
        testrun_test(test_testrun_text_block_comment_header);

        testrun_test(test_testrun_text_block_header_body);
        testrun_test(test_testrun_text_block_source_body);
        testrun_test(test_testrun_text_block_test_body);

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
