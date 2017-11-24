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
 *      @file           testrun_string_tests.c
 *      @author         Markus Toepfer
 *      @date           2017-11-13
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Unit testing for testrun_strings
 *
 *
 *      ------------------------------------------------------------------------
 **/

/*******************************************************************************
 *
 *      TEST SETUP
 *
 ******************************************************************************/

#include "../../include/testrun.h"
#include "../../src/testrun_string.c"

static int testing = 1;

/*******************************************************************************
 *
 *      TEST CASES
 *
 ******************************************************************************/


int test_testrun_string_free(){

        char *string = calloc(1, sizeof(char));

        assert(NULL == testrun_string_free(NULL));

        assert(NULL != string);
        string = testrun_string_free(string);
        assert(NULL == string);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_string_prepare(){

        char   *ptr = NULL;
        size_t size = 0;
        size_t open = 0;
        size_t used = 0;

        assert(false == testrun_string_prepare(NULL, NULL,  NULL,  NULL));
        assert(false == testrun_string_prepare(NULL, &size, &open, &used));
        assert(false == testrun_string_prepare(&ptr, NULL,  &open, &used));
        assert(false == testrun_string_prepare(&ptr, &size, NULL,  &used));
        assert(false == testrun_string_prepare(&ptr, &size, &open, NULL));

        // -------------------------------------------------------------
        // Positive test - new allocation
        // -------------------------------------------------------------

        assert(NULL == ptr);
        assert(true == testrun_string_prepare(&ptr, &size, &open, &used));
        assert(NULL != ptr);
        assert(size == testrun_STRING_DEFAULT_SIZE);
        assert(open == testrun_STRING_DEFAULT_SIZE);
        assert(used == 0);

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
        assert(ptr  != NULL);
        assert(size == 10);
        assert(open == 0);
        assert(used == 0);

        assert(true == testrun_string_prepare(&ptr, &size, &open, &used));

        // check
        assert(ptr  != NULL);
        assert(size == 10);
        assert(open == 6);
        assert(used == 4);

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
        assert(ptr  != NULL);
        assert(size == 11);
        assert(open == 0);
        assert(used == 0);

        assert(true == testrun_string_prepare(&ptr, &size, &open, &used));

        // check
        assert(ptr  != NULL);
        assert(size == 11);
        assert(open == 1);
        assert(used == 10);


        // -------------------------------------------------------------
        // Allocation size without final \0
        // -------------------------------------------------------------

        // reset
        size = 10;
        open = 0;
        used = 0;

        assert(false == testrun_string_prepare(&ptr, &size, &open, &used));

        // check
        assert(ptr  != NULL);
        assert(size == 10);
        assert(open == 0);
        assert(used == 10);

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


        assert(false == testrun_string_append(NULL,  NULL,    NULL, 0));
        assert(false == testrun_string_append(NULL,  &d_size, src,  s_size));
        assert(false == testrun_string_append(&dest, NULL,    src,  s_size));
        assert(false == testrun_string_append(&dest, &d_size, NULL,  s_size));
        assert(false == testrun_string_append(&dest, &d_size, src,  0));

        // -------------------------------------------------------------
        // Positive test, no realloc
        // -------------------------------------------------------------

        d_size = 10;
        dest = calloc(d_size, sizeof(char));
        strcat(dest, "test");
        src    = "1";
        expect = "test1";


        assert(true == testrun_string_append(&dest, &d_size, src,  strlen(src)));
        assert(strncmp(expect, dest, strlen(expect)) == 0);
        assert(d_size == 10);

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

        assert(true == testrun_string_append(&dest, &d_size, src,  strlen(src)));
        assert(strncmp(expect, dest, strlen(expect)) == 0);
        assert(d_size == 6);

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


        assert(true == testrun_string_append(&dest, &d_size, src,  strlen(src)));
        assert(strncmp(expect, dest, strlen(expect)) == 0);
        assert(d_size == 6);

        free(dest);
        dest = NULL;

        // -------------------------------------------------------------
        // Allocate string
        // -------------------------------------------------------------

        dest   = NULL;
        d_size = 0;

        src    = "1";
        expect = "1";

        assert(true == testrun_string_append(&dest, &d_size, src,  strlen(src)));
        assert(strncmp(expect, dest, strlen(expect)) == 0);
        assert(d_size == 2);

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
        assert(false == testrun_string_append(&dest, &d_size, src,  strlen(src)));

        // dest length == strlen(dest);
        d_size = 10;
        assert(false == testrun_string_append(&dest, &d_size, src,  strlen(src)));

        // dest length == strlen(dest) + 1;
        d_size = 11;
        assert(true == testrun_string_append(&dest, &d_size, src,  strlen(src)));
        assert(strncmp(expect, dest, strlen(expect)) == 0);
        assert(d_size == strlen(expect) + 1);

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
        assert(false == testrun_string_append(&dest, &d_size, src,  4));

        // source len  == strlen(source)
        assert(true == testrun_string_append(&dest, &d_size, src,  3));
        assert(strncmp(expect, dest, strlen(expect)) == 0);
        assert(d_size == 15);

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
        assert(true == testrun_string_append(&dest, &d_size, src,  2));
        assert(strncmp(expect, dest, strlen(expect)) == 0);
        assert(d_size == 15);

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

        assert(-1 == testrun_string_write_embeded(   NULL,
                                                0,      0, 0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(-1  == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(-1  == testrun_string_write_embeded(  NULL,
                                                &open,  &used, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(-1  == testrun_string_write_embeded(  &data,
                                                NULL,  &used, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(-1  == testrun_string_write_embeded(  &data,
                                                &open,  NULL, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(-1  == testrun_string_write_embeded(  &data,
                                                &open,  &used,  rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(-1  == testrun_string_write_embeded(  &data,
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

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == 1);
        assert(open == 99);

        bzero(&expect, 5000);
        used = 0;
        open = 100;

        prefix = NULL;
        suffix = NULL;
        source = "source";
        lbreak = NULL;

        sprintf(expect, "%s", source);
        rate = 1;

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                NULL,   0,
                                                source, strlen(source),
                                                NULL,   0,
                                                NULL,   0));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(source));
        assert(open == 100 - strlen(source));


        bzero(&expect, 5000);
        used = 0;
        open = 100;

        prefix = NULL;
        suffix = "suf";
        source = NULL;
        lbreak = NULL;

        sprintf(expect, "%s", suffix);
        rate = 1;

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                NULL,   0,
                                                NULL,   0,
                                                suffix, strlen(suffix),
                                                NULL,   0));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(suffix));
        assert(open == 100 - strlen(suffix));

        bzero(&expect, 5000);
        used = 0;
        open = 100;

        prefix = "pre";
        suffix = NULL;
        source = NULL;
        lbreak = NULL;

        sprintf(expect, "%s", prefix);
        rate = 1;

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(prefix));
        assert(open == 100 - strlen(prefix));


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

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == 100 - strlen(expect));

        // -------------------------------------------------------------
        // Check length use
        // -------------------------------------------------------------

        used = 0;
        open = 100;
        sprintf(expect, "xabcdyyy\n\n\n");
        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, 1,
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == 100 - strlen(expect));

        used = 0;
        open = 100;
        sprintf(expect, "xxxabcdyyy\n\n\n");
        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == 100 - strlen(expect));

        used = 0;
        open = 100;
        sprintf(expect, "xxxabyyy\n\n\n");
        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, 2,
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == 100 - strlen(expect));

        used = 0;
        open = 100;
        sprintf(expect, "xxxabcd\n\n\n");
        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, 0,
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == 100 - strlen(expect));

        used = 0;
        open = 100;
        sprintf(expect, "xxxabcdyyy\n");
        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, 1));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == 100 - strlen(expect));

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

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == real - used);
        assert(open == 1);

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

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == real -used);
        assert(open == 1);

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

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == real - used);
        assert(open == 7);

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

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == real - used);
        assert(open == 1);


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

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == real - used);
        assert(open == 1);

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

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == real - used);
        assert(open == 7);


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

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                0, 0,
                                                lbreak, strlen(lbreak)));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == 1);


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

        assert( strlen(expect) == testrun_string_write_embeded(  &data,
                                                &open,  &used, rate,
                                                prefix, strlen(prefix),
                                                source, strlen(source),
                                                suffix, strlen(suffix),
                                                0, 0));

        assert(strncmp(expect, data, strlen(expect)) == 0);
        assert(used == strlen(expect));
        assert(open == 1);

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

        assert(false == testrun_string_embed(NULL,   NULL,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        assert(false == testrun_string_embed(NULL,   &size,
                                        source, strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        assert(false == testrun_string_embed(&ptr,   NULL,
                                        source, strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        assert(false == testrun_string_embed(&ptr,   &size,
                                        NULL,   strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        assert(false == testrun_string_embed(&ptr,   &size,
                                        source, 0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        // -------------------------------------------------------------
        // check min valid
        // -------------------------------------------------------------

        sprintf(expect, "s");

        assert(NULL == ptr);
        assert(0 == size);

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, 1,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        assert(NULL == ptr);
        assert(0    == size);
        sprintf(expect, "%s", source);

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);


        // -------------------------------------------------------------
        // check all values
        // -------------------------------------------------------------

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        assert(NULL == ptr);
        assert(0    == size);
        sprintf(expect, "%s%s", prefix, source);

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        assert(NULL == ptr);
        assert(0    == size);
        sprintf(expect, "%s%s", source, suffix);

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        NULL,   0,
                                        suffix, strlen(suffix),
                                        NULL,   0,
                                        NULL,   0));

        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        assert(NULL == ptr);
        assert(0    == size);
        sprintf(expect, "%s", source);

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        delim1, strlen(delim1),
                                        NULL,   0));

        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        assert(NULL == ptr);
        assert(0    == size);
        sprintf(expect, "%s%s", source, delim2);

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        delim2, strlen(delim2)));

        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        free(ptr);
        ptr  = NULL;
        size = 0;

        // -------------------------------------------------------------
        // check all together
        // -------------------------------------------------------------

        assert(NULL == ptr);
        assert(0    == size);
        sprintf(expect, "%s%s%s%s", prefix, source, suffix, delim2);

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(NULL == ptr);
        assert(0    == size);
        sprintf(expect, "(a):(b):(c):(d):");

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));
        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == 100);

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

        assert(size == 100);

        size = 2;
        assert(false == testrun_string_embed(&ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(ptr, "test", strlen(ptr)) == 0);

        assert(size == 2);

        size = 3;
        assert(false == testrun_string_embed(&ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(ptr, "test", strlen(ptr)) == 0);
        assert(size == 3);


        size = 4;
        assert(false == testrun_string_embed(&ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(ptr, "test", strlen(ptr)) == 0);
        assert(size == 4);

        // -------------------------------------------------------------
        // Contained string & size fit ... adding default length
        // -------------------------------------------------------------

        size = 5;
        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE + 5);

        bzero(ptr, 100);
        strcat(ptr, "test");

        size = 6;
        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE + 6);

        bzero(ptr, 100);
        strcat(ptr, "test");

        size = 10;
        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE + 10);

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

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == 100);

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

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == 100);

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

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));



        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == 100);

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

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == 100);

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

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        delim2, strlen(delim2)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_embed( &ptr,   &size,
                                        source, strlen(source),
                                        prefix, strlen(prefix),
                                        suffix, strlen(suffix),
                                        delim1, strlen(delim1),
                                        NULL, 0));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(false == testrun_string_unset_end( NULL,    0,      NULL,   0));
        assert(false == testrun_string_unset_end( NULL,    size,   string, len));
        assert(false == testrun_string_unset_end( source,  0,      string, len));
        assert(false == testrun_string_unset_end( source,  size,   NULL,   len));
        assert(false == testrun_string_unset_end( source,  size,   string, 0));

        assert(true  == testrun_string_unset_end( source,  size,   string, len));
        assert(strncmp(source, expect, strlen(source)) == 0);

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

        assert(false == testrun_string_unset_end( source, size,  string, len));
        assert(strncmp(source, expect, strlen(source)) == 0);

        len  = strlen(string);

        assert(true == testrun_string_unset_end( source, size,  string, len));
        assert(strncmp(source, expect, strlen(source)) == 0);

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

        assert(false == testrun_string_unset_end( source, size,  string, len));
        assert(strncmp(source, expect, strlen(source)) == 0);

        // -------------------------------------------------------------
        // size == len
        // -------------------------------------------------------------

        bzero(string, 100);
        strcat(string, source);

        expect = "";

        size = strlen(source);
        len  = strlen(string);

        assert(true == testrun_string_unset_end( source, size,  string, len));
        assert(strlen(source) == 0);
        assert(strncmp(source, expect, strlen(source)) == 0);

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

        assert(false == testrun_string_unset_end( source, size,  string, len));
        assert(strncmp(source, expect, strlen(source)) == 0);

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

        assert(false == testrun_string_unset_end( source, size,  string, len));
        assert(strncmp(source, expect, strlen(source)) == 0);
        bzero(source, 100);
        expect = "source_enx";
        strcat(source, expect);

        assert(false == testrun_string_unset_end( source, size,  string, len));
        assert(strncmp(source, expect, strlen(source)) == 0);

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

        assert(false == testrun_string_replace_first(NULL, NULL,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0));

        assert(false == testrun_string_replace_first(NULL,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        assert(false == testrun_string_replace_first(&ptr,   NULL,
                                                NULL,   strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        assert(false == testrun_string_replace_first(&ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    0));

        assert(false == testrun_string_replace_first(&ptr,   &size,
                                                source, strlen(source),
                                                NULL,   strlen(old),
                                                new,    strlen(new)));

        // new length not correct
        assert(false == testrun_string_replace_first(&ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new) - 1));

        // old length not correct
        assert(false == testrun_string_replace_first(&ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old) - 1,
                                                new,    strlen(new)));

        // -------------------------------------------------------------
        // positive exchange (old + new)
        // -------------------------------------------------------------

        assert(ptr  == NULL);
        assert(size == 0);

        source = "1 2 3 4";
        old    = " ";
        new    = ":";
        expect = "1:2 3 4";

        assert(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        ptr  = testrun_string_free(ptr);
        size = 0;

        // -------------------------------------------------------------
        // positive cut (no new)
        // -------------------------------------------------------------

        assert(ptr  == NULL);
        assert(size == 0);

        source = "1 2 3 4";
        old    = " ";
        new    = ":";
        expect = "12 3 4";

        assert(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                NULL,   0));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        // reset
        ptr  = testrun_string_free(ptr);
        size = 0;

        // -------------------------------------------------------------
        // positive old not found (copy source)
        // -------------------------------------------------------------

        assert(ptr  == NULL);
        assert(size == 0);

        source = "1 2 3 4";
        old    = "x";
        new    = ":";
        expect = "1 2 3 4";

        assert(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == 10 + testrun_STRING_DEFAULT_SIZE);

        // -------------------------------------------------------------
        // append mode append with item replacement
        // -------------------------------------------------------------

        source = "1 2 3 4";
        old    = " ";
        new    = ":";
        expect = "test1 2 3 41:2 3 4";

        assert(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == 10 + testrun_STRING_DEFAULT_SIZE);

        // -------------------------------------------------------------
        // append mode append with item delete
        // -------------------------------------------------------------

        source = "1 2 3 4";
        old    = " ";
        new    = ":";
        expect = "test1 2 3 41:2 3 412 3 4";

        assert(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                NULL,   0));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == 10 + testrun_STRING_DEFAULT_SIZE);


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

        assert(true == testrun_string_replace_first( &ptr,   &size,
                                                source, strlen(source),
                                                old,    strlen(old),
                                                new,    strlen(new)));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == 10 + testrun_STRING_DEFAULT_SIZE);

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

        assert(false == testrun_string_replace_all(  NULL, NULL,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                false));

        assert(false == testrun_string_replace_all(  NULL, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                false));

        assert(false == testrun_string_replace_all(  &ptr, NULL,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                false));

        assert(false == testrun_string_replace_all(  &ptr, &size,
                                                NULL, strlen(source),
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                false));

        assert(false == testrun_string_replace_all(  &ptr, &size,
                                                source, strlen(source),
                                                NULL, strlen(delim1),
                                                NULL, 0,
                                                false));

        assert(false == testrun_string_replace_all(  &ptr, &size,
                                                source, 0,
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                false));

        assert(false == testrun_string_replace_all(  &ptr, &size,
                                                source, strlen(source),
                                                delim1, 0,
                                                NULL, 0,
                                                false));

        // -------------------------------------------------------------
        // exchange will NULL (unset all delimiters)
        // -------------------------------------------------------------

        expect = "1234";
        assert(false == testrun_string_replace_all(  &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                false));

        size = 0;
        free(ptr);
        ptr = NULL;

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                NULL, 0,
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "1:2:3:4:";

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "1:::2:::3:::4:::";

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "1 2 3 4 ";

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "x1x2x3x4x";

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "1x2x3x4x";

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                 source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        source = "CallCall ";
        delim1 = "ll";
        delim2 = "xx";
        expect = "CaxxCaxx";

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "CaxxCa";

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        expect = "wh_at_ever_wh_at_ever_at_";

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                true));

        assert(strncmp(expect, ptr, strlen(ptr)) == 0);
        assert(size == 10 + testrun_STRING_DEFAULT_SIZE);

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

        assert(true == testrun_string_replace_all(   &ptr, &size,
                                                source, strlen(source),
                                                delim1, strlen(delim1),
                                                delim2, strlen(delim2),
                                                false));

        assert(strncmp(expect, ptr, strlen(expect)) == 0);
        assert(size == 10 + testrun_STRING_DEFAULT_SIZE);

        size = 0;
        free(ptr);
        ptr = NULL;

        return testrun_log_success();
}

/*******************************************************************************
 *
 *      TEST CLUSTER
 *
 ******************************************************************************/

int all_tests() {

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

/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/


testrun_run(all_tests);
