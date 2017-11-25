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
 *      @file           testrun_copyright_statement.h
 *      @author         Markus Toepfer
 *      @date           2017-11-25
 *
 *      @ingroup	testrun_lib
 *
 *      @brief		Definition of a copyright statement.
 *
 *
 *      ------------------------------------------------------------------------
 **/

#ifndef testrun_copyright_statement_h
#define testrun_copyright_statement_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "testrun_log.h"
#include "testrun_string.h"

#define TESTRUN_COPYRIGHT_TAG_YEAR      "[COPYRIGHT_YEAR]"
#define TESTRUN_COPYRIGHT_TAG_OWNER     "[COPYRIGHT_OWNER]"
#define TESTRUN_COPYRIGHT_TAG_PROGRAM   "[COPYRIGHT_PROGRAM]"
#define TESTRUN_COPYRIGHT_TAG_VARIANT   "[COPYRIGHT_VARIANT]"
#define TESTRUN_LINEEND     		"\n"

typedef enum {GENERAL, LESSER, AFFERTO} testrun_gpl_v3_types;

/*******************************************************************************
 *
 *      STRUCTURE DEFINITION
 *
 ******************************************************************************/

typedef struct testrun_copyright_statement testrun_copyright_statement;

struct testrun_copyright_statement{

        char *intro;
        char *year;
        char *owner;
        char *note;
        char *text;

        char *variant;
        char *program;

        char *(*to_string) (testrun_copyright_statement const * const statement);

};

/*******************************************************************************
 *
 *      STRUCTURE RELATED FUNCTIONS
 *
 ******************************************************************************/

/*----------------------------------------------------------------------------*/

/**
        Check if all required elements of the structure are set.

        @param statement        copyright statement
        @returns                true if all mandatory elemenents are set
*/
bool testrun_copyright_statement_structure_is_valid(
        testrun_copyright_statement const * const statement);

/*----------------------------------------------------------------------------*/

/**
        Calculate the length required to write the copyright statement
        to and buffer.

        @param statement        copyright statement
        @returns                amount of bytes needed, 0 on error
*/
size_t testrun_copyright_statement_calculate_length(
        testrun_copyright_statement const * const statement);

/*----------------------------------------------------------------------------*/

/**
        Write the copyright statement to a buffer of a given size.
        When the function is successful, a pointer to the end of the written
        string will be returned (pointer to '\0') and the size_t * open will
        be reduced by the amount of written bytes. On error no pointer will be
        returned.

        @param buffer           buffer to write to
        @param open             open bytes of buffer
        @param statement        copyright statement
        @returns                pointer to new end of buffer \0 on success,
                                NULL on error
*/
char *testrun_copyright_statement_write_to(
        char * const buffer,
        size_t * const open,
        testrun_copyright_statement const * const statement);

/*----------------------------------------------------------------------------*/

/**
        Create a string out of a copyright statement structure.

        @parem statement        copyright statement to be converted
        @returns                allocated string (MUST be freed) or NULL
*/
char *testrun_copyright_statement_to_string(
        testrun_copyright_statement const * const statement);

/*----------------------------------------------------------------------------*/

/**
        Create a string out of a copyright statement structure. Including
        TAG replacement for GPL variant and GPL program name.

        @parem statement        copyright statement to be converted
        @returns                allocated string (MUST be freed) or NULL
*/
char *testrun_copyright_gpl_statement_to_string(
        testrun_copyright_statement const * const statement);


/*******************************************************************************
 *
 *      CONTENT RELATED FUNCTIONS
 *
 ******************************************************************************/

/*----------------------------------------------------------------------------*/

/**
 *      Create a default fallback copyright in form of:
 *
 *      ------------------------------------------------------------------------
 *
 *      COPYRIGHT (c) YEAR  NAME
 *      All rights reserved.
 *
 *      note
 *
 *      ------------------------------------------------------------------------
 *
 *      @param  year            (optional) string for the year
 *      @param  owner           (optional) name of the copyright holder
 *      @param  note            (optional) note to be used
 *      @returns                preinitialized copyright statement
 */
testrun_copyright_statement testrun_copyright_statement_default(
        char *year, char *owner, char *note);

/*----------------------------------------------------------------------------*/

