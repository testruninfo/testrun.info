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
 *      @file           testrun_project_app.c
 *      @author         Markus Toepfer
 *      @date           2018-05-11
 *
 *      @ingroup        testrun_project
 *
 *      @brief          Implementation of the app functions.
 *
 *
 *      ------------------------------------------------------------------------
 **/

#include "../include/testrun_project_app.h"
/*----------------------------------------------------------------------------*/

bool testrun_project_app_set_author_dynamic(
        char *buffer, size_t size){

        if (!buffer || size < 1)
                return false;

        bool set = false;
        FILE *in;
        FILE *popen();

        if((in = popen("git config user.name", "r")) != NULL){

                if (fgets(buffer, size, in)!=NULL){

                        fprintf(stdout, "... using git username %s as AUTHOR\n",
                                buffer);
                        set = true;

                }
                pclose(in);
        }

        if (!set) {

                // FALLBACK default AUTHOR TAG
                if (snprintf(buffer, size, "%s",
                        TESTRUN_TAG_DEFAULT_AUTHOR) < 0){
                        goto error;
                }
        }

        return true;
error:
        return false;
}

/*----------------------------------------------------------------------------*/

static void testrun_project_app_print_usage(){

        fprintf(stdout, "\n");
        fprintf(stdout, "USAGE          testrun_project [OPTIONS]... [TARGET_NAME]\n");
        fprintf(stdout, "\n");
        fprintf(stdout, "EXAMPLES       testrun_project --project --dir /home/project name\n");
        fprintf(stdout, "               testrun_project -mg name\n");
        fprintf(stdout, "               testrun_project name\n");
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
        fprintf(stdout, "               -r       --res           set copyright to all rights reserved \n");
        fprintf(stdout, "               -g       --gpl           set copyright to GPL v3 \n");
        fprintf(stdout, "               -a       --apache        set copyright to APACHE v2 \n");
        fprintf(stdout, "               -b       --bsd           set copyright to BSD 3Clause \n");
        fprintf(stdout, "               -m       --mit           set copyright to MIT \n");
        fprintf(stdout, "\n");
        fprintf(stdout, "NOTE\n");
        fprintf(stdout, "\n");
        fprintf(stdout, "               To create a new module for an existing project, this command will try to find a \n");
        fprintf(stdout, "               (default) project root path (which MUST contain the folders include, src, tests),\n");
        fprintf(stdout, "               and fail if no root path is identified.  \n");
        fprintf(stdout, "               If --dir is set, the root path will be searched from the given directory,\n");
        fprintf(stdout, "               otherwise the search will be started at the current working directory.\n");
        fprintf(stdout, "\n");
}

/*----------------------------------------------------------------------------*/

