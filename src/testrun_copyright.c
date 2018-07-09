/***
        ------------------------------------------------------------------------

        Copyright 2018 [COPYRIGHT_OWNER]

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
        @file           testrun_copyright.c
        @author         [AUTHOR]
        @date           2018-07-06

        @ingroup        testrun_lib

        @brief


        ------------------------------------------------------------------------
*/
#include "../include/testrun_copyright.h"
#include "../include/testrun_utils.h"

#define COPYRIGHT_TAG_YEAR      "[COPYRIGHT_YEAR]"
#define COPYRIGHT_TAG_OWNER     "[COPYRIGHT_OWNER]"

#define COPYRIGHT_DEFAULT_INTRO "Copyright (c)"
#define COPYRIGHT_DEFAULT_TEXT  "All rights reserved."

char *testrun_copyright_generate_header_string(
        const char* prefix,
        const char* intro,
        const char* year,
        const char* owner,
        const char* text,
        const char* endnote,
        const char* suffix,
        uint16_t indent,
        bool spaces){

        size_t size_startline = 5000;
        char startline[size_startline];
        memset(startline, 0, size_startline);

        char *result = NULL;
        char *string = NULL;

        size_t size    = 0;
        size_t px_len  = 0;
        size_t sx_len  = 0;
        size_t str_len = 0;
        size_t txt_len = 0;

        /*
         *      SET DEFAULTS.
         */

        if (!intro)
                intro  = COPYRIGHT_DEFAULT_INTRO;

        if (!year)
                year   = COPYRIGHT_TAG_YEAR;

        if (!owner)
                owner  = COPYRIGHT_TAG_OWNER;

        if (prefix)
                px_len = strlen(prefix);

        if (suffix)
                sx_len = strlen(suffix);


        if (text) {

                if (!snprintf(startline, size_startline, "%s %s %s\n\n", 
                        intro, year, owner))
                        goto error;

                str_len = strlen(startline);
                txt_len = strlen(text);

                size   = str_len + txt_len + 2;
                result = calloc(size, sizeof(char));
                if (!result)
                        goto error; 

                if (!snprintf(result, size, "%s%s\n", 
                        startline, 
                        text))
                        goto error;

                string = result;
                result = NULL;

        } else {

                if (!snprintf(startline, size_startline, "%s %s %s\n%s\n", 
                        intro, year, owner, COPYRIGHT_DEFAULT_TEXT))
                        goto error;

                string = strdup(startline);
        }

        if (endnote) {

                str_len = strlen(string);
                txt_len = strlen(endnote);
                size    = str_len + txt_len + 3;

                result = calloc(size, sizeof(char));
                if (!result)
                        goto error; 

                if (!snprintf(result, size, "%s\n%s\n", 
                        string, 
                        endnote)){
                        free(string);
                        goto error;
                }
                        
                free(string);
                string = result;
                result = NULL;
        }

        if (indent > 0) {

                /*
                 *      add some indent,
                 *      to each line of 
                 *      the string.
                 *      
                 *      Indent with spaces.
                 */

                char indent_string[indent + 1];
                memset(indent_string, 0, indent + 1);

                if (spaces){
                        memset(indent_string, ' ', indent);
                } else {
                        memset(indent_string, '\t', indent);
                }

                result = testrun_utils_insert_at_each_line(string, indent_string, NULL);
                free(string);

                if (!result)
                        goto error;

                string = result;
                result = NULL;

        }

        if (prefix || suffix) {

                str_len = strlen(string);
                size    = str_len + px_len + sx_len + 1;
                result  = calloc(size, sizeof(char));
                if (!result)
                        goto error;

                char *ptr = result;
                if (prefix){
                        if (!strncat(ptr, prefix, px_len)){
                                free(string);
                                goto error;
                        }
                }

                ptr += px_len;
                if (!strncat(ptr, string, str_len)){
                        free(string);
                        goto error;
                }

                free(string);
                string = NULL;

                ptr += str_len;

                if (suffix)
                        if (!strncat(ptr, suffix, sx_len))
                                goto error;

                string = result;
                result = NULL;

        }

        if (!result)
                return string;

        free(string);
        return result;

error:
        if (string)
                free(string);

        if (result)
                free(result);
        return NULL;
}

/*      ------------------------------------------------------------------------
 *
 *      EXAMPLE IMPLEMENTATION OF SOME COPYRIGHTS
 *
 *      This part contains some common implementations, which may be used
 *      within custom tools and frameworks using testrun_lib.
 *
 *      ------------------------------------------------------------------------
 */

