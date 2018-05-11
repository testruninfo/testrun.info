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
 *      @file           testrun_project_app_tests.c
 *      @author         Markus Toepfer
 *      @date           2017-11-08
 *
 *      @ingroup
 *
 *      @brief
 *
 *
 *      ------------------------------------------------------------------------
 **/


/*******************************************************************************
 *
 *      TEST SETUP
 *
 ******************************************************************************/

#include "../tools/testrun.h"
#include "../../src/testrun_project_app.c"
#include <sys/stat.h>

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

/*----------------------------------------------------------------------------*/

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------*/


int test_testrun_project_app_set_author_dynamic(){

        bool git_name = true;

        size_t size = 2000;
        char buffer[size];
        char expect[size];

        bzero(buffer, size);
        bzero(expect, size);

        git_name = helper_read_git_user_name(expect, size);

        testrun(!testrun_project_app_set_author_dynamic(NULL, 0));
        testrun(!testrun_project_app_set_author_dynamic(buffer, 0));
        testrun(!testrun_project_app_set_author_dynamic(NULL, size));

        testrun(testrun_project_app_set_author_dynamic(buffer, size));

        if (git_name){
                // GIT enabled
                testrun(strlen(buffer) == strlen(expect));
                testrun(strncmp(buffer, expect, strlen(expect)) == 0);

        } else {
                // GIT disabled
                testrun(strlen(buffer) == strlen(TESTRUN_TAG_DEFAULT_AUTHOR));
                testrun(strncmp(buffer,
                        TESTRUN_TAG_DEFAULT_AUTHOR,
                        strlen(TESTRUN_TAG_DEFAULT_AUTHOR)) == 0);
        }

        // NOTE No username change testing done

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_project_app_print_usage(){

        testrun_project_app_print_usage();
        testrun_log("Check logfile for usage dump");

        // NOTE manual check needed

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_project_app_create_new_project(){

        testrun_config config = testrun_config_default();

        DIR *dp;

        testrun(system("rm -rf ./build/tests/xyz") == 0);

        config.project.name = "xyz";
        config.project.path = "./build/tests";

        testrun(!opendir("./build/tests/xyz"));
        testrun(testrun_project_app_create_new_project(config) == 0);

        dp = opendir("./build/tests/xyz");
        testrun(dp);
        (void) closedir (dp);

        dp = opendir("./build/tests/xyz/include");
        testrun(dp);
        (void) closedir (dp);

        dp = opendir("./build/tests/xyz/tests/unit");
        testrun(dp);
        (void) closedir (dp);

        testrun(system("rm -rf ./build/tests/xyz") == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_project_app_create_new_module(){

        testrun_config config = testrun_config_default();

        DIR *dp;

        testrun(system("rm -rf ./build/tests/xyz") == 0);

        config.project.name = "xyz";
        config.project.path = "./build/tests";

        testrun(!opendir("./build/tests/xyz"));
        testrun(testrun_project_app_create_new_project(config) == 0);

        dp = opendir("./build/tests/xyz");
        testrun(dp);
        (void) closedir (dp);

        dp = opendir("./build/tests/xyz/include");
        testrun(dp);
        (void) closedir (dp);

        dp = opendir("./build/tests/xyz/tests/unit");
        testrun(dp);
        (void) closedir (dp);

        // create a module in xyz (recursive upwards search)

        config.project.name = "abc_module";
        config.project.path = "./build/tests/xyz/tests/unit";

        testrun( -1 == access(
                "./build/tests/xyz/src/abc_module.c",
                F_OK));
        testrun( -1 == access(
                "./build/tests/xyz/tests/unit/abc_module_tests.c",
                F_OK));
        testrun( -1 == access(
                "./build/tests/xyz/include/abc_module.h",
                F_OK));

        testrun(testrun_project_app_create_new_module(config) == 0);

        testrun( -1 != access(
                "./build/tests/xyz/src/abc_module.c",
                F_OK));
        testrun( -1 != access(
                "./build/tests/xyz/tests/unit/abc_module_tests.c",
                F_OK));
        testrun( -1 != access(
                "./build/tests/xyz/include/abc_module.h",
                F_OK));

        // module exists
        testrun(testrun_project_app_create_new_module(config) < 0);

        config.project.name = "123_module";
        config.project.path = "./build/tests/xyz";

        testrun( -1 == access(
                "./build/tests/xyz/src/123_module.c",
                F_OK));
        testrun( -1 == access(
                "./build/tests/xyz/tests/unit/123_module_tests.c",
                F_OK));
        testrun( -1 ==access(
                "./build/tests/xyz/include/123_module.h",
                F_OK));

        testrun(testrun_project_app_create_new_module(config) == 0);

        testrun( -1 != access(
                "./build/tests/xyz/src/123_module.c",
                F_OK));
        testrun( -1 != access(
                "./build/tests/xyz/tests/unit/123_module_tests.c",
                F_OK));
        testrun( -1 != access(
                "./build/tests/xyz/include/123_module.h",
                F_OK));

        testrun(system("rm -rf ./build/tests/xyz") == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CLUSTER                                                    #CLUSTER
 *
 *      ------------------------------------------------------------------------
 */

int all_tests() {

        testrun_init();

        testrun_test(test_testrun_project_app_set_author_dynamic);
        testrun_test(test_testrun_project_app_print_usage);

        testrun_test(test_testrun_project_app_create_new_project);
        testrun_test(test_testrun_project_app_create_new_module);

        /*
         *      To test the last 2 functions some investigation for
         *      argv parameter parsing need to be done.
         *
         *      TBD Testing postponed
         *      (low priority functionality tested over tool usage)
         */

        //testrun_test(test_testrun_project_app_create_dynamic_config);
        //testrun_test(test_testrun_project_app_run);

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

