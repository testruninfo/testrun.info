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
        @file           testrun_utils_test.c
        @author         Markus Toepfer
        @date           2018-07-09

        @ingroup        testrun_lib

        @brief          Unit tests for testrun_utils.


        ------------------------------------------------------------------------
*/

#include "../tools/testrun.h"
#include "../../src/testrun_utils.c"

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST HELPER                                                     #HELPER
 *
 *      ------------------------------------------------------------------------
 */

bool helper_read_git_user_name(char *buffer, size_t size){

        bool set = false;

        /*
         *      Open a pipe and read the git username. This SHOULD work,
         *      as this project is expected to be distibuted via git.
         */

        FILE *in;
        FILE *popen();

        if ((in = popen("git config user.name", "r")) != NULL){

                if (fgets(buffer, size, in) != NULL){
                        set = true;
                }
                pclose(in);
        }

        return set;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------*/

int test_testrun_utils_insert_at_each_line(){
        
        char *result = NULL;
        char *expect = NULL;
        char *text   = NULL;

        testrun(!testrun_utils_insert_at_each_line(NULL, NULL, NULL));

        // no line
        text   = "x";
        testrun(!testrun_utils_insert_at_each_line(NULL, NULL, NULL));

        // one line
        text   = "x\n";
        result = testrun_utils_insert_at_each_line(text, NULL, NULL);
        expect = "x\n";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line
        text   = "\nx";
        result = testrun_utils_insert_at_each_line(text, NULL, NULL);
        expect = "\nx";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (prefix)
        text   = "x\n";
        result = testrun_utils_insert_at_each_line(text, "pre", NULL);
        expect = "prex\n";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (prefix textlen0)
        text   = "\nx";
        result = testrun_utils_insert_at_each_line(text, "pre", NULL);
        expect = "\nx";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (suffix)
        text   = "x\n";
        result = testrun_utils_insert_at_each_line(text, NULL, "suffix");
        expect = "xsuffix\n";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (suffix (textlen0))
        text   = "\nx";
        result = testrun_utils_insert_at_each_line(text, NULL, "suffix");
        expect = "\nx";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (preffix, suffix)
        text   = "x\n";
        result = testrun_utils_insert_at_each_line(text, "preffix", "suffix");
        expect = "preffixxsuffix\n";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // one line (preffix,  suffix)
        text   = "\nx";
        result = testrun_utils_insert_at_each_line(text, "preffix", "suffix");
        expect = "\nx";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

        // multiple line (preffix)
        text   = "x\ny\nz\na";
        result = testrun_utils_insert_at_each_line(text, "_", NULL);
        expect = "_x\n_y\n_z\na";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);

         // multiple line (suffix)
        text   = "x\ny\nz\na";
        result = testrun_utils_insert_at_each_line(text, NULL, "_");
        expect = "x_\ny_\nz_\na";
        testrun(result);
        testrun(0 == strncmp(result, expect, strlen(expect)));
        free(result);



        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

int test_testrun_utils_path_is_project_top_dir() {

        char *bases[] = {
                "./build/tests/resources/test_top/ok",
                "./build/tests/resources/test_top",
                "./build/tests/resources/test_top/no_include",
                "./build/tests/resources/test_top/no_tests",
                "./build/tests/resources/test_top/files_not_dir"
        };

        char *folder = "level1";
        char path[PATH_MAX];

        for (int i = 0; i < 5; i++){

                if (i == 0){
                        testrun(testrun_utils_path_is_project_top_dir(bases[i]));
                } else {
                        testrun(!testrun_utils_path_is_project_top_dir(bases[i]));
                }

                /* No access to folder */
                testrun(0 == chmod(bases[i], 000));
                testrun(!testrun_utils_path_is_project_top_dir(bases[i]));
                testrun(0 == chmod(bases[i],
                        S_IRUSR | S_IWUSR | S_IXUSR |
                        S_IRGRP | S_IXGRP |
                        S_IROTH | S_IXOTH));

                /* recheck with access reset */
                if (i == 0) {
                        testrun(testrun_utils_path_is_project_top_dir(bases[i]));
                } else {
                        testrun(!testrun_utils_path_is_project_top_dir(bases[i]));
                }

                /* check with no access to a path within the folder */
                testrun(sprintf(path, "%s/%s", bases[i], folder) > 0);
                testrun(0 == chmod(path, 000));

                if (i == 0) {
                        testrun(testrun_utils_path_is_project_top_dir(bases[i]));
                } else {
                        testrun(!testrun_utils_path_is_project_top_dir(bases[i]));
                }

                testrun(0 == chmod(path,
                        S_IRUSR | S_IWUSR | S_IXUSR |
                        S_IRGRP | S_IXGRP |
                        S_IROTH | S_IXOTH));


        }

        testrun(testrun_utils_path_is_project_top_dir("."));

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_utils_search_project_path() {

        char *bases[] = {
                "./build/tests/resources/test_top/ok",
                "./build/tests/resources/test_top",
                "./build/tests/resources/test_top/no_include",
                "./build/tests/resources/test_top/no_tests",
                "./build/tests/resources/test_top/files_not_dir"
        };

        char path[PATH_MAX];

        char *level1    = "level1";
        char *level2    = "level2";
        char *level3    = "level3";

        char *result    = NULL;
        char *real      = NULL;

        for (int i = 0; i < 5; i++){


                testrun(sprintf(path, "%s", bases[i]) > 0);
                if (i == 0){
                        result = testrun_utils_search_project_path(path);
                        testrun(result);
                        real = realpath(bases[i], real);
                        testrun(strncmp(result,real, strlen(result)) == 0);
                        free(real);
                        real = NULL;
                        free(result);
                        result = NULL;

                } else {

                        /* @NOTE: this test will be run within the valid project
                         * **testrun_create_module**, so it is actually testing
                         * finding a top project from different folders */

                        real   = realpath(".", real);
                        result = testrun_utils_search_project_path(path);
                        testrun(strncmp(result,real, strlen(result)) == 0);
                        free(real);
                        real = NULL;
                        free(result);
                        result = NULL;
                }

                /* TEST AGAIN DEEPER NESTED */

                testrun(sprintf(path, "%s/%s/%s/%s",
                        bases[i], level1, level2, level3) > 0);

                if (i == 0){
                        result = testrun_utils_search_project_path(path);
                        testrun(result);
                        real = realpath(bases[i], real);
                        testrun(strncmp(result,real, strlen(result)) == 0);
                        free(real);
                        real = NULL;
                        free(result);
                        result = NULL;

                } else {

                        /* @NOTE: this test will be run within the valid project
                         * **testrun_create_module**, so it is actually testing
                         * finding a top project from different folders */

                        real   = realpath(".", real);
                        result = testrun_utils_search_project_path(path);
                        testrun(strncmp(result,real, strlen(result)) == 0);
                        free(real);
                        real = NULL;
                        free(result);
                        result = NULL;
                }

        }

        /* Check a folder outside of the testrun_project structure */
        testrun(!testrun_utils_search_project_path("/tmp"));
        testrun(!testrun_utils_search_project_path("/etc"));
        testrun(!testrun_utils_search_project_path("/home"));

        /* Check NULL input */
        testrun(!testrun_utils_search_project_path(NULL));

        /* Check project base input */

        result = testrun_utils_search_project_path(".");
        testrun(result);
        free(result);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_utils_create_path() {

        char path[PATH_MAX];
        bzero(path, PATH_MAX);

        strcat(path, "build/tests/");

        DIR *dp;

        testrun(testrun_utils_create_path(path), "path existing");
        dp = opendir(path);
        testrun(dp);
        (void) closedir (dp);

        path[8] = '\0';
        //                1234567890
        testrun(!opendir("build/te"));
        testrun(testrun_utils_create_path(path), "path not existing");
        dp = opendir("build/te");
        testrun(dp);
        (void) closedir (dp);
        testrun(rmdir("build/te") == 0);

        // TESTS_TMP_FILES = $(wildcard /tmp/test_*) (defined in makefile)

        snprintf(path, PATH_MAX, "/tmp/test_folder/1/2/3/4/.././5/");

        testrun(testrun_utils_create_path(path), "path not existing");
        dp = opendir("/tmp/test_folder/1/2/3/5/");
        testrun(dp);
        (void) closedir (dp);

        snprintf(path, PATH_MAX, "/tmp/test_folder/1/2/6");
        testrun(testrun_utils_create_path(path), "path not existing");
        dp = opendir("/tmp/test_folder/1/2/6/");
        testrun(dp);
        (void) closedir (dp);

        // check no access
        testrun(0 == chmod("/tmp/test_folder/", 000));
        snprintf(path, PATH_MAX, "/tmp/test_folder/1/2/7");
        testrun(!testrun_utils_create_path(path),
                "no access to path component");
        testrun(0 == chmod("/tmp/test_folder/",
                        S_IRUSR | S_IWUSR | S_IXUSR |
                        S_IRGRP | S_IXGRP |
                        S_IROTH | S_IXOTH));
        testrun(testrun_utils_create_path(path),
                "access to all path components");
        dp = opendir("/tmp/test_folder/1/2/7/");
        testrun(dp);
        (void) closedir (dp);

        // use of shell to delete the folder here (way easier to implement)
        testrun(system("rm -rf /tmp/test_folder") == 0);



        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_utils_create_relative_path() {


        char *root     = "build";
        char *relative = "tests";

        DIR *dp;

        testrun(testrun_utils_create_relative_path(root, relative), "path existing");
        dp = opendir("build/tests");
        testrun(dp);
        (void) closedir (dp);

        root     = "/tmp/test_folder/";
        relative = "abc";

        dp = opendir("/tmp/test_folder/abc");
        testrun(!dp);
        testrun(testrun_utils_create_relative_path(root, relative), "path not existing");
        dp = opendir("/tmp/test_folder/abc");
        testrun(dp);
        (void) closedir (dp);

        root     = "/tmp/test_folder/abc";
        relative = "sub";

        // check no access
        testrun(0 == chmod(root, 000));
        testrun(!testrun_utils_create_relative_path(root, relative),
                "no access to path component");
        testrun(0 == chmod(root,
                        S_IRUSR | S_IWUSR | S_IXUSR |
                        S_IRGRP | S_IXGRP |
                        S_IROTH | S_IXOTH));
        testrun(testrun_utils_create_relative_path(root, relative),
                "access to all path components");
        dp = opendir("/tmp/test_folder/abc/sub");
        testrun(dp);
        (void) closedir (dp);

        // use of shell to delete the folder here (way easier to implement)
        testrun(system("rm -rf /tmp/test_folder") == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_utils_create_project_paths() {

        struct testrun_config config = testrun_config_default();


        // check invalid config
        testrun(!testrun_config_validate(&config));
        testrun(!testrun_utils_create_project_paths(NULL));
        testrun(!testrun_utils_create_project_paths(&config));

        config.project.name = "abcd";
        config.project.path = "/tmp/test_folder";
        testrun(testrun_config_validate(&config));

        DIR *dp;

        dp = opendir("/tmp/test_folder");
        testrun(!dp);
   
        testrun(testrun_utils_create_project_paths(&config));
        dp = opendir("/tmp/test_folder");
        testrun(dp);
        (void) closedir (dp);

        // check root
        dp = opendir("/tmp/test_folder/abcd");
        testrun(dp);
        (void) closedir (dp);

        // check subfolders
        dp = opendir("/tmp/test_folder/abcd/include");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("/tmp/test_folder/abcd/src");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("/tmp/test_folder/abcd/tests");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("/tmp/test_folder/abcd/tests/tools");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("/tmp/test_folder/abcd/config");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("/tmp/test_folder/abcd/doxygen");
        testrun(dp);
        (void) closedir (dp);

        // use of shell to delete the folder here (way easier to implement)
        testrun(system("rm -rf /tmp/test_folder") == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_utils_get_git_author() {

        bool git_name = true;

        size_t size = 2000;
        char buffer[size];
        char expect[size];

        bzero(buffer, size);
        bzero(expect, size);

        git_name = helper_read_git_user_name(expect, size);

        testrun(!testrun_utils_get_git_author(NULL, 0));
        testrun(!testrun_utils_get_git_author(buffer, 0));
        testrun(!testrun_utils_get_git_author(NULL, size));

        testrun(testrun_utils_get_git_author(buffer, size));

        if (git_name){
                // GIT enabled
                testrun(strlen(buffer) == strlen(expect));
                testrun(strncmp(buffer, expect, strlen(expect)) == 0);

        } else {
                // GIT disabled
                testrun(strlen(buffer) == strlen(TESTRUN_TAG_AUTHOR));
                testrun(strncmp(buffer,
                        TESTRUN_TAG_AUTHOR,
                        strlen(TESTRUN_TAG_AUTHOR)) == 0);
        }

        // NOTE No username change testing done

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_utils_create_file() {

        char path[PATH_MAX];
        bzero(path, PATH_MAX);

        char reread[1000];
        bzero(reread, 1000);

        char *content           = "TESTCONTENT";
        char *name              = "filename";
        char *project_path      = "./build/test";
        char *relative_path     = "relative";

        FILE *file;

        testrun(snprintf(path, PATH_MAX, "%s/%s/%s", 
                project_path, relative_path, name));

        // ensure file is not set
        unlink(path);
        testrun( -1 == access(path, F_OK));

        // check positive
        testrun(testrun_utils_create_file(
                name, content, project_path, relative_path));

        testrun( -1 != access(path, F_OK), "failed to access tmp file");

        // check if file exists
        testrun(!testrun_utils_create_file(
                name, content, project_path, relative_path), "file exists");

        // reread the content
        file = fopen(path, "r");
        testrun(file);
        testrun(fgets(reread, 1000, file));
        fclose(file);

        testrun(strncmp(reread, content, strlen(content)) == 0);
        testrun(strlen(reread) == strlen(content));

        testrun(0 == unlink(path), "failed to delete tmp file");

        // check without relative path
        bzero(path, PATH_MAX);

        testrun(snprintf(path, PATH_MAX, "%s/%s", 
                project_path, name));

        testrun( -1 == access(path, F_OK));

        // check positive
        testrun(testrun_utils_create_file(
                name, content, project_path, NULL));

        testrun( -1 != access(path, F_OK), "failed to access tmp file");

        // check if file exists
        testrun(!testrun_utils_create_file(
                name, content, project_path, NULL), "file exists");

        testrun(0 == unlink(path), "failed to delete tmp file");

        project_path      = "./build/test";
        relative_path     = "x/y/z";

        bzero(path, PATH_MAX);
        testrun(snprintf(path, PATH_MAX, "%s/%s/%s", 
                project_path, relative_path, name));

        testrun(testrun_utils_create_file(
                name, content, project_path, relative_path), "file exists");

        testrun(0 == unlink(path), "failed to delete tmp file");

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_utils_chmod() {

        char path[PATH_MAX];
        bzero(path, PATH_MAX);

        char *content           = "TESTCONTENT";
        char *name              = "filename";
        char *project_path      = "./build/test";
        char *relative_path     = "relative";

        struct stat stats;

        testrun(snprintf(path, PATH_MAX, "%s/%s/%s", 
                project_path, relative_path, name));

        testrun( -1 == access(path, F_OK));

        // check not existing
        testrun(!testrun_utils_chmod(name, project_path, relative_path, 0555));

        // check positive
        testrun(testrun_utils_create_file(
                name, content, project_path, relative_path));

        testrun(0 == stat(path, &stats));
        testrun(S_IRUSR & stats.st_mode);
        testrun(S_IWUSR & stats.st_mode);
        testrun(!(S_IXUSR & stats.st_mode));

        testrun(S_IRGRP & stats.st_mode);
        testrun(S_IWGRP & stats.st_mode);
        testrun(!(S_IXGRP & stats.st_mode));

        testrun(S_IROTH & stats.st_mode);
        testrun(!(S_IWOTH & stats.st_mode));
        testrun(!(S_IXOTH & stats.st_mode));

        testrun(testrun_utils_chmod(name, project_path, relative_path, 0555));
        testrun(0 == stat(path, &stats));

        testrun((S_IRUSR & stats.st_mode));
        testrun(!(S_IWUSR & stats.st_mode));
        testrun((S_IXUSR & stats.st_mode));

        testrun((S_IRGRP & stats.st_mode));
        testrun(!(S_IWGRP & stats.st_mode));
        testrun((S_IXGRP & stats.st_mode));

        testrun((S_IROTH & stats.st_mode));
        testrun(!(S_IWOTH & stats.st_mode));
        testrun((S_IXOTH & stats.st_mode));

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_utils_generate_project_root_path() {

        char buffer[PATH_MAX];
        memset(buffer, 0, PATH_MAX);

        struct testrun_config config = testrun_config_default();
        config.project.name = "test";
        config.project.path = ".";

        char *expect = "./test";

        testrun(!testrun_utils_generate_project_root_path(NULL,    NULL,   0));
        testrun(!testrun_utils_generate_project_root_path(&config, buffer, 0));
        testrun(!testrun_utils_generate_project_root_path(&config, NULL,   PATH_MAX));
        testrun(!testrun_utils_generate_project_root_path(NULL,    buffer, PATH_MAX));

        testrun( testrun_utils_generate_project_root_path(&config, buffer, PATH_MAX));
        testrun(0 == strncmp(buffer, expect, strlen(expect)));

        config.project.name = NULL;
        testrun(!testrun_utils_generate_project_root_path(&config, buffer, PATH_MAX));

        config.project.name = "NAME";
        config.project.path = NULL;
        testrun(!testrun_utils_generate_project_root_path(&config, buffer, PATH_MAX));

        config.project.path = "PATH";
        expect = "PATH/NAME";
        testrun(testrun_utils_generate_project_root_path(&config, buffer, PATH_MAX));
        testrun(0 == strncmp(buffer, expect, strlen(expect)));

        return testrun_log_success();
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                    #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int all_tests() {

        testrun_init();
        testrun_test(test_testrun_utils_insert_at_each_line);

        testrun_test(test_testrun_utils_generate_project_root_path);

        testrun_test(test_testrun_utils_path_is_project_top_dir);
        testrun_test(test_testrun_utils_search_project_path);
        testrun_test(test_testrun_utils_create_path);
        testrun_test(test_testrun_utils_create_relative_path);
        testrun_test(test_testrun_utils_create_project_paths);

        testrun_test(test_testrun_utils_get_git_author);
        testrun_test(test_testrun_utils_create_file);
        testrun_test(test_testrun_utils_chmod);
        
        return testrun_counter;
}

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST EXECUTION                                                  #EXEC
 *
 *      ------------------------------------------------------------------------
 */

testrun_run(all_tests);

