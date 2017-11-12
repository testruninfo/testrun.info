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
 *      @file           testrun_strings_tests.c
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
#include "../../src/testrun_strings.c"

static int testing = 1;

/*******************************************************************************
 *
 *      TEST CASES
 *
 ******************************************************************************/

/*----------------------------------------------------------------------------*/

int test_testrun_create_string() {


        char    *ptr = NULL;
        size_t  len  = 0;
        size_t  size = 5000;

        char *source = "source";
        char *prefix = "prefix";
        char *suffix = "suffix";
        char *lbreak = "\n";

        char expect[size];

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%s", source);

        assert(-1 == testrun_create_string(     NULL,   NULL,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(-1 == testrun_create_string(     &ptr,   NULL,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(-1 == testrun_create_string(     NULL,   &len,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(-1 == testrun_create_string(     &ptr,   &len,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(len == 0);
        assert(!ptr);

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s", strlen(source)-2, source);

        assert(-1 == testrun_create_string(     &ptr,   &len,
                                                source, 0,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(0 == testrun_create_string(      &ptr,   &len,
                                                source, strlen(source),
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(len > 0);
        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        free(ptr);
        ptr = NULL;

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s", strlen(source)-2, source);

        assert(0 == testrun_create_string(      &ptr,   &len,
                                                source, strlen(source)-2,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0));

        assert(len > 0);
        assert(len == strlen(source) - 2);
        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        free(ptr);
        ptr = NULL;
        len = 0;

        /*-----------------------------------------------------*/

        assert(len == 0);
        assert(!ptr);

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s", strlen(prefix), prefix);

        assert(-1 ==testrun_create_string(      &ptr,   &len,
                                                NULL,   0,
                                                prefix, 0,
                                                NULL,   0,
                                                NULL,   0));

        assert(0 == testrun_create_string(      &ptr,   &len,
                                                NULL,   0,
                                                prefix, strlen(prefix),
                                                NULL,   0,
                                                NULL,   0));

        assert(len > 0);
        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        free(ptr);
        ptr = NULL;
        len = 0;

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s", strlen(prefix)-2, prefix);

        assert(0 == testrun_create_string(      &ptr,   &len,
                                                NULL,   0,
                                                prefix, strlen(prefix)-2,
                                                NULL,   0,
                                                NULL,   0));

        assert(len > 0);
        assert(len == strlen(prefix) - 2);
        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        free(ptr);
        ptr = NULL;
        len = 0;

        /*-----------------------------------------------------*/

        assert(len == 0);
        assert(!ptr);

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s", strlen(suffix), suffix);

        assert(-1 == testrun_create_string(     &ptr,   &len,
                                                NULL,   0,
                                                NULL,   0,
                                                suffix, 0,
                                                NULL,   0));

        assert(0 == testrun_create_string(      &ptr,   &len,
                                                NULL,   0,
                                                NULL,   0,
                                                suffix, strlen(suffix),
                                                NULL,   0));

        assert(len > 0);
        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        free(ptr);
        ptr = NULL;
        len = 0;

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s", strlen(suffix)-2, suffix);

        assert(0 == testrun_create_string(      &ptr,   &len,
                                                NULL,   0,
                                                NULL,   0,
                                                suffix, strlen(suffix)-2,
                                                NULL,   0));

        assert(len > 0);
        assert(len == strlen(suffix) - 2);
        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        free(ptr);
        ptr = NULL;
        len = 0;

        /*-----------------------------------------------------*/

        assert(len == 0);
        assert(!ptr);

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s", strlen(lbreak), lbreak);

        assert(-1 == testrun_create_string(     &ptr,   &len,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                lbreak, 0));

        assert(0 == testrun_create_string(      &ptr,   &len,
                                                NULL,   0,
                                                NULL,   0,
                                                NULL,   0,
                                                lbreak, strlen(lbreak)));

        assert(len > 0);
        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        free(ptr);
        ptr = NULL;
        len = 0;

        /*-----------------------------------------------------*/

        assert(len == 0);
        assert(!ptr);

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s%.*s%.*s%.*s",
                strlen(prefix), prefix,
                strlen(source), source,
                strlen(suffix), suffix,
                strlen(lbreak), lbreak);

        assert(0 == testrun_create_string(      &ptr,   &len,
                                                source, strlen(source),
                                                prefix, strlen(prefix),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak)));

        assert(len > 0);
        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        free(ptr);
        ptr = NULL;
        len = 0;

         /*-----------------------------------------------------*/

        assert(len == 0);
        assert(!ptr);

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s%.*s%.*s%.*s",
                2, prefix,
                1, source,
                3, suffix,
                strlen(lbreak), lbreak);

        assert(0 == testrun_create_string(      &ptr,   &len,
                                                source, 1,
                                                prefix, 2,
                                                suffix, 3,
                                                lbreak, strlen(lbreak)));

        assert(len > 0);
        assert(ptr);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        free(ptr);
        ptr = NULL;
        len = 0;

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_write_to() {

        size_t  size   = 5000;
        size_t  result = 0;

        char *source = "source";
        char *prefix = "prefix";
        char *suffix = "suffix";
        char *lbreak = "\n";

        char expect[size];
        char ptr[size];

        /* (re) initialize */
        memset(&expect, 0, size);
        memset(&ptr,    0, size);
        sprintf(expect, "%s", source);

        assert(0 == testrun_write_to(   NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        assert(0 == testrun_write_to(   ptr,    0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        assert(0 == testrun_write_to(   NULL,   size,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        result = testrun_write_to(      ptr,   size,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0);
        assert(0 == result);

        result = testrun_write_to(      ptr,   size,
                                        source,strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0);

        assert(strlen(source) == result);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        memset(&expect, 0, size);
        memset(&ptr,    0, size);
        sprintf(expect, "%s", source);

        assert(0 == testrun_write_to(   ptr,   strlen(source),
                                        source,strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0));

        result = testrun_write_to(      ptr,   strlen(source)+1,
                                        source,strlen(source),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0);

        assert(strlen(source) == result);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        /*-----------------------------------------------------*/

        memset(&expect, 0, size);
        memset(&ptr,    0, size);
        sprintf(expect, "%s", prefix);

        result = testrun_write_to(      ptr,   size,
                                        NULL,   0,
                                        prefix,strlen(prefix),
                                        NULL,   0,
                                        NULL,   0);

        assert(strlen(prefix) == result);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        memset(&expect, 0, size);
        memset(&ptr,    0, size);
        sprintf(expect, "%s", prefix);

        assert(0 == testrun_write_to(   ptr,   strlen(prefix),
                                        NULL,   0,
                                        prefix,strlen(prefix),
                                        NULL,   0,
                                        NULL,   0));

        result = testrun_write_to(      ptr,   strlen(prefix)+1,
                                        NULL,   0,
                                        prefix,strlen(prefix),
                                        NULL,   0,
                                        NULL,   0);

        assert(strlen(prefix) == result);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        /*-----------------------------------------------------*/

        memset(&expect, 0, size);
        memset(&ptr,    0, size);
        sprintf(expect, "%s", suffix);

        result = testrun_write_to(      ptr,   size,
                                        NULL,   0,
                                        NULL,   0,
                                        suffix,strlen(suffix),
                                        NULL,   0);

        assert(strlen(suffix) == result);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        memset(&expect, 0, size);
        memset(&ptr,    0, size);
        sprintf(expect, "%s", suffix);

        assert(0 == testrun_write_to(   ptr,   strlen(suffix),
                                        NULL,   0,
                                        NULL,   0,
                                        suffix,strlen(suffix),
                                        NULL,   0));

        result = testrun_write_to(      ptr,   strlen(suffix)+1,
                                        NULL,   0,
                                        NULL,   0,
                                        suffix,strlen(suffix),
                                        NULL,   0);

        assert(strlen(suffix) == result);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        /*-----------------------------------------------------*/

        memset(&expect, 0, size);
        memset(&ptr,    0, size);
        sprintf(expect, "%s", lbreak);

        result = testrun_write_to(      ptr,   size,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        lbreak,strlen(lbreak));

        assert(strlen(lbreak) == result);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        memset(&expect, 0, size);
        memset(&ptr,    0, size);
        sprintf(expect, "%s", lbreak);

        assert(0 == testrun_write_to(   ptr,   strlen(lbreak),
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        lbreak,strlen(lbreak)));

        result = testrun_write_to(      ptr,   strlen(lbreak)+1,
                                        NULL,   0,
                                        NULL,   0,
                                        NULL,   0,
                                        lbreak,strlen(lbreak));

        assert(strlen(lbreak) == result);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s%.*s%.*s%.*s",
                strlen(prefix), prefix,
                strlen(source), source,
                strlen(suffix), suffix,
                strlen(lbreak), lbreak);

        result = testrun_write_to(      ptr,   size,
                                        source,strlen(source),
                                        prefix,strlen(prefix),
                                        suffix,strlen(suffix),
                                        lbreak,strlen(lbreak));

        assert(result);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s%.*s%.*s%.*s",
                2, prefix,
                1, source,
                3, suffix,
                strlen(lbreak), lbreak);

        result = testrun_write_to(      ptr,   size,
                                        source,1,
                                        prefix,2,
                                        suffix,3,
                                        lbreak,strlen(lbreak));

        assert(result);
        assert(strncmp(expect, ptr, strlen(expect)) == 0);

        return testrun_log_OK();
}

/*----------------------------------------------------------------------------*/

int test_testrun_embed_string() {

        size_t  len    = 5000;
        size_t  size   = 0;
        char    *out   = NULL;

        char *source = "source";
        char *prefix = "prefix";
        char *suffix = "suffix";
        char *lbreak = "\n";
        char *delim  = "\n";

        char expect[5000];

        /* (re) initialize */
        memset(&expect, 0, len);
        sprintf(expect, "%s", source);

        assert(-1 == testrun_embed_string(      NULL, NULL,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                false));

        assert(-1 == testrun_embed_string(      &out, NULL,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                false));

        assert(-1 == testrun_embed_string(      NULL, &size,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                false));

        assert(-1 == testrun_embed_string(      &out, &size,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                false));

        assert(0 == testrun_embed_string(       &out, &size,
                                                source, 1,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                false));

        assert(size == 1);
        assert(out);
        assert(strncmp(expect, out, 1) == 0);
        free(out);
        out = NULL;

        assert(0 == testrun_embed_string(       &out, &size,
                                                source, strlen(source),
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                NULL, 0,
                                                false));
        assert(size == (strlen(source)));
        assert(out);
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        /*-----------------------------------------------------*/

        assert(0 == testrun_embed_string(       &out, &size,
                                                source, strlen(source),
                                                prefix, strlen(prefix),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak),
                                                delim,  strlen(delim),
                                                false));

         /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "%.*s%.*s%.*s%.*s",
                strlen(prefix), prefix,
                strlen(source), source,
                strlen(suffix), suffix,
                strlen(lbreak), lbreak);

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        /*-----------------------------------------------------*/

        source = "a b c";
        prefix = NULL;
        suffix = NULL;
        lbreak = "\n";
        delim  = " ";

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "a\nb\nc\n");


        assert(0 == testrun_embed_string(       &out, &size,
                                                source, strlen(source),
                                                0,0,
                                                0,0,
                                                lbreak, strlen(lbreak),
                                                delim,  strlen(delim),
                                                false));

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        /*-----------------------------------------------------*/

        source = "a,b,c";
        prefix = NULL;
        suffix = NULL;
        lbreak = "\n";
        delim  = ",";

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "a\nb\nc\n");


        assert(0 == testrun_embed_string(       &out, &size,
                                                source, strlen(source),
                                                0,0,
                                                0,0,
                                                lbreak, strlen(lbreak),
                                                delim,  strlen(delim),
                                                false));

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        /*-----------------------------------------------------*/

        source = "a:b:c";
        prefix = " ";
        suffix = " ";
        lbreak = ":";
        delim  = ":";

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, " a : b : c :");


        assert(0 == testrun_embed_string(       &out, &size,
                                                source, strlen(source),
                                                prefix, strlen(prefix),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak),
                                                delim,  strlen(delim),
                                                false));

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        /*-----------------------------------------------------*/

        source = "a:b:c";
        prefix = " ";
        suffix = " ";
        lbreak = ":";
        delim  = ":";

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, " a : b : c ");


        assert(0 == testrun_embed_string(       &out, &size,
                                                source, strlen(source),
                                                prefix, strlen(prefix),
                                                suffix, strlen(suffix),
                                                lbreak, strlen(lbreak),
                                                delim,  strlen(delim),
                                                true));

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        /*-----------------------------------------------------*/

        source = "Whatever";
        prefix = NULL;
        suffix = NULL;
        lbreak = "HEN";
        delim  = "hat";

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "WHENever");


        assert(0 == testrun_embed_string(       &out, &size,
                                                source, strlen(source),
                                                0,0,
                                                0,0,
                                                lbreak, strlen(lbreak),
                                                delim,  strlen(delim),
                                                true));

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;

        /*-----------------------------------------------------*/

        source = "a\nb\nc\n";
        prefix = "#";
        suffix = NULL;
        lbreak = "\n";
        delim  = "\n";

        /* (re) initialize */
        memset(&expect, 0, size);
        sprintf(expect, "#a\n#b\n#c\n");


        assert(0 == testrun_embed_string(       &out, &size,
                                                source, strlen(source),
                                                prefix, strlen(prefix),
                                                0,0,
                                                lbreak, strlen(lbreak),
                                                delim,  strlen(delim),
                                                true));

        assert(out);
        assert(size == strlen(expect));
        assert(strncmp(expect, out, strlen(expect)) == 0);
        free(out);
        out  = NULL;
        size = 0;


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

        testrun_test(test_testrun_create_string);
        testrun_test(test_testrun_write_to);
        testrun_test(test_testrun_embed_string);

        return 1;
}

/*******************************************************************************
 *
 *      ACTIVATED TESTS
 *
 ******************************************************************************/


testrun_run(all_tests);
