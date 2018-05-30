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

        @file           testrun_lib.c
        @author         Markus Toepfer
        @date           2017-11-29

        @ingroup        testrun_lib

        @brief          Generic lib related functions.


        ------------------------------------------------------------------------
*/

#include "../include/testrun_lib.h"

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_path(
        char *buffer, size_t size,
        testrun_config const * const config,
        char *relative){

        if (!buffer || !config || !relative || size < 1)
                return false;

        if (snprintf(buffer, size, "%s/%s",
                config->project.path, relative) < 0)
                return false;

        if (!testrun_path_create(buffer, PATH_MAX)){
                log_error("Failed to create %s", buffer);
                return false;
        }

        log_info("Project %s ... created PATH %s", config->project.name, buffer);

        return true;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_file(
        char *filename,
        char *content,
        testrun_config const * const config,
        char *path_relative){

        if (!filename || !config || !content)
                return false;

        FILE *file;

        int r;

        char path_absolute[PATH_MAX];
        bzero(path_absolute, PATH_MAX);

        if (path_relative) {

                if (snprintf(path_absolute, PATH_MAX, "%s/%s/%s",
                        config->project.path, path_relative, filename) < 0)
                        return false;
        } else {

                if (snprintf(path_absolute, PATH_MAX, "%s/%s",
                        config->project.path, filename) < 0)
                        return false;
        }

        if (access(path_absolute, F_OK) != -1){
                log_error("File exists %s\n", path_absolute);
                return false;
        }

        file = fopen(path_absolute, "w");
        if (!file) {
                log_error("Could not create/open file %s\n", path_absolute);
                return false;
        }

        r = fputs(content, file);
        if (r < 0)
                return false;

        fclose(file);

        if (0 == strncmp(filename, "install.sh", strlen("install.sh")))
                if (chmod(path_absolute, 0555))
                        return false;

        if (0 == strncmp(filename, "uninstall.sh", strlen("uninstall.sh")))
                if (chmod(path_absolute, 0555))
                        return false;

        log_info("Wrote file %s\n", path_absolute);

        return true;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_file_content_function(
        char *filename,
        testrun_config const * const config,
        char *path_relative,
        char *(*function_content)(struct testrun_config)){

        if (!filename || !config || !function_content)
                return false;

        bool r = false;

        char *content = function_content(*config);
        if (!content)
                return false;

        r = testrun_lib_create_file(filename, content, config, path_relative);
        content = testrun_string_free(content);

        return r;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_folder_structure(
        testrun_config const * const config){

        if (!config)
                return false;

        // PROJECT ROOT
        if (!testrun_path_create(config->project.path, PATH_MAX))
                return false;

        char path[PATH_MAX];
        bzero(path, PATH_MAX);


        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "copyright"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "config"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "config/install"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "config/data"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "docs"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "doxygen"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "include"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "src"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "tests"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "tests/tools"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "tests/unit"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "tests/acceptance"))
                return false;

        if (!testrun_lib_create_path(path, PATH_MAX, config,
                "tests/resources"))
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_config_files(
        testrun_config const * const config){

        if (!config)
                return false;

        char *content = NULL;

        char path[PATH_MAX];
        bzero(path, PATH_MAX);

        content = testrun_lib_script_service_install_content(*config);

        if (!content)
                goto error;

        if (!testrun_lib_create_file(
                "install.sh",
                content, config,
                "config/install"))
                goto error;

        content = testrun_string_free(content);
        content = testrun_lib_script_service_uninstall_content(*config);

        if (!content)
                goto error;

        if (!testrun_lib_create_file(
                "uninstall.sh",
                content, config,
                "config/install"))
                goto error;

        content = testrun_string_free(content);
        content = testrun_text_block_service_file(config);

        if (!content)
                goto error;

        if (snprintf(path, PATH_MAX, "%s.service",
                config->project.name) < 0)
                goto error;

        if (!testrun_lib_create_file(
                path, content, config,
                "config/install"))
                goto error;

        content = testrun_string_free(content);
        content = testrun_text_block_socket_file(config);

        if (!content)
                goto error;

        if (snprintf(path, PATH_MAX, "%s.socket",
                config->project.name) < 0)
                goto error;

        if (!testrun_lib_create_file(path, content, config,
                "config/install"))
                goto error;

        content = testrun_string_free(content);

        content = "THIS FOLDER WILL BE COPIED TO /etc/config";
        if (!testrun_lib_create_file(
                "readme",
                content, config,
                "config/data"))
                goto error;

        return true;
error:
        testrun_string_free(content);
        return false;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_copyright_files(
        testrun_config const * const config){

        if (!config)
                return false;

        char *content  = NULL;

        char path[PATH_MAX];
        bzero(path, PATH_MAX);

        // COPYRIGHT
        content = testrun_copyright_to_string(&config->copyright);
        if (!content)
                goto error;

        if (!testrun_lib_create_file(
                TESTRUN_FILE_COPYRIGHT,
                content, config,
                "copyright"))
                goto error;

        content = testrun_string_free(content);

        if (config->copyright.full_text != NULL){

                content = config->copyright.full_text();
                if (!content)
                        goto error;

                if (!testrun_lib_create_file(
                        config->copyright.filename, content, config,
                        "copyright"))
                        goto error;

                content = testrun_string_free(content);

        }

        return true;
error:
        testrun_string_free(content);
        return false;
}

/*----------------------------------------------------------------------------*/

static bool testrun_lib_create_files(
        testrun_config const * const config,
        char const * const readme){

        if (!config)
                return false;

        char path[PATH_MAX];
        bzero(path, PATH_MAX);

        char *content = NULL;

        // MAKEFILES
        content = testrun_lib_makefile_content(*config);
        if (!content)
                goto error;

        if (!testrun_lib_create_file(TESTRUN_FILE_MAKEFILE,
                content, config, NULL))
                goto error;

        content = testrun_string_free(content);
        content = testrun_lib_makefile_general_content(*config);

        if (!content)
                goto error;

        if (!testrun_lib_create_file(TESTRUN_FILE_MAKEFILE_MAIN,
                content, config, NULL))
                goto error;

        content = testrun_string_free(content);

        // gitignore
        content = testrun_text_block_gitignore_file(config);
        if (!content)
                goto error;

        if (!testrun_lib_create_file(".gitignore",
                content, config, NULL))
                goto error;

        content = testrun_string_free(content);

        // README
        if (readme) {

                if (!testrun_lib_create_file(TESTRUN_FILE_README,
                        (char*) readme, config, NULL))
                        goto error;

        } else {

                content = testrun_text_block_readme(config, NULL, NULL, NULL);
                if (!content)
                        goto error;

                if (!testrun_lib_create_file(TESTRUN_FILE_README,
                        content, config, NULL))
                        goto error;

        }

        content = testrun_string_free(content);

        // CHANGELOG
        content = testrun_text_block_changelog_file(config);
        if (!content)
                goto error;

        if (!testrun_lib_create_file(
                "CHANGELOG", content, config,
                "docs"))
                goto error;

        content = testrun_string_free(content);

        // DOXYGEN
        content = testrun_text_block_doxygen_config(config);
        if (!content)
                goto error;

        if (!testrun_lib_create_file(
                TESTRUN_FILE_DOXYGEN,
                content, config,
                "doxygen"))
                goto error;

        content = testrun_string_free(content);

        if (!testrun_lib_create_copyright_files(config))
                goto error;

        if (!testrun_lib_create_config_files(config))
                goto error;

        if (!testrun_lib_create_test_tools(config))
                goto error;

        if (!testrun_lib_create_module_files(config->project.name, config))
                goto error;

        return true;
error:
        content = testrun_string_free(content);
        return false;
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_project(
        testrun_config const * const config,
        char const * const readme){

        if (!config)
                return false;

        if (!testrun_lib_create_folder_structure(config)){
                log_error("Failed to create a project folder structure for: %s",
                        config->project.name);
        }

        if (!testrun_lib_create_files(config, readme)){
                log_error("Failed to create the files for project: %s",
                        config->project.name);
        }

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_test_tools(
        testrun_config const * const config){

        if (!config)
                return false;

        char *content = NULL;

        // TESTRUN.H
        content = testrun_lib_testrun_header_content();
        if (!content)
                return false;

        if (!testrun_lib_create_file(
                TESTRUN_FILE_TESTRUN_HEADER,
                content, config, "tests/tools")){
                content = testrun_string_free(content);
                return false;
        }

        content = testrun_string_free(content);

        // RUNNER
        if (!testrun_lib_create_file_content_function(
                TESTRUN_SCRIPT_RUNNER,
                config,
                "tests/tools",
                testrun_lib_script_folder_runner_content))
                return false;

        // ACCEPTANCE
        if (!testrun_lib_create_file_content_function(
                TESTRUN_SCRIPT_ACCEPTANCE,
                config,
                "tests/tools",
                testrun_lib_script_acceptance_tests_content))
                return false;


        // UNIT
        if (!testrun_lib_create_file_content_function(
                TESTRUN_SCRIPT_UNIT,
                config,
                "tests/tools",
                testrun_lib_script_unit_tests_content))
                return false;


        // COVERAGE
        if (!testrun_lib_create_file_content_function(
                TESTRUN_SCRIPT_COVERAGE,
                config,
                "tests/tools",
                testrun_lib_script_coverage_tests_content))
                return false;

        // LOC
        if (!testrun_lib_create_file_content_function(
                TESTRUN_SCRIPT_LOC,
                config,
                "tests/tools",
                testrun_lib_script_loc_tests_content))
                return false;

        // GCOV
        if (!testrun_lib_create_file_content_function(
                TESTRUN_SCRIPT_GCOV,
                config,
                "tests/tools",
                testrun_lib_script_gcov_tests_content))
                return false;

        // GPROF
        if (!testrun_lib_create_file_content_function(
                TESTRUN_SCRIPT_GPROF,
                config,
                "tests/tools",
                testrun_lib_script_gprof_tests_content))
                return false;

        // initialize folders (e.g. for GIT checkout)
        content = "USE THIS FOLDER FOR UNIT TESTS";
        if (!testrun_lib_create_file(TESTRUN_FILE_README,
                content, config,
                "tests/unit"))
                return false;

        content = "USE THIS FOLDER FOR ACCEPTANCE TESTS";
        if (!testrun_lib_create_file(TESTRUN_FILE_README,
                content, config,
                "tests/acceptance"))
                return false;

        content = "USE THIS FOLDER FOR TEST RESOURCES";
        if (!testrun_lib_create_file(TESTRUN_FILE_README,
                content, config,
                "tests/resources"))
                return false;

        return true;
}

/*----------------------------------------------------------------------------*/

bool testrun_lib_create_module_files(
        char const * const name,
        testrun_config const * const config){

        if (!name || !config)
                return false;

        char filename[PATH_MAX];
        bzero(filename, PATH_MAX);

        char *content = NULL;

        // MODULE HEADER
        if (snprintf(filename, PATH_MAX, "%s.h", name) < 0)
                goto error;

        content = testrun_lib_c_file_content(name, TESTRUN_HEADER, config);
        if (!content)
                goto error;

        if (!testrun_lib_create_file(
                filename, content, config,
                "include"))
                goto error;

        content = testrun_string_free(content);

        // MODULE SOURCE
        if (snprintf(filename, PATH_MAX, "%s.c", name) < 0)
                goto error;

        content = testrun_lib_c_file_content(name, TESTRUN_SOURCE, config);
        if (!content)
                goto error;

        if (!testrun_lib_create_file(filename, content, config,
                "src"))
                goto error;

        content = testrun_string_free(content);

        // MODULE UNIT TEST
        if (snprintf(filename, PATH_MAX, "%s_tests.c", name) < 0)
                goto error;

        content = testrun_lib_c_file_content(name, TESTRUN_TEST, config);
        if (!content)
                goto error;

        if (!testrun_lib_create_file(filename, content, config,
                "tests/unit"))
                goto error;

        content = testrun_string_free(content);

        return true;
error:
        content = testrun_string_free(content);
        return false;
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_testrun_header_content(){

        char    *result = NULL;
        size_t  re_size = 0;

        char    *step1  = NULL;
        char    *step2  = NULL;
        size_t  size1   = 0;
        size_t  size2   = 0;

        size_t size = 10000;
        char buffer[size];
        bzero(buffer, size);

        char *name   = "testrun";
        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author,
                "This file is part of the testrun project. http://testrun.info");

        testrun_config config   = testrun_config_default();
        config.project.name     = name;
        config.copyright        = copyright;
        config.author           = author;

        char *line = testrun_text_block_splitline(0, 81, true);

        char *head = testrun_copyright_default_c_header(
                        &copyright, true);

        char *docu = testrun_text_block_c_header_documentation(
                        name, TESTRUN_HEADER, &config, false,
                        "Simple serial test execution framework.");

        char *lineend = "\n";
        size_t le_len = strlen(lineend);

        /*
         *      SXLINE    = lineend
         *      STRIPLINE = SXLINE stripline SXLINE
         */
        if (snprintf(buffer, size,
                "%s%sSXLINE"
                "#ifndef testrun_hSXLINE"
                "#define testrun_hSXLINE"
                "SXLINE"
                "#include <unistd.h>     /* C89/C90 */SXLINE"
                "#include <stdlib.h>     /* C89/C90 */SXLINE"
                "#include <stdio.h>      /* C89/C90 */SXLINE"
                "#include <string.h>     /* C89/C90 */SXLINE"
                "#include <errno.h>      /* C89/C90 */SXLINE"
                "#include <time.h>       /* C89/C90 */SXLINE"
                "STRIPLINE"
                "#define testrun_errno() \\SXLINE"
                "        (errno == 0 ? \"NONE\" :  strerror(errno))SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Log a failure. Failure: Inability to perform a function as expected.SXLINE"
                "*/SXLINE"
                "#define testrun_log_failure(msg, ...) \\SXLINE"
                "        fprintf(stderr, \"\\t[FAIL]\\t%%s line:%%d errno:%%s message: \" msg \"\\n\",\\SXLINE"
                "               __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Log an error. Error: Difference between expected and actual result.SXLINE"
                "*/SXLINE"
                "#define testrun_log_error(msg, ...) \\SXLINE"
                "        fprintf(stderr, \"\\t[ERROR]\\t%%s line:%%d errno:%%s message: \" msg \"\\n\",\\SXLINE"
                "        __FUNCTION__, __LINE__, testrun_errno(), ##__VA_ARGS__)SXLINE"
                "STRIPLINE"
                "#define testrun_log_success(msg, ...) \\SXLINE"
                "        fprintf(stdout, \"\\t[OK] \\t%%s \" msg \"\\n\", __FUNCTION__, ##__VA_ARGS__)SXLINE"
                "STRIPLINE"
                "#define testrun_log(msg, ...) \\SXLINE"
                "        fprintf(stdout, \"\\t\" msg \"\\n\", ##__VA_ARGS__)SXLINE"
                "STRIPLINE"
                "#define testrun_log_function_info(msg, ...) \\SXLINE"
                "        fprintf(stdout, \"\\t[INFO] \\t%%s line:%%d message: \" msg \"\\n\", \\SXLINE"
                "                __FUNCTION__, __LINE__, ##__VA_ARGS__)SXLINE"
                "STRIPLINE"
                "#define testrun_log_clock(start, end) \\SXLINE"
                "        fprintf(stdout, \"\\tClock ticks function: ( %%s ) | %%f s | %%.0f ms \\n\", \\SXLINE"
                "        __func__, \\SXLINE"
                "        ((double)(end - start)) / CLOCKS_PER_SEC, \\SXLINE"
                "        (((double)(end - start)) / CLOCKS_PER_SEC ) * 1000)SXLINE"
                "STRIPLINE"
                "#define testrun_init()  \\SXLINE"
                "        int result = 0; \\SXLINE"
                "        int testrun_counter = 0;SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Run a single atomar test. Return the surrounding block on error.SXLINE"
                "        This function will leave the context block running on error. TheSXLINE"
                "        Mindset is a defused assert. LEAVE THE FUNCTION NOT THE PROGRAM.SXLINE"
                "SXLINE"
                "        @param test     boolean decision input.SXLINE"
                "        @returns        the calling function on error with -1SXLINE"
                "*/SXLINE"
                "#define testrun_check(test, ... )\\SXLINE"
                "        if (!(test)) { testrun_log_error(__VA_ARGS__);  return -1;}SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Alias to @see testrun_check.SXLINE"
                "*/SXLINE"
                "#define testrun(test, ...)\\SXLINE"
                "        testrun_check(test, __VA_ARGS__ )SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Run a single test (execute a function pointer. Runs a test function.SXLINE"
                "        On non negative return value of the function run, a testrun_counterSXLINE"
                "        is increased, on negative result, the negative result will be returned.SXLINE"
                "SXLINE"
                "        @param test     function pointer of the test to runSXLINE"
                "        @NOTE           The surrounding block is left on negative result of theSXLINE"
                "                        function pointer execution.SXLINE"
                "*/SXLINE"
                "#define testrun_test(test)\\SXLINE"
                "        result = test(); testrun_counter++; if (result < 0) return result;SXLINE"
                "STRIPLINE"
                "/**SXLINE"
                "        Runs a function pointer, which SHALL contain the test function pointersSXLINE"
                "        to run. The function pointer is wrapped in a main procedure, which andSXLINE"
                "        allows indepentent testruns of the input testcluster over externalSXLINE"
                "        execution.SXLINE"
                "SXLINE"
                "        A clock will be started, as soon as the main is executed and the theSXLINE"
                "        time is stopped again, at the end of the execution. The differenceSXLINE"
                "        will be printed and is the runtime of the whole input testcluster.SXLINE"
                "SXLINE"
                "        A run will fail, as soon as one of the tests in the testcluster fails.SXLINE"
                "        (Fail on first) or will run all functions dependent on the testsetup.SXLINE"
                "SXLINE"
                "        @param testcluster      function pointer to be executed.SXLINE"
                "*/SXLINE"
                "#define testrun_run(testcluster) int main(int argc, char *argv[]) {\\SXLINE"
                "        argc = argc;\\SXLINE"
                "        clock_t start1_t, end1_t; \\SXLINE"
                "        start1_t = clock(); \\SXLINE"
                "        testrun_log(\"\\ntestrun\\t%%s\", argv[0]);\\SXLINE"
                "        int64_t result = testcluster();\\SXLINE"
                "        if (result > 0) \\SXLINE"
                "                testrun_log(\"ALL TESTS RUN (%%jd tests)\", result);\\SXLINE"
                "        end1_t = clock(); \\SXLINE"
                "        testrun_log_clock(start1_t, end1_t); \\SXLINE"
                "        testrun_log("");\\SXLINE"
                "        result >=0 ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE); \\SXLINE"
                "}SXLINE"
                "SXLINE"
                "/*--------------- For EXAMPLE code check http://testrun.info -----------------*/"
                "SXLINE"
                "SXLINE"
                "#endif /* testrun_h */"
                ,head, docu) < 0)
                        goto error;


        char old[1000];
        char new[1000];
        bzero(old, 1000);
        bzero(new, 1000);

        if (snprintf(old, 1000, "STRIPLINE") < 0)
                goto error;

        if (snprintf(new, 1000, "SXLINE%sSXLINESXLINE", line) < 0)
                goto error;

        // insert the stripline
        if (!testrun_string_replace_all(&step1, &size1,
                buffer, size,
                old, strlen(old),
                new, strlen(new),
                false))
                goto error;

        // insert the newlines
        if (snprintf(old, 1000, "SXLINE") < 0)
                goto error;

        if (snprintf(new, 1000, "%s", lineend) < 0)
                goto error;

        if (!testrun_string_replace_all(&step2, &size2,
                step1, size1,
                old, strlen(old),
                new, strlen(new),
                false))
                goto error;

        if (!testrun_string_clear_whitespace_before_lineend(&result, &re_size,
                step2, size2,
                lineend, le_len))
                goto error;

        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        line  = testrun_string_free(line);
        head  = testrun_string_free(head);
        docu  = testrun_string_free(docu);

        return result;
error:
        line  = testrun_string_free(line);
        head  = testrun_string_free(head);
        docu  = testrun_string_free(docu);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        return NULL;
}


/*----------------------------------------------------------------------------*/

char *testrun_lib_script_folder_runner_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = TESTRUN_SCRIPT_RUNNER;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        snprintf(description, d_size,
        "Run each test.test of a folder and log Ok or NOK\n"
        TESTRUN_TAG_OFFSET"for each executed testfile of the folder.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"EXAMPLE OUTPUT\n"
        "#\n"
        TESTRUN_TAG_OFFSET"[OK]  1/5 filename1.test\n"
        TESTRUN_TAG_OFFSET"[NOK] 2/5 filename2.test\n"
        "#\n"
        TESTRUN_TAG_OFFSET"MODES\n"
        "#\n"
        TESTRUN_TAG_OFFSET"(1) RUN ALL TESTS (log but ignore errors)\n"
        TESTRUN_TAG_OFFSET"    use script with 2 parameters\n"
        TESTRUN_TAG_OFFSET"    e.g. ./%s logfile /path\n"
        "#\n"
        TESTRUN_TAG_OFFSET"    This mode will not return a test failure and\n"
        TESTRUN_TAG_OFFSET"    may be used to run all tests and return success\n"
        TESTRUN_TAG_OFFSET"    if all tests was run. (test results are logged)\n"
        "#\n"
        TESTRUN_TAG_OFFSET"(2) FAIL ON ERROR (Fail on first error)\n"
        TESTRUN_TAG_OFFSET"    use script with 3 parameters\n"
        TESTRUN_TAG_OFFSET"    e.g. ./%s logfile /path 1\n"
        "#\n"
        TESTRUN_TAG_OFFSET"    This mode returns -1 on the first test failure.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"PARAMETER\n"
        "#\n"
        TESTRUN_TAG_OFFSET"(1) path to logfile destination\n"
        TESTRUN_TAG_OFFSET"(2) path to folder with test cases\n"
        "#"
        ,config.project.name, config.project.name);

        snprintf(usage, d_size,
                "./%s /path/to/logfile /path/to/test/dir",
                config.project.name);

        snprintf(dependencies, d_size, "bash, tail, ls, grep, wc");


        snprintf(content, c_size,
        "\n"
        "#       ------------------------------------------------------------------------\n"
        "#       CONFIGURE SCRIPT BASED ON PRESET VARIBALES OR ON INPUT\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "if [ -z $1 ]; then\n"
        "        echo \"ERROR ... NO LOGFILE INPUT TO SRCIPT\"\n"
        "        exit 1\n"
        "fi\n"
        "LOGFILE=$1\n"
        "\n"
        "if [ -z $2 ]; then\n"
        "        echo \"ERROR ... FOLDER INPUT TO SRCIPT\"\n"
        "        exit 1\n"
        "fi\n"
        "FOLDER=$2\n"
        "\n"
        "FAIL_ON_ERROR=0\n"
        "if [ ! -z $3 ]; then\n"
        "        FAIL_ON_ERROR=1  \n"
        "fi\n"
        "\n"
        "if [ ! -w $LOGFILE ]; then\n"
        "        echo \"ERROR ... LOGFILE NOT WRITABLE\"\n"
        "        exit 1\n"
        "fi\n"
        "\n"
        "#       ------------------------------------------------------------------------\n"
        "#       PERFORM TESTRUN\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "FILES=`ls  $FOLDER/ | grep \"\\.test\" | wc -l`\n"
        "if [ $? -ne 0 ]; then\n"
        "        echo \"ERROR ... could not count files of $FOLDER\"\n"
        "        exit 1\n"
        "fi\n"
        "c=0\n"
        "\n"
        "if [ $FILES -eq 0 ]; then\n"
        "        exit 0\n"
        "fi\n"
        "\n"
        "for i in $FOLDER/*.test\n"
        "do\n"
        "        c=$((c+1))\n"
        "\n"
        "        # RUN EXECUTABLE\n"
        "        $i 2>&1 >> $LOGFILE\n"
        "\n"
        "        # CHECK RETURN OF EXECUTABLE\n"
        "        if [ $? -ne 0 ]; then\n"
        "\n"
        "                echo \"NOK\\t(\"$c\"/\"$FILES\")\\t\"$i\n"
        "\n"
        "                if [ $FAIL_ON_ERROR -eq 1 ]; then\n"
        "                        exit 1\n"
        "                fi\n"
        "        else\n"
        "                echo \"OK\\t(\"$c\"/\"$FILES\")\\t\"$i\n"
        "        fi\n"
        "done\n"
        "exit 0\n");

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_script_acceptance_tests_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = TESTRUN_SCRIPT_ACCEPTANCE;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        char testrunner[PATH_MAX];
        char buffer[PATH_MAX];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(testrunner,       PATH_MAX);
        bzero(buffer,           PATH_MAX);

        if (snprintf(testrunner, PATH_MAX, "tests/tools/%s",
                TESTRUN_SCRIPT_RUNNER) < 0)
                return NULL;

        snprintf(description, d_size,
        "Run all test executables build/test/acceptance/*.test\n"
        TESTRUN_TAG_OFFSET"Run the whole folder, until an error occurs.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"MODE         FAIL ON ERROR (Fail on first test error)\n"
        "#\n"
        TESTRUN_TAG_OFFSET"LOGFILE      build/test/log/acceptancetest.<time>.log\n"
        "#"
        );

        snprintf(usage, d_size,
                "./testrun_simple_acceptance_tests.sh /path/to/project");

        snprintf(dependencies, d_size, "bash, touch, chmod, ls, wc, date");


        snprintf(content, c_size,
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE ACCEPTANCE TESTING\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"build/tests/log/acceptance_\".$start_time.\"log\"\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "\n"
        "# SET THE FOLDER\n"
        "FOLDER=\"build/tests/acceptance\"\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               REPORT ACCEPTANCE TESTING\"                >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "# RUN THE RUNNER\n"
        "sh %s  $LOGFILE $FOLDER FAIL_ON_ERROR\n"
        "RESULT=$?\n"
        "\n"
        "end_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# FINISH THE REPORT\n"
        "echo \"-------------------------------------------------------\">> $LOGFILE\n"
        "echo \"DONE \\t ACCEPTANCE TEST RUN\"  >> $LOGFILE\n"
        "if [ $RESULT -eq 0 ]; then\n"
        "        echo \"RESULT\\t SUCCESS\"  >> $LOGFILE\n"
        "else\n"
        "        echo \"RESULT\\t FAILURE\"  >> $LOGFILE\n"
        "fi\n"
        "echo \"START \\t $start_time\" >> $LOGFILE\n"
        "echo \"END   \\t $end_time\" >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\">> $LOGFILE\n"
        "\n"
        "# DUMP THE REPORT\n"
        "cat $LOGFILE\n"
        "echo \"\"\n"
        "exit $RESULT\n"
        ,
        testrunner);

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_script_unit_tests_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = TESTRUN_SCRIPT_UNIT;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        char testrunner[PATH_MAX];
        char buffer[PATH_MAX];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(testrunner,       PATH_MAX);
        bzero(buffer,           PATH_MAX);

        if (snprintf(testrunner, PATH_MAX, "tests/tools/%s",
                TESTRUN_SCRIPT_RUNNER) < 0)
                return NULL;

        snprintf(description, d_size,
        "Run all test executables build/test/unit/*.test\n"
        TESTRUN_TAG_OFFSET"Run the whole folder, until an error occurs.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"MODE         FAIL ON ERROR (Fail on first test error)\n"
        "#\n"
        TESTRUN_TAG_OFFSET"LOGFILE      build/test/log/unittest.<time>.log\n"
        "#"
        );

        snprintf(usage, d_size,
                "./testrun_simple_unit_tests.sh /path/to/project");

        snprintf(dependencies, d_size, "bash, touch, chmod, ls, wc, date");


        snprintf(content, c_size,
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE UNIT TESTING\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"build/tests/log/unit_\".$start_time.\"log\"\n"
        "echo \" (log)   $start_time\" > $LOGFILE\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "\n"
        "# SET THE FOLDER\n"
        "FOLDER=\"build/tests/unit\"\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               REPORT UNIT TESTING\"                      >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "\n"
        "# RUN THE RUNNER\n"
        "sh %s  $LOGFILE $FOLDER FAIL_ON_ERROR\n"
        "RESULT=$?\n"
        "\n"
        "end_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S.%%N\")\n"
        "\n"
        "# FINISH THE REPORT\n"
        "echo \"-------------------------------------------------------\">> $LOGFILE\n"
        "echo \"DONE \\t UNIT TEST RUN\"  >> $LOGFILE\n"
        "if [ $RESULT -eq 0 ]; then\n"
        "        echo \"RESULT\\t SUCCESS\"  >> $LOGFILE\n"
        "else\n"
        "        echo \"RESULT\\t FAILURE\"  >> $LOGFILE\n"
        "fi\n"
        "echo \"START \\t $start_time\" >> $LOGFILE\n"
        "echo \"END   \\t $end_time\" >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\">> $LOGFILE\n"
        "\n"
        "# DUMP THE REPORT\n"
        "cat $LOGFILE\n"
        "echo \"\"\n"
        "exit $RESULT\n"
        ,
        testrunner);

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}


/*----------------------------------------------------------------------------*/

char *testrun_lib_script_coverage_tests_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = TESTRUN_SCRIPT_COVERAGE;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        char path_src[PATH_MAX];
        char path_unit[PATH_MAX];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(path_src,         PATH_MAX);
        bzero(path_unit,        PATH_MAX);

        snprintf(description, d_size,
        "Count functions of folder src against their counterparts\n"
        TESTRUN_TAG_OFFSET"in the unit test folder.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"CONVENTION\n"
        "#\n"
        TESTRUN_TAG_OFFSET"Each function in any file of the source folder src\n"
        TESTRUN_TAG_OFFSET"will have a corresponding test function prefixed\n"
        TESTRUN_TAG_OFFSET"with \"test_\" in any unit test file.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"EXAMPLE      function | test_function\n"
        "#\n"
        TESTRUN_TAG_OFFSET"NOTE         This simple coverage test just covers the\n"
        TESTRUN_TAG_OFFSET"             observance of the given coding convention.\n"
        "#");

        snprintf(usage, d_size,
                "./%s /path/to/project",
                TESTRUN_SCRIPT_COVERAGE);

        snprintf(dependencies, d_size, "bash, ctags, awk, sed, grep");


        snprintf(content, c_size,
        "\n"
        "start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S\")\n"
        "PREFIX=\"test_\"\n"
        "\n"
        "LIBDIR=$1\n"
        "SRCDIR=$1/src\n"
        "TESTDIR=$1/tests\n"
        "\n"
        "# SET A LOGFILE\n"
        "LOGFILE=\"$LIBDIR/build/tests/log/coverage_\".$start_time.\"log\"\n"
        "touch $LOGFILE\n"
        "chmod a+w $LOGFILE\n"
        "\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"               REPORT COVERAGE TESTING\"                  >> $LOGFILE\n"
        "echo \"-------------------------------------------------------\" >> $LOGFILE\n"
        "echo \"   TIME \\t $start_time\" >> $LOGFILE\n"
        "echo \"\" >> $LOGFILE\n"
        "\n"
        "# GENERATE CTAGS SOURCE\n"
        "cd $SRCDIR\n"
        "if [ $? -ne 0 ]; then\n"
        "        exit 1\n"
        "fi\n"
        "ctags --c-types=f -R\n"
        "# remove the ctags stuff, to leave just the function lines\n"
        "sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions\n"
        "# remove anything but the function names\n"
        "awk '{print $1 }' $SRCDIR/functions > \\\n"
        "        $SRCDIR/functionNames\n"
        "# count the lines\n"
        "sourceFkt=\"$(cat functions | wc -l)\"\n"
        "echo \"   count source\\t\" $sourceFkt >> $LOGFILE\n"
        "\n"
        "# GENERATE CTAGS TESTS\n"
        "cd $TESTDIR\n"
        "if [ $? -ne 0 ]; then\n"
        "        exit 1\n"
        "fi\n"
        "ctags --c-types=f -R\n"
        "# remove the ctags stuff, to leave just the function lines\n"
        "sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions\n"
        "# remove anything but the function names\n"
        "awk '{print $1 }' $TESTDIR/functions > \\\n"
        "        $TESTDIR/functionNames\n"
        "# count the lines\n"
        "testFkt=\"$(cat functions | grep -i ^$PREFIX | wc -l)\"\n"
        "echo \"   count tests\\t\" $testFkt >> $LOGFILE\n"
        "\n"
        "echo \"\\nUNTESTED: \" >> $LOGFILE\n"
        "# Found functions:\n"
        "while read line;\n"
        "do\n"
        "        grep -n '^test_'$line'$' $TESTDIR/functionNames > \\\n"
        "        /dev/null || echo $line >> $LOGFILE\n"
        "done < $SRCDIR/functionNames\n"
        "\n"
        "if [ $sourceFkt != 0 ]; then\n"
        "        echo \"............................................\"  >> $LOGFILE\n"
        "        echo \"COVERAGE: $sourceFkt $testFkt\" | \\\n"
        "        awk '{ printf $1 \" %%.2f %%%% \\n\", $3/$2*100}' >> $LOGFILE\n"
        "fi\n"
        "\n"
        "cat $LOGFILE\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"\"\n"
        "\n"
        "# cleanup remove the files we created\n"
        "rm $SRCDIR/tags\n"
        "rm $SRCDIR/functions\n"
        "rm $SRCDIR/functionNames\n"
        "rm $TESTDIR/tags\n"
        "rm $TESTDIR/functions\n"
        "rm $TESTDIR/functionNames\n");

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_script_loc_tests_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = TESTRUN_SCRIPT_LOC;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        snprintf(description, d_size,
        "Count the lines of src and unit | acceptance tests.\n"
        TESTRUN_TAG_OFFSET"This file uses no error checking.");

        snprintf(usage, d_size,
                "./%s /path/to/project",
                TESTRUN_SCRIPT_LOC);

        snprintf(dependencies, d_size, "bash, find, xargs, wc");

        snprintf(content, c_size,
        "echo \"-------------------------------------------------------\"\n"
        "echo \"               SIMPLE LOC COUNTER\"\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"(LOC) src\"\n"
        "find $1/src -name '*.c' | xargs wc -l\n"
        "echo \"(LOC) tests/unit\"\n"
        "find $1/tests/unit -name '*.c' | xargs wc -l\n"
        "echo \"(LOC) tests/acceptance\"\n"
        "find $1/tests/acceptance -name '*.c' | xargs wc -l\n"
        "echo \"-------------------------------------------------------\"\n"
        "echo \"\"\n");

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_script_gcov_tests_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = TESTRUN_SCRIPT_GCOV;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        char path_src[PATH_MAX];
        char path_unit[PATH_MAX];
        char path_acceptance[PATH_MAX];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(path_src,         PATH_MAX);
        bzero(path_unit,        PATH_MAX);
        bzero(path_acceptance,  PATH_MAX);

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(path_src,         PATH_MAX);
        bzero(path_unit,        PATH_MAX);

        snprintf(description, d_size,
        "Run gcov based coverage tests on all test cases.");

        snprintf(usage, d_size,"./testrun_gcov.sh /path/to/project");

        snprintf(dependencies, d_size, "bash, gcov");

        snprintf(content, c_size,
"start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S\")\n"
"\n"
"LOGFOLDER=\"build/tests/log\"\n"
"\n"
"# SET A LOGFILE\n"
"LOGFILE=$LOGFOLDER\"/gcov_\".$start_time.\"log\"\n"
"echo \" (log)   $start_time\" > $LOGFILE\n"
"touch $LOGFILE\n"
"chmod a+w $LOGFILE\n"
"\n"
"# SET THE FOLDER\n"
"FOLDER=\"build/tests/unit\"\n"
"SRC_FOLDER=\"tests/unit\"\n"
"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"               GCOV RUNNER\"\n"
"echo \"-------------------------------------------------------\"\n"
"\n"
"# RUN THE RUNNER\n"
"#sh ./tests/./tools/testrun_runner.sh  $LOGFILE $FOLDER FAIL_ON_ERROR\n"
"#RESULT=$?\n"
"\n"
"for test in $FOLDER/*.test; do\n"
"    $test\n"
"done\n"
"\n"
"FILES=`ls  $FOLDER/ | grep \".test\" | wc -l`\n"
"if [ $? -ne 0 ]; then\n"
"        echo \"ERROR ... could not count files of $FOLDER\"\n"
"        exit 1\n"
"fi\n"
"c=0\n"
"\n"
"if [ $FILES -eq 0 ]; then\n"
"        exit 0\n"
"fi\n"
"\n"
"for i in $SRC_FOLDER/*_tests.c\n"
"do\n"
"        # RUN GCOV\n"
"        echo $i\n"
"        gcov $i\n"
"done\n"
"\n"
"# move coverage to build/tests/logs\n"
"mv *.gcov $LOGFOLDER\n"
"exit 0\n"
"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"\"\n"

                );

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/

char *testrun_lib_script_gprof_tests_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.project.name    = TESTRUN_SCRIPT_GPROF;
        config.copyright       = copyright;
        config.author          = author;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        char path_src[PATH_MAX];
        char path_unit[PATH_MAX];
        char path_acceptance[PATH_MAX];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(path_src,         PATH_MAX);
        bzero(path_unit,        PATH_MAX);
        bzero(path_acceptance,  PATH_MAX);

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);
        bzero(path_src,         PATH_MAX);
        bzero(path_unit,        PATH_MAX);

        snprintf(description, d_size,
        "Run gprof based analysis tests on all test cases.");

        snprintf(usage, d_size,
                "./testrun_gprof.sh /path/to/project");

        snprintf(dependencies, d_size, "bash, gprof");

        snprintf(content, c_size,
"start_time=$(date \"+%%Y.%%m.%%d-%%H.%%M.%%S\")\n"
"\n"
"LOGFOLDER=\"build/tests/log\"\n"
"\n"
"# SET A LOGFILE\n"
"LOGFILE=$LOGFOLDER\"/gprof_\".$start_time.\"log\"\n"
"echo \" (log)   $start_time\" > $LOGFILE\n"
"touch $LOGFILE\n"
"chmod a+w $LOGFILE\n"
"\n"
"# SET THE FOLDER\n"
"FOLDER=\"build/tests/unit\"\n"
"SRC_FOLDER=\"tests/unit\"\n"
"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"               GPROF RUNNER\"\n"
"echo \"-------------------------------------------------------\"\n"
"\n"
"# Execute the test once and profile the execution\n"
"for test in $FOLDER/*.test; do\n"
"        name=${test##*/}\n"
"        echo \"Profiling\" $name\n"
"    $test\n"
"    gprof $test gmon.out > $name.profile\n"
"done\n"
"\n"
"# move profile to build/tests/logs\n"
"mv *.profile $LOGFOLDER\n"
"exit 0\n"
"\n"
"echo \"-------------------------------------------------------\"\n"
"echo \"\"\n");

        return testrun_text_block_script(&config,
                description, usage, dependencies, content);
}
/*----------------------------------------------------------------------------*/

char * testrun_lib_c_file_content(
        char const * const module_name,
        testrun_extension_t filetype,
        struct testrun_config const * const config){

        if (!config)
                return false;

        if (!module_name)
                return false;

        char *body = NULL;

        char *head = testrun_copyright_default_c_header(
                        &config->copyright, true);

        char *docu = testrun_text_block_c_header_documentation(
                        module_name,
                        filetype,
                        config, false, NULL);

        char source[PATH_MAX];
        bzero(source, PATH_MAX);

        char header[PATH_MAX];
        bzero(header, PATH_MAX);

        if (!head || !docu)
                return NULL;

        switch(filetype) {

                case TESTRUN_HEADER:

                        body = testrun_text_block_header_body(
                                module_name);
                        break;

                case TESTRUN_SOURCE:

                        if (snprintf(source, PATH_MAX,
                                "../include/%s.h", module_name) < 0)
                                goto error;

                        body = testrun_text_block_source_body(
                                source);

                        break;

                case TESTRUN_TEST:

                        if (snprintf(source, PATH_MAX,
                                "../../src/%s.c", module_name) < 0)
                                goto error;

                        if (snprintf(header, PATH_MAX,
                                "../tools/testrun.h") < 0)
                                goto error;

                        body = testrun_text_block_test_body(
                                source, header);

                        break;

                default:
                        goto error;
        }

        if (!body)
                goto error;

        char    *result = NULL;
        size_t  size    = strlen(head) + strlen(docu) + strlen(body) + 2;

        result = calloc(size, sizeof(char));
        if (!result)
                goto error;

        if (!snprintf(result, size, "%s%s%s\n", head, docu, body))
                goto error;

        head = testrun_string_free(head);
        docu = testrun_string_free(docu);
        body = testrun_string_free(body);
        return result;
error:
        head = testrun_string_free(head);
        docu = testrun_string_free(docu);
        body = testrun_string_free(body);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_makefile_content(testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.copyright       = copyright;
        config.author          = author;

        char *step1 = NULL;
        char *step2 = NULL;
        char *step3 = NULL;
        char *wrong = "#!/usr/bin/env bash";
        char *line  = testrun_text_block_splitline(8, 81, false);
        line[0]     = '#';

        size_t size1 = 0;
        size_t size2 = 0;
        size_t size3 = 0;

        size_t c_size = 10000;
        size_t d_size = 5000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);


        snprintf(description, d_size,
        "This makefile defines project specific parameter.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"These parameter are:\n"
        TESTRUN_TAG_OFFSET"(1) used compiler and special flags\n"
        TESTRUN_TAG_OFFSET"(2) name and version\n"
        TESTRUN_TAG_OFFSET"(3) installation prefix\n"
        TESTRUN_TAG_OFFSET"(4) used libraries\n"
        TESTRUN_TAG_OFFSET"(5) general makefiles used"
        );

        snprintf(usage, d_size, "make");
        snprintf(dependencies, d_size, "make & compiler");

        snprintf(content, c_size,
        "CC = gcc\n"
        "\n"
        "PROJECT         := %s\n"
        "VERSION         := 0.0.1\n"
        "\n"
        "# project path recalculation (if used included from parent make)\n"
        "PROJECTMK       := $(abspath $(lastword $(MAKEFILE_LIST)))\n"
        "\n"
        "# base directory for installation\n"
        "PREFIX          := /usr/local\n"
        "\n"
        "# LIBS USED (uncommented example includes)\n"
        "#LIBS           += `pkg-config --libs libsystemd`\n"
        "#LIBS           += `pkg-config --libs uuid`\n"
        "#LIBS           += `pkg-config --libs openssl`\n"
        "\n"
        "# EXTRA CFLAGS (example)\n"
        "MODCFLAGS       += -std=gnu11\n"
        "MODCFLAGS       += -rdynamic\n"
        "#MODCFLAGS      += -fopenmp\n"
        "\n"
        "# EXTRA LFLAGS (example)\n"
        "#MODLFLAGS      += -pthread\n"
        "\n"
        "#TMP FILE DEFINITION\n"
        "TESTS_TMP_FILES = $(wildcard /tmp/test_*)\n"
        "\n"
        "# INCLUDE BASE MAKEFILE\n"
        "include %s\n"
        ,config.project.name,
        TESTRUN_FILE_MAKEFILE_MAIN);

        step1 = testrun_text_block_script(&config,
                description, usage, dependencies, content);

        if (!step1)
                goto error;

        size1 = strlen(step1);

        if (!testrun_string_replace_first(
                &step2, &size2,
                step1,  size1,
                wrong, strlen(wrong),
                line, strlen(line)))
                goto error;

        /*
         *      NOTE The next part replaces File [PROJECT], which is set
         *      based on the configuraion with File makefile.
         *      This will only work, as long as the indent used in
         *      shell headers is fixed 8 spaces.
         *      (20171201: Currently not configuable and fixed to 8 spaces)
         *
         *      Using this strategy we generate something like:
         *
         *      #       File            makefile
         *      #       Authors         Markus Toepfer
         *      #       Date            2017-12-01
         *      #
         *      #       Project         [PROJECT]
         *
         *      Project is set correct everywhere in the makefile,
         *      based on the configuration. File is overriden with
         *      correct filename.
         */

        snprintf(description, d_size, "%s\n#       Authors",
                config.project.name);

        snprintf(usage, d_size, "makefile\n#       Authors");

        if (!testrun_string_replace_first(
                &step3, &size3,
                step2,  size2,
                description, strlen(description),
                usage, strlen(usage)))
                goto error;

        line  = testrun_string_free(line);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        return step3;
error:
        line  = testrun_string_free(line);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        step3 = testrun_string_free(step3);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_makefile_general_content(testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        config.copyright       = copyright;
        config.author          = author;

        char *step1 = NULL;
        char *step2 = NULL;
        char *step3 = NULL;
        char *wrong = "#!/usr/bin/env bash";
        char *line  = testrun_text_block_splitline(8, 81, false);
        line[0]     = '#';

        size_t size1 = 0;
        size_t size2 = 0;
        size_t size3 = 0;

        size_t c_size = 50000;
        size_t d_size = 10000;

        char content[c_size];
        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        bzero(content,          c_size);
        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        char *all_target = NULL;
        char *install    = NULL;
        char *uninstall  = NULL;

        switch (config.project.type){

                case TESTRUN_LIB:
                        all_target = "all_lib";
                        install    = "install_lib";
                        uninstall  = "uninstall_lib";
                        break;
                case TESTRUN_EXEC:
                        all_target = "all_exec";
                        install    = "install_exec";
                        uninstall  = "uninstall_exec";
                        break;
                case TESTRUN_SERVICE:
                        all_target = "all_exec";
                        install    = "install_service";
                        uninstall  = "uninstall_service";
                        break;
                default:
                        goto error;
        }


        snprintf(description, d_size,
        "Generic makefile for testrun based projects.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"Target of this makefile is an independent library\n"
        TESTRUN_TAG_OFFSET"or executable to be installed at either PREFIX/lib\n"
        TESTRUN_TAG_OFFSET"or PREFIX/bin.\n"
        TESTRUN_TAG_OFFSET"\n"
        TESTRUN_TAG_OFFSET"The TESTING part contains all required functionality\n"
        TESTRUN_TAG_OFFSET"to use the testrun tools via a makefile. It may be\n"
        TESTRUN_TAG_OFFSET"seen as a makefile integrated testrunner framework.\n"
        TESTRUN_TAG_OFFSET"\n"
        TESTRUN_TAG_OFFSET"in particular:\n"
        TESTRUN_TAG_OFFSET"\n"
        TESTRUN_TAG_OFFSET"    \"make clean && make tested\"\n"
        TESTRUN_TAG_OFFSET"\n"
        TESTRUN_TAG_OFFSET"may be used to build all sources as well as tests from\n"
        TESTRUN_TAG_OFFSET"scratch and perform an integrated testrun over all after\n"
        TESTRUN_TAG_OFFSET"compilation.\n"
        TESTRUN_TAG_OFFSET"\n"
        TESTRUN_TAG_OFFSET"    \"make testrun-gcov\"\n"
        TESTRUN_TAG_OFFSET"\n"
        TESTRUN_TAG_OFFSET"may be used to rebuild the whole project with gcov\n"
        TESTRUN_TAG_OFFSET"coverage testing flag enabled.\n"
        TESTRUN_TAG_OFFSET"\n"
        TESTRUN_TAG_OFFSET"    \"make testrun-gprof\"\n"
        TESTRUN_TAG_OFFSET"\n"
        TESTRUN_TAG_OFFSET"may be used to rebuild the whole project with gprof\n"
        TESTRUN_TAG_OFFSET"profiling flags enabled.\n"
        TESTRUN_TAG_OFFSET"\n"
        TESTRUN_TAG_OFFSET"Following folder structure is required\n"
        TESTRUN_TAG_OFFSET"\n"
        TESTRUN_TAG_OFFSET"    sources MUST be located at src/\n"
        TESTRUN_TAG_OFFSET"    inludes MUST be located at include/\n"
        TESTRUN_TAG_OFFSET"    tests   MUST be located at tests/\n"
        TESTRUN_TAG_OFFSET"    build   MUST be located at build/\n"
        TESTRUN_TAG_OFFSET"\n"
        TESTRUN_TAG_OFFSET"ALL TEST SCRIPTS MAY BE EXCHANGED WITH CUSTOM RUNNERS"
        );

        snprintf(usage, d_size, "SHOULD be used included by parent makefile");
        snprintf(dependencies, d_size, "testrun (makefile & service scripts), doxygen (if used)");

        snprintf(content, c_size,
"# Switch on colors\n"
"GCC_COLORS ?= 'gcc colors available, use them!'\n"
"export GCC_COLORS\n"
"\n"
"# ----- Compiler flags -----------------------------------------------------\n"
"\n"
"CFLAGS\t\t\t= -Wall -Wextra -fPIC -Iinclude\n"
"\n"
"CFLAGS\t\t\t+= $(MODCFLAGS)\n"
"LFLAGS\t\t\t+= $(MODLFLAGS)\n"
"\n"
"# ----- Project path calculation (if used included) ------------------------\n"
"\n"
"PROJECTPATH\t\t:= $(abspath $(dir $(PROJECTMK)))\n"
"DIRNAME\t\t:= $(notdir $(patsubst %%/,%%,$(dir $(PROJECTMK))))\n"
"\n"
"# ----- Package config setup -----------------------------------------------\n"
"\n"
"LIBNAME\t\t\t:= lib$(DIRNAME)\n"
"LIBNAMEPC\t\t:= $(LIBNAME).pc\n"
"\n"
"INCDIR\t\t\t:= $(PREFIX)/include/$(DIRNAME)\n"
"LIBDIR\t\t\t:= $(PREFIX)/lib\n"
"EXECDIR\t\t\t:= $(PREFIX)/bin\n"
"\n"
"# ----- TARGETS ------------------------------------------------------------\n"
"\n"
"INSTALL\t\t\t:= install\n"
"\n"
"EXECUTABLE\t\t= bin/$(DIRNAME)\n"
"\n"
"STATIC\t\t\t= build/lib$(DIRNAME).a\n"
"SHARED\t\t\t= $(patsubst %%.a,%%.so,$(STATIC))\n"
"\n"
"# Source and object files to compile\n"
"HEADERS\t\t\t= $(wildcard include/*.h)\n"
"SOURCES\t\t\t= $(wildcard src/**/*.c src/*.c)\n"
"OBJECTS\t\t\t= $(patsubst %%.c,%%.o,$(SOURCES))\n"
"\n"
"# Test sources and targets\n"
"TESTS_SOURCES\t= $(wildcard tests/**/*_tests.c tests/*_tests.c)\n"
"TESTS_TARGET\t= $(patsubst tests/%%.c, build/tests/%%.test, $(TESTS_SOURCES))\n"
"\n"
"# GCOV support\n"
"GCOV_FILES\t\t= $(patsubst  %%.c,%%.gcno,$(SOURCES))\n"
"GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcov,$(SOURCES))\n"
"GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcda,$(SOURCES))\n"
"GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcno,$(TESTS_SOURCES))\n"
"GCOV_FILES\t\t+= $(patsubst %%.c,%%.gcda,$(TESTS_SOURCES))\n"
"\n"
"ifdef USE_GCOV\n"
"CFLAGS += -fprofile-arcs -ftest-coverage\n"
"LFLAGS += -lgcov --coverage\n"
"endif\n"
"\n"
"ifdef USE_GPROF\n"
"CFLAGS += -pg\n"
"endif\n"
"\n"
"# ----- DEFAULT MAKE RULES -------------------------------------------------\n"
"\n"
"%%.o : %%.c $(HEADERS)\n"
"\t@echo \" (CC)    $@\"\n"
"\t@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)\n"
"\n"
"%%_tests.o : %%_tests.c\n"
"\t@echo \" (CC)    $@\"\n"
"\t@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)\n"
"\n"
"all:\t\t\t%s\n"
"install:\t\t%s\n"
"uninstall:\t\t%s\n"
"\n"
"all_lib:\t\tstart lib tests pkgconfig done\n"
"all_exec:\t\tstart lib tests $(EXECUTABLE) done\n"
"all_service:\tall_exec\n"
"\n"
"lib:\t\t\tbuild sources\n"
"sources:\t\tbuild $(STATIC) $(SHARED)\n"
"tests:\t\t\ttests-resources $(TESTS_TARGET)\n"
"\n"
"$(STATIC):  $(OBJECTS)\n"
"\t@echo \" (AR)    $@ $(OBJECTS)\"\n"
"\t@ar rcs $@ $(OBJECTS)\n"
"\t@ranlib $@\n"
"\n"
"$(SHARED): $(STATIC) $(OBJECTS)\n"
"\t@echo \" (CC)    $@ $(OBJECTS)\"\n"
"\t@$(CC) -shared -o $@ $(OBJECTS) $(LIBS) $(LFLAGS)\n"
"\n"
"$(EXECUTABLE): $(OBJECTS)\n"
"\t@echo \" (CC)    $@ $(OBJECTS)\"\n"
"\t$(CC) -o $@ $(STATIC) $(LIBS) $(LFLAGS)\n"
"\n"
"\n"
"# ----- BUILD & CLEANUP ----------------------------------------------------\n"
"\n"
"build:\n"
"\t@mkdir -p bin\n"
"\t@mkdir -p build\n"
"\t@mkdir -p build/tests\n"
"\t@mkdir -p build/tests/unit\n"
"\t@mkdir -p build/tests/acceptance\n"
"\t@mkdir -p build/tests/log\n"
"\t@echo \" (MK)    directories for build\"\n"
"\n"
".PHONY: clean\n"
"clean:\n"
"\t@echo \" (CLEAN) $(LIBNAME)\"\n"
"\t@rm -rf build bin doxygen/documentation $(OBJECTS) $(TESTS_OBJECTS) \\n"
"\t\t$(LIBNAMEPC) $(TESTS_TMP_FILES) $(GCOV_FILES) *.gcov *.profile *.pc *.out\n"
"\n"
"\n"
"# ----- DOCUMENATION -------------------------------------------------------\n"
"\n"
"#NOTE requires doxygen.PHONY: documentation\n"
"documentation:\n"
"\tdoxygen ./doxygen/doxygen.config\n"
"\n"
"\n"
"# ----- INFORMATION PRINTING -----------------------------------------------\n"
"\n"
"# print out a variable of the make file (e.g. \"make print-PROJECTPATH\")\n"
".PHONY: print\n"
"print-%%  : ; @echo $* = $($*)\n"
"\n"
".PHONY: start\n"
"start:\n"
"\t@echo \"\\n (HINT)    $(PROJECT) \t\t ==> running make\\n\"\n"
"\n"
".PHONY: done\n"
"done:\n"
"\t@echo\n"
"\t@echo \" (DONE)  make $(PROJECT)\"\n"
"\t@echo \" (HINT)  with unit testing      ==> 'make tested'\"\n"
"\t@echo \" (HINT)  perform installation   ==> 'sudo make install\\n\"\n"
"\t@echo \" (HINT)  generate documentation ==> 'make documentation\\n\"\n"
"\n"
"\n"
"# ----- TESTING ------------------------------------------------------------\n"
"\n"
"# ALL IN ONE CALL (compile source, test and run test)\n"
"tested: all testrun done\n"
"\n"
"# copy test resources to build\n"
"tests-resources:\n"
"\t@echo \" (CP)    tests/resources\"\n"
"\t@cp -r tests/resources build/tests\n"
"\n"
"build/tests/acceptance/%%_tests.test: tests/acceptance/%%_tests.o\n"
"\t@echo \" (CC)    $(@)\"\n"
"\t@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)\n"
"\n"
"build/tests/unit/%%_tests.test: tests/unit/%%_tests.o\n"
"\t@echo \" (CC)    $(@)\"\n"
"\t@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)\n"
"\n"
"# build a specific executable test (testname) under build/tests\n"
"# NOTE: May be usefull for module development in large projects\n"
"test:\n"
"\t@echo \" (CC)    $(testname)\"\n"
"\t@$(CC) $(CFLAGS) $(LFLAGS) $(patsubst build/tests/%%.test, \\\n"
"\t\ttests/%%.c,$(testname)) -ldl $(STATIC) -Wl,-rpath=$(RPATH) -g -o\\\n"
"\t\t$(patsubst tests/%%.c,build/tests/%%.test,$(testname)) $(LIBS)\n"
"\n"
"# TESTRUN runners ----------------------------------------------------------\n"
"\n"
"# ACCEPTANCE TEST script invocation\n"
".PHONY: testrun-acceptance\n"
"testrun-acceptance:\n"
"\tsh tests/tools/testrun_simple_acceptance_tests.sh\n"
"\n"
"# UNIT TEST script invocation\n"
".PHONY: testrun-unit\n"
"testrun-unit:\n"
"\tsh tests/tools/testrun_simple_unit_tests.sh\n"
"\n"
"# COVERAGE TEST script invocation\n"
".PHONY: testrun-coverage\n"
"testrun-coverage:\n"
"\tsh tests/tools/testrun_simple_coverage_tests.sh $(PROJECTPATH)\n"
"\n"
"# LOC TEST script invocation\n"
".PHONY: testrun-loc\n"
"testrun-loc:\n"
"\tsh tests/tools/testrun_simple_loc.sh $(PROJECTPATH)\n"
"\n"
"# TESTRUN all scripts\n"
".PHONY: testrun\n"
"testrun:\n"
"\t@echo \" (HINT)  $(PROJECT) \\t\\t\\t==> running tests\\n\"\n"
"\tsh tests/tools/testrun_simple_unit_tests.sh\n"
"\tsh tests/tools/testrun_simple_acceptance_tests.sh\n"
"\tsh tests/tools/testrun_simple_coverage_tests.sh $(PROJECTPATH)\n"
"\tsh tests/tools/testrun_simple_loc.sh $(PROJECTPATH)\n"
"\n"
"# TESTRUN gcov -------------------------------------------------------------\n"
"\n"
"testrun-gcov: clean\n"
"\tmake USE_GCOV=1 all\n"
"\tsh tests/tools/testrun_gcov.sh $(PROJECTPATH)\n"
"\n"
"# TESTRUN gprof ------------------------------------------------------------\n"
"\n"
"testrun-gprof: clean\n"
"\tmake USE_GPROF=1 all\n"
"\tsh tests/tools/testrun_gprof.sh $(PROJECTPATH)\n"
"\n"
"# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------\n"
"\n"
".PHONY: pkgconfig\n"
"pkgconfig:\n"
"\t@echo 'prefix='$(PREFIX)                        >  $(LIBNAMEPC)\n"
"\t@echo 'exec_prefix=$${prefix}'                  >> $(LIBNAMEPC)\n"
"\t@echo 'libdir='$(LIBDIR)                        >> $(LIBNAMEPC)\n"
"\t@echo 'includedir='$(INCDIR)                    >> $(LIBNAMEPC)\n"
"\t@echo ''                                        >> $(LIBNAMEPC)\n"
"\t@echo 'Name: '$(LIBNAME)                        >> $(LIBNAMEPC)\n"
"\t@echo 'Description: '                           >> $(LIBNAMEPC)\n"
"\t@echo 'Version: '$(VERSION)                     >> $(LIBNAMEPC)\n"
"\t@echo 'URL: http://testrun.info'                >> $(LIBNAMEPC)\n"
"\t@echo 'Libs: -L$${libdir} -l'$(DIRNAME)         >> $(LIBNAMEPC)\n"
"\t@echo 'Cflags: -I$${includedir}'                >> $(LIBNAMEPC)\n"
"\n"
"# ----- INSTALLATION -------------------------------------------------------\n"
"\n"
"# Installation as a library ------------------------------------------------\n"
"\n"
"install_lib: $(SHARED) $(STATIC)\n"
"\t@echo \" (OK)    installed $(LIBNAME) to $(LIBDIR)\"\n"
"\t@mkdir -p $(LIBDIR)/pkgconfig\n"
"\t@mkdir -p $(INCDIR)\n"
"\t@$(INSTALL) -m 0644 -t $(INCDIR) $(shell find include -name \"*.h\")\n"
"\t@$(INSTALL) -m 0755 $(SHARED) $(LIBDIR)\n"
"\t@$(INSTALL) -m 0755 $(STATIC) $(LIBDIR)\n"
"\t@$(INSTALL) -m 0644 $(LIBNAMEPC) $(LIBDIR)/pkgconfig\n"
"\t@ldconfig\n"
"\n"
"uninstall_lib:\n"
"\t@echo \" (OK)    uninstalled $(LIBNAME) from $(LIBDIR)\"\n"
"\t@rm -rf $(INCDIR)\n"
"\t@rm -rf $(LIBDIR)/$(LIBNAME).a\n"
"\t@rm -rf $(LIBDIR)/$(LIBNAME).so\n"
"\t@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)\n"
"\n"
"# Installation as an executable --------------------------------------------\n"
"\n"
"install_exec: $(SHARED) $(STATIC)\n"
"\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\"\n"
"\t@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)\n"
"\n"
"uninstall_exec:\n"
"\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\"\n"
"\t@rm -rf $(EXECDIR)/$(DIRNAME)\n"
"\n"
"# Installation as a service (outsourced to script)--------------------------\n"
"\n"
"install_service: $(EXECUTABLE)\n"
"\t./config/install/install.sh\n"
"\n"
"uninstall_service:\n"
"\t./config/install/uninstall.sh\n",
all_target, install, uninstall);

        step1 = testrun_text_block_script(&config,
                description, usage, dependencies, content);

        if (!step1)
                goto error;
        size1 = strlen(step1);

        if (!testrun_string_replace_first(
                &step2, &size2,
                step1,  size1,
                wrong, strlen(wrong),
                line, strlen(line)))
                goto error;

        /*
         *      NOTE The next part replaces File [PROJECT], which is set
         *      based on the configuraion with File makefile.
         *      This will only work, as long as the indent used in
         *      shell headers is fixed 8 spaces.
         *      (20171201: Currently not configuable and fixed to 8 spaces)
         *
         *      Using this strategy we generate something like:
         *
         *      #       File            makefile
         *      #       Authors         Markus Toepfer
         *      #       Date            2017-12-01
         *      #
         *      #       Project         [PROJECT]
         *
         *      Project is set correct everywhere in the makefile,
         *      based on the configuration. File is overriden with
         *      correct filename.
         */

        snprintf(description, d_size, "%s\n#       Authors",
                config.project.name);

        snprintf(usage, d_size, "makefile_general.mk\n#       Authors");

        if (!testrun_string_replace_first(
                &step3, &size3,
                step2,  size2,
                description, strlen(description),
                usage, strlen(usage)))
                goto error;

        line  = testrun_string_free(line);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        return step3;
error:
        line  = testrun_string_free(line);
        step1 = testrun_string_free(step1);
        step2 = testrun_string_free(step2);
        step3 = testrun_string_free(step3);
        return NULL;
}

/*----------------------------------------------------------------------------*/

char *testrun_lib_script_service_install_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        //config.project.name    = config.project.service.install_script;
        config.copyright       = copyright;
        config.author          = author;

        size_t d_size = 5000;

        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        char * content = NULL;
        char * step1   = NULL;
        char * step2   = NULL;

        size_t size2 = 0;

        snprintf(description, d_size,
        "Install and enable the service.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"CONVENTION\n"
        "#\n"
        TESTRUN_TAG_OFFSET"(1) Installation will be done at:\n"
        TESTRUN_TAG_OFFSET"    /usr/local/bin\n"
        "#\n"
        TESTRUN_TAG_OFFSET"(2) Configuration files will be placed at\n"
        TESTRUN_TAG_OFFSET"    /etc/%s\n"
        "#\n"
        TESTRUN_TAG_OFFSET"(3) Service definition will be placed at \n"
        TESTRUN_TAG_OFFSET"    /etc/systemd/system/%s.service\n"
        "#\n"
        TESTRUN_TAG_OFFSET"(4) Service socket definition will be placed at \n"
        TESTRUN_TAG_OFFSET"    /etc/systemd/system/%s.socket\n"
        "#\n"
        TESTRUN_TAG_OFFSET"THIS SCRIPT MUST BE RUN AS ROOT."
        ,config.project.name, config.project.name, config.project.name);

        snprintf(usage, d_size,
                "./install.sh /path/to/project");

        snprintf(dependencies, d_size, "bash systemctl grep cp ");

        content = testrun_text_block_script_install(&config);
        if (!content)
                goto error;

        step1   = testrun_text_block_script(&config,
                description, usage, dependencies, content);
        if (!step1)
                goto error;

        /*
         *      NOTE The next part replaces File [PROJECT], which is set
         *      based on the configuraion with File install.sh.
         *      This will only work, as long as the indent used in
         *      shell headers is fixed 8 spaces.
         *      (20171201: Currently not configuable and fixed to 8 spaces)
         *
         *      Using this strategy we generate something like:
         *
         *      #       File            install.sh
         *      #       Authors         Markus Toepfer
         *      #       Date            2017-12-01
         *      #
         *      #       Project         [PROJECT]
         *
         *      Project is set correct everywhere in the makefile,
         *      based on the configuration. File is overriden with
         *      correct filename.
         */

        snprintf(description, d_size, "%s\n#       Authors",
                config.project.name);

        snprintf(usage, d_size, "install.sh\n#       Authors");

        if (!testrun_string_replace_first(
                &step2, &size2,
                step1,  strlen(step1),
                description, strlen(description),
                usage, strlen(usage)))
                goto error;

        content = testrun_string_free(content);
        step1   = testrun_string_free(step1);
        return step2;

error:
        content = testrun_string_free(content);
        step1   = testrun_string_free(step1);
        step2   = testrun_string_free(step2);
        return NULL;
}


/*----------------------------------------------------------------------------*/

char *testrun_lib_script_service_uninstall_content(
        testrun_config config){

        char *author = "Markus Toepfer";

        testrun_copyright copyright = testrun_copyright_apache_version_2(
                "2017", author, NULL);

        //config.project.name    = config.project.service.install_script;
        config.copyright       = copyright;
        config.author          = author;

        size_t d_size = 5000;

        char description[d_size];
        char usage[d_size];
        char dependencies[d_size];

        bzero(description,      d_size);
        bzero(usage,            d_size);
        bzero(dependencies,     d_size);

        char * content = NULL;
        char * step1   = NULL;
        char * step2   = NULL;

        size_t size2 = 0;

        snprintf(description, d_size,
        "Disable and deinstall the service.\n"
        "#\n"
        TESTRUN_TAG_OFFSET"THIS SCRIPT MUST BE RUN AS ROOT.");

        snprintf(usage, d_size,
                "./uninstall.sh /path/to/project");

        snprintf(dependencies, d_size, "bash systemctl grep cp ");

        content = testrun_text_block_script_uninstall(&config);
        if (!content)
                goto error;

        step1   = testrun_text_block_script(&config,
                description, usage, dependencies, content);
        if (!step1)
                goto error;

        /*
         *      NOTE The next part replaces File [PROJECT], which is set
         *      based on the configuraion with File install.sh.
         *      This will only work, as long as the indent used in
         *      shell headers is fixed 8 spaces.
         *      (20171201: Currently not configuable and fixed to 8 spaces)
         *
         *      Using this strategy we generate something like:
         *
         *      #       File            uninstall.sh
         *      #       Authors         Markus Toepfer
         *      #       Date            2017-12-01
         *      #
         *      #       Project         [PROJECT]
         *
         *      Project is set correct everywhere in the makefile,
         *      based on the configuration. File is overriden with
         *      correct filename.
         */

        snprintf(description, d_size, "%s\n#       Authors",
                config.project.name);

        snprintf(usage, d_size, "uninstall.sh\n#       Authors");

        if (!testrun_string_replace_first(
                &step2, &size2,
                step1,  strlen(step1),
                description, strlen(description),
                usage, strlen(usage)))
                goto error;

        content = testrun_string_free(content);
        step1   = testrun_string_free(step1);
        return step2;

error:
        content = testrun_string_free(content);
        step1   = testrun_string_free(step1);
        step2   = testrun_string_free(step2);
        return NULL;
}
