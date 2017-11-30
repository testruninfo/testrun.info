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

        @file           testrun_module_tests.c
        @author         Markus Toepfer
        @date           2017-11-26

        @ingroup        testrun_lib

        @brief


       ------------------------------------------------------------------------
*/



#include "../../include/testrun.h"
#include "../../src/testrun_module.c"


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

int test_testrun_module_c_file_content() {

        size_t size = 2500;
        char expect[size];
        bzero(expect, size);

        char *date   = testrun_time_string(TESTRUN_SCOPE_DAY);

        char *result = NULL;
        char *name   = "test";

        testrun_config config = testrun_config_default();

        // -------------------------------------------------------------
        // Positive test - HEADER
        // -------------------------------------------------------------


        result = testrun_module_c_file_content(name, TESTRUN_HEADER, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s.h"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        "#ifndef %s_h"                TESTRUN_LINEEND
        "#define %s_h"                TESTRUN_LINEEND
        TESTRUN_LINEEND
        "#endif /* %s_h */" TESTRUN_LINEEND,
        name, date, name, name, name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - SOURCE
        // -------------------------------------------------------------

        result = testrun_module_c_file_content(name, TESTRUN_SOURCE, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s.c"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        "#include \".././include/%s.h\""  TESTRUN_LINEEND
        TESTRUN_LINEEND,
        name, date, name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Positive test - TESTS
        // -------------------------------------------------------------

        result = testrun_module_c_file_content(name, TESTRUN_TEST, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s_test.c"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "#include \".././tools/testrun.h\"" TESTRUN_LINEEND
        "#include \"../.././src/%s.c\"" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST HELPER                                                     #HELPER" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST CASES                                                      #CASES" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/**" TESTRUN_LINEEND
        "int test_ ... (){" TESTRUN_LINEEND
        "        testrun(true);" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        return testrun_log_success();" TESTRUN_LINEEND
        "}" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST CLUSTER                                                    #CLUSTER" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "int all_tests() {" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "        testrun_init();" TESTRUN_LINEEND
        "        //testrun_test(test_);" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "        return testrun_counter;" TESTRUN_LINEEND
        "}" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST EXECUTION                                                  #EXEC" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "testrun_run(all_tests);" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        name, date, name);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);



        // -------------------------------------------------------------
        // Configuration changes
        // -------------------------------------------------------------

        // move include header to project/src/include
        config.project.path.to_include = ".";
        // move tests to project/src/tests
        config.project.path.tests.to_project = ".";
        // move tests tools to project/tools
        config.project.path.tests.to_tools = "..";


        // -------------------------------------------------------------
        // Configuration changes (No change in header)
        // -------------------------------------------------------------

        result = testrun_module_c_file_content(name, TESTRUN_HEADER, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s.h"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        "#ifndef %s_h"                TESTRUN_LINEEND
        "#define %s_h"                TESTRUN_LINEEND
        TESTRUN_LINEEND
        "#endif /* %s_h */" TESTRUN_LINEEND,
        name, date, name, name, name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);


        // -------------------------------------------------------------
        // Configuration changes (source to header path)
        // -------------------------------------------------------------

        result = testrun_module_c_file_content(name, TESTRUN_SOURCE, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s.c"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        "#include \".././include/%s.h\""  TESTRUN_LINEEND
        TESTRUN_LINEEND,
        name, date, name);

        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        // -------------------------------------------------------------
        // Configuration changes (tests to tools path, tests to src path
        // -------------------------------------------------------------

        result = testrun_module_c_file_content(name, TESTRUN_TEST, &config);
        snprintf(expect, size,
        "/***" TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        Copyright (c) [COPYRIGHT_YEAR] [COPYRIGHT_OWNER]" TESTRUN_LINEEND
        "        All rights reserved."          TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*//**" TESTRUN_LINEEND
        "        @file           %s_test.c"          TESTRUN_LINEEND
        "        @author         [AUTHOR]"      TESTRUN_LINEEND
        "        @date           %s"            TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @ingroup        [PROJECT]"     TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        @brief"                        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        ------------------------------------------------------------------------" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "#include \"../../tools/testrun.h\"" TESTRUN_LINEEND
        "#include \"../.././src/%s.c\"" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST HELPER                                                     #HELPER" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST CASES                                                      #CASES" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/**" TESTRUN_LINEEND
        "int test_ ... (){" TESTRUN_LINEEND
        "        testrun(true);" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "        return testrun_log_success();" TESTRUN_LINEEND
        "}" TESTRUN_LINEEND
        "*/" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*----------------------------------------------------------------------------*/" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST CLUSTER                                                    #CLUSTER" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "int all_tests() {" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "        testrun_init();" TESTRUN_LINEEND
        "        //testrun_test(test_);" TESTRUN_LINEEND
        "" TESTRUN_LINEEND
        "        return testrun_counter;" TESTRUN_LINEEND
        "}" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "/*" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      TEST EXECUTION                                                  #EXEC" TESTRUN_LINEEND
        " *" TESTRUN_LINEEND
        " *      ------------------------------------------------------------------------" TESTRUN_LINEEND
        " */" TESTRUN_LINEEND
        TESTRUN_LINEEND
        "testrun_run(all_tests);" TESTRUN_LINEEND
        TESTRUN_LINEEND,
        name, date, name);
        //log("EXPECT|\n%s|END|%jd\n", expect, strlen(expect));
        //log("START|\n%s|END|%jd\n", result, strlen(result));
        testrun(strncmp(result, expect, strlen(expect)) == 0);
        result = testrun_string_free(result);

        date = testrun_string_free(date);
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

       testrun_test(test_testrun_module_c_file_content);

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