static testrun_config testrun_project_app_create_dynamic_config(
        bool *success, bool *project, int argc, char *argv[]){

        testrun_config config = testrun_config_default();

        if (!success || !project || !argv || (argc < 1))
                goto error;

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

        char *owner = NULL;
        char *note  = NULL;

        /* UNSET ... the options we want the get from user input*/
        config.project.name     = NULL;
        config.project.path     = NULL;
        config.project.url      = NULL;
        config.project.type     = TESTRUN_LIB;

        static char date[10];
        bzero(date, 10);

        char *string = NULL;

        if (!testrun_time_write_to(TESTRUN_SCOPE_YEAR, date, 10))
                goto error;

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

                c = getopt_long (argc, argv, "?hvpnouxw:d:abgmr",
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

                        case 'p':
                                flag_project = 1;
                                break;

                        case 'h':
                        case '?':
                                testrun_project_app_print_usage();
                                goto error;
                                break;

                        case 'n':
                                printf ("option -n (NAME of PROJECT) `%s'\n", optarg);
                                config.project.name = optarg;
                                break;

                        case 'o':
                                printf ("option -o (OWNER of PROJECT) `%s'\n", optarg);
                                owner = optarg;
                                break;

                        case 'u':
                                printf ("option -u (AUTHOR of PROJECT) `%s'\n", optarg);
                                config.author = optarg;
                                break;

                        case 'x':
                                printf ("option -x (NOTE to PROJECT) `%s'\n", optarg);
                                note = optarg;
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
                                testrun_project_app_print_version();
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
                                testrun_project_app_print_usage();
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
                config.project.type = TESTRUN_LIB;
        } else if (flag_exec == 1){
                fprintf(stdout, "... creating exec \n");
                config.project.type = TESTRUN_EXEC;
        } else if (flag_service == 1) {
                fprintf(stdout, "... creating service \n");
                config.project.type = TESTRUN_SERVICE;
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
                        testrun_project_app_print_usage();
                        goto error;
                }
        }

        fprintf(stdout, "... using name %s \n",config.project.name);

        /* ... set desired copyright */

        if (flag_gpl != 0) {

                config.copyright = testrun_copyright_GPL_v3(
                        date, owner, note,
                        config.project.name, GENERAL);

        }  else if (flag_bsd3 != 0) {

                config.copyright = testrun_copyright_BSD_3Clause(
                        date, owner, note);

        }  else if (flag_mit != 0) {

                config.copyright = testrun_copyright_MIT(
                        date, owner, note);

        }  else if (flag_res != 0) {

                config.copyright = testrun_copyright_default(
                        date, owner, note);

        } else {

                // APACHE 2 as default copyright statement
                config.copyright = testrun_copyright_apache_version_2(
                        date, owner, note);

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

/*----------------------------------------------------------------------------*/

int testrun_project_app_create_new_project(testrun_config config){

        if (!config.project.name)
                goto error;

        fprintf(stdout, "START creating a new project %s\n",
                config.project.name);

        char path[PATH_MAX];
        bzero(path, PATH_MAX);

        DIR *dp;

        if (!config.project.path) {

                if (snprintf(path, PATH_MAX, "%s", config.project.name) < 0){
                        fprintf(stderr, "ERROR, failed to concat project root path\n");
                        goto error;
                }

        } else if (snprintf(path, PATH_MAX, "%s/%s",
                config.project.path, config.project.name) < 0){
                fprintf(stderr, "ERROR, failed to concat project root path\n");
                goto error;
        }

        config.project.path = path;

        dp = opendir(config.project.path);
        if (dp) {
                fprintf(stderr, "ERROR, path project already exists %s\n",
                config.project.path);
                (void) closedir (dp);
                goto error;
        }

        if (!testrun_path_create(config.project.path, PATH_MAX)){
                fprintf(stderr, "ERROR, failed to create project root path %s\n",
                        config.project.path);
                        (void) closedir (dp);
                goto error;
        } else {
                fprintf(stdout, "Created project root path %s\n",
                        config.project.path);
                        (void) closedir (dp);
        }

        if (!testrun_lib_create_project(&config, NULL)){
                fprintf(stderr, "ERROR,failed to create project %s\n",
                config.project.name);
                goto error;
        }

        fprintf(stderr, "Created project %s\n",
                config.project.name);

        return 0;
error:
        return -1;
}

/*----------------------------------------------------------------------------*/

int testrun_project_app_create_new_module(testrun_config config){

        if (!config.project.name)
                goto error;

        fprintf(stdout, "START creating a new module %s with path %s\n",
                config.project.name,
                config.project.path);

        bool free_current = false;
        char *current_path = NULL;
        char *root_path    = NULL;

        char *project_name = NULL;
        char *module_name  = NULL;

        char path[PATH_MAX];
        bzero(path, PATH_MAX);

        if (config.project.path == NULL) {

                current_path = getcwd(current_path, PATH_MAX);
                free_current = true;

                if (!current_path){
                        fprintf(stderr, "ERROR, failed to set start_path %s\n",
                                config.project.name);
                        goto error;
                }


        } else {
                current_path = config.project.path;
        }

        fprintf(stdout, "Starting project path search at %s\n", current_path);

        root_path = testrun_path_search_project_path(current_path);
        if (!root_path){
                fprintf(stderr, "ERROR, failed to find project root path %s\n",
                        config.project.name);
                goto error;
        }


        if (snprintf(path, PATH_MAX, "%s", root_path) < 0)
                goto error;

        project_name = basename(root_path);
        module_name  = config.project.name;
        config.project.name = project_name;

        fprintf(stdout, "Found project root path at %s\n", path);
        config.project.path = path;

        //testrun_config_dump(stdout, &config);

        if (!testrun_lib_create_module_files(module_name, &config))
                goto error;

        if (free_current)
                testrun_string_free(current_path);
        root_path    = testrun_string_free(root_path);
        return 0;
error:
        if (free_current)
                testrun_string_free(current_path);
        root_path    = testrun_string_free(root_path);
        return -1;
}

/*----------------------------------------------------------------------------*/

int testrun_project_app_run(int argc, char *argv[]){

        size_t size = 1000;
        char buffer[size];
        bzero(buffer, size);

        if (argc < 2)
                goto error;

        bool project = false;
        bool success = false;

        testrun_config config = testrun_project_app_create_dynamic_config(
                &success, &project, argc, argv);

        if (!success)
                goto done;

        if (!config.author){

                if (!testrun_project_app_set_author_dynamic(buffer, size))
                        goto error;

                config.author = buffer;
        }

        //testrun_config_dump(stdout, config);

        if (project)
                return testrun_project_app_create_new_project(config);
        else
                return testrun_project_app_create_new_module(config);

done:
        return 0;
error:
        return -1;
}
