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

        @file           testrun_text_block.c
        @author         Markus Toepfer
        @date           2017-11-26

        @ingroup

        @brief


        ------------------------------------------------------------------------
*/

#include "../include/testrun_text_block.h"

/*
 *      ------------------------------------------------------------------------
 *
 *      C FILE BODYs                                                    #CBC
 *
 *      Default body content of the standard c files of a module.
 *
 *      ------------------------------------------------------------------------
 */

char *testrun_text_block_header_body(char *name){

        if (!name)
                return NULL;

        char *snipped = calloc(500, sizeof(char));
        if (!snipped)
                return NULL;

        sprintf(snipped,
                "#ifndef %s_h" TESTRUN_LINEEND
                "#define %s_h" TESTRUN_LINEEND
                TESTRUN_LINEEND
                "#endif /* %s_h */" TESTRUN_LINEEND,
                name, name, name);

        return snipped;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_source_body(char *name){

        if (!name)
                return NULL;

        char *snipped = calloc(500, sizeof(char));
        if (!snipped)
                return NULL;

        sprintf(snipped,
                "#include \"%s\"" TESTRUN_LINEEND,
                name);
        return snipped;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_test_body(
        char *path_src,
        char *path_testrun){

        if (!path_src || !path_testrun)
                return NULL;

        size_t len_src = strlen(path_src);
        size_t len_run = strlen(path_testrun);

        if ( (len_run < 4) || (len_src < 4) )
                return NULL;

        if (path_testrun[len_run -1 ]   != 'h')
                return NULL;
        if (path_testrun[len_run -2 ]   != '.')
                return NULL;

        if (path_src[len_src -1 ]       != 'c')
                return NULL;
        if (path_src[len_src -2 ]       != '.')
                return NULL;

        char *snipped = calloc(2500, sizeof(char));
        if (!snipped)
                return NULL;

        char *splitline = testrun_text_block_splitline(0, 81, true);
        char *helper    = testrun_text_block_comment_header(
                        "TEST HELPER", "HELPER");
        char *cases     = testrun_text_block_comment_header(
                        "TEST CASES", "CASES");
        char *cluster   = testrun_text_block_comment_header(
                        "TEST CLUSTER", "CLUSTER");
        char *exec      = testrun_text_block_comment_header(
                        "TEST EXECUTION", "EXEC");

        if (!splitline || !helper || !cases || !cluster || !exec)
                goto error;

        sprintf(snipped,
                TESTRUN_LINEEND
                "#include \"%s\""                       TESTRUN_LINEEND
                "#include \"%s\""                       TESTRUN_LINEEND
                TESTRUN_LINEEND
                "%s"                                    // #HELPER
                TESTRUN_LINEEND
                "%s"                                    // splitline
                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "%s"                                    // #CASES
                TESTRUN_LINEEND
                "%s"                                    // splitline
                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "/**"                                   TESTRUN_LINEEND
                "int test_ ... (){"                     TESTRUN_LINEEND
                "        testrun(true);"                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        return testrun_log_success();" TESTRUN_LINEEND
                "}"                                     TESTRUN_LINEEND
                "*/"                                    TESTRUN_LINEEND
                TESTRUN_LINEEND
                "%s"                                    // splitline
                TESTRUN_LINEEND
                TESTRUN_LINEEND
                "%s"                                    // #CLUSTER
                TESTRUN_LINEEND
                "int all_tests() {"                     TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        testrun_init();"               TESTRUN_LINEEND
                "        //testrun_test(test_);"        TESTRUN_LINEEND
                TESTRUN_LINEEND
                "        return testrun_counter;"       TESTRUN_LINEEND
                "}"TESTRUN_LINEEND
                TESTRUN_LINEEND
                "%s"                                    // #EXEC
                TESTRUN_LINEEND
                "testrun_run(all_tests);"               TESTRUN_LINEEND,
                path_testrun, path_src,
                helper,
                splitline,
                cases,
                splitline,
                splitline,
                cluster,
                exec
                );

        testrun_string_free(splitline);
        testrun_string_free(helper);
        testrun_string_free(cases);
        testrun_string_free(cluster);
        testrun_string_free(exec);
        return snipped;
error:
        testrun_string_free(splitline);
        testrun_string_free(helper);
        testrun_string_free(cases);
        testrun_string_free(cluster);
        testrun_string_free(exec);
        return NULL;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      C HEADER CONTENT BLOCKS                                         #CHC
 *
 *      Default body content of the standard c files of a module.
 *
 *      ------------------------------------------------------------------------
 */

char *testrun_text_block_c_header_documentation(
        char *filename,
        char *extension,
        char *author,
        char *date,
        char *project,
        bool documentation_tag_open){

        if (!filename)
                filename = TESTRUN_TAG_DEFAULT_MODULE;

        if (!extension)
                extension = "c";

        if (!author)
                author = TESTRUN_TAG_DEFAULT_AUTHOR;

        if (!date)
                date = TESTRUN_TAG_DEFAULT_DATE;

        if (!project)
                project = TESTRUN_TAG_DEFAULT_PROJECT;

        size_t linelength = 81;
        size_t i = 0;
        size_t indent = 8;


        char *step1  = NULL;
        char *step2  = NULL;
        char *step3  = NULL;
        char *suffix = "*/";

        size_t size1  = 0;
        size_t size2  = 0;
        size_t size3  = 0;
        size_t px_len = linelength;
        size_t le_len = strlen(TESTRUN_LINEEND);

        size_t size = 1000;
        size_t open = size;

        char string[size];
        char prefix[px_len];
        bzero(string, size);
        bzero(prefix, px_len);

        // (1) prepare the content header

        /* ------------------------------------------ */

        strncat(string, TESTRUN_TAG_FILE, open);
        size = strlen(TESTRUN_TAG_FILE);
        open -= size;
        for (i = size; i < 2*indent; i++){
                strncat(string, " ", open);
                open--;
        }

        size = strlen(filename);
        if (size > open)
                goto error;
        strncat(string, filename, open);
        open -= size;

        size = strlen(extension);
        if (open < size + 1)
                goto error;
        strcat(string, ".");
        open--;
        strncat(string, extension, open);
        open -= size;

        if (open < le_len)
                goto error;
        strcat(string, TESTRUN_LINEEND);
        open -= le_len;

        /* ------------------------------------------ */

        size = strlen(TESTRUN_TAG_AUTHOR);
        if (size > open)
                goto error;
        strncat(string, TESTRUN_TAG_AUTHOR, open);
        open -= size;
        for (i = size; i < 2*indent; i++){
                strncat(string, " ", open);
                open--;
        }

        size = strlen(author);
        if (size > open)
                goto error;
        strncat(string, author, open);
        open -= size;

        if (open < le_len)
                goto error;
        strcat(string, TESTRUN_LINEEND);
        open -= le_len;

        /* ------------------------------------------ */

        size = strlen(TESTRUN_TAG_DATE);
        if (size > open)
                goto error;
        strncat(string, TESTRUN_TAG_DATE, open);
        open -= size;
        for (i = size; i < 2*indent; i++){
                strncat(string, " ", open);
                open--;
        }

        size = strlen(date);
        if (size > open)
                goto error;
        strncat(string, date, open);
        open -= size;

        if (open < le_len)
                goto error;
        strcat(string, TESTRUN_LINEEND);
        open -= le_len;

        /* ------------------------------------------ */

        if (open < le_len)
                goto error;
        strcat(string, TESTRUN_LINEEND);
        open -= le_len;

        /* ------------------------------------------ */

        size = strlen(TESTRUN_TAG_GROUP);
        if (size > open)
                goto error;
        strncat(string, TESTRUN_TAG_GROUP, open);
        open -= size;
        for (i = size; i < 2*indent; i++){
                strncat(string, " ", open);
                open--;
        }

        size = strlen(project);
        if (size > open)
                goto error;
        strncat(string, project, open);
        open -= size;

        if (open < le_len)
                goto error;
        strcat(string, TESTRUN_LINEEND);
        open -= le_len;

        /* ------------------------------------------ */

        if (open < le_len)
                goto error;
        strcat(string, TESTRUN_LINEEND);
        open -= le_len;

        /* ------------------------------------------ */

        size = strlen(TESTRUN_TAG_BRIEF);
        if (size > open)
                goto error;
        strncat(string, TESTRUN_TAG_BRIEF, open);
        open -= size;
        for (i = size; i < 2*indent; i++){
                strncat(string, " ", open);
                open--;
        }

        if (open < 3 * le_len)
                goto error;
        strcat(string, TESTRUN_LINEEND);
        strcat(string, TESTRUN_LINEEND);
        strcat(string, TESTRUN_LINEEND);
        open -= 3 * le_len;

        /* ------------------------------------------ */

        if (open < linelength)
                goto error;

        for (i = 0; i < linelength - 1 - indent; i++){
                strcat(string, "-");
        }

        /* ------------------------------------------ */

        for (i = 0; i < indent; i++){
                prefix[i] = ' ';
        }

        // (2) prefix the content header wiht indent

        if (!testrun_string_embed(&step1, &size1,
                string, strlen(string),
                prefix, strlen(prefix),
                NULL, 0,
                TESTRUN_LINEEND, le_len,
                TESTRUN_LINEEND, le_len))
                goto error;

        // (3) prefix and suffix the result of step1

        if (documentation_tag_open){

                snprintf(prefix, px_len, "/**%s", TESTRUN_LINEEND);

                if (!testrun_string_embed(&step2, &size2,
                        step1,  size1,
                        prefix, strlen(prefix),
                        suffix, strlen(suffix),
                        NULL, 0,
                        NULL, 0))
                        goto error;
        } else {

                if (!testrun_string_embed(&step2, &size2,
                        step1,  size1,
                        NULL, 0,
                        suffix, strlen(suffix),
                        NULL, 0,
                        NULL, 0))
                        goto error;
        }

        // (4) remove whitespace
        if (!testrun_string_clear_whitespace_before_lineend(&step3, &size3,
                step2, size2,
                TESTRUN_LINEEND, le_len))
                goto error;

        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        return step3;
error:
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        step3 = testrun_string_free(step3);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_comment_header(
        char *name, char *tag){

        if (!name)
                name = TESTRUN_TAG_DEFAULT_NAME;

        if (!tag)
                tag = TESTRUN_TAG_DEFAULT_TAG;

        char    *out    = NULL;
        size_t  out_len = 0;

        size_t  i       = 0;
        size_t  indent  = 8;
        size_t  linelen = 81;
        size_t  le_size = strlen(TESTRUN_LINEEND);

        // This calculation is based on TAGS < indent with indent of 8
        size_t  offset  = ((linelen/indent) - 1) * indent;

        linelen += le_size;
        size_t  re_size = 7 * linelen;

        char result[re_size];
        bzero(result, re_size);
        char linein[linelen];
        bzero(linein, linelen);

        char *splitline = testrun_text_block_splitline(
                indent-2, linelen - le_size -2, false);

        for (i = 0; i < indent; i++){
                if (i == 1)
                        linein[i] = '*';
                else
                        linein[i] = ' ';
        }

        size_t string_len =     strlen(linein) +
                                strlen(name) +
                                strlen(tag) +
                                linelen;

        char string[string_len];
        bzero(string, string_len);

        strcat(string, linein);
        strcat(string, name);
        for (i = (strlen(linein) + strlen(name)); i < offset; i++){
                string[i] = ' ';
        }
        strcat(string, "#");
        strcat(string, tag);

        snprintf(result, re_size,
                "/*"    TESTRUN_LINEEND            // PREFIX
                " *%s"  TESTRUN_LINEEND            // PREFIX
                " *"    TESTRUN_LINEEND            // PREFIX
                "%s"    TESTRUN_LINEEND            // STRING
                " *"    TESTRUN_LINEEND            // SUFFIX
                " *%s"  TESTRUN_LINEEND            // SUFFIX
                " */"   TESTRUN_LINEEND,           // SUFFIX
                splitline, string, splitline);

         if (!testrun_string_clear_whitespace_before_lineend(&out, &out_len,
                result, re_size,
                TESTRUN_LINEEND, le_size))
                goto error;

        splitline = testrun_string_free(splitline);
        return out;
error:
        out = testrun_string_free(out);
        splitline = testrun_string_free(splitline);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_splitline(
        size_t indent, size_t linelength, bool commented){

        if (linelength <= indent + 1)
                return NULL;

        if (commented)
                if (linelength <= indent + 4 + 1)
                        return NULL;

        size_t used = 0, i = 0;

        char *line = calloc(linelength, sizeof(char));

        if (commented){
                strcat(line, "/*");
                used += 2;
        }

        for (i = used; i < linelength -1; i++){
                if (i < used + indent)
                        line[i] = ' ';
                else
                        line[i] = '-';
        }

        if (commented){
                line[i-2] = '*';
                line[i-1] = '/';
        }

        return line;
}
