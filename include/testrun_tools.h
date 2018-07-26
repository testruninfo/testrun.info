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
        @file           testrun_tools.h
        @author         Markus Toepfer
        @date           2018-07-10

        @ingroup        testrun_lib

        @brief          Definition of the testrun tools enabled within the
                        library.

                        which are:

                                (1) testrun.h     C HEADER FILE
                                (2) testrun*.sh   BASH based FILE RUNNER
                                (3) makefile*     testrun standard makefiles

                        All functions MUST return an allocated string, which
                        may be written "AS IS" to a file.

                        In addition to the basic tools, content of some general
                        files of a standard project MUST be generated using the
                        toolset. (e.g. README, gitignore, doxygen.conf)

        ------------------------------------------------------------------------
*/
#ifndef testrun_tools_h
#define testrun_tools_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum {LIB, EXEC, SERVICE} testrun_makefile_target;

typedef struct testrun_tools testrun_tools;

struct testrun_tools {

        char *(*testrun_header) ();
        char *(*testrun_header_openmp) ();

        char *(*testrun_simple_tests)           (const char *type,
                                                 const char *project,
                                                 const char *file_name,
                                                 const char *runner_script,
                                                 const char *path_logfile,
                                                 const char *path_tests,
                                                 const char *path_tools);

        char *(*testrun_runner)                 (const char *project,
                                                 const char *file_name);

        char *(*testrun_loc)                    (const char *project,
                                                 const char *file_name,
                                                 const char *path_header,
                                                 const char *path_source,
                                                 const char *path_tests);

        char *(*testrun_simple_coverage)        (const char *project,
                                                 const char *file_name,
                                                 const char *test_prefix,
                                                 const char *path_logfile,
                                                 const char *path_source,
                                                 const char *path_tests);

        char *(*testrun_gcov)                   (const char *project,
                                                 const char *file_name,
                                                 const char *path_logfile,
                                                 const char *path_tests_exec,
                                                 const char *path_tests_source,
                                                 const char *exec_suffix,
                                                 const char *src_suffix);

        char *(*testrun_gprof)                  (const char *project,
                                                 const char *file_name,
                                                 const char *path_logfile,
                                                 const char *path_tests_exec,
                                                 const char *exec_suffix);

        char *(*makefile_configurable)          (const char *project,
                                                 const char *file_name,
                                                 const char *version,
                                                 const char *cflags,
                                                 const char *project_url,
                                                 const char *project_desc,
                                                 const char *path_service,
                                                 const char *makefile_general_name);

        char *(*makefile_common)                (const char *project,
                                                 const char *file_name,
                                                 const char *path_bin,
                                                 const char *path_build,
                                                 const char *path_include,
                                                 const char *path_source,
                                                 const char *path_tests,
                                                 const char *path_tools,
                                                 const char *path_doxygen,
                                                 const char *suffix_test_source,
                                                 const char *suffix_test_exec,
                                                 const char *script_unit_tests,
                                                 const char *script_acceptance_tests,
                                                 const char *script_coverage_tests,
                                                 const char *script_loc,
                                                 const char *script_gcov,
                                                 const char *script_gprof,
                                                 testrun_makefile_target target);

        /* General files used in testrun projects */

        char *(*gitignore)                      ();
        char *(*readme)                         (const char *project_name,
                                                 const char *description,
                                                 const char *copyright_string);

        char *(*doxygen)                         (const char *project_name,
                                                  const char *path_doxygen,
                                                  const char *path_mainfile,
                                                  const char *input);

        char *(*service_file)                   (const char *service_name,
                                                 const char *install_path);

        char *(*socket_file)                    (const char *service_name);

};

/*
 *      ------------------------------------------------------------------------
 *
 *      Generate the default testrun tools
 *
 *      ------------------------------------------------------------------------
 */

testrun_tools testrun_tools_default();

/*----------------------------------------------------------------------------*/

/**
        Validate that all function pointers,
        are set.

        @param lib     pointer to lib implementation
*/
bool testrun_tools_validate(const testrun_tools *self);

#endif /* testrun_tools_h */

