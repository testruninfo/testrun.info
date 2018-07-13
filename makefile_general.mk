#!/usr/bin/env bash
#
#       Copyright 2017 Markus Toepfer
#
#       Licensed under the Apache License, Version 2.0 (the "License");
#       you may not use this file except in compliance with the License.
#       You may obtain a copy of the License at
#
#               http://www.apache.org/licenses/LICENSE-2.0
#
#       Unless required by applicable law or agreed to in writing, software
#       distributed under the License is distributed on an "AS IS" BASIS,
#       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#       See the License for the specific language governing permissions and
#       limitations under the License.
#
#       ------------------------------------------------------------------------
#
#       File            makefile_general.mk
#       Authors         Markus Toepfer
#       Date            2018-02-18
#
#       Project         testrun_lib
#
#       Description     Generic makefile for testrun based projects.
#
#                       Target of this makefile is an independent library
#                       or executable to be installed at either PREFIX/lib
#                       or PREFIX/bin.
#
#                       The TESTING part contains all required functionality
#                       to use the testrun tools via a makefile. It may be
#                       seen as a makefile integrated testrunner framework.
#
#                       in particular:
#
#                           "make clean && make tested"
#
#                       may be used to build all sources as well as tests from
#                       scratch and perform an integrated testrun over all after
#                       compilation.
#
#                           "make gcov"
#
#                       may be used to rebuild the whole project with gcov
#                       coverage testing flag enabled.
#
#                           "make gprof"
#
#                       may be used to rebuild the whole project with gprof
#                       profiling flags enabled.
#
#                       Following folder structure is required
#
#                           bin     MUST be located at bin
#                           build   MUST be located at build
#                           inludes MUST be located at include
#                           sources MUST be located at src
#                           tests   MUST be located at tests
#
#                       ALL TEST SCRIPTS MAY BE EXCHANGED WITH CUSTOM RUNNERS
#
#       Usage           SHOULD be used included by parent makefile
#
#       NOTE            aligned with tab width 4
#
#       Dependencies    testrun (makefile & service scripts), doxygen (if used)
#
#       Last changed    2018-07-12
#       ------------------------------------------------------------------------
        
# Switch on colors
GCC_COLORS ?= 'gcc colors available, use them!'
export GCC_COLORS

# ----- Compiler flags -----------------------------------------------------

CFLAGS			= -Wall -Wextra -fPIC -Iinclude

CFLAGS			+= $(MODCFLAGS)
LFLAGS			+= $(MODLFLAGS)

# ----- Project path calculation (if used included) ------------------------

PROJECTPATH		:= $(abspath $(dir $(PROJECTMK)))
DIRNAME			:= $(notdir $(patsubst %/,%,$(dir $(PROJECTMK))))

# ----- Package config setup -----------------------------------------------

LIBNAME			:= lib$(DIRNAME)
LIBNAMEPC		:= $(LIBNAME).pc

INCDIR			:= $(PREFIX)/usr/local/include/$(DIRNAME)
LIBDIR			:= $(PREFIX)/usr/local/lib
EXECDIR			:= $(PREFIX)/usr/local/bin
CONFDIR			:= $(PREFIX)/etc/$(DIRNAME)
SOCKDIR			:= $(PREFIX)/etc/systemd/system

# ----- TARGETS ------------------------------------------------------------

INSTALL			:= install

EXECUTABLE		= bin/$(DIRNAME)

STATIC			= build/lib$(DIRNAME).a
SHARED			= $(patsubst %.a,%.so,$(STATIC))

