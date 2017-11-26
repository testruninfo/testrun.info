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

        @file           testrun_string_tests.c
        @author         Markus Toepfer
        @date           2017-11-13

        @ingroup        testrun_lib

        @brief          Unit testing for testrun_strings


        ------------------------------------------------------------------------
*/


#include "../../include/testrun2.h"
#include "../../src/testrun_string.c"

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */


int test_testrun_string_free(){

        char *string = calloc(1, sizeof(char));

        testrun(NULL == testrun_string_free(NULL));

        testrun(NULL != string);
        string = testrun_string_free(string);
        testrun(NULL == string);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_string_prepare(){

        char   *ptr = NULL;
        size_t size = 0;
        size_t open = 0;
        size_t used = 0;

        testrun(false == testrun_string_prepare(NULL, NULL,  NULL,  NULL));
        testrun(false == testrun_string_prepare(NULL, &size, &open, &used));
        testrun(false == testrun_string_prepare(&ptr, NULL,  &open, &used));
        testrun(false == testrun_string_prepare(&ptr, &size, NULL,  &used));
        testrun(false == testrun_string_prepare(&ptr, &size, &open, NULL));

        // -------------------------------------------------------------
        // Positive test - new allocation
        // -------------------------------------------------------------

        testrun(NULL == ptr);
        testrun(true == testrun_string_prepare(&ptr, &size, &open, &used));
        testrun(NULL != ptr);
        testrun(size == testrun_STRING_DEFAULT_SIZE);
        testrun(open == testrun_STRING_DEFAULT_SIZE);
        testrun(used == 0);

        // reset
        ptr  = testrun_string_free(ptr);
        size = 0;
        open = 0;
        used = 0;

        // -------------------------------------------------------------
        // Positive test - existing allocation
        // -------------------------------------------------------------

        size = 10;
        ptr  = calloc(size, sizeof(char));
        strcat(ptr, "test");

        // validate
        testrun(ptr  != NULL);
        testrun(size == 10);
        testrun(open == 0);
        testrun(used == 0);

        testrun(true == testrun_string_prepare(&ptr, &size, &open, &used));

        // check
        testrun(ptr  != NULL);
        testrun(size == 10);
        testrun(open == 6);
        testrun(used == 4);

        // reset
        ptr  = testrun_string_free(ptr);
        size = 0;
        open = 0;
        used = 0;

        // -------------------------------------------------------------
        // Allocation all allocated data used
        // -------------------------------------------------------------

        size = 11;
        ptr  = calloc(size, sizeof(char));
        strcat(ptr, "0123456789");

        // validate
        testrun(ptr  != NULL);
        testrun(size == 11);
        testrun(open == 0);
        testrun(used == 0);

        testrun(true == testrun_string_prepare(&ptr, &size, &open, &used));

        // check
        testrun(ptr  != NULL);
        testrun(size == 11);
        testrun(open == 1);
        testrun(used == 10);


        // -------------------------------------------------------------
        // Allocation size without final \0
        // -------------------------------------------------------------

        // reset
        size = 10;
        open = 0;
        used = 0;

        testrun(false == testrun_string_prepare(&ptr, &size, &open, &used));

        // check
        testrun(ptr  != NULL);
        testrun(size == 10);
        testrun(open == 0);
        testrun(used == 10);

        // reset
        ptr  = testrun_string_free(ptr);
        size = 0;
        open = 0;
        used = 0;

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_string_append(){

        char    *dest   = NULL;
        char    *src    = NULL;
        char    *expect = NULL;

        size_t  d_size  = 0;
        size_t  s_size  = 0;


        testrun(false == testrun_string_append(NULL,  NULL,    NULL, 0));
        testrun(false == testrun_string_append(NULL,  &d_size, src,  s_size));
        testrun(false == testrun_string_append(&dest, NULL,    src,  s_size));
        testrun(false == testrun_string_append(&dest, &d_size, NULL,  s_size));
        testrun(false == testrun_string_append(&dest, &d_size, src,  0));

        // -------------------------------------------------------------
        // Positive test, no realloc
        // -------------------------------------------------------------

        d_size = 10;
        dest = calloc(d_size, sizeof(char));
        strcat(dest, "test");
        src    = "1";
        expect = "test1";


        testrun(true == testrun_string_append(&dest, &d_size, src,  strlen(src)));
        testrun(strncmp(expect, dest, strlen(expect)) == 0);
        testrun(d_size == 10);

        free(dest);
        dest = NULL;


        // -------------------------------------------------------------
        // Positive test, no realloc
        // -------------------------------------------------------------

        d_size = 6;
        dest = calloc(d_size, sizeof(char));
        strcat(dest, "test");
        src    = "1";
        expect = "test1";

        testrun(true == testrun_string_append(&dest, &d_size, src,  strlen(src)));
        testrun(strncmp(expect, dest, strlen(expect)) == 0);
        testrun(d_size == 6);

        free(dest);
        dest = NULL;

        // -------------------------------------------------------------
        // Positive test, realloc
        // -------------------------------------------------------------

        d_size = 5;
        dest = calloc(d_size, sizeof(char));
        strcat(dest, "test");
        src    = "1";
        expect = "test1";


        testrun(true == testrun_string_append(&dest, &d_size, src,  strlen(src)));
        testrun(strncmp(expect, dest, strlen(expect)) == 0);
        testrun(d_size == 6);

        free(dest);
        dest = NULL;

        // -------------------------------------------------------------
        // Allocate string
        // -------------------------------------------------------------

        dest   = NULL;
        d_size = 0;

        src    = "1";
        expect = "1";

        testrun(true == testrun_string_append(&dest, &d_size, src,  strlen(src)));
        testrun(strncmp(expect, dest, strlen(expect)) == 0);
        testrun(d_size == 2);

        free(dest);
        dest = NULL;

        // -------------------------------------------------------------
        // Check append not CUT
        // -------------------------------------------------------------

        d_size = 15;
        dest = calloc(d_size, sizeof(char));
        strcat(dest, "0123456789");
        src    = "xxx";
        expect = "0123456789xxx";

        // dest length shorter dest string
        d_size = 9;
        testrun(false == testrun_string_append(&dest, &d_size, src,  strlen(src)));

        // dest length == strlen(dest);
        d_size = 10;
        testrun(false == testrun_string_append(&dest, &d_size, src,  strlen(src)));

        // dest length == strlen(dest) + 1;
        d_size = 11;
        testrun(true == testrun_string_append(&dest, &d_size, src,  strlen(src)));
        testrun(strncmp(expect, dest, strlen(expect)) == 0);
        testrun(d_size == strlen(expect) + 1);

        free(dest);
        dest = NULL;

        // -------------------------------------------------------------
        // Check source len bound to src string length
        // -------------------------------------------------------------

        d_size = 15;
        dest = calloc(d_size, sizeof(char));
        strcat(dest, "0123456789");
        src    = "xxx";
        expect = "0123456789xxx";

        // source len > strlen(source)
        testrun(false == testrun_string_append(&dest, &d_size, src,  4));

        // source len  == strlen(source)
        testrun(true == testrun_string_append(&dest, &d_size, src,  3));
        testrun(strncmp(expect, dest, strlen(expect)) == 0);
        testrun(d_size == 15);

        free(dest);
        dest = NULL;

        // -------------------------------------------------------------
        // Check source len < strlen(source)
        // -------------------------------------------------------------

        d_size = 15;
        dest = calloc(d_size, sizeof(char));
        strcat(dest, "0123456789");
        src    = "xxx";
        expect = "0123456789xx";

        // source len  < strlen(source)
        testrun(true == testrun_string_append(&dest, &d_size, src,  2));
        testrun(strncmp(expect, dest, strlen(expect)) == 0);
        testrun(d_size == 15);

        free(dest);
        dest = NULL;

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_string_write_embeded(){

        size_t used = 0;
        size_t open = 100;
        size_t rate = 0;
        size_t real = 0;

        char *prefix = NULL;
        char *suffix = NULL;
        char *source = NULL;
        char *lbreak = NULL;

        char *data   = calloc(open, sizeof(char));
        char expect[5000];

        testrun(-1 == testrun_string_write_embeded(   NULL,
                                                0,      0, 0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        testrun(-1  == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        testrun(-1  == testrun_string_write_embeded(  NULL,
                                                &open,  &used, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        testrun(-1  == testrun_string_write_embeded(  &data,
                                                NULL,  &used, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        testrun(-1  == testrun_string_write_embeded(  &data,
                                                &open,  NULL, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        testrun(-1  == testrun_string_write_embeded(  &data,
                                                &open,  &used,  rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        testrun(-1  == testrun_string_write_embeded(  &data,
                                                &open,  &used,  0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));


        // -------------------------------------------------------------
        // Write each parameter once
        // -------------------------------------------------------------


        bzero(&expect, 5000);
        prefix = NULL;
        suffix = NULL;
        source = NULL;
        lbreak = "\n";

        sprintf(expect , "\n");

        rate = 1;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == 1);
        testrun(open == 99);

        bzero(&expect, 5000);
        used = 0;
        open = 100;

        prefix = NULL;
        suffix = NULL;
        source = "source";
        lbreak = NULL;

        sprintf(expect, "%s", source);
        rate = 1;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                NULL,   0,
                                                source, strlen(source),
                                                NULL,   0,
                                                NULL,   0));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(source));
        testrun(open == 100 - strlen(source));


        bzero(&expect, 5000);
        used = 0;
        open = 100;

        prefix = NULL;
        suffix = "suf";
        source = NULL;
        lbreak = NULL;

        sprintf(expect, "%s", suffix);
        rate = 1;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                suffix, strlen(suffix),
                                                NULL,   0));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(suffix));
        testrun(open == 100 - strlen(suffix));

        bzero(&expect, 5000);
        used = 0;
        open = 100;

        prefix = "pre";
        suffix = NULL;
        source = NULL;
        lbreak = NULL;

        sprintf(expect, "%s", prefix);
        rate = 1;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(prefix));
        testrun(open == 100 - strlen(prefix));


        // -------------------------------------------------------------
        // TEST parameter combined
        // -------------------------------------------------------------

        bzero(&expect, 5000);
        used = 0;
        open = 100;

        prefix = "xxx";
        suffix = "yyy";
        source = "abcd";
        lbreak = "\n\n\n";

        sprintf(expect, "xxxabcdyyy\n\n\n");
        rate = 1;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == 100 - strlen(expect));

        // -------------------------------------------------------------
        // Check length use
        // -------------------------------------------------------------

        used = 0;
        open = 100;
        sprintf(expect, "xabcdyyy\n\n\n");
        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, 1,
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == 100 - strlen(expect));

        used = 0;
        open = 100;
        sprintf(expect, "xxxabcdyyy\n\n\n");
        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == 100 - strlen(expect));

        used = 0;
        open = 100;
        sprintf(expect, "xxxabyyy\n\n\n");
        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, 2,
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == 100 - strlen(expect));

        used = 0;
        open = 100;
        sprintf(expect, "xxxabcd\n\n\n");
        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, 0,
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == 100 - strlen(expect));

        used = 0;
        open = 100;
        sprintf(expect, "xxxabcdyyy\n");
        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, 1));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == 100 - strlen(expect));

        // -------------------------------------------------------------
        // Check reallocate by min rate
        // -------------------------------------------------------------

        free(data);
        data = calloc(10, sizeof(char));

        bzero(&expect, 5000);
        used = 0;
        open = 10;

        prefix = "p";
        suffix = "s";
        source = "0123456789";
        lbreak = "\n";

        sprintf(expect, "%s%s%s%s", prefix, source, suffix, lbreak);

        // rate < min required, expected reallocation
        rate = 1;
        real = strlen(expect) + 1;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == real - used);
        testrun(open == 1);

        // -------------------------------------------------------------
        // Check reallocate with rate == required
        // -------------------------------------------------------------

        free(data);
        data = calloc(10, sizeof(char));

        bzero(&expect, 5000);
        used = 0;
        open = 10;

        prefix = "p";
        suffix = "s";
        source = "0123456789";
        lbreak = "\n";

        sprintf(expect, "%s%s%s%s", prefix, source, suffix, lbreak);

        // rate == min required, expected reallocation
        rate = 3;
        real = open + rate + 1;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == real -used);
        testrun(open == 1);

        // -------------------------------------------------------------
        // Check reallocate with rate > required
        // -------------------------------------------------------------

        free(data);
        data = calloc(10, sizeof(char));

        bzero(&expect, 5000);
        used = 0;
        open = 10;

        prefix = "p";
        suffix = "s";
        source = "0123456789";
        lbreak = "\n";

        sprintf(expect, "%s%s%s%s", prefix, source, suffix, lbreak);

        // rate == min required, expected reallocation
        rate = 10;
        real = open + rate;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == real - used);
        testrun(open == 7);

        // -------------------------------------------------------------
        // Check reallocate with rate < required (FALLBACK MIN REALLOC)
        // -------------------------------------------------------------

        free(data);
        data = calloc(10, sizeof(char));

        bzero(&expect, 5000);
        used = 0;
        open = 10;

        prefix = "p";
        suffix = "s";
        source = "0123456789";
        lbreak = "\n";

        sprintf(expect, "%s%s%s%s", prefix, source, suffix, lbreak);

        // rate == min required, expected reallocation
        rate = 2;
        real = strlen(expect) + 1;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == real - used);
        testrun(open == 1);


        // -------------------------------------------------------------
        // Check without any preallocation
        // -------------------------------------------------------------

        free(data);
        data = NULL;

        bzero(&expect, 5000);
        used = 0;
        open = 0;

        prefix = "p";
        suffix = "s";
        source = "0123456789";
        lbreak = "\n";

        sprintf(expect, "%s%s%s%s", prefix, source, suffix, lbreak);

        // rate == min required, expected reallocation
        rate = 2;
        real = strlen(expect) + 1;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == real - used);
        testrun(open == 1);

        // -------------------------------------------------------------
        // Check without any preallocation (and rate > required)
        // -------------------------------------------------------------

        free(data);
        data = NULL;

        bzero(&expect, 5000);
        used = 0;
        open = 0;

        prefix = "p";
        suffix = "s";
        source = "0123456789";
        lbreak = "\n";

        sprintf(expect, "%s%s%s%s", prefix, source, suffix, lbreak);

        // rate == min required, expected reallocation
        rate = 20;
        real = 20;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == real - used);
        testrun(open == 7);


        // -------------------------------------------------------------
        // USE CASE CHECK
        // -------------------------------------------------------------

        free(data);
        data = NULL;

        bzero(&expect, 5000);
        used = 0;
        open = 0;

        prefix = "* ";
        suffix = NULL;
        source = "new comment line";
        lbreak = "\n";

        sprintf(expect, "* new comment line\n");

        // rate == min required, expected reallocation
        rate = 1;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                0, 0,
                                                lbreak, strlen(lbreak)));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == 1);


        free(data);
        data = NULL;

        bzero(&expect, 5000);
        used = 0;
        open = 0;

        prefix = "(";
        suffix = ")";
        source = "string";
        lbreak = NULL;

        sprintf(expect, "(string)");

        // rate == min required, expected reallocation
        rate = 1;

        testrun( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                0, 0));

        testrun(strncmp(expect, data, strlen(expect)) == 0);
        testrun(used == strlen(expect));
        testrun(open == 1);

        free(data);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_string_embed(){

        size_t size  = 0;
        char   *ptr  = NULL;

        char *source = "source";
        char *prefix = "prefix";
        char *suffix = "suffix";
        char *delim1 = ",";
        char *delim2 = ":";

        char expect[1000];

        testrun(false == testrun_string_embed(NULL,   NULL,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        testrun(false == testrun_string_embed(NULL,   &size,
                                        source, strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        testrun(false == testrun_string_embed(&ptr,   NULL,
                                        source, strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        testrun(false == testrun_string_embed(&ptr,   &size,
                                        NULL,   strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        testrun(false == testrun_string_embed(&ptr,   &size,
                                        source, 0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        // -------------------------------------------------------------
        // check min valid
        // -------------------------------------------------------------

        sprintf(expect, "s");

        testrun(NULL == ptr);
        testrun(0 == size);

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, 1,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        testrun(ptr);
        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        testrun(NULL == ptr);
        testrun(0    == size);
        sprintf(expect, "%s", source);

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        testrun(ptr);
        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);


        // -------------------------------------------------------------
        // check all values
        // -------------------------------------------------------------

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        testrun(NULL == ptr);
        testrun(0    == size);
        sprintf(expect, "%s%s", prefix, source);

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        testrun(ptr);
        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        testrun(NULL == ptr);
        testrun(0    == size);
        sprintf(expect, "%s%s", source, suffix);

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        NULL,   0,
                                        suffix, strlen(suffix),
                                        NULL,   0,
                                        NULL,   0));

        testrun(ptr);
        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        testrun(NULL == ptr);
        testrun(0    == size);
        sprintf(expect, "%s", source);

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        delim1, strlen(delim1),
                                        NULL,   0));

        testrun(ptr);
        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        testrun(NULL == ptr);
        testrun(0    == size);
        sprintf(expect, "%s%s", source, delim2);

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        delim2, strlen(delim2)));

        testrun(ptr);
        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // check all together
        // -------------------------------------------------------------

        testrun(NULL == ptr);
        testrun(0    == size);
        sprintf(expect, "%s%s%s%s", prefix, source, suffix, delim2);

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(ptr);
        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // Delimiter change
        // -------------------------------------------------------------

        source = "a,b,c,d";
        prefix = "(";
        suffix = ")";
        delim1 = ",";
        delim2 = ":";

        testrun(NULL == ptr);
        testrun(0    == size);
        sprintf(expect, "(a):(b):(c):(d):");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));
        testrun(ptr);
        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // Embed and add to existing string (append mode)
        // -------------------------------------------------------------

        size = 100;
        ptr  = calloc(size, sizeof(char));
        strcat(ptr, "test");

        source = "a,b,c,d";
        prefix = "(";
        suffix = ")";
        delim1 = ",";
        delim2 = ":";

        sprintf(expect, "test(a):(b):(c):(d):");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(ptr);
        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == 100);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // Contained string & size fit
        // -------------------------------------------------------------

        size = 100;
        ptr  = calloc(size, sizeof(char));
        strcat(ptr, "test");

        source = "a,b,c,d";
        prefix = "(";
        suffix = ")";
        delim1 = ",";
        delim2 = ":";

        sprintf(expect, "test(a):(b):(c):(d):");

        testrun(size == 100);

        size = 2;
        testrun(false == testrun_string_embed(&ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(ptr, "test", strlen(ptr)) == 0);

        testrun(size == 2);

        size = 3;
        testrun(false == testrun_string_embed(&ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(ptr, "test", strlen(ptr)) == 0);
        testrun(size == 3);


        size = 4;
        testrun(false == testrun_string_embed(&ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(ptr, "test", strlen(ptr)) == 0);
        testrun(size == 4);

        // -------------------------------------------------------------
        // Contained string & size fit ... adding default length
        // -------------------------------------------------------------

        size = 5;
        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE + 5);

        bzero(ptr, 100);
        strcat(ptr, "test");

        size = 6;
        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE + 6);

        bzero(ptr, 100);
        strcat(ptr, "test");

        size = 10;
        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE + 10);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // delim1 not found
        // -------------------------------------------------------------

        // append mode
        size = 100;
        ptr  = calloc(size, sizeof(char));
        strcat(ptr, "test");

        source = "a,b,c,d";
        prefix = "(";
        suffix = ")";
        delim1 = "x";
        delim2 = ":";

        sprintf(expect, "test(a,b,c,d)");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == 100);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // preallocated
        size = 100;
        ptr  = calloc(size, sizeof(char));

        source = "a,b,c,d";
        prefix = "(";
        suffix = ")";
        delim1 = "x";
        delim2 = ":";

        sprintf(expect, "(a,b,c,d)");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == 100);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // unallocated
        size = 0;
        ptr  = NULL;

        source = "a,b,c,d";
        prefix = "(";
        suffix = ")";
        delim1 = "x";
        delim2 = ":";

        sprintf(expect, "(a,b,c,d)");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // multichar delimiter to single char delimiter
        // -------------------------------------------------------------

        // append mode
        size = 100;
        ptr  = calloc(size, sizeof(char));
        strcat(ptr, "test");

        source = "a...b...c...d";
        prefix = "(";
        suffix = ")";
        delim1 = "...";
        delim2 = " ";

        sprintf(expect, "test(a) (b) (c) (d) ");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));



        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == 100);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // preallocated
        size = 100;
        ptr  = calloc(size, sizeof(char));

        source = "a...b...c...d";
        prefix = "(";
        suffix = ")";
        delim1 = "...";
        delim2 = " ";

        sprintf(expect, "(a) (b) (c) (d) ");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == 100);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // unallocated
        size = 0;
        ptr  = NULL;

        source = "a...b...c...d";
        prefix = "(";
        suffix = ")";
        delim1 = "...";
        delim2 = " ";

        sprintf(expect, "(a) (b) (c) (d) ");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // mutlichar delimiter fit
        // -------------------------------------------------------------

         // unallocated
        size = 0;
        ptr  = NULL;

        source = "a....b...c..d";
        prefix = "(";
        suffix = ")";
        delim1 = "...";
        delim2 = " ";

        sprintf(expect, "(a) (.b) (c..d) ");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // apply to all lines
        // -------------------------------------------------------------

         // unallocated
        size = 0;
        ptr  = NULL;

        source = "a\nb\nc\nd";
        prefix = "(";
        suffix = ")";
        delim1 = "\n";
        delim2 = "\n";

        sprintf(expect, "(a)\n(b)\n(c)\n(d)\n");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // empty delimiter content handling
        // -------------------------------------------------------------

         // unallocated
        size = 0;
        ptr  = NULL;

        source = "a\n\n\n";
        prefix = "(";
        suffix = ")";
        delim1 = "\n";
        delim2 = "\n";

        sprintf(expect, "(a)\n()\n()\n");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // no delimiter 2 (cut out delimiter)
        // -------------------------------------------------------------

        // unallocated
        size = 0;
        ptr  = NULL;

        source = "a\n\n\n";
        prefix = "(";
        suffix = ")";
        delim1 = "\n";
        delim2 = NULL;

        sprintf(expect, "(a)()()");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        NULL, 0));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // no delimiter 1 (don't change orig)
        // -------------------------------------------------------------

        // unallocated
        size = 0;
        ptr  = NULL;

        source = "abcd\n";
        prefix = NULL;
        suffix = NULL;
        delim1 = "1";
        delim2 = "2";

        sprintf(expect, "abcd\n");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        NULL, 0,
                                        NULL, 0,
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // no delimiter 1 (don't change orig)
        // -------------------------------------------------------------

        // unallocated
        size = 0;
        ptr  = NULL;

        source = "abcd\n";
        prefix = NULL;
        suffix = NULL;
        delim1 = "1234";
        delim2 = "2";

        sprintf(expect, "abcd\n");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        NULL, 0,
                                        NULL, 0,
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // no delimiter 1 (don't change orig)
        // -------------------------------------------------------------

        // unallocated
        size = 0;
        ptr  = NULL;

        source = "abcd\n";
        prefix = NULL;
        suffix = NULL;
        delim1 = "123456";
        delim2 = "2";

        sprintf(expect, "abcd\n");

        testrun(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        NULL, 0,
                                        NULL, 0,
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;



        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_string_unset_end(){

        size_t size  = 100;
        size_t len   = 100;

        char *source = calloc(size, sizeof(char));
        char *string = calloc(len, sizeof(char));
        char *expect = "source_";

        strcat(source, "source_end");
        strcat(string, "end");

        size = strlen(source);
        len  = strlen(string);

        testrun(false == testrun_string_unset_end( NULL,    0,      NULL,   0));
        testrun(false == testrun_string_unset_end( NULL,    size,   string, len));
        testrun(false == testrun_string_unset_end( source,  0,      string, len));
        testrun(false == testrun_string_unset_end( source,  size,   NULL,   len));
        testrun(false == testrun_string_unset_end( source,  size,   string, 0));

        testrun(true  == testrun_string_unset_end( source,  size,   string, len));
        testrun(strncmp(source, expect, strlen(source)) == 0);

        // -------------------------------------------------------------
        // len != strlen(string)
        // -------------------------------------------------------------

        bzero(source, 100);
        bzero(string, 100);

        strcat(source, "source_end");
        strcat(string, "end");

        expect = "source_end";

        size = strlen(source);
        len  = strlen(string) + 1;

        testrun(false == testrun_string_unset_end( source, size,  string, len));
        testrun(strncmp(source, expect, strlen(source)) == 0);

        len  = strlen(string);

        testrun(true == testrun_string_unset_end( source, size,  string, len));
        testrun(strncmp(source, expect, strlen(source)) == 0);

        // -------------------------------------------------------------
        // size < len
        // -------------------------------------------------------------

        bzero(source, 100);
        bzero(string, 100);

        strcat(source, "source_end");
        strcat(string, "source_end1");

        expect = "source_end";

        size = strlen(source);
        len  = strlen(string);

        testrun(false == testrun_string_unset_end( source, size,  string, len));
        testrun(strncmp(source, expect, strlen(source)) == 0);

        // -------------------------------------------------------------
        // size == len
        // -------------------------------------------------------------

        bzero(string, 100);
        strcat(string, source);

        expect = "";

        size = strlen(source);
        len  = strlen(string);

        testrun(true == testrun_string_unset_end( source, size,  string, len));
        testrun(strlen(source) == 0);
        testrun(strncmp(source, expect, strlen(source)) == 0);

        // -------------------------------------------------------------
        // size == len (no match)
        // -------------------------------------------------------------

        bzero(source, 100);
        bzero(string, 100);

        strcat(source, "source_end");
        strcat(string, "sourcexend");

        expect = "source_end";

        size = strlen(source);
        len  = strlen(string) + 1;

        testrun(false == testrun_string_unset_end( source, size,  string, len));
        testrun(strncmp(source, expect, strlen(source)) == 0);

        // -------------------------------------------------------------
        // no last match
        // -------------------------------------------------------------

        bzero(source, 100);
        bzero(string, 100);

        strcat(source, "source_endx");
        strcat(string, "end");

        expect = "source_endx";

        size = strlen(source);
        len  = strlen(string) + 1;

        testrun(false == testrun_string_unset_end( source, size,  string, len));
        testrun(strncmp(source, expect, strlen(source)) == 0);
        bzero(source, 100);
        expect = "source_enx";
        strcat(source, expect);

        testrun(false == testrun_string_unset_end( source, size,  string, len));
        testrun(strncmp(source, expect, strlen(source)) == 0);

        free(source);
        source = NULL;
        free(string);
        string = NULL;


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_string_replace_first(){


        size_t size  = 0;
        char   *ptr  = NULL;

        char *source = "1\n2\n3\n4";
        char *old    = "\n";
        char *new    = ":";
        char *expect = "1:2\n3\n4";

        testrun(false == testrun_string_replace_first(NULL, NULL,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0));

        testrun(false == testrun_string_replace_first(NULL,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        testrun(false == testrun_string_replace_first(&ptr,   NULL,
                                                NULL,   strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        testrun(false == testrun_string_replace_first(&ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    0));

        testrun(false == testrun_string_replace_first(&ptr,   &size,
                                                source, strlen(source),
                                                NULL,   strlen(old),
                                                new,    strlen(new)));

        // new length not correct
        testrun(false == testrun_string_replace_first(&ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new) - 1));

        // old length not correct
        testrun(false == testrun_string_replace_first(&ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old) - 1,
                                                new,    strlen(new)));

        // -------------------------------------------------------------
        // positive exchange (old + new)
        // -------------------------------------------------------------

        testrun(ptr  == NULL);
        testrun(size == 0);

        source = "1 2 3 4";
        old    = " ";
        new    = ":";
        expect = "1:2 3 4";

        testrun(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        ptr  = testrun_string_free(ptr);
        size = 0;

        // -------------------------------------------------------------
        // positive cut (no new)
        // -------------------------------------------------------------

        testrun(ptr  == NULL);
        testrun(size == 0);

        source = "1 2 3 4";
        old    = " ";
        new    = ":";
        expect = "12 3 4";

        testrun(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                NULL,   0));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        ptr  = testrun_string_free(ptr);
        size = 0;

        // -------------------------------------------------------------
        // positive old not found (copy source)
        // -------------------------------------------------------------

        testrun(ptr  == NULL);
        testrun(size == 0);

        source = "1 2 3 4";
        old    = "x";
        new    = ":";
        expect = "1 2 3 4";

        testrun(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        ptr  = testrun_string_free(ptr);
        size = 0;

        // -------------------------------------------------------------
        // append mode (copy append, no item replacement)
        // -------------------------------------------------------------

        size = 10;
        ptr  = calloc(size, sizeof(char));
        strcat(ptr, "test");

        source = "1 2 3 4";
        old    = "x";
        new    = ":";
        expect = "test1 2 3 4";

        testrun(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == 10 + testrun_STRING_DEFAULT_SIZE);

        // -------------------------------------------------------------
        // append mode append with item replacement
        // -------------------------------------------------------------

        source = "1 2 3 4";
        old    = " ";
        new    = ":";
        expect = "test1 2 3 41:2 3 4";

        testrun(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == 10 + testrun_STRING_DEFAULT_SIZE);

        // -------------------------------------------------------------
        // append mode append with item delete
        // -------------------------------------------------------------

        source = "1 2 3 4";
        old    = " ";
        new    = ":";
        expect = "test1 2 3 41:2 3 412 3 4";

        testrun(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                NULL,   0));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == 10 + testrun_STRING_DEFAULT_SIZE);


        // reset
        ptr  = testrun_string_free(ptr);
        size = 0;

        // -------------------------------------------------------------
        // example test
        // -------------------------------------------------------------

        size = 10;
        ptr  = calloc(size, sizeof(char));
        strcat(ptr, "test");

        source = "whenever_whenever";
        old    = "en";
        new    = "_at_";
        expect = "testwh_at_ever_whenever";

        testrun(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == 10 + testrun_STRING_DEFAULT_SIZE);

        ptr  = testrun_string_free(ptr);
        size = 0;



        return testrun_log_success();
}
/*----------------------------------------------------------------------------*/

int test_testrun_string_replace_all(){


        size_t size  = 0;
        char   *ptr  = NULL;

        char *source = "1\n2\n3\n4";
        char *delim1 = "\n";
        char *delim2 = ":";
        char *expect = "1:2:3:4";

        testrun(false == testrun_string_replace_all(  NULL, NULL,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                false));

        testrun(false == testrun_string_replace_all(  NULL, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                false));

        testrun(false == testrun_string_replace_all(  &ptr, NULL,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                false));

        testrun(false == testrun_string_replace_all(  &ptr, &size,
                                                NULL, strlen(source),
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                false));

        testrun(false == testrun_string_replace_all(  &ptr, &size,
                                                source, strlen(source),
                                                NULL, strlen(delim1),
                                                NULL, 0,
                                                false));

        testrun(false == testrun_string_replace_all(  &ptr, &size,
                                                source, 0,
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                false));

        testrun(false == testrun_string_replace_all(  &ptr, &size,
                                                source, strlen(source),
                                                delim1, 0,
                                                NULL, 0,
                                                false));

        // -------------------------------------------------------------
        // exchange will NULL (unset all delimiters)
        // -------------------------------------------------------------

        expect = "1234";
        testrun(false == testrun_string_replace_all(  &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                false));

        size = 0;
        free(ptr);
        ptr = NULL;

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;


        // -------------------------------------------------------------
        // exchange with delimiter of same size
        // -------------------------------------------------------------

        source = "1\n2\n3\n4";
        delim1 = "\n";
        delim2 = ":";
        expect = "1:2:3:4";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "1:2:3:4:";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        // -------------------------------------------------------------
        // exchange with delimiter of bigger size
        // -------------------------------------------------------------

        source = "1\n2\n3\n4";
        delim1 = "\n";
        delim2 = ":::";
        expect = "1:::2:::3:::4";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "1:::2:::3:::4:::";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        // -------------------------------------------------------------
        // exchange with delimiter of smaller size
        // -------------------------------------------------------------

        source = "1:::2:::3:::4";
        delim1 = ":::";
        delim2 = " ";
        expect = "1 2 3 4";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "1 2 3 4 ";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        // -------------------------------------------------------------
        // delimiter not found
        // -------------------------------------------------------------

        source = "1:::2:::3:::4";
        delim1 = " ";
        delim2 = "\r\n";
        expect = source;

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        // -------------------------------------------------------------
        // exchange with same delimiter
        // -------------------------------------------------------------

        source = "1 2 3 4";
        delim1 = " ";
        delim2 = " ";
        expect = source;

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        // -------------------------------------------------------------
        // delimiter first
        // -------------------------------------------------------------

        source = " 1 2 3 4";
        delim1 = " ";
        delim2 = "x";
        expect = "x1x2x3x4";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "x1x2x3x4x";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        // -------------------------------------------------------------
        // delimiter last
        // -------------------------------------------------------------

        source = "1 2 3 4 ";
        delim1 = " ";
        delim2 = "x";
        expect = "1x2x3x4";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "1x2x3x4x";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                 source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        // -------------------------------------------------------------
        // Inside string replacement
        // -------------------------------------------------------------

        source = "Whatever ";
        delim1 = "at";
        delim2 = "en";
        expect = "Whenever";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        source = "CallCall ";
        delim1 = "ll";
        delim2 = "xx";
        expect = "CaxxCaxx";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "CaxxCa";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        // -------------------------------------------------------------
        // Inside string replacement (Growing string)
        // -------------------------------------------------------------

        source = "abcxxdef";
        delim1 = "xx";
        delim2 = "0123456789";
        expect = "abc0123456789def";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        // -------------------------------------------------------------
        // Inside string replacement (Shrinking string)
        // -------------------------------------------------------------

        source = "abc0123456789def";
        delim1 = "0123456789";
        delim2 = "xx";
        expect = "abcxxdef";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        // -------------------------------------------------------------
        // Example testing
        // -------------------------------------------------------------

        size = 0;
        free(ptr);
        ptr = NULL;

        source = "whenever_whenever";
        delim1 = "en";
        delim2 = "_at_";
        expect = "wh_at_ever_wh_at_ever";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "wh_at_ever_wh_at_ever_at_";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        // -------------------------------------------------------------
        // Example testing 2
        // -------------------------------------------------------------

        size = 0;
        free(ptr);
        ptr = NULL;

        size = 10;
        ptr  = calloc(size, sizeof(char));
        strcat(ptr, "test");

        source = "whenever_whenever";
        delim1 = "en";
        delim2 = "_at_";
        expect = "testwh_at_ever_wh_at_ever_at_";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        testrun(strncmp(expect, ptr, strlen(ptr)) == 0);
        testrun(size == 10 + testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        size = 10;
        ptr  = calloc(size, sizeof(char));
        strcat(ptr, "test");

        source = "whenever_whenever";
        delim1 = "en";
        delim2 = "_at_";
        expect = "testwh_at_ever_wh_at_ever";

        testrun(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        testrun(strncmp(expect, ptr, strlen(expect)) == 0);
        testrun(size == 10 + testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                   #CLUSTER
 *
 *      ------------------------------------------------------------------------
 *
 *      Cluster all tests in a way to be run and executed one after the other
 *      that is the same way the setup SHOULD be done for assert based testing.
 *
 *      YOU MAY RUN THIS CLUSTER BY CHANGING testrun_run(???); TO
 *
 *              testrun_run(cluster_tests_non_configurable);
 *
 *      ------------------------------------------------------------------------
 */
int cluster_tests_non_configurable() {

       testrun_init();

       testrun_test(test_testrun_string_free);
       testrun_test(test_testrun_string_prepare);
       testrun_test(test_testrun_string_append);
       testrun_test(test_testrun_string_write_embeded);
       testrun_test(test_testrun_string_embed);
       testrun_test(test_testrun_string_unset_end);
       testrun_test(test_testrun_string_replace_first);
       testrun_test(test_testrun_string_replace_all);

       return testrun_counter;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TESTS CONFIGURABLE
 *
 *      Cluster the same test series as above in a way to be configurable for
 *      the type of testrun (parallel or sequential).
 *
 *      ------------------------------------------------------------------------
 */

int64_t cluster_tests_configurable(
        int(*tests[])(),
        size_t slot,
        size_t max){

        testrun_init();

        testrun_add(test_testrun_string_free);
        testrun_add(test_testrun_string_prepare);
        testrun_add(test_testrun_string_append);
        testrun_add(test_testrun_string_write_embeded);
        testrun_add(test_testrun_string_embed);
        testrun_add(test_testrun_string_unset_end);
        testrun_add(test_testrun_string_replace_first);
        testrun_add(test_testrun_string_replace_all);

        return testrun_counter;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TESTS CONFIGURATION
 *
 *      Implementation of the required functions for parallel and
 *      sequential testcase configuration of the testrun2.h header.
 *
 *      ------------------------------------------------------------------------
 */

bool testrun_configure_parallel(
        int (*testcases[])(),
        size_t * const start,
        size_t const * const max){

        int r = 0;

        if (!testcases || !start || !max)
                return false;

        r = testrun_add_testcases(testcases,
                        start, max, cluster_tests_configurable);

        return (r < 0)? false : true;
}

/*----------------------------------------------------------------------------*/

bool testrun_configure_sequential(
        int (*testcases[])(),
        size_t *const start,
        size_t const * const max){

        int r = 0;

        if (!testcases || !start || !max)
                return false;

        r = testrun_add_testcases(testcases,
                        start, max, cluster_tests_configurable);

        return (r < 0)? false : true;

}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST EXECUTION                                                  #EXEC
 *
 *      ------------------------------------------------------------------------
 *
 *      EXECUTION sequence for the test series:
 *
 *      OPTIONS
 *
 *      (1) run the testconfiguration in parallel and sequential:
 *
 *              testrun_run_tests(25,25,false);
 *
 *      (2) run the testconfiguration in parallel only:
 *
 *              testrun_run_tests(25,0,false);
 *
 *      (3) run the testconfiguration in sequential only:
 *
 *              testrun_run_tests(0,25,false);
 *
 *      (4) run the testconfiguration in sequential only, break_on_first_error:
 *
 *              testrun_run_tests(0,25,true);
 *
 *      NOTE the last execution sequence (4) is the same as running the test
 *      using the function cluster_tests_non_configurable.
 *
 *      YOU MAY RUN THIS EXECUTION SEQUENCE BY CHANGING testrun_run(???); TO
 *
 *              testrun_run(run_tests);
 *
 *      ------------------------------------------------------------------------
 */
int64_t run_tests() {

        return testrun_run_tests(25,0,true);
}


/*
 *      ------------------------------------------------------------------------
 *
 *      TEST ACTIVATED                                                  #ACTIV
 *
 *      ------------------------------------------------------------------------
 */

//testrun_run(run_tests);
testrun_run(cluster_tests_non_configurable);


/*
 *      ------------------------------------------------------------------------
 *
 *      TEST RESULTS AND NOTES                                         #RESULTS
 *
 *      ------------------------------------------------------------------------
 *
 *      NOTE example testrun with 25 enabled test cases in parallel and
 *      sequential shows a slightly better runtime in sequential run.
 *
 *      testrun_run(cluster_tests_non_configurable);
 *      results around 110 ms
 *
 *      testrun_run(run_tests); with sequential testrun_run_tests(0,25,true);
 *      results around 125 ms
 *
 *      testrun_run(run_tests); with parallel testrun_run_tests(25,0,true);
 *      results around 200 ms - 300 ms
 *
 *      So the overhead for OpenMP is not worth to run the tests in parallel
 *      here. This may be different for other test scenarios. Check your
 *      tests and scenarios to select the best fitting testrun for you.
 *
 *      NOTE For these tests here non configuration tests runs the fastest!
 *      NOTE as soon as this test series is compiled using OpenMP there will
 *      be 8 bytes in 1 block shown as still reachable when running
 *      with valgrind.
 *
 *      ------------------------------------------------------------------------
 *
 *      EXAMPLE RUN TIMES:
 *      ALL TESTS RUN (8 tests)
 *      Clock ticks function: ( main ) | 0.110468 | 110 ms
 *
 *      Serial RUN (8) TESTS: success 8 error 0)
 *      Clock ticks function: ( testrun_sequential ) | 0.096326 | 96 ms
 *      ALL TESTS RUN (8 tests)
 *      Clock ticks function: ( main ) | 0.123577 | 124 ms
 *
 *      Parallel RUN (8) TESTS in 0 threads: success 8 error 0)
 *      Clock ticks function: ( testrun_parallel ) | 0.264922 | 265 ms
 *      ALL TESTS RUN (8 tests)
 *      Clock ticks function: ( main ) | 0.292388 | 292 ms
 *
 *      ------------------------------------------------------------------------
 *
 *      TEST RUN with following machine data:
 *      markus@PC:~/projects/testrun.info/testrun_lib$ lscpu
 *      Architecture:          x86_64
 *      CPU op-mode(s):        32-bit, 64-bit
 *      Byte Order:            Little Endian
 *      CPU(s):                4
 *      On-line CPU(s) list:   0-3
 *      Thread(s) pro Kern:    2
 *      Kern(e) pro Socket:    2
 *      Socket(s):             1
 *      NUMA-Knoten:           1
 *      Anbieterkennung:       GenuineIntel
 *      Prozessorfamilie:      6
 *      Modell:                69
 *      Model name:            Intel(R) Core(TM) i7-4600U CPU @ 2.10GHz
 *      Stepping:              1
 *      CPU MHz:               2654.681
 *      CPU max MHz:           3300,0000
 *      CPU min MHz:           800,0000
 *      BogoMIPS:              5387.67
 *      Virtualisierung:       VT-x
 *      L1d Cache:             32K
 *      L1i Cache:             32K
 *      L2 Cache:              256K
 *      L3 Cache:              4096K
 *      NUMA node0 CPU(s):     0-3
 *      Flags:                 fpu vme de pse tsc msr pae mce cx8 apic sep mtrr
 *                             pge mca cmov pat pse36 clflush dts acpi mmx fxsr
 *                             sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp
 *                             lm constant_tsc arch_perfmon pebs bts rep_good
 *                             nopl xtopology nonstop_tsc aperfmperf pni
 *                             pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2
 *                             ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2
 *                             x2apic movbe popcnt tsc_deadline_timer aes xsave
 *                             avx f16c rdrand lahf_lm abm epb tpr_shadow vnmi
 *                             flexpriority ept vpid fsgsbase tsc_adjust bmi1
 *                             avx2 smep bmi2 erms invpcid xsaveopt dtherm ida
 *                             arat pln pts
 *      markus@PC:~/projects/testrun.info/testrun_lib$ cat /proc/version
 *      Linux version 4.10.0-38-generic (buildd@lgw01-amd64-059)
 *      (gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4) )
 *      #42~16.04.1-Ubuntu SMP Tue Oct 10 16:32:20 UTC 2017
 *
 *      ------------------------------------------------------------------------
 */
