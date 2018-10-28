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
#       File            makefile
#       Authors         Markus Toepfer
#       Authors         ...
#       Date            2018-02-18
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
#       Last changed    2018-07-12
#       ------------------------------------------------------------------------

CC = gcc

PROJECT			:= testrun_lib
VERSION			:= 1.1.2

# project path recalculation (if used included from a parent make)
PROJECTMK		:= $(abspath $(lastword $(MAKEFILE_LIST)))

# prefix for base directory for installation (default is /)
#PREFIX			:= some_path

# include all pkgconfig files available at PREFIX
export PKG_CONFIG_PATH = $(PREFIX)/usr/local/lib/pkgconfig

# LIBS USED (uncommented example includes)
# ... will allow to include libs installed under PREFIX
#LIBS			+= `pkg-config --cflags --libs libtestrun.info`
#LIBS			+= `pkg-config --libs libsystemd`
#LIBS			+= `pkg-config --libs uuid`
#LIBS			+= `pkg-config --libs openssl`

# MODULE BASED CFLAGS (example)
MODCFLAGS		+= -std=c11 -D _DEFAULT_SOURCE -D _POSIX_C_SOURCE=200809 

# EXTRA CFLAGS (example parallel or other GCC custom flags)
MODCFLAGS		+= -fopenmp
MODCFLAGS		+= -g
#MODCFLAGS		+= -rdynamic

# EXTRA LFLAGS (example)
#MODLFLAGS		+= -pthread

# PKG_CONFIG_DATA (used during LIBRARY install)
PROJECT_URL		= "http://testrun.info"
PROJECT_DESC		= "A small library to build testrunner frameworks."

# SERVICE_CONFIG_DATA
SERVICE_DATA		= "config"

# TMP FILE DEFINITION
TESTS_TMP_FILES	= $(wildcard /tmp/test_*)

# INCLUDE BASE MAKEFILE
include makefile_general.mk