char *testrun_copyright_generate_reserved(
        const char* prefix,
        const char* intro,
        const char* year,
        const char* owner,
        const char* endnote,
        const char* suffix,
        uint16_t indent,
        bool spaces){

        char *text = COPYRIGHT_DEFAULT_TEXT;

        return testrun_copyright_generate_header_string(
                prefix,
                intro,
                year,
                owner,
                text,
                endnote,
                suffix,
                indent,
                spaces);
}

/*----------------------------------------------------------------------------*/

struct testrun_copyright testrun_copyright_functions_reserved(){

        testrun_copyright copyright = {

                .generate_header_string         = testrun_copyright_generate_reserved,
                .generate_full_text_licence     = NULL
        };

        return copyright;
}

/*----------------------------------------------------------------------------*/

char *testrun_copyright_generate_apache_version_2(
        const char* prefix,
        const char* intro,
        const char* year,
        const char* owner,
        const char* endnote,
        const char* suffix,
        uint16_t indent,
        bool spaces){

        char *text =
        "Licensed under the Apache License, Version 2.0 (the \"License\");\n"
        "you may not use this file except in compliance with the License.\n"
        "You may obtain a copy of the License at\n"
        "\n"
        "        http://www.apache.org/licenses/LICENSE-2.0\n"
        "\n"
        "Unless required by applicable law or agreed to in writing, software\n"
        "distributed under the License is distributed on an \"AS IS\" BASIS,\n"
        "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
        "See the License for the specific language governing permissions and\n"
        "limitations under the License.";

        return testrun_copyright_generate_header_string(
                prefix,
                intro,
                year,
                owner,
                text,
                endnote,
                suffix,
                indent,
                spaces);
}

/*----------------------------------------------------------------------------*/

