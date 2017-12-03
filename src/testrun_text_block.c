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

        @ingroup        testrun_lib

        @brief          Implementation of standard text block elements.


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

static bool testrun_text_block_write_space(
        char *buffer, size_t * size, size_t len){

        if (!buffer || !size)
                return false;

        if (len >= *size)
                return false;

        for (size_t i = 0; i < len; i++) {
                *buffer = ' ';
                buffer++;
        }

        *size -= len;

        return true;
}

/*----------------------------------------------------------------------------*/

/*
 *      Write a documentation header line with indent, tag, offset and lineend
 *      to an buffer.
 *
 *      (1) Move the *buffer to point to the stringend '\0'
 *      (2) Reduces the *size by the amount of written bytes.
 */
static bool testrun_text_block_write_docu_line(
        char **buffer,  size_t * size,
        size_t indent,  size_t offset,
        char *tag,      size_t tag_length,
        char *content,  size_t content_length,
        char *lineend,  size_t le_len){

        if (!buffer || !size || !tag)
                return false;

        //char *pointer = *buffer;

        if ((indent + tag_length) >= offset){
                offset = 1;
        } else {
                offset -= indent;
                offset -= tag_length;
        }

        if (offset < 1)
                offset = 1;

        if (*size < (indent + tag_length + offset + content_length + le_len))
                return false;

        if (!testrun_text_block_write_space(*buffer, size, indent))
                return false;

        *buffer += indent;

        //log("indent     |%s|%jd|%jd", pointer, strlen(pointer), *size);

        if (!strncat(*buffer, tag, strnlen(tag, tag_length)))
                return false;

        *buffer += strnlen(tag, tag_length);
        *size   -= tag_length;

        //log("tag        |%s|%jd|%jd", pointer, strlen(pointer), *size);

        if (!testrun_text_block_write_space(*buffer, size, offset))
                return false;

        *buffer += offset;

        //log("offset     |%s|%jd|%jd", pointer, strlen(pointer), *size);

        if (!strncat(*buffer, content, strnlen(content, content_length)))
                return false;

        *buffer += content_length;
        *size  -= content_length;

        //log("content    |%s|%jd|%jd", pointer, strlen(pointer), *size);

        if (!strncat(*buffer, lineend, strnlen(lineend, le_len)))
                return false;

        *buffer += le_len;
        *size   -= le_len;

        //log("lineend    |%s|%jd|%jd", pointer, strlen(pointer), *size);
        return true;
}

/*----------------------------------------------------------------------------*/

/*
 *      Write a documentation header line with indent, tag, offset and lineend
 *      to an buffer.
 *
 *      (1) Move the *buffer to point to the stringend '\0'
 *      (2) Reduces the *size by the amount of written bytes.
 */
static bool testrun_text_block_write_shell_commented(
        char **buffer, size_t * size, char *string){

        if (!buffer || !size)
                return false;

        if (*size < 2)
                return false;

        if (string){

                if (*size < (2 + strlen(string)))
                        return false;

                if (snprintf(*buffer, *size, "%s%s", "#", string)< 0)
                        return false;

                *buffer += (1 + strlen(string));
                *size   -= (1 + strlen(string));

        } else {

                if (snprintf(*buffer, *size, "%s", "#")< 0)
                        return false;

                *buffer += 1;
                *size   -= 1;
        }

        return true;
}
/*----------------------------------------------------------------------------*/

