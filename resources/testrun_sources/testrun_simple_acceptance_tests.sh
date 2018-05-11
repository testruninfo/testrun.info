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
#       This file is part of the testrun project. http://testrun.info
#
#       -----------------------------------------------------------------------
# 
#       Content         Run all test executables build/test/acceptance/*.test
#
#       Description     Run each test.test until an error occurs. If an error 
#                       occurs, skip remaining tests of the executable.
#               
#                       This is a fail on first error for each test.test.
#               
#                       In addition a logfile for each run will be created 
#                       at build/test/log/acceptancetest.<time>.log
#                       
#       Authors         Markus Toepfer
#
#       Usage           ./testrun_simple_acceptance_tests.sh /path/to/project
#                       
#       Dependencies	bash, touch, chmod, ls, wc, date
#   
#       -----------------------------------------------------------------------

echo "-------------------------------------------------------"
echo "               SIMPLE ACCEPTANCE TESTING"
echo "-------------------------------------------------------"
start_time=$(date "+%Y.%m.%d-%H.%M.%S.%N")

# SET A LOGFILE
LOGFILE="build/test/log/acceptance_".$start_time."log"
echo " (log)   "$LOGFILE
touch $LOGFILE
chmod a+w $LOGFILE

# SET THE FOLDER
FOLDER="build/test/acceptance"

sh tests/tools/testrun_runner.sh  $LOGFILE $FOLDER FAIL_ON_ERROR
RESULT=$?

echo ""
end_time=$(date "+%Y.%m.%d-%H.%M.%S.%N")

# PRINT THE REPORT (which is written to the LOGFILE)
echo "START \t $start_time" >> $LOGFILE
echo "END   \t $end_time" >> $LOGFILE
echo "REPORT-------------------------------------------------"
cat $LOGFILE
echo "-------------------------------------------------------"
echo ""

exit $RESULT