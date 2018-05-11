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
#       File            makefile
#       Authors         Markus Toepfer
#       Date            2018-02-09
#
#       Project         testrun_lib
#
#       Description     This makefile defines project specific parameter.
#
#                       These parameter are:
#                       (1) used compiler and special flags
#                       (2) name and version
#                       (3) installation prefix
#                       (4) used libraries
#                       (5) general makefiles used
#
#       Usage           make
#
#       Dependencies    make & compiler
#
#       Last changed    2018-02-09
#       ------------------------------------------------------------------------

CC = gcc

PROJECT         := testrun_lib
VERSION         := 1.0.0

# project path recalculation (if used included from parent make)
PROJECTMK       := $(abspath $(lastword $(MAKEFILE_LIST)))

# base directory for installation
PREFIX          := /usr/local

# LIBS USED (uncommented example includes)
#LIBS           += `pkg-config --libs uuid`

# EXTRA CFLAGS (example)
MODCFLAGS       += -std=gnu11
MODCFLAGS       += -fopenmp

# EXTRA LFLAGS (example)
#MODLFLAGS      += -pthread

# additional CFLAGS e.g. GCC only flags
MODCFLAGS      	+= -rdynamic

# TMP FILE DEFINITION
TESTS_TMP_FILES = $(wildcard /tmp/test_*)

# INCLUDE BASE MAKEFILE
include makefile_general.mk