char *testrun_text_block_c_header_documentation(
        char *module_name,
        testrun_extension_t extension,
        struct testrun_config const * const config,
        bool documentation_tag_open,
        char *brief){

        if (!config || !module_name)
                return NULL;

        char   *result = NULL;
        size_t re_size = 0;

        // going to create:
/**

        @file           [MODULE_NAME].h
        @author         [AUTHOR_NAME]
        @date           [CREATION_DATE]

        @ingroup        [PROJECT]

        @brief


        ------------------------------------------------------------------------
*/

        char *file_extension = NULL;

        switch (extension) {

                case TESTRUN_HEADER:
                        file_extension = config->format.extensions.c_header;
                        break;
                case TESTRUN_SOURCE:
                        file_extension = config->format.extensions.c_source;
                        break;
                case TESTRUN_TEST:
                        file_extension = config->format.extensions.c_test;
                        break;
                default:
                        return NULL;
        }

        char   *lineend = config->format.line_end;
        size_t  linelen = config->format.line_length + strlen(lineend) + 1;

        size_t size   = 12 * linelen;
        size_t length = 0;
        size_t le_len = strlen(lineend);

        char buffer[size];
        bzero(buffer, size);
        char *pointer = buffer;

        // (1) Prepare content data

        char filename[linelen];
        bzero(filename, linelen);

        if (snprintf(filename, linelen, "%s%s",
                module_name, file_extension) < 0)
                goto error;

        char *date  = testrun_time_string(TESTRUN_SCOPE_DAY);
        char *split = testrun_text_block_splitline(
                        config->format.indent_c,
                        linelen - le_len,false);

        // (2) Write header buffer

        if (documentation_tag_open){

                if (!strncat(pointer, "/**", 3))
                        goto error;

                size    -= 3;
                pointer += 3;

                if (!strncat(pointer, lineend, strnlen(lineend, le_len)))
                        goto error;

                size    -= le_len;
                pointer += le_len;
        }

        //log("comment    |%s|%jd|%jd", buffer, strlen(buffer), size);

        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                config->format.indent_c,
                config->format.offset_docu,
                TESTRUN_TAG_FILE,       strlen(TESTRUN_TAG_FILE),
                filename,               strlen(filename),
                lineend,                le_len))
                goto error;

        //log("TESTRUN_TAG_FILE    |%s|%jd|%jd", buffer, strlen(buffer), size);

        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                config->format.indent_c,
                config->format.offset_docu,
                TESTRUN_TAG_AUTHOR,     strlen(TESTRUN_TAG_AUTHOR),
                config->author,         strlen(config->author),
                lineend,                le_len))
                goto error;

        //log("TESTRUN_TAG_AUTHOR    |%s|%jd|%jd", buffer, strlen(buffer), size);

        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                config->format.indent_c,
                config->format.offset_docu,
                TESTRUN_TAG_DATE,       strlen(TESTRUN_TAG_DATE),
                date,                   strlen(date),
                lineend,                le_len))
                goto error;

        //log("TESTRUN_TAG_DATE    |%s|%jd|%jd", buffer, strlen(buffer), size);

        if (!strncat(pointer, lineend, size))
                goto error;

        pointer += le_len;
        size    -= le_len;

        //log("line    |%s|%jd|%jd", buffer, strlen(buffer), size);

        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                config->format.indent_c,
                config->format.offset_docu,
                TESTRUN_TAG_GROUP,      strlen(TESTRUN_TAG_GROUP),
                config->project.name,  strlen(config->project.name),
                lineend,                le_len))
                goto error;

        //log("TESTRUN_TAG_GROUP    |%s|%jd|%jd", buffer, strlen(buffer), size);

        if (!strncat(pointer, lineend, size))
                goto error;

        pointer += le_len;
        size    -= le_len;

        //log("line    |%s|%jd|%jd", buffer, strlen(buffer), size);

        length = 0;
        if (brief)
                length = strlen(brief);

        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                config->format.indent_c,
                config->format.offset_docu,
                TESTRUN_TAG_BRIEF,      strlen(TESTRUN_TAG_BRIEF),
                brief,                  length,
                lineend,                le_len))
                goto error;

        //log("TESTRUN_TAG_BRIEF    |%s|%jd|%jd", buffer, strlen(buffer), size);


        if (snprintf(pointer, size, "%s%s%s%s*/%s",
                lineend,
                lineend,
                split, lineend,
                lineend) < 0)
                goto error;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                buffer, size,
                lineend, le_len))
                goto error;

        //log("outro    |%s|%jd|%jd", buffer, strlen(buffer), size);
        split = testrun_string_free(split);
        date  = testrun_string_free(date);
        return result;
