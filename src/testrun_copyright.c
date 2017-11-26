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

        @file           testrun_copyright.c
        @author         Markus Toepfer
        @date           2017-11-25

        @ingroup        testrun_lib

        @brief          Implementation of a a copyright statement structure.


        ------------------------------------------------------------------------
*/

#include "../include/testrun_copyright.h"

/*
 *      ------------------------------------------------------------------------
 *
 *      STRUCTURE RELATED FUNCTIONS                                     #SRF
 *
 *      Functions which manipulate or support the structure itself.
 *
 *      ------------------------------------------------------------------------
 */

bool testrun_copyright_structure_is_valid(
        testrun_copyright const * const statement){

        /*
         *      Check all mandatory elements, which need to be set.
         */

        if (!statement)
                return false;

        if (!statement->intro)
                return false;

        if (!statement->year)
                return false;

        if (!statement->owner)
                return false;

        if (!statement->text)
                return false;

        if (!statement->to_string)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

size_t testrun_copyright_calculate_length(
        testrun_copyright const * const statement){

        if (!testrun_copyright_structure_is_valid(statement))
                return 0;

        size_t length = 0;

        // Intro part e.g. "COPYRIGHT (c) 2017 NAME\n"
        length += strlen(statement->intro);
        length += 1; // space |intro year|
        length += strlen(statement->year);
        length += 1; // space |year name|
        length += strlen(statement->owner);
        length += strlen(TESTRUN_LINEEND);

        // Text block e.g. "All rights reserved.\n"
        length += strlen(statement->text);
        length += strlen(TESTRUN_LINEEND);

        // Note block e.g. "This file is part of the testrun project.\n"
        if (statement->note) {
                length += strlen(TESTRUN_LINEEND);      // empty line in
                length += strlen(statement->note);      // note
                length += strlen(TESTRUN_LINEEND);      // line end
        }

        return length;
}

/*----------------------------------------------------------------------------*/

char *testrun_copyright_write_to(
        char * const buffer,
        size_t * const open,
        testrun_copyright const * const statement){

        if (!buffer || !open || !statement)
                return NULL;

        if (!testrun_copyright_structure_is_valid(statement))
                return NULL;

        size_t written  = 0;
        size_t required = testrun_copyright_calculate_length(statement) + 1;
        if ((required == 0) || (required > *open))
                return NULL;

        if (statement->note){

                written = snprintf(buffer, *open, "%s %s %s%s%s%s%s%s%s",
                        statement->intro, statement->year, statement->owner,
                        TESTRUN_LINEEND,
                        statement->text,
                        TESTRUN_LINEEND,
                        TESTRUN_LINEEND,
                        statement->note,
                        TESTRUN_LINEEND);

        } else {

                written = snprintf(buffer, *open, "%s %s %s%s%s%s",
                        statement->intro, statement->year, statement->owner,
                        TESTRUN_LINEEND,
                        statement->text,
                        TESTRUN_LINEEND);
        }

        if (written == required - 1){
                *open -= written;
                return buffer + written;
        }

        // delete everything written
        bzero(buffer, required);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_copyright_to_string(
        testrun_copyright const * const statement){

        if (!testrun_copyright_structure_is_valid(statement))
                return NULL;

        size_t size = testrun_copyright_calculate_length(statement) + 1;
        if (size == 0)
                return NULL;

        char *string = calloc(size, sizeof(char));
        if (!string)
                return NULL;

        if (!testrun_copyright_write_to(string, &size, statement)){
                free(string);
                return NULL;
        }

        return string;
}

/*----------------------------------------------------------------------------*/

char *testrun_copyright_gpl_statement_to_string(
        testrun_copyright const * const statement){

        if (!testrun_copyright_structure_is_valid(statement))
                return NULL;

        if (!statement->program || !statement->variant)
                return NULL;

        size_t open = 0;
        size_t size = testrun_copyright_calculate_length(statement) + 1;
        if (size == 1)
                return NULL;

        char *string = calloc(size, sizeof(char));
        if (!string)
                return NULL;

        // size is needed below, therefore open is used as variable length
        open = size;
        if (!testrun_copyright_write_to(string, &open, statement)){
                free(string);
                return NULL;
        }

        char *replace1 = NULL;
        char *replace2 = NULL;
        size_t length  = 0;

        // replace all TAGs TESTRUN_COPYRIGHT_TAG_PROGRAM
        if (!testrun_string_replace_all(
                &replace1, &length,
                string, size,
                TESTRUN_COPYRIGHT_TAG_PROGRAM,
                strlen(TESTRUN_COPYRIGHT_TAG_PROGRAM),
                statement->program, strlen(statement->program),
                false)){
                string   = testrun_string_free(string);
                replace1 = testrun_string_free(replace1);
                return NULL;
        }

        // replace all TAGs TESTRUN_COPYRIGHT_TAG_VARIANT
        if (!testrun_string_replace_all(
                &replace2, &size,
                replace1, length,
                TESTRUN_COPYRIGHT_TAG_VARIANT,
                strlen(TESTRUN_COPYRIGHT_TAG_VARIANT),
                statement->variant, strlen(statement->variant),
                false)){
                string   = testrun_string_free(string);
                replace1 = testrun_string_free(replace1);
                replace2 = testrun_string_free(replace2);
                return NULL;
        }

        string   = testrun_string_free(string);
        replace1 = testrun_string_free(replace1);

        return replace2;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      OUTPUT FORMAT RELATED FUNCTIONS                                 #OFRF
 *
 *      Special output formats, which allow preconfigured and standardized
 *      output of the copyright statements, as well as custom output defintion.
 *
 *      ------------------------------------------------------------------------
 */

char *testrun_copyright_to_header(
        testrun_copyright const * const statement,
        char const * const prefix,      size_t prefix_length,
        char const * const suffix,      size_t suffix_length,
        char const * const line_prefix, size_t line_prefix_length,
        char const * const line_suffix, size_t line_suffix_length){

        if (!statement || !statement->to_string)
                return NULL;

        size_t  length_orig = 0;
        size_t  length_end  = 0;

        size_t  size1       = 0;
        size_t  size2       = 0;
        size_t  size3       = 0;

        char    *step1      = NULL;
        char    *step2      = NULL;
        char    *unset      = NULL;
        char    *orig       = statement->to_string(statement);

        if (!orig)
                return NULL;

        length_orig = strlen(orig);
        length_end  = strlen(TESTRUN_LINEEND);

        // prefix and suffix each line of the statement
        if (!testrun_string_embed(&step1, &size1,
                orig, length_orig,
                line_prefix, line_prefix_length,
                line_suffix, line_suffix_length,
                TESTRUN_LINEEND, length_end,
                TESTRUN_LINEEND, length_end))
                goto error;

        /*
         *      UNSET unwanted strings, string_embed will add a final outro
         *      lineend and prefix, which is prefixed and suffixed.
         *      We don't want that in this setup, so we need to unset the
         *      addtional line dynamically based on the input parameter.
         *
         *      Unwanted combinations are:
         *
         *      (1)     additional lineend
         *      (2)     prefix + additional lineend
         *      (3)     suffix + additional lineend
         *      (4)     prefix + suffix + additional lineend
         */

        if (line_prefix && line_suffix) {

                /*
                 *      If both are present, the lineend need to be
                 *      prefixed with prefix and suffix.
                 *
                 *      So we prefix the suffix with the prefix and suffix
                 *      the suffix with the lineend.
                 */

                if (!testrun_string_embed(&unset, &size3,
                        line_suffix, line_suffix_length,
                        line_prefix, line_prefix_length,
                        TESTRUN_LINEEND, length_end,
                        NULL, 0,
                        NULL, 0))
                        goto error;

                if (!testrun_string_unset_end(step1, strlen(step1),
                        unset, strlen(unset)))
                        goto error;

        } else if (line_suffix) {

                /*
                 *      If a suffix is present, we need to prefix the
                 *      lineend with the suffix.
                 */

                if (!testrun_string_embed(&unset, &size3,
                        TESTRUN_LINEEND, length_end,
                        line_suffix, line_suffix_length,
                        NULL, 0,
                        NULL, 0,
                        NULL, 0))
                        goto error;

                if (!testrun_string_unset_end(step1, strlen(step1),
                        unset, strlen(unset)))
                        goto error;

        } else {

                /*
                 *      If no suffix is present, we may need to prefix the
                 *      lineend with the prefix.
                 */

                if (!testrun_string_embed(&unset, &size3,
                        TESTRUN_LINEEND, length_end,
                        line_prefix, line_prefix_length,
                        NULL, 0,
                        NULL, 0,
                        NULL, 0))
                        goto error;

                if (!testrun_string_unset_end(step1, strlen(step1),
                        unset, strlen(unset)))
                        goto error;

        }

        // prefix and suffix the result of step1
        if (!testrun_string_embed(&step2, &size2,
                step1, size1,
                prefix, prefix_length,
                suffix, suffix_length,
                NULL, 0,
                NULL, 0))
                goto error;

        unset = testrun_string_free(unset);
        orig  = testrun_string_free(orig);
        step1 = testrun_string_free(step1);

        return step2;

error:
        unset = testrun_string_free(unset);
        orig  = testrun_string_free(orig);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_copyright_default_c_header(
        testrun_copyright const * const statement, bool docu_open){

        if (!statement)
                return NULL;

}

/*----------------------------------------------------------------------------*/

char *testrun_copyright_default_shell_header(
        testrun_copyright const * const statement){

        if (!statement)
                return NULL;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      CONTENT RELATED FUNCTIONS                                       #CRF
 *
 *      Functions which create content copyright statements.
 *      NOTE: These statements are hardcoded with intention.
 *
 *      ------------------------------------------------------------------------
 */

testrun_copyright testrun_copyright_default(
        char *year, char *owner, char *note){

        if (!year)
                year = TESTRUN_COPYRIGHT_TAG_YEAR;

        if (!owner)
                owner = TESTRUN_COPYRIGHT_TAG_OWNER;

        testrun_copyright copyright = {
                .intro = "Copyright (c)",
                .year  = year,
                .owner = owner,
                .note  = note,
                .text  = "All rights reserved.",

                .to_string = testrun_copyright_to_string
        };

        return copyright;
}

/*----------------------------------------------------------------------------*/

testrun_copyright testrun_copyright_apache_version_2(
        char *year, char *owner, char *note){

        if (!year)
                year = TESTRUN_COPYRIGHT_TAG_YEAR;

        if (!owner)
                owner = TESTRUN_COPYRIGHT_TAG_OWNER;

        char *text =
        TESTRUN_LINEEND
        "Licensed under the Apache License, Version 2.0 (the \"License\");"
        TESTRUN_LINEEND
        "you may not use this file except in compliance with the License."
        TESTRUN_LINEEND
        "You may obtain a copy of the License at"
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "    http://www.apache.org/licenses/LICENSE-2.0"
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "Unless required by applicable law or agreed to in writing, software"
        TESTRUN_LINEEND
        "distributed under the License is distributed on an \"AS IS\" BASIS,"
        TESTRUN_LINEEND
        "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."
        TESTRUN_LINEEND
        "See the License for the specific language governing permissions and"
        TESTRUN_LINEEND
        "limitations under the License.";

        testrun_copyright copyright = {
                .intro = "Copyright",
                .year  = year,
                .owner = owner,
                .note  = note,
                .text  = text,

                .to_string = testrun_copyright_to_string
        };

        return copyright;
}

/*----------------------------------------------------------------------------*/

testrun_copyright testrun_copyright_GPL_v3(
        char *year, char *owner, char *note, char *program,
        testrun_gpl_v3_types type){

        char *variant = NULL;

        if (!year)
                year = TESTRUN_COPYRIGHT_TAG_YEAR;

        if (!owner)
                owner = TESTRUN_COPYRIGHT_TAG_OWNER;

        if (!program)
                program = TESTRUN_COPYRIGHT_TAG_PROGRAM;

        switch (type) {
                case LESSER:
                        variant = "Lesser General";
                        break;
                case AFFERTO:
                        variant = "Afferto General";
                        break;
                default:
                        variant = "General";
                        break;
        }

        char *text =
        TESTRUN_LINEEND
        "This file is part of "
        TESTRUN_COPYRIGHT_TAG_PROGRAM
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_COPYRIGHT_TAG_PROGRAM
        " is free software: you can redistribute it and/or modify"
        TESTRUN_LINEEND
        "it under the terms of the GNU "
        TESTRUN_COPYRIGHT_TAG_VARIANT
        " Public License as published by"
        TESTRUN_LINEEND
        "the Free Software Foundation, either version 3 of the License, or"
        TESTRUN_LINEEND
        "(at your option) any later version. "
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        TESTRUN_COPYRIGHT_TAG_PROGRAM
        " is distributed in the hope that it will be useful,"
        TESTRUN_LINEEND
        "but WITHOUT ANY WARRANTY; without even the implied warranty of"
        TESTRUN_LINEEND
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
        TESTRUN_LINEEND
        "GNU "TESTRUN_COPYRIGHT_TAG_VARIANT" Public License for more details."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "You should have received a copy of the GNU "
        TESTRUN_COPYRIGHT_TAG_VARIANT" Public License"
        TESTRUN_LINEEND
        "along with " TESTRUN_COPYRIGHT_TAG_PROGRAM
        ". If not, see <http://www.gnu.org/licenses/>.";

        testrun_copyright copyright = {
                .intro     = "Copyright (c)",
                .year      = year,
                .owner     = owner,
                .note      = note,
                .text      = text,

                .variant   = variant,
                .program   = program,

                .to_string = testrun_copyright_gpl_statement_to_string
        };

        //log("%s | %jd",copyright.text, strlen(copyright.text));

        return copyright;
}

/*----------------------------------------------------------------------------*/

testrun_copyright testrun_copyright_BSD_3Clause(
        char *year, char *owner, char *note){

        if (!year)
                year = TESTRUN_COPYRIGHT_TAG_YEAR;

        if (!owner)
                owner = TESTRUN_COPYRIGHT_TAG_OWNER;

        char *text =
        "All rights reserved."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "Redistribution and use in source and binary forms, with or without"
        TESTRUN_LINEEND
        "modification, are permitted provided that the following conditions are met:"
        TESTRUN_LINEEND
        "    * Redistributions of source code must retain the above copyright"
        TESTRUN_LINEEND
        "      notice, this list of conditions and the following disclaimer."
        TESTRUN_LINEEND
        "    * Redistributions in binary form must reproduce the above copyright"
        TESTRUN_LINEEND
        "      notice, this list of conditions and the following disclaimer in the"
        TESTRUN_LINEEND
        "      documentation and/or other materials provided with the distribution."
        TESTRUN_LINEEND
        "    * Neither the name of the copyright holder nor the"
        TESTRUN_LINEEND
        "      names of its contributors may be used to endorse or promote products"
        TESTRUN_LINEEND
        "      derived from this software without specific prior written permission."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND"
        TESTRUN_LINEEND
        "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED"
        TESTRUN_LINEEND
        "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE"
        TESTRUN_LINEEND
        "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY"
        TESTRUN_LINEEND
        "DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES"
        TESTRUN_LINEEND
        "(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;"
        TESTRUN_LINEEND
        "LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND"
        TESTRUN_LINEEND
        "ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT"
        TESTRUN_LINEEND
        "(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS"
        TESTRUN_LINEEND
        "SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.";

        testrun_copyright copyright = {
                .intro     = "Copyright (c)",
                .year      = year,
                .owner     = owner,
                .note      = note,
                .text      = text,

                .to_string = testrun_copyright_to_string
        };

        //log("%s | %jd",copyright.text, strlen(copyright.text));

        return copyright;
}

/*----------------------------------------------------------------------------*/

testrun_copyright testrun_copyright_BSD_2Clause(
        char *year, char *owner, char *note){

        if (!year)
                year = TESTRUN_COPYRIGHT_TAG_YEAR;

        if (!owner)
                owner = TESTRUN_COPYRIGHT_TAG_OWNER;

        char *text =
        "All rights reserved."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "Redistribution and use in source and binary forms, with or without"
        TESTRUN_LINEEND
        "modification, are permitted provided that the following conditions are met:"
        TESTRUN_LINEEND
        "1. Redistributions of source code must retain the above copyright notice, this"
        TESTRUN_LINEEND
        "   list of conditions and the following disclaimer."
        TESTRUN_LINEEND
        "2. Redistributions in binary form must reproduce the above copyright notice,"
        TESTRUN_LINEEND
        "   this list of conditions and the following disclaimer in the documentation"
        TESTRUN_LINEEND
        "   and/or other materials provided with the distribution."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND"
        TESTRUN_LINEEND
        "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED"
        TESTRUN_LINEEND
        "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE"
        TESTRUN_LINEEND
        "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR"
        TESTRUN_LINEEND
        "ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES"
        TESTRUN_LINEEND
        "(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;"
        TESTRUN_LINEEND
        "LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND"
        TESTRUN_LINEEND
        "ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT"
        TESTRUN_LINEEND
        "(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS"
        TESTRUN_LINEEND
        "SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.";

        testrun_copyright copyright = {
                .intro     = "Copyright (c)",
                .year      = year,
                .owner     = owner,
                .note      = note,
                .text      = text,

                .to_string = testrun_copyright_to_string
        };

        //log("%s | %jd",copyright.text, strlen(copyright.text));

        return copyright;
}

/*----------------------------------------------------------------------------*/

testrun_copyright testrun_copyright_MIT(
        char *year, char *owner, char *note){

        if (!year)
                year = TESTRUN_COPYRIGHT_TAG_YEAR;

        if (!owner)
                owner = TESTRUN_COPYRIGHT_TAG_OWNER;

        char *text =
        TESTRUN_LINEEND
        "Permission is hereby granted, free of charge, to any person obtaining a copy"
        TESTRUN_LINEEND
        "of this software and associated documentation files (the \"Software\"), to deal"
        TESTRUN_LINEEND
        "in the Software without restriction, including without limitation the rights"
        TESTRUN_LINEEND
        "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell"
        TESTRUN_LINEEND
        "copies of the Software, and to permit persons to whom the Software is"
        TESTRUN_LINEEND
        "furnished to do so, subject to the following conditions:"
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "The above copyright notice and this permission notice shall be included in all"
        TESTRUN_LINEEND
        "copies or substantial portions of the Software."
        TESTRUN_LINEEND
        TESTRUN_LINEEND
        "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
        TESTRUN_LINEEND
        "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
        TESTRUN_LINEEND
        "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE"
        TESTRUN_LINEEND
        "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
        TESTRUN_LINEEND
        "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
        TESTRUN_LINEEND
        "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE"
        TESTRUN_LINEEND
        "SOFTWARE.";

        testrun_copyright copyright = {
                .intro     = "Copyright (c)",
                .year      = year,
                .owner     = owner,
                .note      = note,
                .text      = text,

                .to_string = testrun_copyright_to_string
        };

        //log("%s | %jd",copyright.text, strlen(copyright.text));

        return copyright;
}
