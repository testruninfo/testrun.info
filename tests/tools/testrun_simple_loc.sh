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
#       File            testrun_simpe_coverage_tests.sh
#       Authors         Markus Toepfer
#       Date            2017-11-30
#
#       Project         testrun_simpe_coverage_tests.sh
#
#       Description     Count the lines of src and unit | acceptance tests.
#                       This file uses no error checking.
#
#       Usage           ./testrun_simple_loc.sh /path/to/project
#
#       Dependencies    bash, find, xargs, wc
#
#       Last changed    2017-11-30
#       ------------------------------------------------------------------------

echo "-------------------------------------------------------"
echo "               SIMPLE LOC COUNTER"
echo "-------------------------------------------------------"
echo "(LOC) src"
find $1/./src -name '*.c' | xargs wc -l
echo "(LOC) tests/unit"
find $1/./tests/unit -name '*.c' | xargs wc -l
echo "(LOC) tests/acceptance"
find $1/./tests/acceptance -name '*.c' | xargs wc -l
echo "-------------------------------------------------------"
echo ""
