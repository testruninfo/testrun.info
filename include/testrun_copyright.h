/***
        ------------------------------------------------------------------------

        Copyright 2018 Markus Toepfer

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.

        ------------------------------------------------------------------------
*//**
        @file           testrun_copyright.h
        @author         Markus Toepfer
        @date           2018-07-06

        @ingroup        testrun_lib

        @brief          Definition of a generic function set for copyright 
                        statements, as well as a definition of some common
                        copyright statement functions. 

        @NOTE           style pointer base functions

        ------------------------------------------------------------------------
*/
#ifndef testrun_copyright_h
#define testrun_copyright_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#include "testrun_log.h"


/*
 *      ------------------------------------------------------------------------
 *
 *      GENERIC FUNCTION STRUCTURE DEFINITION
 *
 *
 *      Simple convention: Function of the testrun copyright structure MUST 
 *      return an allocated zero terminated string, or NULL.
 *
 *      ------------------------------------------------------------------------
 */

typedef struct testrun_copyright testrun_copyright;

struct testrun_copyright{

        char *(*generate_header_string)         (const char *prefix,   
                                                 const char *intro,    
                                                 const char *year,     
                                                 const char *owner,    
                                                 const char *end_note, 
                                                 const char *suffix,   
                                                 uint16_t line_indent,
                                                 bool indent_using_spaces);

        char *(*generate_full_text_licence)     ();

};

/**     ------------------------------------------------------------------------

        Default "generate_header_string"

        @param prefix           prefix to be written as is in front
        @param intro            e.g. "Copyright", "Copyright (c)"
        @param year             e.g. "2018", "2016,2017,2018"
        @param owner            e.g. "DLR e.V.", "Markus Toepfer"
        @param text             Copyright statement e.g. "All rights reserved."
        @param end_note         optional end note e.g. link to project homepage
        @param suffix           suffix to be written as is at the end 
        @param indent           line indent to be set in front of each line, 
                                except of prefix and suffix.
        @param spaces           if true, spaces will be added, tabs in false

        @returns                allocated c string or NULL
*/
char *testrun_copyright_generate_header_string(
        const char* prefix,
        const char* intro,
        const char* year,
        const char* owner,
        const char* text,
        const char* end_note,
        const char* suffix,
        uint16_t indent,
        bool spaces); 

/*      ------------------------------------------------------------------------
 *
 *      EXAMPLE IMPLEMENTATION OF SOME COPYRIGHTS
 *
 *      This part contains some common implementations, which may be used
 *      within custom tools and frameworks using testrun_lib.
 *
 *      ------------------------------------------------------------------------
 */

/**     ------------------------------------------------------------------------

        Function set for a default copyright of form:

        ------------------------------------------------------------------------

        Copyright (c) YEAR NAME" 
        All rights reserved.

        NOTE

        ------------------------------------------------------------------------

*/
struct testrun_copyright testrun_copyright_functions_reserved();

/**     ------------------------------------------------------------------------

        Function set for a APACHE VERSION 2 copyright of form:

        ------------------------------------------------------------------------

        Copyright (c) YEAR NAME" 
        
        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.

        NOTE

        ------------------------------------------------------------------------

*/
struct testrun_copyright testrun_copyright_functions_apache_version_2();

#endif /* testrun_copyright_h */