char *testrun_copyright_apache_version_2_full_text(){

        char *result = NULL;
        
        size_t size  = 15000;
        char buffer[size];

        /*
         *      SOURCE http://www.apache.org/licenses/LICENSE-2.0.txt
         *      copied 2017-12-06
         */

        if (snprintf(buffer, size,
        "                              Apache License\n"
        "                        Version 2.0, January 2004\n"
        "                     http://www.apache.org/licenses/\n"
        "\n"
        "TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION\n"
        "\n"
        "1. Definitions.\n"
        "\n"
        "   \"License\" shall mean the terms and conditions for use, reproduction,\n"
        "   and distribution as defined by Sections 1 through 9 of this document.\n"
        "\n"
        "   \"Licensor\" shall mean the copyright owner or entity authorized by\n"
        "   the copyright owner that is granting the License.\n"
        "\n"
        "   \"Legal Entity\" shall mean the union of the acting entity and all\n"
        "   other entities that control, are controlled by, or are under common\n"
        "   control with that entity. For the purposes of this definition,\n"
        "   \"control\" means (i) the power, direct or indirect, to cause the\n"
        "   direction or management of such entity, whether by contract or\n"
        "   otherwise, or (ii) ownership of fifty percent (50%%) or more of the\n"
        "   outstanding shares, or (iii) beneficial ownership of such entity.\n"
        "\n"
        "   \"You\" (or \"Your\") shall mean an individual or Legal Entity\n"
        "   exercising permissions granted by this License.\n"
        "\n"
        "   \"Source\" form shall mean the preferred form for making modifications,\n"
        "   including but not limited to software source code, documentation\n"
        "   source, and configuration files.\n"
        "\n"
        "   \"Object\" form shall mean any form resulting from mechanical\n"
        "   transformation or translation of a Source form, including but\n"
        "   not limited to compiled object code, generated documentation,\n"
        "   and conversions to other media types.\n"
        "\n"
        "   \"Work\" shall mean the work of authorship, whether in Source or\n"
        "   Object form, made available under the License, as indicated by a\n"
        "   copyright notice that is included in or attached to the work\n"
        "   (an example is provided in the Appendix below).\n"
        "\n"
        "   \"Derivative Works\" shall mean any work, whether in Source or Object\n"
        "   form, that is based on (or derived from) the Work and for which the\n"
        "   editorial revisions, annotations, elaborations, or other modifications\n"
        "   represent, as a whole, an original work of authorship. For the purposes\n"
        "   of this License, Derivative Works shall not include works that remain\n"
        "   separable from, or merely link (or bind by name) to the interfaces of,\n"
        "   the Work and Derivative Works thereof.\n"
        "\n"
        "   \"Contribution\" shall mean any work of authorship, including\n"
        "   the original version of the Work and any modifications or additions\n"
        "   to that Work or Derivative Works thereof, that is intentionally\n"
        "   submitted to Licensor for inclusion in the Work by the copyright owner\n"
        "   or by an individual or Legal Entity authorized to submit on behalf of\n"
        "   the copyright owner. For the purposes of this definition, \"submitted\"\n"
        "   means any form of electronic, verbal, or written communication sent\n"
        "   to the Licensor or its representatives, including but not limited to\n"
        "   communication on electronic mailing lists, source code control systems,\n"
        "   and issue tracking systems that are managed by, or on behalf of, the\n"
        "   Licensor for the purpose of discussing and improving the Work, but\n"
        "   excluding communication that is conspicuously marked or otherwise\n"
        "   designated in writing by the copyright owner as \"Not a Contribution.\"\n"
        "\n"
        "   \"Contributor\" shall mean Licensor and any individual or Legal Entity\n"
        "   on behalf of whom a Contribution has been received by Licensor and\n"
        "   subsequently incorporated within the Work.\n"
        "\n"
        "2. Grant of Copyright License. Subject to the terms and conditions of\n"
        "   this License, each Contributor hereby grants to You a perpetual,\n"
        "   worldwide, non-exclusive, no-charge, royalty-free, irrevocable\n"
        "   copyright license to reproduce, prepare Derivative Works of,\n"
        "   publicly display, publicly perform, sublicense, and distribute the\n"
        "   Work and such Derivative Works in Source or Object form.\n"
        "\n"
        "3. Grant of Patent License. Subject to the terms and conditions of\n"
        "   this License, each Contributor hereby grants to You a perpetual,\n"
        "   worldwide, non-exclusive, no-charge, royalty-free, irrevocable\n"
        "   (except as stated in this section) patent license to make, have made,\n"
        "   use, offer to sell, sell, import, and otherwise transfer the Work,\n"
        "   where such license applies only to those patent claims licensable\n"
        "   by such Contributor that are necessarily infringed by their\n"
        "   Contribution(s) alone or by combination of their Contribution(s)\n"
        "   with the Work to which such Contribution(s) was submitted. If You\n"
        "   institute patent litigation against any entity (including a\n"
        "   cross-claim or counterclaim in a lawsuit) alleging that the Work\n"
        "   or a Contribution incorporated within the Work constitutes direct\n"
        "   or contributory patent infringement, then any patent licenses\n"
        "   granted to You under this License for that Work shall terminate\n"
        "   as of the date such litigation is filed.\n"
        "\n"
        "4. Redistribution. You may reproduce and distribute copies of the\n"
        "   Work or Derivative Works thereof in any medium, with or without\n"
        "   modifications, and in Source or Object form, provided that You\n"
        "   meet the following conditions:\n"
        "\n"
        "   (a) You must give any other recipients of the Work or\n"
        "       Derivative Works a copy of this License; and\n"
        "\n"
        "   (b) You must cause any modified files to carry prominent notices\n"
        "       stating that You changed the files; and\n"
        "\n"
        "   (c) You must retain, in the Source form of any Derivative Works\n"
        "       that You distribute, all copyright, patent, trademark, and\n"
        "       attribution notices from the Source form of the Work,\n"
        "       excluding those notices that do not pertain to any part of\n"
        "       the Derivative Works; and\n"
        "\n"
        "   (d) If the Work includes a \"NOTICE\" text file as part of its\n"
        "       distribution, then any Derivative Works that You distribute must\n"
        "       include a readable copy of the attribution notices contained\n"
        "       within such NOTICE file, excluding those notices that do not\n"
        "       pertain to any part of the Derivative Works, in at least one\n"
        "       of the following places: within a NOTICE text file distributed\n"
        "       as part of the Derivative Works; within the Source form or\n"
        "       documentation, if provided along with the Derivative Works; or,\n"
        "       within a display generated by the Derivative Works, if and\n"
        "       wherever such third-party notices normally appear. The contents\n"
        "       of the NOTICE file are for informational purposes only and\n"
        "       do not modify the License. You may add Your own attribution\n"
        "       notices within Derivative Works that You distribute, alongside\n"
        "       or as an addendum to the NOTICE text from the Work, provided\n"
        "       that such additional attribution notices cannot be construed\n"
        "       as modifying the License.\n"
        "\n"
        "   You may add Your own copyright statement to Your modifications and\n"
        "   may provide additional or different license terms and conditions\n"
        "   for use, reproduction, or distribution of Your modifications, or\n"
        "   for any such Derivative Works as a whole, provided Your use,\n"
        "   reproduction, and distribution of the Work otherwise complies with\n"
        "   the conditions stated in this License.\n"
        "\n"
        "5. Submission of Contributions. Unless You explicitly state otherwise,\n"
        "   any Contribution intentionally submitted for inclusion in the Work\n"
        "   by You to the Licensor shall be under the terms and conditions of\n"
        "   this License, without any additional terms or conditions.\n"
        "   Notwithstanding the above, nothing herein shall supersede or modify\n"
        "   the terms of any separate license agreement you may have executed\n"
        "   with Licensor regarding such Contributions.\n"
        "\n"
        "6. Trademarks. This License does not grant permission to use the trade\n"
        "   names, trademarks, service marks, or product names of the Licensor,\n"
        "   except as required for reasonable and customary use in describing the\n"
        "   origin of the Work and reproducing the content of the NOTICE file.\n"
        "\n"
        "7. Disclaimer of Warranty. Unless required by applicable law or\n"
        "   agreed to in writing, Licensor provides the Work (and each\n"
        "   Contributor provides its Contributions) on an \"AS IS\" BASIS,\n"
        "   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or\n"
        "   implied, including, without limitation, any warranties or conditions\n"
        "   of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A\n"
        "   PARTICULAR PURPOSE. You are solely responsible for determining the\n"
        "   appropriateness of using or redistributing the Work and assume any\n"
        "   risks associated with Your exercise of permissions under this License.\n"
        "\n"
        "8. Limitation of Liability. In no event and under no legal theory,\n"
        "   whether in tort (including negligence), contract, or otherwise,\n"
        "   unless required by applicable law (such as deliberate and grossly\n"
        "   negligent acts) or agreed to in writing, shall any Contributor be\n"
        "   liable to You for damages, including any direct, indirect, special,\n"
        "   incidental, or consequential damages of any character arising as a\n"
        "   result of this License or out of the use or inability to use the\n"
        "   Work (including but not limited to damages for loss of goodwill,\n"
        "   work stoppage, computer failure or malfunction, or any and all\n"
        "   other commercial damages or losses), even if such Contributor\n"
        "   has been advised of the possibility of such damages.\n"
        "\n"
        "9. Accepting Warranty or Additional Liability. While redistributing\n"
        "   the Work or Derivative Works thereof, You may choose to offer,\n"
        "   and charge a fee for, acceptance of support, warranty, indemnity,\n"
        "   or other liability obligations and/or rights consistent with this\n"
        "   License. However, in accepting such obligations, You may act only\n"
        "   on Your own behalf and on Your sole responsibility, not on behalf\n"
        "   of any other Contributor, and only if You agree to indemnify,\n"
        "   defend, and hold each Contributor harmless for any liability\n"
        "   incurred by, or claims asserted against, such Contributor by reason\n"
        "   of your accepting any such warranty or additional liability.\n"
        "\n"
        "END OF TERMS AND CONDITIONS\n"
        "\n"
        "APPENDIX: How to apply the Apache License to your work.\n"
        "\n"
        "   To apply the Apache License to your work, attach the following\n"
        "   boilerplate notice, with the fields enclosed by brackets \"[]\"\n"
        "   replaced with your own identifying information. (Don't include\n"
        "   the brackets!)  The text should be enclosed in the appropriate\n"
        "   comment syntax for the file format. We also recommend that a\n"
        "   file or class name and description of purpose be included on the\n"
        "   same \"printed page\" as the copyright notice for easier\n"
        "   identification within third-party archives.\n"
        "\n"
        "Copyright [yyyy] [name of copyright owner]\n"
        "\n"
        "Licensed under the Apache License, Version 2.0 (the \"License\");\n"
        "you may not use this file except in compliance with the License.\n"
        "You may obtain a copy of the License at\n"
        "\n"
        "    http://www.apache.org/licenses/LICENSE-2.0\n"
        "\n"
        "Unless required by applicable law or agreed to in writing, software\n"
        "distributed under the License is distributed on an \"AS IS\" BASIS,\n"
        "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
        "See the License for the specific language governing permissions and\n"
        "limitations under the License.\n"
        ) < 0)
                goto error;

        result = calloc(strlen(buffer) + 1, sizeof(char));
        if (!result)
                goto error;

        if (!strcat(result, buffer))
                goto error;

        return result;
error:
        if (result)
                free(result);

        return NULL;
}

/*----------------------------------------------------------------------------*/

struct testrun_copyright testrun_copyright_functions_apache_version_2(){

        testrun_copyright copyright = {

                .generate_header_string         = testrun_copyright_generate_apache_version_2,
                .generate_full_text_licence     = testrun_copyright_apache_version_2_full_text
        };

        return copyright;
}

/*----------------------------------------------------------------------------*/