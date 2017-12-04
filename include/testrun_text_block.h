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

        @file           testrun_text_block.h
        @author         Markus Toepfer
        @date           2017-11-27

        @ingroup        testrun_lib

        @brief          Definition of standard text block elements.


        ------------------------------------------------------------------------
*/

#ifndef testrun_text_block_h
#define testrun_text_block_h

#include "testrun_string.h"
#include "testrun_config.h"
#include "testrun_time.h"
#include "testrun_path.h"


/*----------------------------------------------------------------------------*/

/*
 *      ------------------------------------------------------------------------
 *
 *      C FILE BODY CONTENT                                             #CBC
 *
 *      Default body content of the standard c files of a module.
 *
 *      ------------------------------------------------------------------------
 */

/**
        Generate the content body of a C header file.

        @param name     input name of the module
        @return         body for the file name.h
 */
char *testrun_text_block_header_body(
        char *name);

/*----------------------------------------------------------------------------*/

/**
        Generate the content body of a C source file.

        @param name     relative path to header file e.g. ../include/name.h
        @return         body for the file name.c
 */
char *testrun_text_block_source_body(
        char *name);

/*----------------------------------------------------------------------------*/

/**
        Generate the content body of a C test file.

        @param path_src relative path to source file e.g. ../../src/name.c
        @param path_run relative path to testrun.h   e.g. ../tools/testrun.h
        @return         body for the file name_tests.c
 */
char *testrun_text_block_test_body(
        char *path_src,
        char *path_run);

/*----------------------------------------------------------------------------*/

/*
 *      ------------------------------------------------------------------------
 *
 *      C HEADER CONTENT BLOCKS                                         #CHC
 *
 *      Default blocks for the header
 *
 *      ------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------*/

/**
        Create the testrun default C documentation header block-

        Output will be formated like:

        |**
                @file           [MODULE_NAME].h
                @author         [AUTHOR_NAME]
                @date           [CREATION_DATE]

                @ingroup        [PROJECT]

                @brief


                ----------------------------------------------------------------
        *|

        NOTE: the slash  is replaced with | to prevent compiler error messages

        @param module_name      name for the module
        @param extention        type of file to create
        @param config           configuration to be used
        @param brief            (optional) addition content for brief intro
        @param docu_open        on true  the last line will be *||**
                                on false the last line will be *|
        @returns                allocated string with statement or NULL
*/
char *testrun_text_block_c_header_documentation(
        char *module_name,
        testrun_extension_t extension,
        struct testrun_config const * const config,
        bool docu_open,
        char *brief);

/*----------------------------------------------------------------------------*/

/**
        Create the testrun default Shell documentation header block-

        Output will be formated like:

        #
        #       File            [FILE].sh
        #       Authors         [AUTHOR_NAME]
        #       Date            [CREATION_DATE]
        #
        #       Project         [PROJECT]
        #
        #       Description     Description of the file, including some
        #                       statements about the purpose of the file,
        #                       or any other description ...
        #
        #       Usage           ./file.sh /path/to/something
        #
        #       Dependencies    touch, cmod, ls, wc
        #
        #       Last changed    [CHANGED_DATA]  ... optional reason
        #
        #       -------------------------------------------------------------

        @param module_name      name for the module
        @param config           configuration to be used
        @param description      (optional) addition content for description
        @param usage            (optional) addition content for usage
        @param dependencies     (optional) addition content for dependencies
        @returns                allocated string with statement or NULL
*/
char *testrun_text_block_sh_header_documentation(
        char *module_name,
        struct testrun_config const * const config,
        char *description,
        char *usage,
        char *dependencies);
/*----------------------------------------------------------------------------*/

/*
 *      ------------------------------------------------------------------------
 *
 *      OTHER STANDARD TEXT BLOCKS                                       #OTHER
 *
 *      Default body content of the standard c files of a module.
 *
 *      ------------------------------------------------------------------------
 */

/**
        Create a formated comment block.

        Example:

        |*
         *      ----------------------------------------------------------------
         *
         *      NAME                                                       #TAG
         *
         *      ----------------------------------------------------------------
         *|

        NOTE: the slash  is replaced with | to prevent compiler error messages

*/
char *testrun_text_block_comment_header(
        char *name, char *tag);

/*----------------------------------------------------------------------------*/

/**
        Create a splitline.

        |*--------------------------------------------------------------------*|

        NOTE: the slash  is replaced with | to prevent compiler error messages

        @param indent   indent before start
        @param linesize size of the line (including terminating NULL)
        @param commentd if true comment tags will be set in front and end
*/
char *testrun_text_block_splitline(
        size_t indent, size_t linelength, bool commented);


/*----------------------------------------------------------------------------*/

/**
        Create script content. Will combine, copyright, documentation header
        and script content within a united string, which is ready to be written
        to a file.

        @param config           configuration to be used
        @param description      (optional) addition content for description
        @param usage            (optional) addition content for usage
        @param dependencies     (optional) addition content for dependencies
        @param content          (optional) content body of the script
        @returns                allocated string with statement or NULL
*/
char *testrun_text_block_script(
        testrun_config *config,
        char *description,
        char *usage,
        char *dependencies,
        char *content);

/*----------------------------------------------------------------------------*/

/**
        Create a default README.MD

        @param config           configuration to be used
        @param description      (optional) addition content for description
        @param usage            (optional) addition content for usage
        @param dependencies     (optional) addition content for dependencies
        @param content          (optional) content body of the script
        @returns                allocated string with statement or NULL
*/
char *testrun_text_block_readme(
        testrun_config *config,
        char *description,
        char *usage,
        char *installation);

/**
        Create a default doxygen.config

        @param config           configuration to be used
        @returns                allocated string with statement or NULL
*/
char *testrun_text_block_doxygen_config(
        testrun_config *config);



#endif /* testrun_text_block_h */
