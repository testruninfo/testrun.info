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
        @file           testrun_config_test.c
        @author         Markus Toepfer
        @date           2018-07-12

        @ingroup        testrun_lib

        @brief          Unit tests for testrun_config.


        ------------------------------------------------------------------------
*/

#include "../tools/testrun.h"
#include "../../src/testrun_config.c"

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST CASES                                                      #CASES
 *
 *      ------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------*/

int test_testrun_config_validate(){
        
        testrun_config config = testrun_config_default();
        testrun(!testrun_config_validate(&config));

        config.project.name = "name";
        testrun(!testrun_config_validate(&config));

        config.project.path = "something";
        testrun(testrun_config_validate(&config));

        config.copyright.copyright = testrun_copyright_gpl_version_3();
        testrun(testrun_config_validate(&config));

        config.copyright.copyright = testrun_copyright_reserved();
        testrun(testrun_config_validate(&config));

        config.project.name = NULL;
        testrun(!testrun_config_validate(&config));
        config.project.name = "something";
        testrun(testrun_config_validate(&config));


        config.test_prefix = NULL;
        testrun(!testrun_config_validate(&config));
        config.test_prefix = "something";
        testrun(testrun_config_validate(&config));

        config.test_suffix_source = NULL;
        testrun(!testrun_config_validate(&config))
        config.test_suffix_source = "something";
        testrun(testrun_config_validate(&config));

        config.test_suffix_exec = NULL;
        testrun(!testrun_config_validate(&config));
        config.test_suffix_exec = "something";
        testrun(testrun_config_validate(&config));

        config.project.search_project_path = NULL;
        testrun(!testrun_config_validate(&config));
        config.project.search_project_path = testrun_utils_search_project_path;
        testrun(testrun_config_validate(&config));

        config.path.bin = NULL;
        testrun(!testrun_config_validate(&config));
        config.path.bin = "something";
        testrun(testrun_config_validate(&config));

        config.path.build = NULL;
        testrun(!testrun_config_validate(&config));
        config.path.build = "something";
        testrun(testrun_config_validate(&config));

        config.path.include = NULL;
        testrun(!testrun_config_validate(&config));
        config.path.include = "something";
        testrun(testrun_config_validate(&config));

        config.path.source = NULL;
        testrun(!testrun_config_validate(&config));
        config.path.source = "something";
        testrun(testrun_config_validate(&config));

        config.path.tests = NULL;
        testrun(!testrun_config_validate(&config));
        config.path.tests = "something";
        testrun(testrun_config_validate(&config));

        config.path.tools = NULL;
        testrun(!testrun_config_validate(&config));
        config.path.tools = "something";
        testrun(testrun_config_validate(&config));

        config.path.service = NULL;
        testrun(!testrun_config_validate(&config));
        config.path.service = "something";
        testrun(testrun_config_validate(&config));

        config.path.doxygen = NULL;
        testrun(!testrun_config_validate(&config));
        config.path.doxygen = "something";
        testrun(testrun_config_validate(&config));

        config.script.lines_of_code = NULL;
        testrun(!testrun_config_validate(&config));
        config.script.lines_of_code = "something";
        testrun(testrun_config_validate(&config));

        config.script.test_coverage = NULL;
        testrun(!testrun_config_validate(&config));
        config.script.test_coverage = "something";
        testrun(testrun_config_validate(&config));

        config.script.test_unit = NULL;
        testrun(!testrun_config_validate(&config));
        config.script.test_unit = "something";
        testrun(testrun_config_validate(&config));

        config.script.test_acceptance = NULL;
        testrun(!testrun_config_validate(&config));
        config.script.test_acceptance = "something";
        testrun(testrun_config_validate(&config));

        config.script.test_runner = NULL;
        testrun(!testrun_config_validate(&config));
        config.script.test_runner = "something";
        testrun(testrun_config_validate(&config));

        config.script.test_gcov = NULL;
        testrun(!testrun_config_validate(&config));
        config.script.test_gcov = "something";
        testrun(testrun_config_validate(&config));

        config.script.test_gprof = NULL;
        testrun(!testrun_config_validate(&config));
        config.script.test_gprof = "something";
        testrun(testrun_config_validate(&config));

        config.makefile.configurable = NULL;
        testrun(!testrun_config_validate(&config));
        config.makefile.configurable = "something";
        testrun(testrun_config_validate(&config));

        config.makefile.common = NULL;
        testrun(!testrun_config_validate(&config));
        config.makefile.common = "something";
        testrun(testrun_config_validate(&config));

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_config_default(){
        
        testrun_config config = testrun_config_default();
        
        testrun(0 == strncmp(config.test_prefix, 
                TEST_PREFIX, strlen(TEST_PREFIX)));
        testrun(0 == strncmp(config.test_suffix_source, 
                TEST_SUFFIX_SOURCE, strlen(TEST_SUFFIX_SOURCE)));
        testrun(0 == strncmp(config.test_suffix_exec, 
                TEST_SUFFIX_EXEC, strlen(TEST_SUFFIX_EXEC)));

        testrun(NULL == config.project.path);
        testrun(NULL == config.project.name);
        testrun(NULL != config.project.url);
        testrun(NULL != config.project.description);
        testrun(config.project.search_project_path == 
                testrun_utils_search_project_path);

        testrun(0 == strncmp(config.path.bin, 
                TESTRUN_FOLDER_BIN, strlen(TESTRUN_FOLDER_BIN)));
        testrun(0 == strncmp(config.path.build, 
                TESTRUN_FOLDER_BUILD, strlen(TESTRUN_FOLDER_BUILD)));
        testrun(0 == strncmp(config.path.include, 
                TESTRUN_FOLDER_INCLUDE, strlen(TESTRUN_FOLDER_INCLUDE)));
        testrun(0 == strncmp(config.path.source, 
                TESTRUN_FOLDER_SOURCE, strlen(TESTRUN_FOLDER_SOURCE)));
        testrun(0 == strncmp(config.path.tests, 
                TESTRUN_FOLDER_TESTS, strlen(TESTRUN_FOLDER_TESTS)));
        testrun(0 == strncmp(config.path.tools, 
                TESTRUN_FOLDER_TOOLS, strlen(TESTRUN_FOLDER_TOOLS)));
        testrun(0 == strncmp(config.path.service, 
                TESTRUN_FOLDER_SERVICE, strlen(TESTRUN_FOLDER_SERVICE)));
        testrun(0 == strncmp(config.path.doxygen, 
                TESTRUN_FOLDER_DOXYGEN, strlen(TESTRUN_FOLDER_DOXYGEN)));

        testrun(0 == strncmp(config.script.lines_of_code,
                TESTRUN_SCRIPT_LOC, strlen(TESTRUN_SCRIPT_LOC)));
        testrun(0 == strncmp(config.script.test_coverage,
                TESTRUN_SCRIPT_COVERAGE, strlen(TESTRUN_SCRIPT_COVERAGE)));
        testrun(0 == strncmp(config.script.test_unit,
                TESTRUN_SCRIPT_UNIT, strlen(TESTRUN_SCRIPT_UNIT)));
        testrun(0 == strncmp(config.script.test_acceptance,
                TESTRUN_SCRIPT_ACCEPTANCE, strlen(TESTRUN_SCRIPT_ACCEPTANCE)));
        testrun(0 == strncmp(config.script.test_runner,
                TESTRUN_SCRIPT_RUNNER, strlen(TESTRUN_SCRIPT_RUNNER)));
        testrun(0 == strncmp(config.script.test_gcov,
                TESTRUN_SCRIPT_GCOV, strlen(TESTRUN_SCRIPT_GCOV)));
        testrun(0 == strncmp(config.script.test_gprof,
                TESTRUN_SCRIPT_GPORF, strlen(TESTRUN_SCRIPT_GPORF)));

        testrun(0 == strncmp(config.makefile.configurable,
                TESTRUN_MAKEFILE, strlen(TESTRUN_MAKEFILE)));
        testrun(0 == strncmp(config.makefile.common,
                TESTRUN_MAKEFILE_COMMON, strlen(TESTRUN_MAKEFILE_COMMON)));
        testrun(LIB == config.makefile.target);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_config_print_usage(){

        testrun_log("TEST OUTPUT FOR PRINT USAGE FOLLOWS.");

        // check stdout 
        testrun_config_print_usage(NULL);

        testrun_log("------------------------------------");

        // check stdout 
        testrun_config_print_usage("app_name");

        testrun_log("TEST OUTPUT FOR PRINT USAGE DONE.");


        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_config_read_user_input(){

        testrun_config config = { 0 };

        bool success = false;
        bool project = false;

        char *args[100] = { 0 };

        args[0] = "program_name";
        args[1] = "test";

        config = testrun_config_read_user_input(NULL, &success, &project, 2, args);
        testrun(success);
        testrun(!project);
        testrun( 0 == strncmp(config.project.name, "test", strlen("test")));

        // not enough args 
        config = testrun_config_read_user_input(NULL, &success, &project, 1, args);
        testrun(!success);
        testrun(!project);
        testrun( 0 == config.project.name );

        // TBD check argument inputs (Howto check for -d et all?)
        // TBD How to test the input loop with getopt?
        // TBD NOTE TEST INCOMPLETE

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
        testrun_test(test_testrun_config_validate);
        testrun_test(test_testrun_config_default);

        testrun_test(test_testrun_config_print_usage);
        testrun_test(test_testrun_config_read_user_input);


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