error:
        split  = testrun_string_free(split);
        date   = testrun_string_free(date);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_sh_header_documentation(
        char *module_name,
        struct testrun_config const * const config,
        char *description,
        char *usage,
        char *dependencies){

        if (!config || !module_name)
                return NULL;

        char   *result = NULL;
        size_t re_size = 0;

        // going to create:
/*
#
#       File            [FILE].sh
#       Authors         [AUTHOR_NAME]
#       Date            [CREATION_DATE]
#
#       Project         [PROJECT]
#
#       ------------------------------------------------------------------------
*/

        char   *lineend = config->format.line_end;
        size_t  linelen = config->format.line_length + strlen(lineend) + 1;

        size_t size   = 18 * linelen;
        size_t le_len = strlen(lineend);

        char buffer[size];
        bzero(buffer, size);
        char *pointer = buffer;

        size_t length = 0;

        char *tag     = NULL;
        char comment[linelen];
        bzero(comment, linelen);

        char *date  = testrun_time_string(TESTRUN_SCOPE_DAY);
        char *split = testrun_text_block_splitline(7,linelen - le_len - 1,false);

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, lineend))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, NULL))
                goto error;

        tag = "File";
        /*
        if (snprintf(comment, linelen, "%s%s",
                module_name, config->format.extensions.shell) < 0)
                goto error;
        */
        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                7,
                config->format.offset_docu - 1,
                tag,           strlen(tag),
                module_name,   strlen(module_name),
                lineend,        le_len))
                goto error;

        //log("TESTRUN_TAG_FILE    |%s|%jd|%jd", buffer, strlen(buffer), size);

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, NULL))
                goto error;

        tag = "Authors";
        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                7,
                config->format.offset_docu - 1,
                tag,            strlen(tag),
                config->author, strlen(config->author),
                lineend,        le_len))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, NULL))
                goto error;

        tag = "Date";
        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                7,
                config->format.offset_docu - 1,
                tag,           strlen(tag),
                date,          strlen(date),
                lineend,        le_len))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, lineend))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, NULL))
                goto error;

        tag = "Project";
        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                7,
                config->format.offset_docu - 1,
                tag,           strlen(tag),
                config->project.name,        strlen(config->project.name),
                lineend,        le_len))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, lineend))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, NULL))
                goto error;

        tag = "Description";
        length = 0;
        if (description)
                length = strlen(description);
        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                7,
                config->format.offset_docu - 1,
                tag,           strlen(tag),
                description,   length,
                lineend,       le_len))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, lineend))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, NULL))
                goto error;

        tag = "Usage";
        length = 0;
        if (usage)
                length = strlen(usage);
        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                7,
                config->format.offset_docu - 1,
                tag,           strlen(tag),
                usage,          length,
                lineend,       le_len))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, lineend))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, NULL))
                goto error;

        tag = "Dependencies";
        length = 0;
        if (dependencies)
                length = strlen(dependencies);
        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                7,
                config->format.offset_docu - 1,
                tag,           strlen(tag),
                dependencies,  length,
                lineend,       le_len))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, lineend))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, NULL))
                goto error;

        tag = "Last changed";
        if (!testrun_text_block_write_docu_line(
                &pointer, &size,
                7,
                config->format.offset_docu - 1,
                tag,           strlen(tag),
                date,          strlen(date),
                lineend,        le_len))
                goto error;

        if (snprintf(pointer, size, "#%s%s%s",
                split, lineend, lineend) < 0)
                goto error;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                buffer, size,
                lineend, le_len))
                goto error;

        split = testrun_string_free(split);
        date  = testrun_string_free(date);
        return result;
error:
        split  = testrun_string_free(split);
        date   = testrun_string_free(date);
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

/*----------------------------------------------------------------------------*/

