#       ------------------------------------------------------------------------
#
#       Copyright 2017 Markus Toepfer, Michael Beer
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
#       Authors         Michael Beer
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
#                       (5) general generic makefiles used
#
#       Usage           make
#
#       Dependencies    make & compiler
#
#       Last changed    2018-07-06
#       ------------------------------------------------------------------------

CC = gcc

PROJECT         := testrun_lib
VERSION         := 1.0.1

# project path recalculation (if used included from parent make)
PROJECTMK       := $(abspath $(lastword $(MAKEFILE_LIST)))

# A base directory for installation may be set using a prefix,
# PREFIX definition may be inline within this script or may be
# done using an external SHELL variable $PREFIX
# The prefix MUST be an absolute PATH 

#PREFIX 		:= /inline/example

# PREFIXED non Standard PATH package config files,
# This implementation will force to use the prefixed 
# version instead of a parallel installed pkg-config version.
export PKG_CONFIG_PATH = $(PREFIX)/usr/local/lib/pkgconfig

# LIBS USED (uncommented example includes)
# NOTE for pkg-confdig custom libs use --cflags to include the 
# cflags defined within the pkg-config configuration
#LIBS           += `pkg-config --cflags --libs uuid`

# EXTRA CFLAGS (examples)
MODCFLAGS       += -std=c11 
MODCFLAGS 		+= -D _DEFAULT_SOURCE 
MODCFLAGS 		+= -D _POSIX_C_SOURCE=200809
MODCFLAGS       += -g
MODCFLAGS       += -fopenmp

# EXTRA LFLAGS (example)
#MODLFLAGS      += -pthread

# additional CFLAGS e.g. GCC only flags
MODCFLAGS      	+= -rdynamic

# TMP FILE DEFINITION
TESTS_TMP_FILES = $(wildcard /tmp/test_*)

# INCLUDE BASE MAKEFILE
include makefile_general.mk
