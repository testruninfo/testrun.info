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
 *      @file           testrun_header.h
 *      @author         Markus Toepfer
 *      @date           2017-11-12
 *
 *      @ingroup
 *
 *      @brief
 *
 *
 *      ------------------------------------------------------------------------
 **/

#ifndef testrun_header_h
#define testrun_header_h

/*----------------------------------------------------------------------------
 *
 *      HEADER DEFINITIONS
 *
 *      These definitions are used to create the standard header of the project
 *      files including the copyright, as well as the documentation token.
 *
 *----------------------------------------------------------------------------*/

 /* Maximum header length is 100 lines * 80 characters */
#define TESTRUN_HEADER_MAX_LENGTH 8000

#define TESTRUN_COPYRIGHT_INTRO  \
"/*** \n\
 *      ------------------------------------------------------------------------\n\
 *\n"

#define TESTRUN_COPYRIGHT_OUTRO  \
" *\n\
 *      ------------------------------------------------------------------------\n\
 ***//**\n\
 *"
#define TESTRUN_HEADER_OUTRO  \
"\n *\n\
 *      ------------------------------------------------------------------------\n\
 **/\n"

#define TESTRUN_HEADER_LINE_INTRO       " *      "
#define TESTRUN_HEADER_LINE_EMPTY       "\n *"
#define TESTRUN_HEADER_DOCU_FILE        "\n *      @file           "
#define TESTRUN_HEADER_DOCU_AUTHOR      "\n *      @author         "
#define TESTRUN_HEADER_DOCU_DATE        "\n *      @date           "
#define TESTRUN_HEADER_DOCU_GROUP       "\n *      @ingroup"
#define TESTRUN_HEADER_DOCU_BRIEF       "\n *      @brief"
#define TESTRUN_LINE_EMPTY              "\n"


/*----------------------------------------------------------------------------*/

/**
 *      Create a header snippetd from a source, using a line_intro, a
 *      general intro, as well as a general outro.
 *
 *      @param  source  source to be used
 *      @param  prefix  prefix to the added to each line of source data.
 *      @param  intro   intro to be added to source
 *      @parem  outro   outro to be added to source
 *      @return         allocated string (MUST be freed) with copyright or NULL
 */
char *testrun_create_header_snippet(
        char *source,
        char *prefix,
        char *intro,
        char *outro);

/*----------------------------------------------------------------------------*/

char *testrun_create_copyright_header(
        char *source,
        char *prefix,
        char *intro,
        char *outro);



#endif /* testrun_header_h */
