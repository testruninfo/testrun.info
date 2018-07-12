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
        @file           testrun_tools.h
        @author         [AUTHOR]
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

        ------------------------------------------------------------------------
*/
#ifndef testrun_tools_h
#define testrun_tools_h

#define TEST_PREFIX        "test_" // usecase function      <-> test_function
#define TEST_SUFFIX_SOURCE "_test" // usecase source.c      <-> source_test.c
#define TEST_SUFFIX_EXEC   ".test" // usecase source_test.c <-> source.test

typedef enum {LIB, EXEC, SERVICE} testrun_makefile_target;

#include <stdio.h>
#include <string.h>

typedef struct testrun_tools testrun_tools;

struct testrun_tools {

        char *(*testrun_header) ();
        char *(*testrun_header_openmp) ();

        char *(*testrun_simple_tests)           (const char *type,
                                                 const char *project,
                                                 const char *file_name,
                                                 const char *runner_script,
                                                 const char *path_logfile, 
                                                 const char *path_tests);

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


};

#endif /* testrun_tools_h */

