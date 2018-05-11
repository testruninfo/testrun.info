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

        @file           testrun_config.c
        @author         Markus Toepfer
        @date           2017-11-25

        @ingroup        testrun_lib

        @brief          Implementation of the testrun_config structure


        ------------------------------------------------------------------------
*/

#include "../include/testrun_config.h"


testrun_config testrun_config_default(){

        testrun_config config = {

                .author  = TESTRUN_TAG_DEFAULT_AUTHOR,
                .project =
                {
                        .name    = TESTRUN_TAG_DEFAULT_PROJECT,
                        .url     = TESTRUN_URL,
                        .type    = TESTRUN_LIB,
                        .path    = TESTRUN_PATH_DUMMY,
                        .doxygen = true
                },
                .copyright =
                {
                        .intro          = TESTRUN_COPYRIGHT_INTRO_DEFAULT,
                        .year           = TESTRUN_COPYRIGHT_TAG_YEAR,
                        .owner          = TESTRUN_COPYRIGHT_TAG_OWNER,
                        .text           = TESTRUN_COPYRIGHT_TEXT_DEFAULT,

                        .note           = NULL,
                        .variant        = NULL,
                        .program        = NULL,

                        .to_string      = testrun_copyright_to_string
                },
                .format =
                {
                        .line_length    = 80,
                        .indent_c       = 8,
                        .offset_docu    = 3 * 8
                }
        };

        return config;
}

/*----------------------------------------------------------------------------*/

bool testrun_config_dump(
        FILE *stream, testrun_config const * const config){

        if (!stream || !config)
                return NULL;

        char *type = NULL;

        switch(config->project.type){

                case TESTRUN_SERVICE:
                        type = "service";
                        break;
                case TESTRUN_EXEC:
                        type = "exec";
                        break;
                default:
                        type = "lib";
        }

        char *copyright = config->copyright.to_string(&config->copyright);

        fprintf(stream, "----------------------------\n");
        fprintf(stream, "CONFIG DUMP %p\n", config);
        fprintf(stream, "----------------------------\n");
        fprintf(stream, "\n");
        fprintf(stream, "%s\t\t%s\n", TESTRUN_KEY_AUTHOR,            config->author);
        fprintf(stream, "%s \n", TESTRUN_KEY_PROJECT);
        fprintf(stream, "----------------------------\n");
        fprintf(stream, "%s\t\t%s\n", TESTRUN_KEY_NAME,              config->project.name);
        fprintf(stream, "%s\t\t%s\n", TESTRUN_KEY_URL,               config->project.url);
        fprintf(stream, "%s\t\t%s\n", TESTRUN_KEY_TYPE,              type);
        fprintf(stream, "%s\t\t%s\n", TESTRUN_KEY_PATH,              config->project.path);
        fprintf(stream, "----------------------------\n");
        fprintf(stream, "%s \n", TESTRUN_KEY_FORMAT);
        fprintf(stream, "----------------------------\n");
        fprintf(stream, "%s\t%jd\n", TESTRUN_KEY_LINE_LENGTH,       config->format.line_length);
        fprintf(stream, "%s\t%jd\n", TESTRUN_KEY_INDENT_C,          config->format.indent_c);
        fprintf(stream, "%s\t%jd\n", TESTRUN_KEY_OFFSET_DOCU,       config->format.offset_docu);
        fprintf(stream, "----------------------------\n");
        fprintf(stream, "%s (text dump)\n", TESTRUN_KEY_COPYRIGHT);
        fprintf(stream, "----------------------------\n");
        fprintf(stream, "%s\n", copyright);
        fprintf(stream, "----------------------------\n");

        copyright = testrun_string_free(copyright);

        return true;
}

/*----------------------------------------------------------------------------*/