# Source and object files to compile
HEADERS			= $(wildcard include/*.h)
SOURCES			= $(wildcard src/**/*.c src/*.c)
OBJECTS			= $(patsubst %.c,%.o,$(SOURCES))

# Test sources and targets
TESTS_SOURCES   = $(wildcard tests/**/*_tests.c tests/*_tests.c)
TESTS_TARGET    = $(patsubst tests/%.c, build/tests/%.test, $(TESTS_SOURCES))

# GCOV support
GCOV_FILES		=  $(patsubst %.c,%.gcno,$(SOURCES))
GCOV_FILES		+= $(patsubst %.c,%.gcov,$(SOURCES))
GCOV_FILES		+= $(patsubst %.c,%.gcda,$(SOURCES))
GCOV_FILES		+= $(patsubst %.c,%.gcno,$(TESTS_SOURCES))
GCOV_FILES		+= $(patsubst %.c,%.gcda,$(TESTS_SOURCES))

ifdef USE_GCOV
CFLAGS += -fprofile-arcs -ftest-coverage
LFLAGS += -lgcov --coverage
endif

ifdef USE_GPROF
CFLAGS += -pg
endif

# ----- TEST_SCRIPTS -------------------------------------------------------

TEST_SCRIPT_UNIT		= tests/tools/testrun_simple_unit_tests.sh
TEST_SCRIPT_ACCEPTANCE	= tests/tools/testrun_simple_acceptance_tests.sh
TEST_SCRIPT_COVERAGE	= tests/tools/testrun_simple_coverage_tests.sh
TEST_SCRIPT_LOC			= tests/tools/testrun_simple_loc.sh
TEST_SCRIPT_GCOV		= tests/tools/testrun_gcov.sh
TEST_SCRIPT_GPROF		= tests/tools/testrun_gprof.sh

# ----- DEFAULT MAKE RULES -------------------------------------------------

%.o : %.c $(HEADERS)
	@echo " (CC)    $@"
	@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)

%_tests.o : %_tests.c
	@echo " (CC)    $@"
	@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)

all:			all_lib
install:		install_lib
uninstall:		uninstall_lib

all_lib:		start lib tests pkgconfig done
all_exec:		start lib tests $(EXECUTABLE) done
all_service:	all_exec

lib:			build sources
sources:		build $(STATIC) $(SHARED)
tests:			tests-resources $(TESTS_TARGET)

$(STATIC):  $(OBJECTS)
	@echo " (AR)    $@ $(OBJECTS)"
	@ar rcs $@ $(OBJECTS)
	@ranlib $@

$(SHARED): $(STATIC) $(OBJECTS)
	@echo " (CC)    $@ $(OBJECTS)"
	@$(CC) -shared -o $@ $(OBJECTS) $(LIBS) $(LFLAGS)

$(EXECUTABLE): $(OBJECTS)
	@echo " (CC)    $@ $(OBJECTS)"
	$(CC) -o $@ $(STATIC) $(LIBS) $(LFLAGS)

# ----- BUILD & CLEANUP ----------------------------------------------------

build:
	@mkdir -p bin
	@mkdir -p build
	@mkdir -p build/tests
	@mkdir -p build/tests/unit
	@mkdir -p build/tests/acceptance
	@mkdir -p build/tests/log
	@echo " (MK)    directories for build"

.PHONY: clean
clean:
	@echo " (CLEAN) $(LIBNAME)"
	@rm -rf bin build doxygen/documentation $(OBJECTS) $(TESTS_OBJECTS) \
	$(LIBNAMEPC) $(TESTS_TMP_FILES) $(GCOV_FILES) *.gcov *.profile *.pc *.out


# ----- DOCUMENATION -------------------------------------------------------

#NOTE requires doxygen.PHONY: documentation
documentation:
	doxygen doxygen/doxygen.config

# ----- INFORMATION PRINTING -----------------------------------------------

# print out a variable of the make file (e.g. "make print-PROJECTPATH")
.PHONY: print
print-%  : ; @echo $* = $($*)

.PHONY: start
start:
	@echo "\n (HINT)    $(PROJECT)           ==> running make\n"

.PHONY: done
done:
	@echo
	@echo " (DONE)  make $(PROJECT)"
	@echo " (HINT)  with unit testing      ==> 'make tested'"
	@echo " (HINT)  perform installation   ==> 'sudo make install"
	@echo " (HINT)  generate documentation ==> 'make documentation"

# ----- TESTING ------------------------------------------------------------

# ALL IN ONE CALL (compile source, test and run test)
tested: all testrun done

# copy test resources to build
tests-resources:
	@echo " (CP)    tests/resources"
	@cp -r tests/resources build/tests

build/tests/acceptance/%_tests.test: tests/acceptance/%_tests.o
	@echo " (CC)    $(@)"
	@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)

build/tests/unit/%_tests.test: tests/unit/%_tests.o
	@echo " (CC)    $(@)"
	@$(CC) $(CFLAGS) $(LFLAGS) $^ -ldl $(STATIC) -Wl,-rpath=$(RPATH) -o $(@) $(LIBS)

# TESTRUN runners ----------------------------------------------------------

# ACCEPTANCE TEST script invocation
.PHONY: testrun-acceptance
testrun-acceptance:
	sh $(TEST_SCRIPT_ACCEPTANCE)

# UNIT TEST script invocation
.PHONY: testrun-unit
testrun-unit:
	sh $(TEST_SCRIPT_UNIT)

# COVERAGE TEST script invocation
.PHONY: testrun-coverage
testrun-coverage:
	sh $(TEST_SCRIPT_COVERAGE) $(PROJECTPATH)

# LOC TEST script invocation
.PHONY: testrun-loc
testrun-loc:
	sh $(TEST_SCRIPT_LOC) $(PROJECTPATH)

# TESTRUN all scripts
.PHONY: testrun
testrun:
	@echo " (HINT)  $(PROJECT) \t\t\t==> running tests\n"
	sh $(TEST_SCRIPT_UNIT)
	sh $(TEST_SCRIPT_ACCEPTANCE)
	sh $(TEST_SCRIPT_COVERAGE) $(PROJECTPATH)
	sh $(TEST_SCRIPT_LOC) $(PROJECTPATH)

# TESTRUN gcov -------------------------------------------------------------

.PHONY: testrun-gcov
testrun-gcov: clean
	make USE_GCOV=1 all
	sh $(TEST_SCRIPT_GCOV) $(PROJECTPATH)

# TESTRUN gprof ------------------------------------------------------------

.PHONY: testrun-gprof
testrun-gprof: clean
	make USE_GPROF=1 all
	sh $(TEST_SCRIPT_PROF) $(PROJECTPATH)

# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------

.PHONY: pkgconfig
pkgconfig:
	@echo 'prefix='$(PREFIX)'/usr/local/' >  $(LIBNAMEPC)
	@echo 'exec_prefix=$${prefix}' >> $(LIBNAMEPC)
	@echo 'libdir=$${prefix}/lib' >> $(LIBNAMEPC)
	@echo 'includedir=$${prefix}/include' >> $(LIBNAMEPC)
	@echo '' >> $(LIBNAMEPC)
	@echo 'Name: '$(LIBNAME) >> $(LIBNAMEPC)
	@echo 'Description: '$(PROJECT_DESC) >> $(LIBNAMEPC)
	@echo 'Version: '$(VERSION) >> $(LIBNAMEPC)
	@echo 'URL: ' $(PROJECT_URL) >> $(LIBNAMEPC)
	@echo 'Libs: -L$${libdir} -l'$(DIRNAME) >> $(LIBNAMEPC)
	@echo 'Cflags: -I$${includedir}' >> $(LIBNAMEPC)

# ----- INSTALLATION -------------------------------------------------------

# Installation as a library ------------------------------------------------

.PHONY: install_lib
install_lib: $(SHARED) $(STATIC)
	@echo " (OK)    installed $(LIBNAME) to $(LIBDIR)"
	@mkdir -p $(LIBDIR)/pkgconfig
	@mkdir -p $(INCDIR)
	@$(INSTALL) -m 0644 -t $(INCDIR) $(shell find include -name "*.h")
	@$(INSTALL) -m 0755 $(SHARED) $(LIBDIR)
	@$(INSTALL) -m 0755 $(STATIC) $(LIBDIR)
	@$(INSTALL) -m 0644 $(LIBNAMEPC) $(LIBDIR)/pkgconfig
	@ldconfig

.PHONY: uninstall_lib
uninstall_lib:
	@echo " (OK)    uninstalled $(LIBNAME) from $(LIBDIR)"
	@rm -rf $(INCDIR)
	@rm -rf $(LIBDIR)/$(LIBNAME).a
	@rm -rf $(LIBDIR)/$(LIBNAME).so
	@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)

# Installation as an executable --------------------------------------------

.PHONY: install_exec
install_exec: $(SHARED) $(STATIC)
	@echo " (OK)    installed $(DIRNAME) to $(EXECDIR)"
	@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)

.PHONY: uninstall_exec
uninstall_exec:
	@echo " (OK)    uninstalled $(DIRNAME) from $(EXECDIR)"
	@rm -rf $(EXECDIR)/$(DIRNAME)

# Installation as a service ------------------------------------------------
.PHONY: install_service
install_service: $(EXECUTABLE)
	@echo " (OK)    installed $(DIRNAME) to $(EXECDIR)"
	@mkdir -p $(SOCKDIR)
	@$(INSTALL) -m 0755 bin/$(DIRNAME)  $(EXECDIR)
	@$(INSTALL) -m 0755 -d $(SERVICE_DATA)/etc   $(CONFDIR)
	@$(INSTALL) -m 0755 $(SERVICE_DATA)/*.service $(SOCKDIR)
	@$(INSTALL) -m 0755 $(SERVICE_DATA)/*.socket $(SOCKDIR)
	@# IF INSTALLATION IS DONE UNPREFIXED TO /etc, the service will be enabled 
	$(shell if [ -z "$(PREFIX)" ]; then $(SERVICE_START); fi)


.PHONY: uninstall_service
uninstall_service:
	@echo " (OK)    uninstalled $(DIRNAME) from $(EXECDIR)"
	@rm -rf $(EXECDIR)/$(DIRNAME)
	@rm -rf $(CONFDIR)
	@rm -rf $(SOCKDIR)/$(DIRNAME)*
	@# IF INSTALLATION WAS DONE UNPREFIXED TO /etc, the service will be disabled 
	$(shell if [ -z "$(PREFIX)" ]; then $(SERVICE_STOP); fi)
