#!/usr/bin/env bash
#
#       -----------------------------------------------------------------------
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
#       This file is part of the testrun project. http://testrun.info
#       
#       -----------------------------------------------------------------------
#       
#       Content         Count the lines of $1/src and $1/tests/unit 
#                       and print it to STDOUT.
#       
#       Authors         Markus Toepfer
#       
#       Usage           ./testrun_simple_loc.sh /path/to/project
#       
#       Note            This files uses no error checking mechanism.
#       
#       Dependencies	bash, find, xargs, wc
#   
#   --------------------------------------------------------------------------

echo "-------------------------------------------------------"
echo "               SIMPLE LOC COUNTER"
echo "-------------------------------------------------------"
echo "(LOC) src"
find $1/src -name '*.c' | xargs wc -l
echo "(LOC) tests/unit"
find $1/tests/unit -name '*.c' | xargs wc -l
echo "(LOC) tests/acceptance"
find $1/tests/acceptance -name '*.c' | xargs wc -l
echo "-------------------------------------------------------"
echo ""