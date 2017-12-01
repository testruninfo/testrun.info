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
#       Date            2017-12-01
#
#       Project         makefile
#
#       Description     Count the lines of src and unit | acceptance tests.
#                       This file uses no error checking.
#
#       Usage           ./testrun_simple_loc.sh /path/to/project
#
#       Dependencies    bash, find, xargs, wc
#
#       Last changed    2017-12-01
#       ------------------------------------------------------------------------

CC = gcc

PROJECT         := [PROJECT]
VERSION         := 0.0.1

# project path recalculation (if used included)
PROJECTMK       := $(abspath $(lastword $(MAKEFILE_LIST)))

# base directory to install the lib
PREFIX          := /usr/local

# LIBS USED (uncommented example includes)
#LIBS           += `pkg-config --libs libsystemd`
#LIBS           += `pkg-config --libs uuid`
#LIBS           += `pkg-config --libs openssl`

# EXTRA CFLAGS
MODCFLAGS       += -std=gnu11
MODCFLAGS       += -fopenmp

#GCC only flags
MODCFLAGS       += -rdynamic

# INCLUDE BASE MAKEFILE
include testrun_makefile.main
include testrun_makefile.test
#include testrun_makefile.src