char *testrun_text_block_script(
        testrun_config *config,
        char *description,
        char *usage,
        char *dependencies,
        char *content){

        if (!config)
                return NULL;

        char    *result = NULL;
        size_t  re_size = 0;

        char    *step1  = NULL;
        char    *step2  = NULL;
        char    *step3  = NULL;
        size_t  size1   = 0;
        size_t  size2   = 0;
        size_t  size3   = 0;
        size_t  length  = 0;

        char *head = testrun_copyright_default_shell_header(
                        &config->copyright);

        char *docu = testrun_text_block_sh_header_documentation(
                        config->project.name, config,
                        description, usage, dependencies);

        // header offset used by default
        char offset[100];
        bzero(offset, 100);

        length = 100;
        if (!testrun_text_block_write_space(offset, &length,
                config->format.offset_docu))
                goto error;
        offset[0] = '#';
        length = 0;

        if (!head || !docu)
                goto error;

        if (content)
                length = strlen(content);
        else
                length = 0;

        // merge head, docu and content
        if (!testrun_string_embed(&step1, &size1,
                docu, strlen(docu),
                head, strlen(head),
                content, length,
                0,0,0,0))
                goto error;

        length = strlen(offset);

        // perform offset replacement (if any)
        if (!testrun_string_replace_all(&step2, &size2,
                step1,   size1,
                TESTRUN_TAG_OFFSET, strlen(TESTRUN_TAG_OFFSET),
                offset, length,
                false))
                goto error;

        length = strlen(config->format.line_end);

        // perform lineend replacement (if any)
        if (!testrun_string_replace_all(&step3, &size3,
                step2,   size2,
                TESTRUN_TAG_END, strlen(TESTRUN_TAG_END),
                config->format.line_end, length,
                false))
                goto error;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                step3, size3,
                config->format.line_end, length))
                goto error;

        head  = testrun_string_free(head);
        docu  = testrun_string_free(docu);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        step3 = testrun_string_free(step3);

        return result;

