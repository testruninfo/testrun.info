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

        @file           testrun_module.c
        @author         Markus Toepfer
        @date           2017-11-26

        @ingroup

        @brief


        ------------------------------------------------------------------------
*/

#include "../include/testrun_module.h"

/*----------------------------------------------------------------------------*/

char * testrun_module_c_file_content(
        char * module_name,
        testrun_extension_t filetype,
        struct testrun_config const * const config){

        if (!config)
                return false;

        if (!module_name)
                return false;

        char *body = NULL;

        char *head = testrun_copyright_default_c_header(
                        &config->copyright, true);

        char *docu = testrun_text_block_c_header_documentation(
                        module_name,
                        filetype,
                        config, false, NULL);

        char source[PATH_MAX];
        bzero(source, PATH_MAX);

        char header[PATH_MAX];
        bzero(header, PATH_MAX);

        if (!head || !docu)
                return NULL;

        switch(filetype) {

                case TESTRUN_HEADER:

                        body = testrun_text_block_header_body(
                                module_name);
                        break;

                case TESTRUN_SOURCE:

                        if (!testrun_path_source_to_include(source, PATH_MAX,
                                config, module_name))
                                goto error;

                        body = testrun_text_block_source_body(
                                source);

                        break;

                case TESTRUN_TEST:


                        if (!testrun_path_test_to_source(source, PATH_MAX,
                                config, module_name))
                                goto error;

                        if (!testrun_path_test_to_testrun_header(header, PATH_MAX,
                                config))
                                goto error;

                        body = testrun_text_block_test_body(
                                source, header);

                        break;

                default:
                        goto error;
        }

        if (!body)
                goto error;

        char    *result = NULL;
        size_t  size    = strlen(head) + strlen(docu) + strlen(body) +
                          strlen(config->format.line_end) + 1;

        result = calloc(size, sizeof(char));
        if (!result)
                goto error;

        if (!snprintf(result, size, "%s%s%s%s", head, docu, body,
                config->format.line_end))
                goto error;

        head = testrun_string_free(head);
        docu = testrun_string_free(docu);
        body = testrun_string_free(body);
        return result;
error:
        head = testrun_string_free(head);
        docu = testrun_string_free(docu);
        body = testrun_string_free(body);
        return NULL;
}

/*----------------------------------------------------------------------------*/



