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
 *      @file           testrun_copyright_tests.c
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

/*******************************************************************************
 *
 *      TEST SETUP
 *
 ******************************************************************************/

#include "../tools/testrun.h"
#include "../../src/testrun_copyright.c"
#include "../../include/testrun_lib.h"
#include <sys/stat.h>

static int testing = 1;

/*******************************************************************************
 *
 *      TEST CASES
 *
 ******************************************************************************/

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_create_header() {

        size_t size  = 0;
        size_t len   = 5000;
        char *out    = NULL;
        char expect[size];

        char *source = "s";
        char *prefix = "#";
        char *intro  = "i";
        char *outro  = "o";

        assert(-1 == testrun_copyright_create_header(   NULL,   0,
                                                        NULL,   0,
                                                        NULL,   0,
                                                        NULL,   0,
                                                        NULL,   0));

        assert(-1 == testrun_copyright_create_header(   &out,   0,
                                                        NULL,   0,
                                                        NULL,   0,
                                                        NULL,   0,
                                                        NULL,   0));

        assert(-1 == testrun_copyright_create_header(   NULL,   &size,
                                                        NULL,   0,
                                                        NULL,   0,
                                                        NULL,   0,
                                                        NULL,   0));

        assert(-1 == testrun_copyright_create_header(   &out,   &size,
                                                        NULL,   0,
                                                        NULL,   0,
                                                        NULL,   0,
                                                        NULL,   0));


        assert(0 == testrun_copyright_create_header(    &out,   &size,
                                                        source, strlen(source),
                                                        NULL,   0,
                                                        NULL,   0,
                                                        NULL,   0));


        // (re) initialize
        memset(&expect, 0, size);
        sprintf(expect, "s\n");

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        assert(0 == testrun_copyright_create_header(    &out,   &size,
                                                        source, strlen(source),
                                                        prefix, strlen(prefix),
                                                        intro,  strlen(intro),
                                                        outro,  strlen(outro)));


        // (re) initialize
        memset(&expect, 0, size);
        sprintf(expect, "i#s\no");

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        assert(0 == testrun_copyright_create_header(    &out,   &size,
                                                        source, strlen(source),
                                                        0,      0,
                                                        intro,  strlen(intro),
                                                        outro,  strlen(outro)));


        // (re) initialize
        memset(&expect, 0, size);
        sprintf(expect, "is\no");

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        assert(0 == testrun_copyright_create_header(    &out,   &size,
                                                        source, strlen(source),
                                                        prefix, strlen(prefix),
                                                        0,      0,
                                                        outro,  strlen(outro)));


        // (re) initialize
        memset(&expect, 0, size);
        sprintf(expect, "#s\no");

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        assert(0 == testrun_copyright_create_header(    &out,   &size,
                                                        source, strlen(source),
                                                        prefix, strlen(prefix),
                                                        intro,  strlen(intro),
                                                        0,      0));

        // (re) initialize
        memset(&expect, 0, size);
        sprintf(expect, "i#s\n");

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        assert(0 == testrun_copyright_create_header(    &out,   &size,
                                                        source, strlen(source),
                                                        0,      0,
                                                        intro,  strlen(intro),
                                                        0,      0));
        // (re) initialize
        memset(&expect, 0, size);
        sprintf(expect, "is\n");

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        source = "line1\nline2\nline3";
        prefix = "#";
        intro  = "i";
        outro  = "o";

        assert(0 == testrun_copyright_create_header(    &out,   &size,
                                                        source, strlen(source),
                                                        prefix, strlen(prefix),
                                                        intro,  strlen(intro),
                                                        outro,  strlen(outro)));


        // (re) initialize
        memset(&expect, 0, size);
        sprintf(expect, "i"
                        "#line1\n"
                        "#line2\n"
                        "#line3\n"
                        "o");

        printf("SIZE %d\n\n", size);

        assert(out);
        printf("OUT %s", out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;


        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/


int test_testrun_read_copyright_from() {

        char *path1  = "./build/test/resources";
        char *path2  = "./build/test/resources/test_copyright";
        char *path3  = "./build/test/resources/test_copyright/copyright1";
        char *path4  = "./build/test/resources/test_copyright/copyright2";
        char *path5  = "./build/test/resources/test_copyright/copyright3";
        char *path6  = "./build/test/resources/test_copyright/copyright4";

        char path[PATH_MAX];
        char *expect = NULL;
        char *result = NULL;

        assert(!testrun_read_copyright_from(NULL));

        /* Copyright file not found */
        assert(!testrun_read_copyright_from(path1));
        assert(!testrun_read_copyright_from(path2));

        expect = "1\n22\n333\n4444";

        result = testrun_read_copyright_from(path3);
        assert(result);
        assert(strncmp(expect, result, strlen(expect)) == 0);
        free(result);

        expect = "Testline1\n"
                 "Testline2\n"
                 "Testline3\n"
                 "Testline4";


        result = testrun_read_copyright_from(path4);
        assert(result);
        assert(strncmp(expect, result, strlen(expect)) == 0);
        free(result);

        expect = "Copyright 2017 Markus Toepfer\n"
                 "\n"
                 "Licensed under the Apache License, Version 2.0 (the \"License\");\n"
                 "you may not use this file except in compliance with the License.\n"
                 "You may obtain a copy of the License at\n"
                 "\n"
                 "http://www.apache.org/licenses/LICENSE-2.0\n"
                 "\n"
                 "Unless required by applicable law or agreed to in writing, software\n"
                 "distributed under the License is distributed on an \"AS IS\" BASIS,\n"
                 "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
                 "See the License for the specific language governing permissions and\n"
                 "limitations under the License.\n"
                 "\n"
                 "This file is part of the testrun project. http://testrun.info";


        result = testrun_read_copyright_from(path5);
        assert(result);
        assert(strncmp(expect, result, strlen(expect)) == 0);
        free(result);

        assert(0 < sprintf(path, "%s/%s", path5, TESTRUN_FILENAME_COPYRIGHT));

        /* No access to file path/copyright */
        assert(0 == chmod(path, 000));
        assert(!testrun_read_copyright_from(path5));
        assert(0 == chmod(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));

        /* Recheck access ok */
        result = testrun_read_copyright_from(path5);
        assert(result);
        assert(strncmp(expect, result, strlen(expect)) == 0);
        free(result);

        /* No access to folder path */
        assert(0 == chmod(path5, 000));
        assert(!testrun_read_copyright_from(path5));
        assert(0 == chmod(path5,
                S_IRUSR | S_IWUSR | S_IXUSR |
                S_IRGRP | S_IXGRP |
                S_IROTH | S_IXOTH
                ));

        /* Recheck access ok */
        result = testrun_read_copyright_from(path5);
        assert(result);
        assert(strncmp(expect, result, strlen(expect)) == 0);
        free(result);

        /* empty file */
        assert(!testrun_read_copyright_from(path6));

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_copyright_default() {

        size_t size  = 2000;
        char *date   = NULL;
        char *name   = NULL;
        char *result = NULL;
        char expect[size];

        sprintf(expect,
                "%s%s"
                "COPYRIGHT (c) [COPYRIGHT_DATE] [COPYRIGHT_AUTHOR]\n"
                "%sAll rights reserved.\n"
                "%s",
                TESTRUN_COPYRIGHT_INTRO,
                TESTRUN_HEADER_LINE_INTRO,
                TESTRUN_HEADER_LINE_INTRO,
                TESTRUN_COPYRIGHT_OUTRO);

        result = testrun_copyright_default(NULL, NULL, true);
        assert(result);
        assert(strncmp(result, expect, strlen(expect)) == 0);
        memset(&expect, 0, size);
        free(result);

        sprintf(expect,
                "COPYRIGHT (c) [COPYRIGHT_DATE] [COPYRIGHT_AUTHOR]\n"
                "All rights reserved.\n");

        result = testrun_copyright_default(NULL, NULL, false);
        assert(result);
        assert(strncmp(result, expect, strlen(expect)) == 0);
        memset(&expect, 0, size);
        free(result);

        date = "1234";
        name = "test";

        sprintf(expect,
                "%s%s"
                "COPYRIGHT (c) %s %s\n"
                "%sAll rights reserved.\n"
                "%s",
                TESTRUN_COPYRIGHT_INTRO,
                TESTRUN_HEADER_LINE_INTRO,
                date, name,
                TESTRUN_HEADER_LINE_INTRO,
                TESTRUN_COPYRIGHT_OUTRO);

        result = testrun_copyright_default(date, name, true);
        assert(result);
        assert(strncmp(result, expect, strlen(expect)) == 0);
        memset(&expect, 0, size);
        free(result);

        sprintf(expect,
                "COPYRIGHT (c) %s %s\n"
                "All rights reserved.\n",
                date, name);

        result = testrun_copyright_default(date, name, false);
        assert(result);
        assert(strncmp(result, expect, strlen(expect)) == 0);
        memset(&expect, 0, size);
        free(result);

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

/*******************************************************************************
 *
 *      TEST CLUSTER
 *
 ******************************************************************************/

int all_tests() {

       testrun_init();

       testrun_test(test_testrun_copyright_create_header);
       testrun_test(test_testrun_read_copyright_from);
       testrun_test(test_testrun_copyright_default);

       return 1;
}

/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/


testrun_run(all_tests);