error:
        head  = testrun_string_free(head);
        docu  = testrun_string_free(docu);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        step3 = testrun_string_free(step3);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_readme(
        testrun_config *config,
        char *description,
        char *usage,
        char *installation){

        if (!config)
                return NULL;

        char    *result = NULL;
        size_t  re_size = 0;

        char    *step1  = NULL;
        char    *step2  = NULL;
        char    *step3  = NULL;
        char    *step4  = NULL;
        char    *step5  = NULL;
        size_t  size1   = 0;
        size_t  size2   = 0;
        size_t  size3   = 0;
        size_t  size4   = 0;
        size_t  size5   = 0;
        size_t  length  = 0;

        char *copyright = config->copyright.to_string(&config->copyright);

        size_t  size = 10000;
        char buffer[size];

        if (!description)
                description = "[TAG_DESCRIPTION]";

        if (!usage)
                usage = "[TAG_USAGE]";

        if (!installation)
                installation = "[TAG_INSTALL]";

        snprintf(buffer, size,
"# Project %s"                                                                  TESTRUN_TAG_END
TESTRUN_TAG_END
"This module is self supported and may be build, tested, installed and"         TESTRUN_TAG_END
"run independently."                                                            TESTRUN_TAG_END
TESTRUN_TAG_END
"## Overview"                                                                   TESTRUN_TAG_END
TESTRUN_TAG_END
"* [Description](#description)"                                                 TESTRUN_TAG_END
"* [Usage](#usage)"                                                             TESTRUN_TAG_END
"* [Installation](#installation)"                                               TESTRUN_TAG_END
"* [Requirements](#requirements)"                                               TESTRUN_TAG_END
"* [Structure](#structure)"                                                     TESTRUN_TAG_END
"* [Tests](#tests)"                                                             TESTRUN_TAG_END
"* [Tips](#tips)"                                                               TESTRUN_TAG_END
"* [Copyright](#copyright)   "                                                  TESTRUN_TAG_END
TESTRUN_TAG_END
"## Description"                                                                TESTRUN_TAG_END
TESTRUN_TAG_END
"[TAG_DESCRIPTION]"                                                             TESTRUN_TAG_END
TESTRUN_TAG_END
"## Usage"                                                                      TESTRUN_TAG_END
TESTRUN_TAG_END
"[TAG_USAGE]"                                                                   TESTRUN_TAG_END
TESTRUN_TAG_END
"## Installation"                                                               TESTRUN_TAG_END
TESTRUN_TAG_END
"[TAG_INSTALL]"                                                                 TESTRUN_TAG_END
""TESTRUN_TAG_END
"### build sources"                                                             TESTRUN_TAG_END
TESTRUN_TAG_END
"\\`\\`\\`bash"                                                                 TESTRUN_TAG_END
"make"                                                                          TESTRUN_TAG_END
"\\`\\`\\`"                                                                     TESTRUN_TAG_END
TESTRUN_TAG_END
"### build documentation"                                                       TESTRUN_TAG_END
TESTRUN_TAG_END
"\\`\\`\\`bash"                                                                 TESTRUN_TAG_END
"make documentation"                                                            TESTRUN_TAG_END
"\\`\\`\\`"                                                                     TESTRUN_TAG_END
TESTRUN_TAG_END
"### test sources"                                                              TESTRUN_TAG_END
TESTRUN_TAG_END
"\\`\\`\\`bash"                                                                 TESTRUN_TAG_END
"make tested"                                                                   TESTRUN_TAG_END
"\\`\\`\\`"                                                                     TESTRUN_TAG_END
TESTRUN_TAG_END
"### install binaries"                                                          TESTRUN_TAG_END
TESTRUN_TAG_END
"\\`\\`\\`bash"                                                                 TESTRUN_TAG_END
"sudo make install"                                                             TESTRUN_TAG_END
"\\`\\`\\`"                                                                     TESTRUN_TAG_END
TESTRUN_TAG_END
"### uninstall binaries"                                                        TESTRUN_TAG_END
TESTRUN_TAG_END
"\\`\\`\\`bash"                                                                 TESTRUN_TAG_END
"sudo make uninstall"                                                           TESTRUN_TAG_END
"\\`\\`\\`"                                                                     TESTRUN_TAG_END
TESTRUN_TAG_END
"## Requirements"                                                               TESTRUN_TAG_END
TESTRUN_TAG_END
"## Structure"                                                                  TESTRUN_TAG_END
TESTRUN_TAG_END
"### Default structure of the folder:"                                          TESTRUN_TAG_END
TESTRUN_TAG_END
"\\`\\`\\`"                                                                     TESTRUN_TAG_END
"<pre>"                                                                         TESTRUN_TAG_END
"."                                                                             TESTRUN_TAG_END
"├── COPYRIGHT"                                                                 TESTRUN_TAG_END
"├── README.MD"                                                                 TESTRUN_TAG_END
"├── makefile"                                                                  TESTRUN_TAG_END
"├── testrun_makefile.main"                                                     TESTRUN_TAG_END
"├── testrun_makefile.test"                                                     TESTRUN_TAG_END
"│"                                                                             TESTRUN_TAG_END
"├── doxygen"                                                                   TESTRUN_TAG_END
"│   ├── documentation"                                                         TESTRUN_TAG_END
"│   └── doxygen.config"                                                        TESTRUN_TAG_END
"│"                                                                             TESTRUN_TAG_END
"├── docs"                                                                      TESTRUN_TAG_END
"│   ├── CHANGELOG.MD"                                                          TESTRUN_TAG_END
"│   └── ..."                                                                   TESTRUN_TAG_END
"│"                                                                             TESTRUN_TAG_END
"├── include"                                                                   TESTRUN_TAG_END
"│   ├── %s.h"                                                                  TESTRUN_TAG_END
"│   └── ..."                                                                   TESTRUN_TAG_END
"│"                                                                             TESTRUN_TAG_END
"├── src"                                                                       TESTRUN_TAG_END
"│   ├── %s.c"                                                                  TESTRUN_TAG_END
"│   └── ..."                                                                   TESTRUN_TAG_END
"│"                                                                             TESTRUN_TAG_END
"└── tests"                                                                     TESTRUN_TAG_END
"    ├── resources"                                                             TESTRUN_TAG_END
"    ├── tools"                                                                 TESTRUN_TAG_END
"    │   ├── testrun_runner.h"                                                  TESTRUN_TAG_END
"    │   ├── testrun_simple_coverage_tests.sh"                                  TESTRUN_TAG_END
"    │   ├── testrun_simple_unit_tests.sh"                                      TESTRUN_TAG_END
"    │   ├── testrun_simple_acceptance_tests.sh"                                TESTRUN_TAG_END
"    │   └── testrun_simple_loc.sh"                                             TESTRUN_TAG_END
"    ├── acceptance"                                                            TESTRUN_TAG_END
"    │   ├── ..."                                                               TESTRUN_TAG_END
"    │   └── ..."                                                               TESTRUN_TAG_END
"    └── unit"                                                                  TESTRUN_TAG_END
"        ├── %s%s.c"                                                            TESTRUN_TAG_END
"        └── ..."                                                               TESTRUN_TAG_END
TESTRUN_TAG_END
"</pre>"                                                                        TESTRUN_TAG_END
"\\`\\`\\`"                                                                     TESTRUN_TAG_END
TESTRUN_TAG_END
"## Tests"                                                                      TESTRUN_TAG_END
TESTRUN_TAG_END
"All test sources of will be recompiled on each make run. That means, "         TESTRUN_TAG_END
"all module tests will be created new on any change in any source file.  "      TESTRUN_TAG_END
TESTRUN_TAG_END
"### Test a project (all files contained in tests/unit)"                        TESTRUN_TAG_END
TESTRUN_TAG_END
"Test compile and run "                                                         TESTRUN_TAG_END
"~~~"                                                                           TESTRUN_TAG_END
"make tested"                                                                   TESTRUN_TAG_END
"~~~"                                                                           TESTRUN_TAG_END
TESTRUN_TAG_END
"### Test a dedicated source file (single file of tests/unit)"                  TESTRUN_TAG_END
TESTRUN_TAG_END
"To develop a test for a specific source file, "                                TESTRUN_TAG_END
"it may be helpful to use a separated test run. "                               TESTRUN_TAG_END
"This may be done via an integrated make functionality."                        TESTRUN_TAG_END
TESTRUN_TAG_END
"Test compile example"                                                          TESTRUN_TAG_END
"~~~"                                                                           TESTRUN_TAG_END
"make test testname=tests/unit/filename_tests.c "                               TESTRUN_TAG_END
"~~~"                                                                           TESTRUN_TAG_END
TESTRUN_TAG_END
"Test compile and run in valgrind"                                              TESTRUN_TAG_END
"~~~"                                                                           TESTRUN_TAG_END
"make test testname=tests/unit/filename_tests.c && valgrind ./build/test/unit/filename_tests.test "TESTRUN_TAG_END
"~~~"                                                                           TESTRUN_TAG_END
TESTRUN_TAG_END
"## Tips"                                                                       TESTRUN_TAG_END
TESTRUN_TAG_END
"## Copyright"                                                                  TESTRUN_TAG_END
TESTRUN_TAG_END
"[TAG_COPYRIGHT]"                                                               TESTRUN_TAG_END,
        config->project.name,
        config->project.name,
        config->project.name,
        config->project.name,
        config->format.suffix.tests_source);


        length = strlen(description);
        // perform dynamic length adding of description
        if (!testrun_string_replace_all(&step1, &size1,
                buffer,   size,
                "[TAG_DESCRIPTION]", strlen("[TAG_DESCRIPTION]"),
                description, length,
                false))
                goto error;


        length = strlen(usage);
        // perform dynamic length adding of usage
        if (!testrun_string_replace_all(&step2, &size2,
                step1,   size1,
                "[TAG_USAGE]", strlen("[TAG_USAGE]"),
                usage, length,
                false))
                goto error;

        length = strlen(installation);
        // perform dynamic length adding of installation
        if (!testrun_string_replace_all(&step3, &size3,
                step2,   size2,
                "[TAG_INSTALL]", strlen("[TAG_INSTALL]"),
                installation, length,
                false))
                goto error;

        length = strlen(copyright);
        // perform dynamic length adding of copyright
        if (!testrun_string_replace_all(&step4, &size4,
                step3,   size3,
                "[TAG_COPYRIGHT]", strlen("[TAG_COPYRIGHT]"),
                copyright, length,
                false))
                goto error;

        length = strlen(config->format.line_end);
        // perform lineend replacement (if any)
        if (!testrun_string_replace_all(&step5, &size5,
                step4,   size4,
                TESTRUN_TAG_END, strlen(TESTRUN_TAG_END),
                config->format.line_end, length,
                false))
                goto error;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                step5, size5,
                config->format.line_end, length))
                goto error;

        copyright = testrun_string_free(copyright);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        step3 = testrun_string_free(step3);
        step4 = testrun_string_free(step4);
        step5 = testrun_string_free(step5);

        return result;

