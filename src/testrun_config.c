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

        This file is part of the testrun project. http://testrun.info

        ------------------------------------------------------------------------
*//**
        @file           testrun_config.c
        @author         Markus Toepfer
        @date           2018-07-12

        @ingroup        testrun_lib

        @brief          Implementation of default configurations and validation.


        ------------------------------------------------------------------------
*/
#include "../include/testrun_config.h"

/*----------------------------------------------------------------------------*/

bool testrun_config_validate(const struct testrun_config *config){

        // check required default parameter
        if (    !config ||
                !config->test_prefix ||
                !config->test_suffix_source ||
                !config->test_suffix_exec)
                return false;

        // check required project parameter
        if (    !config->project.name ||
                !config->project.path ||
                !config->project.search_project_path)
                return false;

        // check required path parameter
        if (    !config->path.bin       ||
                !config->path.build     ||
                !config->path.include   ||
                !config->path.source    ||
                !config->path.tests     ||
                !config->path.tools     ||
                !config->path.service   ||
                !config->path.doxygen)
                return false;

        // check required copyright parameter
        if (    !config->copyright.copyright.generate_header_string)
                return false;

        // check required script parameter
        if (    !config->script.lines_of_code   ||
                !config->script.test_coverage   ||
                !config->script.test_unit       ||
                !config->script.test_acceptance ||
                !config->script.test_runner     ||
                !config->script.test_gcov       ||
                !config->script.test_gprof)
                return false;

         // check required makefile parameter
        if (    !config->makefile.configurable ||
                !config->makefile.common)
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

struct testrun_config testrun_config_default(){

        struct testrun_config config = {

                .test_prefix            = TEST_PREFIX,
                .test_suffix_source     = TEST_SUFFIX_SOURCE,
                .test_suffix_exec       = TEST_SUFFIX_EXEC,

                .cflags                 = TESTRUN_CFLAGS,
                .version                = TESTRUN_TAG_VERSION,

                .indent.standard        = TESTRUN_DEFAULT_INDENT,
                .indent.c               = TESTRUN_DEFAULT_C_INDENT,
                .indent.script          = TESTRUN_DEFAULT_SHELL_INDENT,

                .project.path           = NULL,
                .project.name           = NULL,
                .project.url            = TESTRUN_WWW_DEFAULT,
                .project.description    = TESTRUN_APP_DEFAULT,
                .project.search_project_path = testrun_utils_search_project_path,

                .path.bin               = TESTRUN_FOLDER_BIN,
                .path.build             = TESTRUN_FOLDER_BUILD,
                .path.copyright         = TESTRUN_FOLDER_COPYRIGHT,
                .path.docs              = TESTRUN_FOLDER_DOCS,
                .path.include           = TESTRUN_FOLDER_INCLUDE,
                .path.source            = TESTRUN_FOLDER_SOURCE,
                .path.tests             = TESTRUN_FOLDER_TESTS,
                .path.tools             = TESTRUN_FOLDER_TOOLS,
                .path.service           = TESTRUN_FOLDER_SERVICE,
                .path.doxygen           = TESTRUN_FOLDER_DOXYGEN,
                .path.logfile           = TESTRUN_FOLDER_LOGFILES,

                .path.source_to_root    = TESTRUN_DEFAULT_TO_ROOT,
                .path.include_to_root   = TESTRUN_DEFAULT_TO_ROOT,
                .path.unit_tests_to_root= TESTRUN_DEFAULT_TESTS_TO_ROOT,

                .script.lines_of_code   = TESTRUN_SCRIPT_LOC,
                .script.test_coverage   = TESTRUN_SCRIPT_COVERAGE,
                .script.test_unit       = TESTRUN_SCRIPT_UNIT,
                .script.test_acceptance = TESTRUN_SCRIPT_ACCEPTANCE,
                .script.test_runner     = TESTRUN_SCRIPT_RUNNER,
                .script.test_gcov       = TESTRUN_SCRIPT_GCOV,
                .script.test_gprof      = TESTRUN_SCRIPT_GPORF,

                .makefile.configurable  = TESTRUN_MAKEFILE,
                .makefile.common        = TESTRUN_MAKEFILE_COMMON,
                .makefile.target        = LIB

        };

        config.copyright.copyright = testrun_copyright_reserved();

        return config;
}

/*----------------------------------------------------------------------------*/

void testrun_config_print_usage(const char *app_name){

        fprintf(stdout, "\n");
        fprintf(stdout, "USAGE          %s [OPTIONS]... [TARGET_NAME]\n", app_name);
        fprintf(stdout, "\n");
        fprintf(stdout, "EXAMPLES       %s --project --dir /home/project name\n", app_name);
        fprintf(stdout, "               %s -pr name\n", app_name);
        fprintf(stdout, "               %s name\n", app_name);
        fprintf(stdout, "\n");
        fprintf(stdout, "OPTIONS        (GENERAL)\n");
        fprintf(stdout, "\n");
        fprintf(stdout, "               -p,     --project       create a new project instead of a module\n");
        fprintf(stdout, "               -n,     --name          define the target name explizit as argument\n");
        fprintf(stdout, "               -o,     --owner         define the target owner explizit as argument\n");
        fprintf(stdout, "               -u,     --author        define an author explizit as argument\n");
        fprintf(stdout, "               -x,     --note          define an copyright end note as argument\n");
        fprintf(stdout, "               -w,     --web           define a webpage explizit as argument\n");
        fprintf(stdout, "               -d,     --dir           set the project's top dir (e.g. ~/home/projects)\n");
        fprintf(stdout, "               -v      --version       print the file version \n");
        fprintf(stdout, "               -h      --help          print this text \n");
        fprintf(stdout, "\n");
        fprintf(stdout, "               (TYPE used only in project creation)\n");
        fprintf(stdout, "\n");
        fprintf(stdout, "                       --lib           flag to create a library\n");
        fprintf(stdout, "                       --exec          flag to create an executable\n");
        fprintf(stdout, "                       --service       flag to create a systemd service\n");
        fprintf(stdout, "\n");
        fprintf(stdout, "               (COPRIGHT)\n");
        fprintf(stdout, "\n");
        fprintf(stdout, "               -r       --res           set copyright to \"All rights reserved.\" \n");
        fprintf(stdout, "               -g       --gpl           set copyright to GPL v3 \n");
        fprintf(stdout, "               -a       --apache        set copyright to APACHE v2 \n");
        fprintf(stdout, "               -b       --bsd           set copyright to BSD 3Clause \n");
        fprintf(stdout, "               -m       --mit           set copyright to MIT \n");
        fprintf(stdout, "\n");
        fprintf(stdout, "NOTE\n");
        fprintf(stdout, "\n");
        fprintf(stdout, "               To create a new module for an existing project, this command will try to find a \n");
        fprintf(stdout, "               (default) project root path, and fail if no root path is identified.\n");
        fprintf(stdout, "               If --dir is set, the root path will be searched from the given directory,\n");
        fprintf(stdout, "               otherwise the search will be started at the current working directory.\n");
        fprintf(stdout, "\n");
}

/*----------------------------------------------------------------------------*/

testrun_config testrun_config_read_user_input(
        const char *app_name, bool *success, bool *project, int argc, char *argv[]){

        /*
         *      NOTE This user input reader is limited and will use the
         *      default config and add some configurable items.
         *
         *      It is a base implementation example.
         */

        testrun_config config = testrun_config_default();

        if (!success || !project || !argv || (argc < 2))
                goto error;

        if (!app_name)
                app_name = TESTRUN_APP_DEFAULT;

        *success = false;
        *project = false;

        int c;
        int option_index = 0;

        static int flag_project = 0;

        static int flag_gpl     = 0;
        static int flag_apache  = 0;
        static int flag_bsd3    = 0;
        static int flag_mit     = 0;
        static int flag_res     = 0;

        static int flag_lib     = 0;
        static int flag_service = 0;
        static int flag_exec    = 0;

        char path[PATH_MAX];
        bzero(path, PATH_MAX);

        /* UNSET ... the options we want the get from user input*/
        config.project.name             = NULL;
        config.project.path             = NULL;
        config.project.url              = NULL;
        config.project.description      = NULL;
        config.makefile.target          = LIB;

        char *string = NULL;

        while (1) {

                static struct option long_options[] = {

                        /* These options set a flag. */

                        /* Project  ... distinguish between project & module */
                        {"project", no_argument, &flag_project, 1},
                        {"module",  no_argument, &flag_project, 0},

                        /* Licence  ... only one MAY be set */
                        {"gpl",     no_argument, &flag_gpl,     1},
                        {"apache",  no_argument, &flag_apache,  1},
                        {"bsd",     no_argument, &flag_bsd3,    1},
                        {"mit",     no_argument, &flag_mit,     1},
                        {"res",     no_argument, &flag_res,     1},

                        /* Type     ... only one MAY be set */
                        {"lib",     no_argument, &flag_lib,     1},
                        {"exec",    no_argument, &flag_exec,    1},
                        {"service", no_argument, &flag_service, 1},

                        /* These options don’t set a flag.
                           We distinguish them by their indices. */
                        {"help",    optional_argument, 0, 'h'},
                        {"version", optional_argument, 0, 'v'},
                        {"owner",   required_argument, 0, 'o'},
                        {"author",  required_argument, 0, 'u'},
                        {"note",    required_argument, 0, 'x'},
                        {"webpage", required_argument, 0, 'w'},
                        {"name",    required_argument, 0, 'n'},
                        {"dir",     required_argument, 0, 'd'},
                        {0, 0, 0, 0}
                };

                /* getopt_long stores the option index here. */

                c = getopt_long (argc, argv, "n:o:u:x:w:n:d:?hpabgmr",
                                 long_options, &option_index);

                /* Detect the end of the options. */
                if (c == -1)
                        break;

                switch (c) {
                        case 0:
                                /* If this option set a flag, do nothing else now. */
                                if (long_options[option_index].flag != 0)
                                        break;

                                printf ("option %s", long_options[option_index].name);
                                if (optarg)
                                        printf (" with arg %s", optarg);
                                printf ("\n");
                                break;

                        case 'h':
                                testrun_config_print_usage(app_name);
                                goto error;
                                break;

                        case '?':
                                testrun_config_print_usage(app_name);
                                goto error;
                                break;

                        case 'p':
                                flag_project = 1;
                                break;

                        case 'n':
                                printf ("option -n (NAME of PROJECT) `%s'\n", optarg);
                                config.project.name = optarg;
                                break;

                        case 'o':
                                printf ("option -o (OWNER of PROJECT) `%s'\n", optarg);
                                config.copyright.owner = optarg;
                                break;

                        case 'u':
                                printf ("option -u (AUTHOR of PROJECT) `%s'\n", optarg);
                                config.copyright.author = optarg;
                                break;

                        case 'x':
                                printf ("option -x (NOTE to PROJECT) `%s'\n", optarg);
                                config.copyright.note = optarg;
                                break;

                        case 'w':
                                printf ("option -w (WEBPAGE of PROJECT) `%s'\n", optarg);
                                config.project.url = optarg;
                                break;

                        case 'd':
                                printf ("option -d (DIR) `%s'\n", optarg);
                                config.project.path = optarg;
                                break;

                        case 'v':
                                testrun_config_print_version();
                                goto error;
                                break;

                        case 'a':
                                flag_apache = 1;
                                break;

                        case 'b':
                                flag_bsd3 = 1;
                                break;

                        case 'g':
                                flag_gpl = 1;
                                break;

                        case 'm':
                                flag_mit = 1;
                                break;

                        case 'r':
                                flag_res = 1;
                                break;

                        default:
                                testrun_config_print_usage(app_name);
                                goto error;

                }
        }

        /* Validate input */

        fprintf(stdout, "Going to create a %s \n",
                (flag_project == 1) ? "PROJECT" : "MODULE");

        /* ... at max 1 copyright statement is selected */
        if (   (flag_gpl +
                flag_apache +
                flag_mit +
                flag_res +
                flag_bsd3) > 1 ){
                fprintf(stderr,"ERROR, multiple copyright statements selected\n");
                goto error;
        }

        /* ... at max 1 service type is selected */
        if (   (flag_lib +
                flag_service +
                flag_exec) > 1 ){
                fprintf(stderr,"ERROR, multiple types selected "
                        "(lib, service, exec)\n");
                goto error;
        }

        if (flag_lib == 1) {
                fprintf(stdout, "... creating lib \n");
                config.makefile.target = LIB;
        } else if (flag_exec == 1){
                fprintf(stdout, "... creating exec \n");
                config.makefile.target = EXEC;
        } else if (flag_service == 1) {
                fprintf(stdout, "... creating service \n");
                config.makefile.target = SERVICE;
        }

        /* ... a project name is set */
        if (config.project.name == NULL){

                /* project name may be unspecified input parameter */
                if (optind < argc) {
                        string = argv[optind++];
                        fprintf(stdout,"Using name: %s\n", string);
                        config.project.name = string;

                } else {
                        fprintf(stderr,"ERROR, no name given, add -n \"name\" or "
                                "add a target_name as argument.\n");
                        testrun_config_print_usage(app_name);
                        goto error;
                }
        }

        fprintf(stdout, "... using name %s \n",config.project.name);

        /* ... set desired copyright */

        if (flag_gpl != 0) {

                struct testrun_copyright_gpl_v3_parameter *parameter = calloc(1, sizeof(struct testrun_copyright_gpl_v3_parameter));

                parameter->type                 = GENERAL;
                parameter->program_name         = config.project.name;
                config.copyright.copyright      = testrun_copyright_gpl_version_3();
                config.copyright.gpl_parameter  = parameter;

        }  else if (flag_bsd3 != 0) {

                config.copyright.copyright = testrun_copyright_bsd_3clause();

        }  else if (flag_mit != 0) {

                config.copyright.copyright = testrun_copyright_mit();

        }  else if (flag_res != 0) {

                config.copyright.copyright = testrun_copyright_reserved();

        } else {

                config.copyright.copyright = testrun_copyright_apache_version_2();

        }

        if (flag_project == 1)
                *project = true;

        *success = true;
        return config;

error:
        if (success)
                *success = false;
        return config;
}
