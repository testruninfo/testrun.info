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

char *testrun_text_block_header_body(char const * const name){

        if (!name)
                return NULL;

        char *snipped = calloc(500, sizeof(char));
        if (!snipped)
                return NULL;

        sprintf(snipped,
                "#ifndef %s_h\n"
                "#define %s_h\n"
                "\n"
                "#endif /* %s_h */\n" ,
                name, name, name);

        return snipped;
}

/*-------------------------------------------------------------------------------------------*/

char *testrun_text_block_source_body(char const * const name){

        if (!name)
                return NULL;

        char *snipped = calloc(500, sizeof(char));
        if (!snipped)
                return NULL;

        sprintf(snipped, "#include \"%s\"\n", name);
        return snipped;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_test_body(
        char const * const path_src,
        char const * const path_testrun){

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
                "\n"
                "#include \"%s\"\n"
                "#include \"%s\"\n"
                "\n"
                "%s"    // #HELPER
                "\n"
                "%s"    // splitline
                "\n"
                "\n"
                "%s"    // #CASES
                "\n"
                "%s"    // splitline
                "\n"
                "\n"
                "int test_case(){\n"
                "        testrun(1 == 1);\n"
                "\n"
                "        return testrun_log_success();\n"
                "}\n"
                "\n"
                "%s"    // splitline
                "\n"
                "\n"
                "%s"    // #CLUSTER
                "\n"
                "int all_tests() {\n"
                "\n"
                "        testrun_init();\n"
                "        testrun_test(test_case);\n"
                "\n"
                "        return testrun_counter;\n"
                "}""\n"
                "\n"
                "%s"    // #EXEC
                "\n"
                "testrun_run(all_tests);\n",
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
        *size   -= content_length;

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
        char const * const module_name,
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
                        file_extension = ".h";
                        break;
                case TESTRUN_SOURCE:
                        file_extension = ".c";
                        break;
                case TESTRUN_TEST:
                        file_extension = "_test.c";
                        break;
                default:
                        return NULL;
        }

        char   *lineend = "\n";
        size_t  linelen = config->format.line_length + 2;

        size_t size   = 12 * linelen;
        size_t length = 0;
        size_t le_len = strlen(lineend);

        char buffer[size];
        bzero(buffer, size);
        char *pointer = buffer;

        // (1) Prepare content data

        char filename[linelen];
        bzero(filename, linelen);

        char *date  = testrun_time_string(TESTRUN_SCOPE_DAY);
        char *split = testrun_text_block_splitline(
                        config->format.indent_c,
                        linelen - le_len,false);

        if (snprintf(filename, linelen, "%s%s",
                module_name, file_extension) < 0)
                goto error;

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

        size_t linelen = config->format.line_length + 2;

        size_t size   = 50 * linelen;

        char buffer[size];
        bzero(buffer, size);
        char *pointer = buffer;

        size_t length = 0;

        char *tag     = NULL;
        char comment[linelen];
        bzero(comment, linelen);

        char *date  = testrun_time_string(TESTRUN_SCOPE_DAY);
        char *split = testrun_text_block_splitline(7,linelen - 2,false);

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, "\n"))
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
                "\n",          1))
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
                "\n",          1))
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
                "\n",          1))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, "\n"))
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
                "\n",          1))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, "\n"))
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
                "\n",          1))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, "\n"))
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
                "\n",          1))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, "\n"))
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
                "\n",          1))
                goto error;

        if (!testrun_text_block_write_shell_commented(
                &pointer, &size, "\n"))
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
                "\n",          1))
                goto error;

        if (snprintf(pointer, size, "#%s%s%s",
                split, "\n", "\n") < 0)
                goto error;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                buffer, size,
                "\n",          1))
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

        // This calculation is based on TAGS < indent with indent of 8
        size_t  offset  = ((linelen/indent) - 1) * indent;

        linelen += 1;
        size_t  re_size = 7 * linelen;

        char result[re_size];
        bzero(result, re_size);
        char linein[linelen];
        bzero(linein, linelen);

        char *splitline = testrun_text_block_splitline(
                indent-2, linelen - 1 -2, false);

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
                "/*\n"   // PREFIX
                " *%s\n" // PREFIX
                " *\n"   // PREFIX
                "%s\n"   // STRING
                " *\n"   // SUFFIX
                " *%s\n" // SUFFIX
                " */\n"  // SUFFIX
                ,splitline, string, splitline);

         if (!testrun_string_clear_whitespace_before_lineend(&out, &out_len,
                result, re_size,
                "\n", 1))
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
        testrun_config const * const config,
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

        // perform lineend replacement (if any)
        if (!testrun_string_replace_all(&step3, &size3,
                step2,   size2,
                "\n", 1,
                "\n", 1,
                false))
                goto error;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                step3, size3,
                "\n", 1))
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
        testrun_config const * const config,
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
        """\n"
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
        "[TAG_COPYRIGHT]\n",
        config->project.name,
        config->project.name,
        config->project.name,
        config->project.name);


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

        // perform lineend replacement (if any)
        if (!testrun_string_replace_all(&step5, &size5,
                step4,   size4,
                "\n", 1,
                "\n", 1,
                false))
                goto error;

        log("%s", result);

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                step5, size5,
                "\n", 1))
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
        testrun_config const * const config){

        if (!config)
                return NULL;

        char    *result = NULL;
        size_t  re_size = 0;

        size_t  size = 5000;
        char buffer[size];

        bzero(buffer, size);

        if (snprintf(buffer, size,
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
                "EXCLUDE_SYMLINKS        = YES\n",
                config->project.name, config->project.name)< 0)
                return NULL;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                buffer, size,
                "\n", 1))
                goto error;

        return result;
error:
        result = testrun_string_free(result);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_service_file(
        testrun_config const * const config){

        if (!config)
                return NULL;

        char    *result = NULL;
        size_t  re_size = 0;

        size_t  size = 5000;
        char buffer[size];

        bzero(buffer, size);

        if (snprintf(buffer, size,
                "[Unit]\n"
                "Description= %s service\n"
                "\n"
                "[Service]\n"
                "ExecStart=/usr/local/bin/%s\n"
                "NonBlocking=True\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target\n"
                , config->project.name, config->project.name)< 0)
                return NULL;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                buffer, size,
                "\n", 1))
                goto error;

        return result;
error:
        result = testrun_string_free(result);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_socket_file(
        testrun_config const * const config){

        if (!config)
                return NULL;

        char    *result = NULL;
        size_t  re_size = 0;

        size_t  size = 5000;
        char buffer[size];

        bzero(buffer, size);

        if (snprintf(buffer, size,
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
                , config->project.name)< 0)
                return NULL;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                buffer, size,
                "\n", 1))
                goto error;

        return result;
error:
        result = testrun_string_free(result);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_script_install(
        testrun_config const * const config){

        if (!config)
                return NULL;

        char    *result = NULL;
        size_t  re_size = 0;

        size_t  size = 10000;
        char buffer[size];

        bzero(buffer, size);

        if (snprintf(buffer, size,
"MODNAME=%s\n"
"CONFIGDIR=\"/etc/$MODNAME\"\n"
"\n"
"# Make sure only root can run our script\n"
"if [[ $EUID -ne 0 ]]; then\n"
"        echo \"\"\n"
"        echo \"This script must be run as root\" 1>&2\n"
"        echo \"\"\n"
"        exit 1\n"
"fi\n"
"\n"
"DIR=\"$( cd \"$( dirname \"${BASH_SOURCE[0]}\" )\" && pwd )\"\n"
"cd $DIR\n"
"\n"
"# ----------------------------------------------------------------------------\n"
"#       CHECK SOCKET CONFIG DONE\n"
"# ----------------------------------------------------------------------------\n"
"\n"
"grep '^ListenStream' $MODNAME.socket\n"
"if [[ $? -eq 1 ]]; then\n"
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
"cp ${DIR}/../../bin/$MODNAME /usr/local/bin/$MODNAME\n"
"if [ $? -eq 0 ]; then\n"
"        echo \"copied $MODNAME to /usr/local/bin/\"\n"
"else\n"
"        echo \"FAILURE ... stopping, please check manually.\"\n"
"        exit 1\n"
"fi\n"
"\n"
"echo \"... copying systemd service descriptions.\"\n"
"cp ${DIR}/../install/$MODNAME* /etc/systemd/system/\n"
"if [ $? -eq 0 ]; then\n"
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
"if [ $? -eq 0 ]; then\n"
"        echo \"$MODNAME.socket enabled\"\n"
"        systemctl start $MODNAME.socket\n"
"        if [ $? -eq 0 ]; then\n"
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
"if [ $? -eq 0 ]; then\n"
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
"mkdir -p $CONFIGDIR\n"
"mkdir -p $CONFIGDIR\"/%s\"\n"
"cp -r ${DIR}/../config/%s/*      $CONFIGDIR\"/%s\" 2>/dev/null || :\n"
"cp -r ${DIR}/../config/*.config  $CONFIGDIR 2>/dev/null || :\n"
"cp -r ${DIR}/../config/*.conf    $CONFIGDIR 2>/dev/null || :\n"
"cp -r ${DIR}/../config/*.ini     $CONFIGDIR 2>/dev/null || :\n"
"cp -r ${DIR}/../config/*.cfg     $CONFIGDIR 2>/dev/null || :\n"
, config->project.name,
TESTRUN_FOLDER_SERVICE_DATA,
TESTRUN_FOLDER_SERVICE_DATA,
TESTRUN_FOLDER_SERVICE_DATA) < 0)
        return NULL;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                buffer, size,
                "\n", 1))
                goto error;

        return result;
error:
        result = testrun_string_free(result);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_script_uninstall(
        testrun_config const * const config){

        if (!config)
                return NULL;

        char    *result = NULL;
        size_t  re_size = 0;

        size_t  size = 10000;
        char buffer[size];

        bzero(buffer, size);

        if (snprintf(buffer, size,
"MODNAME=%s\n"
"CONFIGDIR=\"/etc/$MODNAME\"\n"
"\n"
"# Make sure only root can run our script\n"
"if [[ $EUID -ne 0 ]]; then\n"
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
, config->project.name) < 0)
        return NULL;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                buffer, size,
                "\n", 1))
                goto error;

        return result;
error:
        result = testrun_string_free(result);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_changelog_file(
        testrun_config const * const config){

        if (!config)
                return NULL;

        char    *date   = testrun_time_string(TESTRUN_SCOPE_DAY);

        char    *result = NULL;
        size_t  re_size = 0;

        size_t  size = 10000;
        char buffer[size];

        bzero(buffer, size);

        if (snprintf(buffer, size,
                "Changelog for project %s created %s\n",
                config->project.name, date) < 0)
                goto error;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                buffer, size,
                "\n", 1))
                goto error;

        date = testrun_string_free(date);
        return result;
error:
        date = testrun_string_free(date);
        result = testrun_string_free(result);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_text_block_gitignore_file(
        testrun_config const * const config){

        if (!config)
                return NULL;

        char    *result = NULL;
        size_t  re_size = 0;

        size_t  size = 10000;
        char buffer[size];

        if (snprintf(buffer, size,
                "# Prerequisites\n"
                "*.d\n"
                "\n"
                "# Object files\n"
                "*.o\n"
                "*.ko\n"
                "*.obj\n"
                "*.elf\n"
                "\n"
                "# Linker output\n"
                "*.ilk\n"
                "*.map\n"
                "*.exp\n"
                "\n"
                "# Precompiled Headers\n"
                "*.gch\n"
                "*.pch\n"
                "\n"
                "# Libraries\n"
                "*.lib\n"
                "*.a\n"
                "*.la\n"
                "*.lo\n"
                "\n"
                "# Shared objects (inc. Windows DLLs)\n"
                "*.dll\n"
                "*.so\n"
                "*.so.*\n"
                "*.dylib\n"
                "\n"
                "# Executables\n"
                "*.exe\n"
                "*.out\n"
                "*.app\n"
                "*.i*86\n"
                "*.x86_64\n"
                "*.hex\n"
                "\n"
                "# Debug files\n"
                "*.dSYM/\n"
                "*.su\n"
                "*.idb\n"
                "*.pdb\n"
                "\n"
                "# Kernel Module Compile Results\n"
                "*.mod*\n"
                "*.cmd\n"
                ".tmp_versions/\n"
                "modules.order\n"
                "Module.symvers\n"
                "Mkfile.old\n"
                "dkms.conf\n"
                "\n"
                "# Local files\n"
                "**/local\n"
                "**/bin/\n"
                "**/gen/\n"
                "**/build/\n"
                "**/docs/doxygen/\n"
                "**/doxygen/documentation/\n"
                "\n"
                "# vagrant (if used)\n"
                ".vagrant\n"
                "\n"
                "# subprojects (if used)\n"
                "*.git\n"
                "\n"
                "# generated package config\n"
                "*.pc\n"
                "\n"
                "# ctags\n"
                ".tags\n"
                "tags\n"
                "functions\n"
                "\n"
                "# IDE\n"
                "\n"
                "## IntelliJ\n"
                ".idea\n"
                "\n"
                "## Sublime\n"
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
                "\n"
                "# Org-mode\n"
                ".org-id-locations\n"
                "*_archive\n"
                "\n"
                "# flymake-mode\n"
                "*_flymake.*\n"
                "\n"
                "# eshell files\n"
                "/eshell/history\n"
                "/eshell/lastdir\n"
                "\n"
                "# elpa packages\n"
                "/elpa/\n"
                "\n"
                "# reftex files\n"
                "*.rel\n"
                "\n"
                "# AUCTeX auto folder\n"
                "/auto/\n"
                "\n"
                "# cask packages\n"
                ".cask/\n"
                "dist/\n"
                "\n"
                "# Flycheck\n"
                "flycheck_*.el\n"
                "\n"
                "# server auth directory\n"
                "/server/\n"
                "\n"
                "# projectiles files\n"
                ".projectile\n"
                "projectile-bookmarks.eld\n"
                "\n"
                "# directory configuration\n"
                ".dir-locals.el\n"
                "\n"
                "# saveplace\n"
                "places\n"
                "\n"
                "# url cache\n"
                "url/cache/\n"
                "\n"
                "# cedet\n"
                "ede-projects.el\n"
                "\n"
                "# smex\n"
                "smex-items\n"
                "\n"
                "# company-statistics\n"
                "company-statistics-cache.el\n"
                "\n"
                "# anaconda-mode\n"
                "anaconda-mode/\n"
        ) < 0)
                goto error;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                buffer, size,
                "\n", 1))
                goto error;

        return result;
error:
        result = testrun_string_free(result);
        return NULL;
}
