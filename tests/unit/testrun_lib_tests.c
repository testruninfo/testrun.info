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
        @file           testrun_lib_test.c
        @author         [AUTHOR]
        @date           2018-07-06

        @ingroup        testrun_lib

        @brief          Unit tests for testrun_lib.


        ------------------------------------------------------------------------
*/

#include "../tools/testrun.h"
#include "../../src/testrun_lib.c"

/*
 *      ------------------------------------------------------------------------
 *
 *      TEST HELPER                                                     #HELPER
 *
 *      ------------------------------------------------------------------------
 */

bool helper_create(const testrun_lib *lib){

        if (lib)
                return true;

        return true;
}

bool helper_create_module(const testrun_lib *lib, const char *module_name){

        if (lib || module_name)
                return true;

        return true;
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

int test_testrun_lib_default(){
        
        struct testrun_lib lib = testrun_lib_default();

        // check config invalid
        testrun(!testrun_config_validate(&lib.config));
        lib.config.project.name = "name";
        lib.config.project.path = "path";
        testrun(testrun_config_validate(&lib.config));

        // check tools valid
        testrun(testrun_tools_validate(&lib.tools));

        // check functions
        testrun(lib.create_project_paths          == testrun_lib_create_project_paths);
        testrun(lib.create_project_files          == testrun_lib_create_project_files);
        testrun(lib.create_module_files           == testrun_lib_create_module_files);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_validate(){
        
        struct testrun_lib lib = testrun_lib_default();

        // check config invalid
        testrun(!testrun_lib_validate(&lib));
        
        lib.config.project.name = "name";
        lib.config.project.path = "path";
        testrun(testrun_lib_validate(&lib));

        // check tools invalid
        lib.tools.testrun_header = NULL;
        testrun(!testrun_lib_validate(&lib));
        lib.tools = testrun_tools_default();
        testrun(testrun_lib_validate(&lib));

        lib.create_project_paths = NULL;
        testrun(!testrun_lib_validate(&lib));
        lib.create_project_paths = helper_create;
        testrun(testrun_lib_validate(&lib));
        lib.create_project_paths = testrun_lib_create_project_paths;
        testrun(testrun_lib_validate(&lib));

        lib.create_project_files = NULL;
        testrun(!testrun_lib_validate(&lib));
        lib.create_project_files = helper_create;
        testrun(testrun_lib_validate(&lib));
        lib.create_project_files = testrun_lib_create_project_files;
        testrun(testrun_lib_validate(&lib));

        lib.create_module_files = NULL;
        testrun(!testrun_lib_validate(&lib));
        lib.create_module_files = helper_create_module;
        testrun(testrun_lib_validate(&lib));
        lib.create_module_files = testrun_lib_create_module_files;
        testrun(testrun_lib_validate(&lib));

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_project_paths(){
        
        struct testrun_lib lib = testrun_lib_default();

        // check config invalid
        testrun(!testrun_lib_validate(&lib));
        
        lib.config.project.name = "abcd";
        lib.config.project.path = "/tmp/test_folder";

        // clean folder
        system("rm -rf /tmp/test_folder");

        DIR *dp;

        dp = opendir("/tmp/test_folder");
        testrun(!dp);

        testrun(!testrun_lib_create_project_paths(NULL));

        testrun(testrun_lib_create_project_paths(&lib));

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
        dp = opendir("/tmp/test_folder/abcd/tests/unit");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("/tmp/test_folder/abcd/tests/acceptance");
        testrun(dp);
        (void) closedir (dp);
        dp = opendir("/tmp/test_folder/abcd/tests/resources");
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
        dp = opendir("/tmp/test_folder/abcd/docs");
        testrun(dp);
        (void) closedir (dp);

        testrun(system("rm -rf /tmp/test_folder") == 0);

        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_makefiles(){
        
        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        FILE *file = NULL;
        char filename[PATH_MAX];
        memset(filename, 0, PATH_MAX);

        char *content = NULL;
        char expect[20000];
        memset(expect, 0, 20000);
        
        lib.config.project.name = "abcd";
        lib.config.project.path = "/tmp/test_folder";

        testrun(testrun_lib_validate(&lib));

        // clean folder
        system("rm -rf /tmp/test_folder");

        DIR *dp;

        testrun(!testrun_lib_create_makefiles(NULL,      NULL));
        testrun(!testrun_lib_create_makefiles(root_path, NULL));
        testrun(!testrun_lib_create_makefiles(NULL,      &lib));
        testrun(testrun_lib_create_makefiles(root_path, &lib));

        // check root
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        snprintf(filename, PATH_MAX, "%s/%s", root_path, lib.config.makefile.configurable);

        file = fopen(filename, "r");
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);  //same as rewind(f);

        content = calloc(size + 1, sizeof(char));
        fread(content, size, 1, file);
        fclose(file);
        content[size] = 0;
      
        // expect all static 
        memset(filename, 0, PATH_MAX);
        sprintf(expect, "%s",
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright 2017 Markus Toepfer\n"
        "#\n"
        "#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
        "#       you may not use this file except in compliance with the License.\n"
        "#       You may obtain a copy of the License at\n"
        "#\n"
        "#               http://www.apache.org/licenses/LICENSE-2.0\n"
        "#\n"
        "#       Unless required by applicable law or agreed to in writing, software\n"
        "#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
        "#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
        "#       See the License for the specific language governing permissions and\n"
        "#       limitations under the License.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            makefile\n"
        "#       Authors         Markus Toepfer\n"
        "#       Authors         ...\n"
        "#       Date            2018-02-18\n"
        "#\n"
        "#       Project         abcd\n"
        "#\n"
        "#       Description     This makefile defines project specific parameter.\n"
        "#\n"
        "#                       These parameter are:\n"
        "#                       (1) used compiler and special flags\n"
        "#                       (2) name and version\n"
        "#                       (3) installation prefix\n"
        "#                       (4) used libraries\n"
        "#                       (5) general makefiles used\n"
        "#\n"
        "#       Usage           make\n"
        "#\n"
        "#       Dependencies    make & compiler\n"
        "#\n"
        "#       Last changed    2018-07-12\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
        "CC = gcc\n"
        "\n"
        "PROJECT\t\t\t:= abcd\n"
        "VERSION\t\t\t:= 0.0.1\n"
        "\n"
        "# project path recalculation (if used included from a parent make)\n"
        "PROJECTMK\t\t:= $(abspath $(lastword $(MAKEFILE_LIST)))\n"
        "\n"
        "# prefix for base directory for installation (default is /)\n"
        "#PREFIX\t\t\t:= some_path\n"
        "\n"
        "# include all pkgconfig files available at PREFIX\n"
        "export PKG_CONFIG_PATH = $(PREFIX)/usr/local/lib/pkgconfig\n"
        "\n"
        "# LIBS USED (uncommented example includes)\n"
        "# ... will allow to include libs installed under PREFIX\n"
        "#LIBS\t\t\t+= `pkg-config --cflags --libs libtestrun.info`\n"
        "#LIBS\t\t\t+= `pkg-config --libs libsystemd`\n"
        "#LIBS\t\t\t+= `pkg-config --libs uuid`\n"
        "#LIBS\t\t\t+= `pkg-config --libs openssl`\n"
        "\n"
        "# MODULE BASED CFLAGS (example)\n"
        "MODCFLAGS\t\t+= -std=c11 -D _DEFAULT_SOURCE -D _POSIX_C_SOURCE=200809\n"
        "\n"
        "# EXTRA CFLAGS (example parallel or other GCC custom flags)\n"
        "#MODCFLAGS\t\t+= -fopenmp\n"
        "#MODCFLAGS\t\t+= -rdynamic\n"
        "\n"
        "# EXTRA LFLAGS (example)\n"
        "#MODLFLAGS\t\t+= -pthread\n"
        "\n"
        "# PKG_CONFIG_DATA (used during LIBRARY install)\n"
        "PROJECT_URL\t\t= \"www.testrun.info\"\n"
        "PROJECT_DESC\t= \"testrun.info\"\n"
        "\n"
        "# SERVICE_CONFIG_DATA (used during SERVICE install)\n"
        "SERVICE_DATA\t= \"config\"\n"
        "SERVICE_START\t= \"$(SERVICE_DATA)/start.sh\"\n"
        "SERVICE_STOP\t= \"$(SERVICE_DATA)/stop.sh\"\n"
        "\n"
        "# TMP FILE DEFINITION\n"
        "TESTS_TMP_FILES\t= $(wildcard /tmp/test_*)\n"
        "\n"
        "# INCLUDE BASE MAKEFILE\n"
        "include makefile_common.mk\n");

        testrun(0 == strncmp(content, expect, strlen(expect)));
        free(content);

        snprintf(filename, PATH_MAX, "%s/%s", root_path, lib.config.makefile.common);

        file = fopen(filename, "r");
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        fseek(file, 0, SEEK_SET);  //same as rewind(f);

        content = calloc(size + 1, sizeof(char));
        fread(content, size, 1, file);
        fclose(file);
        content[size] = 0;
      
        // expect all static 
        memset(expect, 0, 20000);
        sprintf(expect,
        "#!/usr/bin/env bash\n"
        "#\n"
        "#       Copyright 2017 Markus Toepfer\n"
        "#\n"
        "#       Licensed under the Apache License, Version 2.0 (the \"License\");\n"
        "#       you may not use this file except in compliance with the License.\n"
        "#       You may obtain a copy of the License at\n"
        "#\n"
        "#               http://www.apache.org/licenses/LICENSE-2.0\n"
        "#\n"
        "#       Unless required by applicable law or agreed to in writing, software\n"
        "#       distributed under the License is distributed on an \"AS IS\" BASIS,\n"
        "#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
        "#       See the License for the specific language governing permissions and\n"
        "#       limitations under the License.\n"
        "#\n"
        "#       ------------------------------------------------------------------------\n"
        "#\n"
        "#       File            %s\n"
        "#       Authors         Markus Toepfer\n"
        "#       Date            2018-02-18\n"
        "#\n"
        "#       Project         %s\n"
        "#\n"
        "#       Description     Generic makefile for testrun based projects.\n"
        "#\n"
        "#                       Target of this makefile is an independent library\n"
        "#                       or executable to be installed at either PREFIX/lib\n"
        "#                       or PREFIX/bin.\n"
        "#\n"
        "#                       The TESTING part contains all required functionality\n"
        "#                       to use the testrun tools via a makefile. It may be\n"
        "#                       seen as a makefile integrated testrunner framework.\n"
        "#\n"
        "#                       in particular:\n"
        "#\n"
        "#                           \"make clean && make tested\"\n"
        "#\n"
        "#                       may be used to build all sources as well as tests from\n"
        "#                       scratch and perform an integrated testrun over all after\n"
        "#                       compilation.\n"
        "#\n"
        "#                           \"make gcov\"\n"
        "#\n"
        "#                       may be used to rebuild the whole project with gcov\n"
        "#                       coverage testing flag enabled.\n"
        "#\n"
        "#                           \"make gprof\"\n"
        "#\n"
        "#                       may be used to rebuild the whole project with gprof\n"
        "#                       profiling flags enabled.\n"
        "#\n"
        "#                       Following folder structure is required\n"
        "#\n"
        "#                           bin     MUST be located at %s\n"
        "#                           build   MUST be located at %s\n"
        "#                           inludes MUST be located at %s\n"
        "#                           sources MUST be located at %s\n"
        "#                           tests   MUST be located at %s\n"
        "#\n"
        "#                       ALL TEST SCRIPTS MAY BE EXCHANGED WITH CUSTOM RUNNERS\n"
        "#\n"
        "#       Usage           SHOULD be used included by parent makefile\n"
        "#\n"
        "#       NOTE            aligned with tab width 4\n"
        "#\n"
        "#       Dependencies    testrun (makefile & service scripts), doxygen (if used)\n"
        "#\n"
        "#       Last changed    2018-07-12\n"
        "#       ------------------------------------------------------------------------\n"
        "\n"
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
        "DIRNAME\t\t\t:= $(notdir $(patsubst %%/,%%,$(dir $(PROJECTMK))))\n"
        "\n"
        "# ----- Package config setup -----------------------------------------------\n"
        "\n"
        "LIBNAME\t\t\t:= lib$(DIRNAME)\n"
        "LIBNAMEPC\t\t:= $(LIBNAME).pc\n"
        "\n"
        "INCDIR\t\t\t:= $(PREFIX)/usr/local/include/$(DIRNAME)\n"
        "LIBDIR\t\t\t:= $(PREFIX)/usr/local/lib\n"
        "EXECDIR\t\t\t:= $(PREFIX)/usr/local/bin\n"
        "CONFDIR\t\t\t:= $(PREFIX)/etc/$(DIRNAME)\n"
        "SOCKDIR\t\t\t:= $(PREFIX)/etc/systemd/system\n"
        "\n"
        "# ----- TARGETS ------------------------------------------------------------\n"
        "\n"
        "INSTALL\t\t\t:= install\n"
        "\n"
        "EXECUTABLE\t\t= %s/$(DIRNAME)\n"
        "\n"
        "STATIC\t\t\t= %s/lib$(DIRNAME).a\n"
        "SHARED\t\t\t= $(patsubst %%.a,%%.so,$(STATIC))\n"
        "\n"
        "# Source and object files to compile\n"
        "HEADERS\t\t\t= $(wildcard %s/*.h)\n"
        "SOURCES\t\t\t= $(wildcard %s/**/*.c %s/*.c)\n"
        "OBJECTS\t\t\t= $(patsubst %%.c,%%.o,$(SOURCES))\n"
        "\n"
        "# Test sources and targets\n"
        "TESTS_SOURCES   = $(wildcard %s/**/*%s.c %s/*%s.c)\n"
        "TESTS_TARGET    = $(patsubst %s/%%.c, %s/tests/%%%s, $(TESTS_SOURCES))\n"
        "\n"
        "# GCOV support\n"
        "GCOV_FILES\t\t=  $(patsubst %%.c,%%.gcno,$(SOURCES))\n"
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
        "# ----- TEST_SCRIPTS -------------------------------------------------------\n"
        "\n"
        "TEST_SCRIPT_UNIT\t\t= %s\n"
        "TEST_SCRIPT_ACCEPTANCE\t= %s\n"
        "TEST_SCRIPT_COVERAGE\t= %s\n"
        "TEST_SCRIPT_LOC\t\t\t= %s\n"
        "TEST_SCRIPT_GCOV\t\t= %s\n"
        "TEST_SCRIPT_GPROF\t\t= %s\n"
        "\n"
        "# ----- DEFAULT MAKE RULES -------------------------------------------------\n"
        "\n"
        "%%.o : %%.c $(HEADERS)\n"
        "\t@echo \" (CC)    $@\"\n"
        "\t@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)\n"
        "\n"
        "%%%s.o : %%%s.c\n"
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
        "# ----- BUILD & CLEANUP ----------------------------------------------------\n"
        "\n"
        "build:\n"
        "\t@mkdir -p %s\n"
        "\t@mkdir -p %s\n"
        "\t@mkdir -p %s/tests\n"
        "\t@mkdir -p %s/tests/unit\n"
        "\t@mkdir -p %s/tests/acceptance\n"
        "\t@mkdir -p %s/tests/log\n"
        "\t@echo \" (MK)    directories for build\"\n"
        "\n"
        ".PHONY: clean\n"
        "clean:\n"
        "\t@echo \" (CLEAN) $(LIBNAME)\"\n"
        "\t@rm -rf %s %s %s/documentation $(OBJECTS) $(TESTS_OBJECTS) \\\n"
        "\t$(LIBNAMEPC) $(TESTS_TMP_FILES) $(GCOV_FILES) *.gcov *.profile *.pc *.out\n"
        "\n"
        "\n"
        "# ----- DOCUMENATION -------------------------------------------------------\n"
        "\n"
        "#NOTE requires doxygen.PHONY: documentation\n"
        "documentation:\n"
        "\tdoxygen %s/doxygen.config\n"
        "\n"
        "# ----- INFORMATION PRINTING -----------------------------------------------\n"
        "\n"
        "# print out a variable of the make file (e.g. \"make print-PROJECTPATH\")\n"
        ".PHONY: print\n"
        "print-%%  : ; @echo $* = $($*)\n"
        "\n"
        ".PHONY: start\n"
        "start:\n"
        "\t@echo \"\\n (HINT)    $(PROJECT)           ==> running make\\n\"\n"
        "\n"
        ".PHONY: done\n"
        "done:\n"
        "\t@echo\n"
        "\t@echo \" (DONE)  make $(PROJECT)\"\n"
        "\t@echo \" (HINT)  with unit testing      ==> 'make tested'\"\n"
        "\t@echo \" (HINT)  perform installation   ==> 'sudo make install\"\n"
        "\t@echo \" (HINT)  generate documentation ==> 'make documentation\"\n"
        "\n"
        "# ----- TESTING ------------------------------------------------------------\n"
        "\n"
        "# ALL IN ONE CALL (compile source, test and run test)\n"
        "tested: all testrun done\n"
        "\n"
        "# copy test resources to build\n"
        "tests-resources:\n"
        "\t@echo \" (CP)    tests/resources\"\n"
        "\t@cp -r %s/resources %s/tests\n"
        "\n"
        "%s/tests/acceptance/%%%s%s: %s/acceptance/%%%s.o\n"
        "\t@echo \" (CC)    $(@)\"\n"
        "\t@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)\n"
        "\n"
        "%s/tests/unit/%%%s%s: %s/unit/%%%s.o\n"
        "\t@echo \" (CC)    $(@)\"\n"
        "\t@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)\n"
        "\n"
        "# TESTRUN runners ----------------------------------------------------------\n"
        "\n"
        "# ACCEPTANCE TEST script invocation\n"
        ".PHONY: testrun-acceptance\n"
        "testrun-acceptance:\n"
        "\tsh $(TEST_SCRIPT_ACCEPTANCE)\n"
        "\n"
        "# UNIT TEST script invocation\n"
        ".PHONY: testrun-unit\n"
        "testrun-unit:\n"
        "\tsh $(TEST_SCRIPT_UNIT)\n"
        "\n"
        "# COVERAGE TEST script invocation\n"
        ".PHONY: testrun-coverage\n"
        "testrun-coverage:\n"
        "\tsh $(TEST_SCRIPT_COVERAGE) $(PROJECTPATH)\n"
        "\n"
        "# LOC TEST script invocation\n"
        ".PHONY: testrun-loc\n"
        "testrun-loc:\n"
        "\tsh $(TEST_SCRIPT_LOC) $(PROJECTPATH)\n"
        "\n"
        "# TESTRUN all scripts\n"
        ".PHONY: testrun\n"
        "testrun:\n"
        "\t@echo \" (HINT)  $(PROJECT) \\t\\t\\t==> running tests\\n\"\n"
        "\tsh $(TEST_SCRIPT_UNIT)\n"
        "\tsh $(TEST_SCRIPT_ACCEPTANCE)\n"
        "\tsh $(TEST_SCRIPT_COVERAGE) $(PROJECTPATH)\n"
        "\tsh $(TEST_SCRIPT_LOC) $(PROJECTPATH)\n"
        "\n"
        "# TESTRUN gcov -------------------------------------------------------------\n"
        "\n"
        ".PHONY: testrun-gcov\n"
        "testrun-gcov: clean\n"
        "\tmake USE_GCOV=1 all\n"
        "\tsh $(TEST_SCRIPT_GCOV) $(PROJECTPATH)\n"
        "\n"
        "# TESTRUN gprof ------------------------------------------------------------\n"
        "\n"
        ".PHONY: testrun-gprof\n"
        "testrun-gprof: clean\n"
        "\tmake USE_GPROF=1 all\n"
        "\tsh $(TEST_SCRIPT_PROF) $(PROJECTPATH)\n"
        "\n"
        "# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------\n"
        "\n"
        ".PHONY: pkgconfig\n"
        "pkgconfig:\n"
        "\t@echo 'prefix='$(PREFIX)'/usr/local/' >  $(LIBNAMEPC)\n"
        "\t@echo 'exec_prefix=$${prefix}' >> $(LIBNAMEPC)\n"
        "\t@echo 'libdir=$${prefix}/lib' >> $(LIBNAMEPC)\n"
        "\t@echo 'includedir=$${prefix}/include' >> $(LIBNAMEPC)\n"
        "\t@echo '' >> $(LIBNAMEPC)\n"
        "\t@echo 'Name: '$(LIBNAME) >> $(LIBNAMEPC)\n"
        "\t@echo 'Description: '$(PROJECT_DESC) >> $(LIBNAMEPC)\n"
        "\t@echo 'Version: '$(VERSION) >> $(LIBNAMEPC)\n"
        "\t@echo 'URL: ' $(PROJECT_URL) >> $(LIBNAMEPC)\n"
        "\t@echo 'Libs: -L$${libdir} -l'$(DIRNAME) >> $(LIBNAMEPC)\n"
        "\t@echo 'Cflags: -I$${includedir}' >> $(LIBNAMEPC)\n"
         "\n"
        "# ----- INSTALLATION -------------------------------------------------------\n"
        "\n"
        "# Installation as a library ------------------------------------------------\n"
        "\n"
        ".PHONY: install_lib\n"
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
        ".PHONY: uninstall_lib\n"
        "uninstall_lib:\n"
        "\t@echo \" (OK)    uninstalled $(LIBNAME) from $(LIBDIR)\"\n"
        "\t@rm -rf $(INCDIR)\n"
        "\t@rm -rf $(LIBDIR)/$(LIBNAME).a\n"
        "\t@rm -rf $(LIBDIR)/$(LIBNAME).so\n"
        "\t@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)\n"
        "\n"
        "# Installation as an executable --------------------------------------------\n"
        "\n"
        ".PHONY: install_exec\n"
        "install_exec: $(SHARED) $(STATIC)\n"
        "\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\"\n"
        "\t@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)\n"
        "\n"
        ".PHONY: uninstall_exec\n"
        "uninstall_exec:\n"
        "\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\"\n"
        "\t@rm -rf $(EXECDIR)/$(DIRNAME)\n"
        "\n"
        "# Installation as a service ------------------------------------------------\n"
        ".PHONY: install_service\n"
        "install_service: $(EXECUTABLE)\n"
        "\t@echo \" (OK)    installed $(DIRNAME) to $(EXECDIR)\"\n"
        "\t@mkdir -p $(SOCKDIR)\n"
        "\t@$(INSTALL) -m 0755 bin/$(DIRNAME)  $(EXECDIR)\n"
        "\t@$(INSTALL) -m 0755 -d $(SERVICE_DATA)/etc   $(CONFDIR)\n"
        "\t@$(INSTALL) -m 0755 $(SERVICE_DATA)/*.service $(SOCKDIR)\n"
        "\t@$(INSTALL) -m 0755 $(SERVICE_DATA)/*.socket $(SOCKDIR)\n"
        "\t@# IF INSTALLATION IS DONE UNPREFIXED TO /etc, the service will be enabled \n"
        "\t$(shell if [ -z \"$(PREFIX)\" ]; then $(SERVICE_START); fi)\n"
        "\n"
        "\n"
        ".PHONY: uninstall_service\n"
        "uninstall_service:\n"
        "\t@echo \" (OK)    uninstalled $(DIRNAME) from $(EXECDIR)\"\n"
        "\t@rm -rf $(EXECDIR)/$(DIRNAME)\n"
        "\t@rm -rf $(CONFDIR)\n"
        "\t@rm -rf $(SOCKDIR)/$(DIRNAME)*\n"
        "\t@# IF INSTALLATION WAS DONE UNPREFIXED TO /etc, the service will be disabled \n"
        "\t$(shell if [ -z \"$(PREFIX)\" ]; then $(SERVICE_STOP); fi)\n"
        ,
                lib.config.makefile.common,
                lib.config.project.name,
                lib.config.path.bin,
                lib.config.path.build,
                lib.config.path.include,
                lib.config.path.source,
                lib.config.path.tests,
                lib.config.path.bin,
                lib.config.path.build,
                lib.config.path.include,
                lib.config.path.source,
                lib.config.path.source,
                lib.config.path.tests,
                lib.config.test_suffix_source,
                lib.config.path.tests,
                lib.config.test_suffix_source,
                lib.config.path.tests,
                lib.config.path.build,
                lib.config.test_suffix_exec,
                lib.config.script.test_unit,
                lib.config.script.test_acceptance,
                lib.config.script.test_coverage,
                lib.config.script.lines_of_code,
                lib.config.script.test_gcov,
                lib.config.script.test_gprof,
                lib.config.test_suffix_source,
                lib.config.test_suffix_source,
                "all_lib",
                "install_lib",
                "uninstall_lib",
                lib.config.path.bin,
                lib.config.path.build,
                lib.config.path.build,
                lib.config.path.build,
                lib.config.path.build,
                lib.config.path.build,
                lib.config.path.bin,
                lib.config.path.build,
                lib.config.path.doxygen,
                lib.config.path.doxygen,
                lib.config.path.tests,
                lib.config.path.build,
                lib.config.path.build,
                lib.config.test_suffix_source,
                lib.config.test_suffix_exec,
                lib.config.path.tests,
                lib.config.test_suffix_source,
                lib.config.path.build,
                lib.config.test_suffix_source,
                lib.config.test_suffix_exec,
                lib.config.path.tests,
                lib.config.test_suffix_source);

        testrun(0 == strncmp(content, expect, strlen(expect)));
        free(content);

        // check name is configurable

        lib.config.makefile.common = "abcd";
        lib.config.makefile.configurable = "conf";

        snprintf(filename, PATH_MAX, "%s/%s", root_path, lib.config.makefile.common);
        file = fopen(filename, "r");
        testrun(file == NULL);
        snprintf(filename, PATH_MAX, "%s/%s", root_path, lib.config.makefile.configurable);
        file = fopen(filename, "r");
        testrun(file == NULL);

        testrun(testrun_lib_create_makefiles(root_path, &lib));

        file = fopen(filename, "r");
        testrun(file);
        fclose(file);

        snprintf(filename, PATH_MAX, "%s/%s", root_path, lib.config.makefile.common);
        file = fopen(filename, "r");
        testrun(file);
        fclose(file);

        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_gitignore(){
        
        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        FILE *file = NULL;
        char filename[PATH_MAX];
        memset(filename, 0, PATH_MAX);

        size_t size = 10000;
        size_t csize = 0;
        char expect[size];
        memset(expect, 0, size);

        char *content = NULL;

        snprintf(filename, PATH_MAX, "%s/.gitignore", root_path);

        file = fopen(filename, "r");
        testrun(file == NULL);

        testrun(!testrun_lib_create_gitignore(NULL,      NULL));
        testrun(!testrun_lib_create_gitignore(root_path, NULL));
        testrun(!testrun_lib_create_gitignore(NULL,      &lib));
        testrun(testrun_lib_create_gitignore(root_path, &lib));

        file = fopen(filename, "r");
        testrun(file != NULL);
        fseek(file, 0, SEEK_END);
        csize = ftell(file);
        fseek(file, 0, SEEK_SET);

        content = calloc(csize + 1, sizeof(char));
        fread(content, csize, 1, file);
        fclose(file);
        content[csize] = 0;

        testrun(snprintf(expect, size, 
                "# Prerequisites\n"
                "*.d\n"
                "\n"
                "# Object files\n"
                "*.o\n"
                "*.so\n"
                "*.ko\n"
                "*.obj\n"
                "*.elf\n"
                "\n"
                "# Linker output\n"
                "*.ilk\n"
                "*.map\n"
                "*.exp\n"
                "\n"
                "# Precompiled Headers\n"
                "*.gch\n"
                "*.pch\n"
                "\n"
                "# Libraries\n"
                "*.lib\n"
                "*.a\n"
                "*.la\n"
                "*.lo\n"
                "\n"
                "# Shared objects (inc. Windows DLLs)\n"
                "*.dll\n"
                "*.so\n"
                "*.so.*\n"
                "*.dylib\n"
                "\n"
                "# Executables\n"
                "*.exe\n"
                "*.out\n"
                "*.app\n"
                "*.i*86\n"
                "*.x86_64\n"
                "*.hex\n"
                "\n"
                "# Debug files\n"
                "*.dSYM/\n"
                "*.su\n"
                "*.idb\n"
                "*.pdb\n"
                "\n"
                "# Kernel Module Compile Results\n"
                "*.mod*\n"
                "*.cmd\n"
                ".tmp_versions/\n"
                "modules.order\n"
                "Module.symvers\n"
                "Mkfile.old\n"
                "dkms.conf\n"
                "\n"
                "# Local files\n"
                "**/local\n"
                "**/bin/\n"
                "**/gen/\n"
                "**/build/\n"
                "**/docs/doxygen/\n"
                "**/doxygen/documentation/\n"
                "\n"
                "# vagrant (if used)\n"
                ".vagrant\n"
                "\n"
                "# subprojects (if used)\n"
                "*.git\n"
                "\n"
                "# generated package config\n"
                "*.pc\n"
                "\n"
                "# ctags\n"
                ".tags\n"
                "tags\n"
                "functions\n"
                "\n"
                "# IDE\n"
                "\n"
                "## IntelliJ\n"
                ".idea\n"
                "\n"
                "## Sublime\n"
                "*.sublime-workspace\n"
                "*.sublime-project\n"
                "\n"
                "## VIM\n"
                "[._]*.s[a-w][a-z]\n"
                "[._]s[a-w][a-z]\n"
                "*.un~\n"
                "Session.vim\n"
                ".netrwhist\n"
                "*~\n"));

        testrun(0 == strncmp(content, expect, strlen(expect)));
        free(content);
        
        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_readme(){
        
        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        FILE *file = NULL;
        char filename[PATH_MAX];
        memset(filename, 0, PATH_MAX);

        size_t size = 10000;
        size_t csize = 0;
        char expect[size];
        memset(expect, 0, size);

        char *content = NULL;
        char *time_string = testrun_time_string(TESTRUN_SCOPE_YEAR);

        snprintf(filename, PATH_MAX, "%s/%s", root_path, TESTRUN_FILE_README);

        file = fopen(filename, "r");
        testrun(file == NULL);

        testrun(!testrun_lib_create_readme(NULL,      NULL));
        testrun(!testrun_lib_create_readme(root_path, NULL));
        testrun(!testrun_lib_create_readme(NULL,      &lib));
        testrun(testrun_lib_create_readme(root_path, &lib));

        file = fopen(filename, "r");
        testrun(file != NULL);
        fseek(file, 0, SEEK_END);
        csize = ftell(file);
        fseek(file, 0, SEEK_SET);

        content = calloc(csize + 1, sizeof(char));
        fread(content, csize, 1, file);
        fclose(file);
        content[csize] = 0;

        char *copyright = lib.config.copyright.copyright.generate_header_string(
                                NULL,
                                COPYRIGHT_DEFAULT_INTRO,
                                time_string,
                                lib.config.copyright.owner,
                                lib.config.copyright.note,
                                NULL,
                                lib.config.indent.standard,
                                false,
                                lib.config.copyright.gpl_parameter);

        testrun(snprintf(expect, size, 
        "# Project %s\n"
        "\n"
        "This module is self supported and may be build, tested, installed and\n"
        "run independently.\n"
        "\n"
        "## Overview\n"
        "\n"
        "* [Description](#description)\n"
        "* [Usage](#usage)\n"
        "* [Installation](#installation)\n"
        "* [Requirements](#requirements)\n"
        "* [Structure](#structure)\n"
        "* [Tests](#tests)\n"
        "* [Tips](#tips)\n"
        "* [Copyright](#copyright)\n"
        "\n"
        "## Description\n"
        "\n"
        "%s\n"
        "\n"
        "## Usage\n"
        "\n"
        "...\n"
        "\n"
        "## Installation\n"
        "\n"
        "...\n"
        """\n"
        "### build sources\n"
        "\n"
        "\\`\\`\\`bash\n"
        "make\n"
        "\\`\\`\\`\n"
        "\n"
        "### build documentation\n"
        "\n"
        "\\`\\`\\`bash\n"
        "make documentation\n"
        "\\`\\`\\`\n"
        "\n"
        "### test sources\n"
        "\n"
        "\\`\\`\\`bash\n"
        "make tested\n"
        "\\`\\`\\`\n"
        "\n"
        "### install binaries\n"
        "\n"
        "\\`\\`\\`bash\n"
        "sudo make install\n"
        "\\`\\`\\`\n"
        "\n"
        "### uninstall binaries\n"
        "\n"
        "\\`\\`\\`bash\n"
        "sudo make uninstall\n"
        "\\`\\`\\`\n"
        "\n"
        "## Requirements\n"
        "\n"
        "## Structure\n"
        "\n"
        "### Default structure of the folder:\n"
        "\n"
        "\\`\\`\\`\n"
        "<pre>\n"
        ".\n"
        "├── README.MD\n"
        "├── .gitignore\n"
        "├── makefile\n"
        "├── makefile_common.mk\n"
        "│\n"
        "├── copyright\n"
        "│   └── ... \n"
        "│\n"
        "├── doxygen\n"
        "│   ├── documentation\n"
        "│   └── doxygen.config\n"
        "│\n"
        "├── docs\n"
        "│   ├── CHANGELOG.MD\n"
        "│   └── ...\n"
        "│\n"
        "├── include\n"
        "│   ├── %s.h\n"
        "│   └── ...\n"
        "│\n"
        "├── src\n"
        "│   ├── %s.c\n"
        "│   └── ...\n"
        "│\n"
        "└── tests\n"
        "    ├── resources\n"
        "    ├── tools\n"
        "    │   ├── testrun.h\n"
        "    │   ├── testrun_runner.sh\n"
        "    │   ├── testrun_gcov.sh\n"
        "    │   ├── testrun_gprof.sh\n"
        "    │   ├── testrun_simple_coverage_tests.sh\n"
        "    │   ├── testrun_simple_unit_tests.sh\n"
        "    │   ├── testrun_simple_acceptance_tests.sh\n"
        "    │   └── testrun_simple_loc.sh\n"
        "    │\n"
        "    ├── acceptance\n"
        "    │   ├── ...\n"
        "    │   └── ...\n"
        "    │\n"
        "    └── unit\n"
        "        ├── %s_test.c\n"
        "        └── ...\n"
        "\n"
        "</pre>\n"
        "\\`\\`\\`\n"
        "\n"
        "## Tests\n"
        "\n"
        "All test sources will be recompiled on each make run. That means,\n"
        "all module tests will be created new on any change in any source file.\n"
        "\n"
        "### Test a project (all files contained in tests/unit)\n"
        "\n"
        "Test compile and run\n"
        "~~~\n"
        "make tested\n"
        "~~~\n"
        "\n"
        "## Tips\n"
        "\n"
        "## Copyright\n"
        "\n"
        "%s\n",
        lib.config.project.name,
        lib.config.project.description,
        lib.config.project.name,
        lib.config.project.name,
        lib.config.project.name,
        copyright));

        testrun(0 == strncmp(content, expect, strlen(expect)));
        free(content);
        free(copyright);
        free(time_string);
        
        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_changelog(){
        
        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        FILE *file = NULL;
        char filename[PATH_MAX];
        memset(filename, 0, PATH_MAX);

        size_t csize = 0;
        char *content = NULL;

        snprintf(filename, PATH_MAX, "%s/%s/%s", 
                root_path, lib.config.path.docs, TESTRUN_FILE_CHANGELOG);

        file = fopen(filename, "r");
        testrun(file == NULL);

        testrun(!testrun_lib_create_changelog(NULL,      NULL));
        testrun(!testrun_lib_create_changelog(root_path, NULL));
        testrun(!testrun_lib_create_changelog(NULL,      &lib));
        testrun(testrun_lib_create_changelog(root_path,  &lib));

        file = fopen(filename, "r");
        testrun(file != NULL);
        fseek(file, 0, SEEK_END);
        csize = ftell(file);
        fseek(file, 0, SEEK_SET);

        content = calloc(csize + 1, sizeof(char));
        fread(content, csize, 1, file);
        fclose(file);
        content[csize] = 0;

        char *expect = "Project created at ";
        testrun(0 == strncmp(content, expect, strlen(expect)));
        free(content);
        
        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_doxygen(){
        
        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        FILE *file = NULL;
        char filename[PATH_MAX];
        memset(filename, 0, PATH_MAX);

        size_t size = 10000;
        size_t csize = 0;
        char expect[size];
        memset(expect, 0, size);

        char *content = NULL;

        snprintf(filename, PATH_MAX, "%s/%s/%s", 
                root_path, lib.config.path.doxygen, TESTRUN_FILE_DOXYGEN);

        file = fopen(filename, "r");
        testrun(file == NULL);

        testrun(!testrun_lib_create_doxygen(NULL,      NULL));
        testrun(!testrun_lib_create_doxygen(root_path, NULL));
        testrun(!testrun_lib_create_doxygen(NULL,      &lib));
        testrun(testrun_lib_create_doxygen(root_path, &lib));

        file = fopen(filename, "r");
        testrun(file != NULL);
        fseek(file, 0, SEEK_END);
        csize = ftell(file);
        fseek(file, 0, SEEK_SET);

        content = calloc(csize + 1, sizeof(char));
        fread(content, csize, 1, file);
        fclose(file);
        content[csize] = 0;

        char input[1000];
        memset(input, 0, 1000);

        testrun(snprintf(input, 1000, "%s %s %s %s",
                TESTRUN_FILE_README, 
                lib.config.path.include, 
                lib.config.path.source,
                lib.config.path.tests));

        testrun(snprintf(expect, size, 
                "DOXYFILE_ENCODING       = UTF-8\n"
                "PROJECT_NAME            = %s\n"
                "PROJECT_NUMBER          = 0.0.1\n"
                "PROJECT_LOGO            = %s/logo.png\n"
                "PROJECT_BRIEF           = %s\n"
                "OUTPUT_DIRECTORY        = %s/documentation\n"
                "CREATE_SUBDIRS          = NO\n"
                "ALLOW_UNICODE_NAMES     = NO\n"
                "OUTPUT_LANGUAGE         = English\n"
                "MARKDOWN_SUPPORT        = YES\n"
                "AUTOLINK_SUPPORT        = YES\n"
                "USE_MDFILE_AS_MAINPAGE  = %s\n"
                "INPUT                   = %s\n"
                "INPUT_ENCODING          = UTF-8\n"
                "FILE_PATTERNS           = *.h *.c *.js *.py *.sh\n"
                "RECURSIVE               = YES\n"
                "EXCLUDE_SYMLINKS        = YES\n",
                lib.config.project.name, 
                lib.config.path.doxygen, 
                lib.config.project.name, 
                lib.config.path.doxygen, 
                TESTRUN_FILE_README, 
                input));

        testrun(0 == strncmp(content, expect, strlen(expect)));
        free(content);
        
        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_copyright(){
        
        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        FILE *file = NULL;
        char filename[PATH_MAX];
        memset(filename, 0, PATH_MAX);

        size_t size = 10000;
        size_t csize = 0;
        char expect[size];
        memset(expect, 0, size);

        char *content = NULL;

        snprintf(filename, PATH_MAX, "%s/%s/%s", 
                root_path, lib.config.path.copyright, TESTRUN_FILE_COPYRIGHT);

        file = fopen(filename, "r");
        testrun(file == NULL);

        testrun(!testrun_lib_create_copyright(NULL,      NULL));
        testrun(!testrun_lib_create_copyright(root_path, NULL));
        testrun(!testrun_lib_create_copyright(NULL,      &lib));
        testrun(testrun_lib_create_copyright(root_path, &lib));

        file = fopen(filename, "r");
        testrun(file != NULL);
        fseek(file, 0, SEEK_END);
        csize = ftell(file);
        fseek(file, 0, SEEK_SET);

        content = calloc(csize + 1, sizeof(char));
        fread(content, csize, 1, file);
        fclose(file);
        content[csize] = 0;

        char *time_string = testrun_time_string(TESTRUN_SCOPE_YEAR); 
        char *copyright   = lib.config.copyright.copyright.generate_header_string(
                                NULL,
                                COPYRIGHT_DEFAULT_INTRO,
                                time_string,
                                lib.config.copyright.owner,
                                lib.config.copyright.note,
                                NULL,
                                lib.config.indent.standard,
                                false,
                                lib.config.copyright.gpl_parameter);

        testrun(0 == strncmp(content, copyright, strlen(copyright)));
        free(content);
        free(copyright);
        free(time_string);

        // check full text license is written too if configured.
        snprintf(filename, PATH_MAX, "%s/%s/%s", 
                root_path, lib.config.path.copyright, TESTRUN_FILE_COPYRIGHT_FULL);

        file = fopen(filename, "r");
        testrun(file == NULL);

        testrun(system("rm -rf /tmp/test_folder") == 0);

        lib.config.copyright.copyright = testrun_copyright_apache_version_2();
        testrun(testrun_lib_create_copyright(root_path, &lib));

        snprintf(filename, PATH_MAX, "%s/%s/%s", 
                root_path, lib.config.path.copyright, TESTRUN_FILE_COPYRIGHT_FULL);

        file = fopen(filename, "r");
        testrun(file != NULL);
        fclose(file);
        
         snprintf(filename, PATH_MAX, "%s/%s/%s", 
                root_path, lib.config.path.copyright, TESTRUN_FILE_COPYRIGHT);

        file = fopen(filename, "r");
        testrun(file != NULL);
        fclose(file);

        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/


int test_testrun_lib_create_service(){
        
        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        FILE *file = NULL;
        char filename[PATH_MAX];
        memset(filename, 0, PATH_MAX);

        size_t size = 10000;
        size_t csize = 0;
        char expect[size];
        memset(expect, 0, size);

        char *content = NULL;

        lib.config.project.name = "test";

        snprintf(filename, PATH_MAX, "%s/%s/%s.service", 
                root_path, lib.config.path.service, lib.config.project.name);

        file = fopen(filename, "r");
        testrun(file == NULL);

        testrun(!testrun_lib_create_service(NULL,      NULL));
        testrun(!testrun_lib_create_service(root_path, NULL));
        testrun(!testrun_lib_create_service(NULL,      &lib));
        testrun(testrun_lib_create_service(root_path, &lib));

        file = fopen(filename, "r");
        testrun(file != NULL);
        fseek(file, 0, SEEK_END);
        csize = ftell(file);
        fseek(file, 0, SEEK_SET);

        content = calloc(csize + 1, sizeof(char));
        fread(content, csize, 1, file);
        fclose(file);
        content[csize] = 0;
        testrun(snprintf(expect, size, 
                "[Unit]\n"
                "Description= %s service\n"
                "\n"
                "[Service]\n"
                "ExecStart=%s/%s\n"
                "NonBlocking=True\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target\n"
                , lib.config.project.name, TESTRUN_DEFAULT_INSTALL_PATH, lib.config.project.name));

        testrun(0 == strncmp(content, expect, strlen(expect)));
        free(content);

        snprintf(filename, PATH_MAX, "%s/%s/%s.socket", 
                root_path, lib.config.path.service, lib.config.project.name);

        file = fopen(filename, "r");
        testrun(file != NULL);
        fseek(file, 0, SEEK_END);
        csize = ftell(file);
        fseek(file, 0, SEEK_SET);

        content = calloc(csize + 1, sizeof(char));
        fread(content, csize, 1, file);
        fclose(file);
        content[csize] = 0;

        testrun(snprintf(expect, size, 
                "[Unit]\n"
                "Description= %s socket\n"
                "\n"
                "[Socket]\n"
                "\n"
                "# example interface bound\n"
                "# ListenStream=10.0.0.100:12345\n"
                "\n"
                "# example localhost\n"
                "# ListenStream=127.0.0.1:12345\n"
                "\n"
                "# example listen on all\n"
                "# ListenStream=0.0.0.0:12345\n"
                "\n"
                "# example listen on UDP\n"
                "# ListenDatagram=0.0.0.0:12345\n"
                "\n"
                "# Maximum parallel connections for the socket\n"
                "Backlog=2048\n"
                "\n"
                "# TCP Keepalive (1h)\n"
                "KeepAlive=false\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target\n"
                , lib.config.project.name));


        testrun(0 == strncmp(content, expect, strlen(expect)));
        free(content);


        snprintf(filename, PATH_MAX, "%s/%s/%s", 
                root_path, lib.config.path.service, TESTRUN_FILE_README);

        file = fopen(filename, "r");
        testrun(file != NULL);
        fseek(file, 0, SEEK_END);
        csize = ftell(file);
        fseek(file, 0, SEEK_SET);

        content = calloc(csize + 1, sizeof(char));
        fread(content, csize, 1, file);
        fclose(file);
        content[csize] = 0;
        testrun(csize > 0);
        free(content);

        
        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_c_documentation_header(){

        size_t size = 1000;
        char buffer[size];
        char expect[size];

        memset(buffer, 0, size);
        memset(expect, 0, size);

        char *date = testrun_time_string(TESTRUN_SCOPE_DAY);

        testrun(!testrun_lib_create_c_documentation_header(NULL,   size, NULL, NULL, NULL, NULL));
        testrun(!testrun_lib_create_c_documentation_header(buffer, size, NULL, NULL, NULL, NULL));
        testrun(!testrun_lib_create_c_documentation_header(NULL,   size, "ab", NULL, NULL, NULL));

        testrun(testrun_lib_create_c_documentation_header(buffer,  size, "ab", NULL, NULL, NULL));

        testrun(snprintf(expect, size, 
                "%s"
                "        @file           %s\n"
                "        @author         %s\n"
                "        @date           %s\n"
                "\n"
                "        @ingroup        %s\n"
                "\n"
                "        @brief          %s\n"
                "\n"
                "%s\n",
                TESTRUN_DOCUMENTATION_PREFIX,
                "ab",
                TESTRUN_TAG_AUTHOR,
                date,
                TESTRUN_TAG_GROUP,
                "...",
                TESTRUN_DOCUMENTATION_SUFFIX));

        testrun( 0 == strncmp(buffer, expect, strlen(expect)));

        memset(buffer, 0, size);
        memset(expect, 0, size);

        testrun(testrun_lib_create_c_documentation_header(buffer,  size, "ab", "xy", "z", "1"));

        testrun(snprintf(expect, size, 
                "%s"
                "        @file           %s\n"
                "        @author         %s\n"
                "        @date           %s\n"
                "\n"
                "        @ingroup        %s\n"
                "\n"
                "        @brief          %s\n"
                "\n"
                "%s\n",
                TESTRUN_DOCUMENTATION_PREFIX,
                "ab",
                "xy",
                date,
                "z",
                "1",
                TESTRUN_DOCUMENTATION_SUFFIX));

        testrun( 0 == strncmp(buffer, expect, strlen(expect)));

        free(date);
        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_c_header(){

        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        char *date = testrun_time_string(TESTRUN_SCOPE_DAY);

        char *copyright = lib.config.copyright.copyright.generate_header_string(
                                NULL,
                                COPYRIGHT_DEFAULT_INTRO,
                                date,
                                lib.config.copyright.owner,
                                lib.config.copyright.note,
                                NULL,
                                lib.config.indent.standard,
                                false,
                                lib.config.copyright.gpl_parameter);


        FILE *file = NULL;
        char filename[PATH_MAX];
        memset(filename, 0, PATH_MAX);

        size_t size = 10000;
        size_t csize = 0;
        char expect[size];
        memset(expect, 0, size);

        char *content = NULL;
        char *name    = "name";

        lib.config.project.name = "test";

        snprintf(filename, PATH_MAX, "%s/%s/%s.h", 
                root_path, lib.config.path.include, name);

        file = fopen(filename, "r");
        testrun(file == NULL);

        char documentation[1000];
        memset(documentation, 0, 1000);

        free(date);
        date = testrun_time_string(TESTRUN_SCOPE_DAY);

        testrun(snprintf(documentation, 1000, 
                "%s"
                "        @file           %s.h\n"
                "        @author         %s\n"
                "        @date           %s\n"
                "\n"
                "        @ingroup        %s\n"
                "\n"
                "        @brief          %s\n"
                "\n"
                "%s\n",
                TESTRUN_DOCUMENTATION_PREFIX,
                name,
                TESTRUN_TAG_AUTHOR,
                date,
                TESTRUN_TAG_GROUP,
                "Definition of ",
                TESTRUN_DOCUMENTATION_SUFFIX));

        free(date);


        testrun(!testrun_lib_create_c_header(NULL,      NULL, NULL,     NULL));
        testrun(!testrun_lib_create_c_header(NULL,      &lib, copyright, name));
        testrun(!testrun_lib_create_c_header(root_path, &lib, copyright, NULL));
        testrun(!testrun_lib_create_c_header(root_path, NULL, copyright, name));
        testrun(!testrun_lib_create_c_header(root_path, &lib, NULL,      name));

        testrun(testrun_lib_create_c_header(root_path, &lib, copyright, name));

        file = fopen(filename, "r");
        testrun(file != NULL);
        fseek(file, 0, SEEK_END);
        csize = ftell(file);
        fseek(file, 0, SEEK_SET);

        content = calloc(csize + 1, sizeof(char));
        fread(content, csize, 1, file);
        fclose(file);
        content[csize] = 0;

        testrun(snprintf(expect, size, 
                "%s%s"
                "#ifndef %s_h\n"
                "#define %s_h\n"
                "\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      GENERIC FUNCTIONS\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "\n"
                "#endif /* %s_h */\n",
                copyright,
                documentation,
                name,
                name,
                name));

        testrun( 0 == strncmp(content, expect, strlen(expect)));

        free(copyright);
        free(content);

        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_c_source(){

        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        char *date = testrun_time_string(TESTRUN_SCOPE_DAY);

        char *copyright = lib.config.copyright.copyright.generate_header_string(
                                NULL,
                                COPYRIGHT_DEFAULT_INTRO,
                                date,
                                lib.config.copyright.owner,
                                lib.config.copyright.note,
                                NULL,
                                lib.config.indent.standard,
                                false,
                                lib.config.copyright.gpl_parameter);


        FILE *file = NULL;
        char filename[PATH_MAX];
        memset(filename, 0, PATH_MAX);

        size_t size = 10000;
        size_t csize = 0;
        char expect[size];
        memset(expect, 0, size);

        char *content = NULL;
        char *name    = "name";

        lib.config.project.name = "test";

        snprintf(filename, PATH_MAX, "%s/%s/%s.c", 
                root_path, lib.config.path.source, name);

        file = fopen(filename, "r");
        testrun(file == NULL);

        char documentation[1000];
        memset(documentation, 0, 1000);

        free(date);
        date = testrun_time_string(TESTRUN_SCOPE_DAY);

        testrun(snprintf(documentation, 1000, 
                "%s"
                "        @file           %s.c\n"
                "        @author         %s\n"
                "        @date           %s\n"
                "\n"
                "        @ingroup        %s\n"
                "\n"
                "        @brief          %s\n"
                "\n"
                "%s\n",
                TESTRUN_DOCUMENTATION_PREFIX,
                name,
                TESTRUN_TAG_AUTHOR,
                date,
                TESTRUN_TAG_GROUP,
                "Implementation of ",
                TESTRUN_DOCUMENTATION_SUFFIX));

        free(date);


        testrun(!testrun_lib_create_c_source(NULL,      NULL, NULL,     NULL));
        testrun(!testrun_lib_create_c_source(NULL,      &lib, copyright, name));
        testrun(!testrun_lib_create_c_source(root_path, &lib, copyright, NULL));
        testrun(!testrun_lib_create_c_source(root_path, NULL, copyright, name));
        testrun(!testrun_lib_create_c_source(root_path, &lib, NULL,      name));

        testrun(testrun_lib_create_c_source(root_path, &lib, copyright, name));

        file = fopen(filename, "r");
        testrun(file != NULL);
        fseek(file, 0, SEEK_END);
        csize = ftell(file);
        fseek(file, 0, SEEK_SET);

        content = calloc(csize + 1, sizeof(char));
        fread(content, csize, 1, file);
        fclose(file);
        content[csize] = 0;

        testrun(snprintf(expect, size, 
                "%s%s"
                "#include \"../include/%s.h\"\n",
                copyright,
                documentation,
                name));

        //log("\n\nCONTENT\n%s\nEXPECT\n%s\n\n", content, expect);

        testrun( 0 == strncmp(content, expect, strlen(expect)));

        free(copyright);
        free(content);

        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_c_unit_test(){

        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        char *date = testrun_time_string(TESTRUN_SCOPE_DAY);

        char *copyright = lib.config.copyright.copyright.generate_header_string(
                                NULL,
                                COPYRIGHT_DEFAULT_INTRO,
                                date,
                                lib.config.copyright.owner,
                                lib.config.copyright.note,
                                NULL,
                                lib.config.indent.standard,
                                false,
                                lib.config.copyright.gpl_parameter);


        FILE *file = NULL;
        char filename[PATH_MAX];
        memset(filename, 0, PATH_MAX);

        size_t size = 10000;
        size_t csize = 0;
        char expect[size];
        memset(expect, 0, size);

        char *content = NULL;
        char *name    = "name";

        lib.config.project.name = "test";

        snprintf(filename, PATH_MAX, "%s/%s/%s/%s%s.c", 
                root_path, lib.config.path.tests, TESTRUN_FOLDER_UNIT_TESTS, name, lib.config.test_suffix_source);

        file = fopen(filename, "r");
        testrun(file == NULL);

        char documentation[1000];
        memset(documentation, 0, 1000);

        free(date);
        date = testrun_time_string(TESTRUN_SCOPE_DAY);

        testrun(snprintf(documentation, 1000, 
                "%s"
                "        @file           %s%s.c\n"
                "        @author         %s\n"
                "        @date           %s\n"
                "\n"
                "        @ingroup        %s\n"
                "\n"
                "        @brief          %s\n"
                "\n"
                "%s\n",
                TESTRUN_DOCUMENTATION_PREFIX,
                name,
                lib.config.test_suffix_source,
                TESTRUN_TAG_AUTHOR,
                date,
                TESTRUN_TAG_GROUP,
                "Unit tests of ",
                TESTRUN_DOCUMENTATION_SUFFIX));

        free(date);


        testrun(!testrun_lib_create_c_unit_test(NULL,      NULL, NULL,     NULL));
        testrun(!testrun_lib_create_c_unit_test(NULL,      &lib, copyright, name));
        testrun(!testrun_lib_create_c_unit_test(root_path, &lib, copyright, NULL));
        testrun(!testrun_lib_create_c_unit_test(root_path, NULL, copyright, name));
        testrun(!testrun_lib_create_c_unit_test(root_path, &lib, NULL,      name));

        testrun(testrun_lib_create_c_unit_test(root_path, &lib, copyright, name));

        file = fopen(filename, "r");
        testrun(file != NULL);
        fseek(file, 0, SEEK_END);
        csize = ftell(file);
        fseek(file, 0, SEEK_SET);

        content = calloc(csize + 1, sizeof(char));
        fread(content, csize, 1, file);
        fclose(file);
        content[csize] = 0;

        testrun(snprintf(expect, size, 
                "%s%s"
                "#include \"../../tests/tools/testrun.h\"\n"
                "#include \"../../src/%s.c\"\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      TEST CASES                                                      #CASES\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "int test_case(){\n"
                "        testrun(1 == 1);\n"
                "\n"
                "        return testrun_log_success();\n"
                "}\n"
                "\n"
                "/*----------------------------------------------------------------------------*/\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      TEST CLUSTER                                                    #CLUSTER\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "int all_tests() {\n"
                "\n"
                "        testrun_init();\n"
                "        testrun_test(test_case);\n"
                "\n"
                "        return testrun_counter;\n"
                "}\n"
                "\n"
                "/*\n"
                " *      ------------------------------------------------------------------------\n"
                " *\n"
                " *      TEST EXECUTION                                                  #EXEC\n"
                " *\n"
                " *      ------------------------------------------------------------------------\n"
                " */\n"
                "\n"
                "testrun_run(all_tests);\n",
                copyright,
                documentation,
                name));

        //log("\n\nCONTENT\n%s\nEXPECT\n%s\n\n", content, expect);

        testrun( 0 == strncmp(content, expect, strlen(expect)));

        free(copyright);
        free(content);

        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_project_files(){

        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        char project_root[PATH_MAX];
        memset(project_root, 0, PATH_MAX);

        DIR *dp;
        FILE *file = NULL;

        size_t size = 0;

        char path[PATH_MAX];
        memset(path, 0, PATH_MAX);

        testrun(!testrun_lib_create_project_files(NULL));

        // check root not existing
        dp = opendir(root_path);
        testrun(!dp);

        // no name, no path
        testrun(!testrun_lib_create_project_files(&lib));

        lib.config.project.name = "test";
        lib.config.project.path = root_path;

        testrun(testrun_utils_generate_project_root_path(&lib.config, project_root, PATH_MAX));

        testrun(testrun_lib_create_project_files(&lib));

        // check folders

        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.copyright);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.docs);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.include);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.source);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.tests);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s", project_root, lib.config.path.tests, TESTRUN_FOLDER_UNIT_TESTS);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s", project_root, lib.config.path.tests, TESTRUN_FOLDER_ACCEPTANCE_TESTS);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s", project_root, lib.config.path.tests, TESTRUN_FOLDER_RESOURCES);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.tools);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.service);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.doxygen);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        // check top level files

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.makefile.configurable);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.makefile.common);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, TESTRUN_FILE_GIT_IGNORE);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, TESTRUN_FILE_README);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        // check copyright 

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s", project_root, lib.config.path.copyright, TESTRUN_FILE_COPYRIGHT);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        // check doxygen 

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s", project_root, lib.config.path.doxygen, TESTRUN_FILE_DOXYGEN);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        // check changelog 

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s", project_root, lib.config.path.docs, TESTRUN_FILE_CHANGELOG);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        // check c files 

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.h", project_root, lib.config.path.include, lib.config.project.name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.c", project_root, lib.config.path.source, lib.config.project.name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s/%s%s.c", project_root, lib.config.path.tests, TESTRUN_FOLDER_UNIT_TESTS , lib.config.project.name, lib.config.test_suffix_source);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        // check service files
        
        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.service", project_root, lib.config.path.service, lib.config.project.name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.socket", project_root, lib.config.path.service, lib.config.project.name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s", project_root, lib.config.path.service, TESTRUN_FILE_README);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_module_files(){

        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        char project_root[PATH_MAX];
        memset(project_root, 0, PATH_MAX);

        DIR *dp;
        FILE *file = NULL;

        size_t size = 0;

        char path[PATH_MAX];
        memset(path, 0, PATH_MAX);

        char *name = "testname";

        testrun(!testrun_lib_create_module_files(NULL, NULL));
        testrun(!testrun_lib_create_module_files(&lib, NULL));
        testrun(!testrun_lib_create_module_files(NULL, name));

        // check root not existing
        dp = opendir(root_path);
        testrun(!dp);

        // no name, no path
        testrun(!testrun_lib_create_module_files(&lib, name));

        lib.config.project.name = "test";
        lib.config.project.path = root_path;

        testrun(testrun_lib_create_module_files(&lib, name));
        testrun(testrun_utils_generate_project_root_path(&lib.config, project_root, PATH_MAX));

        // check folders

        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.include);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.source);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.tests);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s", project_root, lib.config.path.tests, TESTRUN_FOLDER_UNIT_TESTS);
        dp = opendir(root_path);
        testrun(dp);
        (void) closedir (dp);

        // check c files 

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.h", project_root, lib.config.path.include, name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.c", project_root, lib.config.path.source, name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s/%s%s.c", project_root, lib.config.path.tests, TESTRUN_FOLDER_UNIT_TESTS , name, lib.config.test_suffix_source);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_project(){

        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        char project_root[PATH_MAX];
        memset(project_root, 0, PATH_MAX);

        DIR *dp;
        FILE *file = NULL;

        size_t size = 0;

        char path[PATH_MAX];
        memset(path, 0, PATH_MAX);

        testrun(!testrun_lib_validate(&lib));

        testrun(!testrun_lib_create_project(NULL));
        testrun(!testrun_lib_create_project(&lib));

        lib.config.project.path = root_path;
        lib.config.project.name = "a";

        // check root not existing
        dp = opendir(root_path);
        testrun(!dp);

        testrun(testrun_utils_generate_project_root_path(&lib.config, project_root, PATH_MAX));

        // check project root not existing
        dp = opendir(project_root);
        testrun(!dp);

        testrun(testrun_lib_create_project(&lib));

        dp = opendir(project_root);
        testrun(dp);
        (void) closedir (dp);

        // try to create at existing project_root
        testrun(!testrun_lib_create_project(&lib));

        // check with different name
        lib.config.project.name = "b";

        testrun(testrun_utils_generate_project_root_path(&lib.config, project_root, PATH_MAX));
        dp = opendir(project_root);
        testrun(!dp);

        testrun(testrun_lib_create_project(&lib));

        dp = opendir(project_root);
        testrun(dp);
        (void) closedir (dp);

        // check files are created too (random file checl)
        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.h", project_root, lib.config.path.include, lib.config.project.name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        testrun(system("rm -rf /tmp/test_folder") == 0);
        return testrun_log_success();
}

/*----------------------------------------------------------------------------*/

int test_testrun_lib_create_module(){

        char *root_path = "/tmp/test_folder/not_existing";
        struct testrun_lib lib = testrun_lib_default();

        char project_root[PATH_MAX];
        memset(project_root, 0, PATH_MAX);
        
        FILE *file = NULL;

        size_t size = 0;

        char path[PATH_MAX];
        memset(path, 0, PATH_MAX);

        char *name = "module_a";

        lib.config.project.path = root_path;
        lib.config.project.name = "a";
        testrun(testrun_lib_create_project(&lib));

        testrun(testrun_utils_generate_project_root_path(&lib.config, project_root, PATH_MAX));

        testrun(!testrun_lib_create_module(NULL, NULL));
        testrun(!testrun_lib_create_module(&lib, NULL));
        testrun(!testrun_lib_create_module(NULL, name));

        // create from path out of project
        lib.config.project.path = root_path;
        testrun(!testrun_lib_create_module(&lib, name));

        // create from project top path
        lib.config.project.path = project_root;
        testrun(testrun_lib_create_module(&lib, name));

        // check c files 
        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.h", project_root, lib.config.path.include, name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.c", project_root, lib.config.path.source, name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s/%s%s.c", project_root, lib.config.path.tests, TESTRUN_FOLDER_UNIT_TESTS , name, lib.config.test_suffix_source);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        // try to create existing module
        testrun(!testrun_lib_create_module(&lib, name));

        name = "module_b";
        // try to create new module
        testrun(testrun_lib_create_module(&lib, name));

        // check c files 
        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.h", project_root, lib.config.path.include, name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.c", project_root, lib.config.path.source, name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s/%s%s.c", project_root, lib.config.path.tests, TESTRUN_FOLDER_UNIT_TESTS , name, lib.config.test_suffix_source);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        // create from path within project
        name = "module_c";
        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s", project_root, lib.config.path.include);

        testrun(testrun_lib_create_module(&lib, name));

        // check c files 
        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.h", project_root, lib.config.path.include, name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.c", project_root, lib.config.path.source, name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s/%s%s.c", project_root, lib.config.path.tests, TESTRUN_FOLDER_UNIT_TESTS , name, lib.config.test_suffix_source);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

         // create from other path within project
        name = "module_d";
        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s", project_root, lib.config.path.tests, TESTRUN_FOLDER_UNIT_TESTS);

        testrun(testrun_lib_create_module(&lib, name));

        // check c files 
        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.h", project_root, lib.config.path.include, name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s.c", project_root, lib.config.path.source, name);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        memset(path, 0, PATH_MAX);
        snprintf(path, PATH_MAX, "%s/%s/%s/%s%s.c", project_root, lib.config.path.tests, TESTRUN_FOLDER_UNIT_TESTS , name, lib.config.test_suffix_source);
        file = fopen(path, "r");
        testrun(file);
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        testrun(size > 0);
        fclose(file);

        testrun(system("rm -rf /tmp/test_folder") == 0);
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

        testrun_test(test_testrun_lib_default);
        testrun_test(test_testrun_lib_validate);
        testrun_test(test_testrun_lib_create_project_paths);
        testrun_test(test_testrun_lib_create_makefiles);
        testrun_test(test_testrun_lib_create_gitignore);
        testrun_test(test_testrun_lib_create_readme);
        testrun_test(test_testrun_lib_create_changelog);
        testrun_test(test_testrun_lib_create_doxygen);
        testrun_test(test_testrun_lib_create_copyright);
        testrun_test(test_testrun_lib_create_service);
 
        testrun_test(test_testrun_lib_create_c_documentation_header);
        testrun_test(test_testrun_lib_create_c_header);
        testrun_test(test_testrun_lib_create_c_source);
        testrun_test(test_testrun_lib_create_c_unit_test);

        testrun_test(test_testrun_lib_create_project_files);
        testrun_test(test_testrun_lib_create_module_files);

        testrun_test(test_testrun_lib_create_project);
        testrun_test(test_testrun_lib_create_module);

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

