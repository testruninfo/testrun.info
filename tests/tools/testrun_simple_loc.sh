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
#       File            testrun_simple_loc.sh
#       Authors         Markus Toepfer
#       Date            2017-11-30
#
#       Project         testrun.info
#
#       Description     Count the lines of header, src and tests.
#                       This file uses no error checking.
#
#       Usage           ./testrun_simple_loc.sh /path/to/project
#
#       Dependencies    bash, find, xargs, wc
#
#       Last changed    2018-07-11
#       ------------------------------------------------------------------------

FOLDER_INC="include"
FOLDER_SRC="src"
FOLDER_TST="tests/unit"
echo "-------------------------------------------------------"
echo "               SIMPLE LOC COUNTER"
echo "-------------------------------------------------------"
echo ""
echo "(LOC) HEADER"
find $1/$FOLDER_INC -name '*.h' | xargs wc -l
echo ""
echo "(LOC) SOURCE"
find $1/$FOLDER_SRC -name '*.c' | xargs wc -l
echo ""
echo "(LOC) TESTS"
find $1/$FOLDER_TST -name '*.c' | xargs wc -l
echo ""
echo "-------------------------------------------------------"
echo ""