/**
 *      Create a default APACHE VERSION 2 copyright in form of:
 *
 *      ------------------------------------------------------------------------
 *
 *      Copyright YEAR NAME
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 *
 *      note
 *
 *      ------------------------------------------------------------------------
 *
 *      @param  year    (optional) string for the year
 *      @param  owner   (optional) name of the copyright holder
 *      @param  note    (optional) note to be used
 *      @return         allocated string (MUST be freed) with copyright or NULL
 */
testrun_copyright_statement testrun_copyright_statement_apache_version_2(
        char *year, char *owner, char *note);

/*----------------------------------------------------------------------------*/

/**
 *      Create a default GPL VERSION 3 copyright in form of:
 *
 *      ------------------------------------------------------------------------
 *
 *      Copyright (c) YEAR NAME
 *
 *      This file is part of [COPYRIGHT_PROGRAM]
 *
 *      [COPYRIGHT_PROGRAM] is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU [variant] Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      [COPYRIGHT_PROGRAM] is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU [variant] Public License for more details.
 *
 *
 *      You should have received a copy of the GNU [variant] Public License
 *      along with [COPYRIGHT_PROGRAM].  If not, see <http://www.gnu.org/licenses/>.
 *
 *      note
 *      ------------------------------------------------------------------------
 *
 *      @param  year    (optional) string for the year
 *      @param  owner   (optional) name of the copyright holder
 *      @param  note    (optional) note to be used
 *      @return         allocated string (MUST be freed) with copyright or NULL
 */
testrun_copyright_statement testrun_copyright_statement_GPL_v3(
        char *year, char *owner, char *note, char *program,
        testrun_gpl_v3_types type);

/*----------------------------------------------------------------------------*/

/**
 *      Create a default BSD 3-Clause copyright in form of:
 *
 *      ------------------------------------------------------------------------
 *
 *      Copyright (c) YEAR NAME
 *      All rights reserved.
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are met:
 *          * Redistributions of source code must retain the above copyright
 *            notice, this list of conditions and the following disclaimer.
 *          * Redistributions in binary form must reproduce the above copyright
 *            notice, this list of conditions and the following disclaimer in the
 *            documentation and/or other materials provided with the distribution.
 *          * Neither the name of the copyright holder nor the
 *            names of its contributors may be used to endorse or promote products
 *            derived from this software without specific prior written permission.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *      ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *      WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *      DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 *      DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *      (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *      LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *      ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *      SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *      note
 *      ------------------------------------------------------------------------
 *
 *      @param  year    (optional) string for the year
 *      @param  owner   (optional) name of the copyright holder
 *      @param  note    (optional) note to be used
 *      @return         allocated string (MUST be freed) with copyright or NULL
 */
testrun_copyright_statement testrun_copyright_statement_BSD_3Clause(
        char *year, char *owner, char *note);

/*----------------------------------------------------------------------------*/

/**
 *      Create a default BSD 2-Clause copyright in form of:
 *
 *      ------------------------------------------------------------------------
 *
 *      Copyright (c) YEAR NAME
 *      All rights reserved.
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are met:
 *
 *      1. Redistributions of source code must retain the above copyright notice, this
 *         list of conditions and the following disclaimer.
 *      2. Redistributions in binary form must reproduce the above copyright notice,
 *         this list of conditions and the following disclaimer in the documentation
 *         and/or other materials provided with the distribution.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *      ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *      WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *      DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *      ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *      (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *      LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *      ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *      SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *      note
 *      ------------------------------------------------------------------------
 *
 *      @param  year    (optional) string for the year
 *      @param  owner   (optional) name of the copyright holder
 *      @param  note    (optional) note to be used
 *      @return         allocated string (MUST be freed) with copyright or NULL
 */
testrun_copyright_statement testrun_copyright_statement_BSD_2Clause(
        char *year, char *owner, char *note);

/*----------------------------------------------------------------------------*/

/**
 *      Create a default MIT copyright in form of:
 *
 *      ------------------------------------------------------------------------
 *
 *      Copyright (c) YEAR NAME
 *
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in all
 *      copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *      SOFTWARE.
 *
 *      note
 *      ------------------------------------------------------------------------
 *
 *      @param  year    (optional) string for the year
 *      @param  owner   (optional) name of the copyright holder
 *      @param  note    (optional) note to be used
 *      @return         allocated string (MUST be freed) with copyright or NULL
 */
testrun_copyright_statement testrun_copyright_statement_MIT(
        char *year, char *owner, char *note);

#endif /* testrun_copyright_statement_h */
