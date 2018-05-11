#       ------------------------------------------------------------------------
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
#       Project         [PROJECT]
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
#                           sources MUST be located at src/
#                           inludes MUST be located at include/
#                           tests   MUST be located at tests/
#                           build   MUST be located at build/
#
#                       ALL TEST SCRIPTS MAY BE EXCHANGED WITH CUSTOM RUNNERS
#
#       Usage           SHOULD be used included by parent makefile
#
#       Dependencies    testrun (makefile & service scripts), doxygen (if used)
#
#       Last changed    2018-02-18
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

INCDIR			:= $(PREFIX)/include/$(DIRNAME)
LIBDIR			:= $(PREFIX)/lib
EXECDIR			:= $(PREFIX)/bin

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
TESTS_SOURCES	= $(wildcard tests/**/*_tests.c tests/*_tests.c)
TESTS_TARGET	= $(patsubst tests/%.c, build/tests/%.test, $(TESTS_SOURCES))

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

# ----- DEFAULT MAKE RULES -------------------------------------------------

%.o : %.c $(HEADERS)
	@echo " (CC)    $@"
	@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)

%_tests.o : %_tests.c
	@echo " (CC)    $@"
	@$(CC) $(CFLAGS) -o $@ -c $< $(LIBS)

all:			all_exec
install:		install_exec
uninstall:		uninstall_exec

all_lib:		start lib tests pkgconfig done
all_exec:		start lib tests $(EXECUTABLE) done
all_service:		all_exec

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
	@rm -rf build bin doxygen/documentation $(OBJECTS) $(TESTS_OBJECTS) \n		$(LIBNAMEPC) $(TESTS_TMP_FILES) $(GCOV_FILES) *.gcov *.profile *.pc *.out


# ----- DOCUMENATION -------------------------------------------------------

#NOTE requires doxygen.PHONY: documentation
documentation:
	doxygen ./doxygen/doxygen.config

# ----- INFORMATION PRINTING -----------------------------------------------

# print out a variable of the make file (e.g. "make print-PROJECTPATH")
.PHONY: print
print-%  : ; @echo $* = $($*)

.PHONY: start
start:
	@echo "\n (HINT)    $(PROJECT) 		 ==> running make\n"

.PHONY: done
done:
	@echo
	@echo " (DONE)  make $(PROJECT)"
	@echo " (HINT)  with unit testing      ==> 'make tested'"
	@echo " (HINT)  perform installation   ==> 'sudo make install\n"
	@echo " (HINT)  generate documentation ==> 'make documentation\n"


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

# build a specific executable test (testname) under build/tests
# NOTE: May be usefull for module development in large projects
test:
	@echo " (CC)    $(testname)"
	@$(CC) $(CFLAGS) $(LFLAGS) $(patsubst build/tests/%.test, \
		tests/%.c,$(testname)) -ldl $(STATIC) -Wl,-rpath=$(RPATH) -g -o\
		$(patsubst tests/%.c,build/tests/%.test,$(testname)) $(LIBS)

# TESTRUN runners ----------------------------------------------------------

# ACCEPTANCE TEST script invocation
.PHONY: testrun-acceptance
testrun-acceptance:
	sh tests/tools/testrun_simple_acceptance_tests.sh

# UNIT TEST script invocation
.PHONY: testrun-unit
testrun-unit:
	sh tests/tools/testrun_simple_unit_tests.sh

# COVERAGE TEST script invocation
.PHONY: testrun-coverage
testrun-coverage:
	sh tests/tools/testrun_simple_coverage_tests.sh $(PROJECTPATH)

# LOC TEST script invocation
.PHONY: testrun-loc
testrun-loc:
	sh tests/tools/testrun_simple_loc.sh $(PROJECTPATH)

# TESTRUN all scripts
.PHONY: testrun
testrun:
	@echo " (HINT)  $(PROJECT) \t\t\t==> running tests\n"
	sh tests/tools/testrun_simple_unit_tests.sh
	sh tests/tools/testrun_simple_acceptance_tests.sh
	sh tests/tools/testrun_simple_coverage_tests.sh $(PROJECTPATH)
	sh tests/tools/testrun_simple_loc.sh $(PROJECTPATH)

# TESTRUN gcov -------------------------------------------------------------

testrun-gcov: clean
	make USE_GCOV=1 all
	sh tests/tools/testrun_gcov.sh $(PROJECTPATH)

# TESTRUN gprof ------------------------------------------------------------

testrun-gprof: clean
	make USE_GPROF=1 all
	sh tests/tools/testrun_gprof.sh $(PROJECTPATH)

# ----- PKGCONFIG LIBRARY BUILD --------------------------------------------

.PHONY: pkgconfig
pkgconfig:
	@echo 'prefix='$(PREFIX)                        >  $(LIBNAMEPC)
	@echo 'exec_prefix=$${prefix}'                  >> $(LIBNAMEPC)
	@echo 'libdir=$${prefix}/lib' 					>> $(LIBNAMEPC)
	@echo 'includedir=$${prefix}/include' 			>> $(LIBNAMEPC)
	@echo ''                                        >> $(LIBNAMEPC)
	@echo 'Name: '$(LIBNAME)                        >> $(LIBNAMEPC)
	@echo 'Description: '                           >> $(LIBNAMEPC)
	@echo 'Version: '$(VERSION)                     >> $(LIBNAMEPC)
	@echo 'URL: http://testrun.info'                >> $(LIBNAMEPC)
	@echo 'Libs: -L$${libdir} -l'$(DIRNAME)         >> $(LIBNAMEPC)
	@echo 'Cflags: -I$${includedir}'                >> $(LIBNAMEPC)

# ----- INSTALLATION -------------------------------------------------------

# Installation as a library ------------------------------------------------

install_lib: $(SHARED) $(STATIC)
	@echo " (OK)    installed $(LIBNAME) to $(LIBDIR)"
	@mkdir -p $(LIBDIR)/pkgconfig
	@mkdir -p $(INCDIR)
	@$(INSTALL) -m 0644 -t $(INCDIR) $(shell find include -name "*.h")
	@$(INSTALL) -m 0755 $(SHARED) $(LIBDIR)
	@$(INSTALL) -m 0755 $(STATIC) $(LIBDIR)
	@$(INSTALL) -m 0644 $(LIBNAMEPC) $(LIBDIR)/pkgconfig
	@ldconfig

uninstall_lib:
	@echo " (OK)    uninstalled $(LIBNAME) from $(LIBDIR)"
	@rm -rf $(INCDIR)
	@rm -rf $(LIBDIR)/$(LIBNAME).a
	@rm -rf $(LIBDIR)/$(LIBNAME).so
	@rm -rf $(LIBDIR)/pkgconfig/$(LIBNAMEPC)

# Installation as an executable --------------------------------------------

install_exec: $(SHARED) $(STATIC)
	@echo " (OK)    installed $(DIRNAME) to $(EXECDIR)"
	@$(INSTALL) -m 0755 bin/$(DIRNAME) $(EXECDIR)

uninstall_exec:
	@echo " (OK)    uninstalled $(DIRNAME) from $(EXECDIR)"
	@rm -rf $(EXECDIR)/$(DIRNAME)

# Installation as a service (outsourced to script)--------------------------

install_service: $(EXECUTABLE)
	./config/install/install.sh

uninstall_service:
	./config/install/uninstall.sh
