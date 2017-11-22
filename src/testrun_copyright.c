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
 *      @file           testrun_copyright.c
 *      @author         Markus Toepfer
 *      @date           2017-11-12
 *
 *      @ingroup        testrun_lib
 *
 *      @brief          Implemenation of all copyright related functionality.
 *
 *
 *      ------------------------------------------------------------------------
 **/

#include "../include/testrun_copyright.h"


/*----------------------------------------------------------------------------*/

int testrun_copyright_create_header(
        char **result, size_t *size,
        const char * const source, const size_t const source_length,
        const char * const prefix, const size_t const prefix_length,
        const char * const intro,  const size_t const intro_length,
        const char * const outro,  const size_t const outro_length){

        if (!result || !size || !source)
                return -1;

        char  *step1  = NULL;
        size_t length = 0;

        /* add a line prefix */

        if (!testrun_string_embed(&step1, &length,
                (char*) source, source_length,
                prefix,         prefix_length,
                0,0,
                LINEBREAK,      strlen(LINEBREAK),
                LINEBREAK,      strlen(LINEBREAK)))
                goto error;

        /* add intro and outro */

        printf("STEP1 |%s|%jd\n", step1, length);

        if (!testrun_string_embed(result, size,
                step1,          length,
                intro,          intro_length,
                outro,          outro_length,
                0,0,
                0,0))
                goto error;

        free(step1);
        return 0;

error:
        if (step1)
                free(step1);
        if (*result){
                free(*result);
                *result = NULL;
        }

        return -1;
}

/*----------------------------------------------------------------------------*/

char *testrun_read_copyright_from(
        const char * const path){

        char *dataBuffer = NULL;

        if (!path)
                return NULL;

        size_t length = strlen(path) +
                        strlen(TESTRUN_FILENAME_COPYRIGHT) + 2;

        char filename[length];

        if (snprintf(filename,length,"%s/%s",path,TESTRUN_FILENAME_COPYRIGHT) < 0)
                goto error;

        if(access( filename, F_OK ) == -1 )
                goto error;

        FILE *fp = fopen(filename, "r");

        if (fp != NULL) {

                if (fseek(fp, 0L, SEEK_END) == 0) {

                        size_t filesize = ftell(fp);

                        if ((int) filesize == -1) {
                                fclose(fp);
                                goto error;
                        }

                        dataBuffer  = calloc(filesize + 1, sizeof(char));

                        if (fseek(fp, 0L, SEEK_SET) == 0){

                                size_t count = fread(
                                        dataBuffer, sizeof(char), filesize, fp);

                                if (count == 0) {
                                        fclose(fp);
                                        goto error;
                                }

                        } else {

                                fclose(fp);
                                goto error;
                        }
                }

                fclose(fp);

        } else {

                goto error;

        }

        return dataBuffer;

error:
        if (dataBuffer)
                free(dataBuffer);
        return NULL;

}

/*----------------------------------------------------------------------------*/

char *testrun_copyright_default(
        char *date, char *name, bool intro){

        char *snipped = calloc(1000, sizeof(char));
        if (!snipped)
                return NULL;

        if (!date)
                date = "[COPYRIGHT_DATE]";

        if (!name)
                name = "[COPYRIGHT_AUTHOR]";

        if (intro){
                sprintf(snipped,
                        "%s%s"
                        "COPYRIGHT (c) %s %s\n"
                        "%sAll rights reserved.\n"
                        "%s",
                        TESTRUN_COPYRIGHT_INTRO,
                        TESTRUN_HEADER_LINE_INTRO,
                        date, name,
                        TESTRUN_HEADER_LINE_INTRO,
                        TESTRUN_COPYRIGHT_OUTRO);
        } else {
                sprintf(snipped,
                        "COPYRIGHT (c) %s %s\n"
                        "All rights reserved.\n",
                        date, name);
        }

        return snipped;
}