error:
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        step3 = testrun_string_free(step3);
        step4 = testrun_string_free(step4);
        step5 = testrun_string_free(step5);
        result = testrun_string_free(result);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_doxygen_config(
        testrun_config *config){

        if (!config)
                return NULL;

        char    *result = NULL;
        size_t  re_size = 0;

        char    *step1 = NULL;
        size_t  size1  = 0;

        size_t  size = 5000;
        char buffer[size];

        bzero(buffer, size);

        char path_doxygen[PATH_MAX];
        char path_include[PATH_MAX];
        char path_src[PATH_MAX];
        char path_tests[PATH_MAX];

        if (!testrun_path_project_to_doxygen(path_doxygen, PATH_MAX, config))
                return NULL;

        if (!testrun_path_project_to_tests(path_tests, PATH_MAX, config))
                return NULL;

        if (!testrun_path_project_to_include(path_include, PATH_MAX, config))
                return NULL;

        if (!testrun_path_project_to_source(path_src, PATH_MAX, config))
                return NULL;

        if (snprintf(buffer, size,
                "DOXYFILE_ENCODING       = UTF-8"                               TESTRUN_TAG_END
                "PROJECT_NAME            = %s"                                  TESTRUN_TAG_END
                "PROJECT_NUMBER          = 0.0.1"                               TESTRUN_TAG_END
                "PROJECT_LOGO            = %s/logo_doxygen.png"                 TESTRUN_TAG_END
                "PROJECT_BRIEF           = \"%s brief\""                        TESTRUN_TAG_END
                "OUTPUT_DIRECTORY        = %s/documentation"                    TESTRUN_TAG_END
                "CREATE_SUBDIRS          = NO"                                  TESTRUN_TAG_END
                "ALLOW_UNICODE_NAMES     = NO"                                  TESTRUN_TAG_END
                "OUTPUT_LANGUAGE         = English"                             TESTRUN_TAG_END
                "MARKDOWN_SUPPORT        = YES"                                 TESTRUN_TAG_END
                "AUTOLINK_SUPPORT        = YES"                                 TESTRUN_TAG_END
                "INPUT                   = README.MD %s %s %s"                  TESTRUN_TAG_END
                "INPUT_ENCODING          = UTF-8"                               TESTRUN_TAG_END
                "FILE_PATTERNS           = *.h *.c *.js *.py *.sh"              TESTRUN_TAG_END
                "RECURSIVE               = YES"                                 TESTRUN_TAG_END
                "EXCLUDE_SYMLINKS        = YES"                                 TESTRUN_TAG_END,
                config->project.name,  path_doxygen, config->project.name,
                path_doxygen, path_include, path_src, path_tests
        )< 0)
                return NULL;


        if (!testrun_string_replace_all(&step1, &size1,
                buffer,   size,
                TESTRUN_TAG_END, strlen(TESTRUN_TAG_END),
                config->format.line_end, strlen(config->format.line_end),
                false))
                goto error;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                step1, size1,
                config->format.line_end, strlen(config->format.line_end)))
                goto error;

        step1 = testrun_string_free(step1);
        return result;
error:
        step1 = testrun_string_free(step1);
        result = testrun_string_free(result);
        return NULL;
}
