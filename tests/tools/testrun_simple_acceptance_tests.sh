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
#       File            testrun_simple_acceptance_tests.sh
#       Authors         Markus Toepfer
#       Date            2017-11-30
#
#       Project         testrun_simple_acceptance_tests.sh
#
#       Description     Run all test executables build/test/acceptance/*.test
#                       Run the whole folder, until an error occurs.
#
#                       MODE         FAIL ON ERROR (Fail on first test error)
#
#                       LOGFILE      build/test/log/acceptancetest.<time>.log
#
#
#       Usage           ./testrun_simple_acceptance_tests.sh /path/to/project
#
#       Dependencies    bash, touch, chmod, ls, wc, date
#
#       Last changed    2017-11-30
#       ------------------------------------------------------------------------

echo "-------------------------------------------------------"
echo "               SIMPLE ACCEPTANCE TESTING"
echo "-------------------------------------------------------"

start_time=$(date "+%Y.%m.%d-%H.%M.%S.%N")

# SET A LOGFILE
LOGFILE="build/tests/log/acceptance_".$start_time."log"
echo " (log)   $start_time" > $LOGFILE
touch $LOGFILE
chmod a+w $LOGFILE

# SET THE FOLDER
FOLDER="build/tests/acceptance"

echo "-------------------------------------------------------" >> $LOGFILE
echo "               REPORT ACCEPTANCE TESTING"                >> $LOGFILE
echo "-------------------------------------------------------" >> $LOGFILE

# RUN THE RUNNER
sh ./tests/./tools/testrun_runner.sh  $LOGFILE $FOLDER FAIL_ON_ERROR
RESULT=$?

end_time=$(date "+%Y.%m.%d-%H.%M.%S.%N")

# FINISH THE REPORT
echo "-------------------------------------------------------">> $LOGFILE
echo "DONE \t ACCEPTANCE TEST RUN"  >> $LOGFILE
if [ $RESULT -eq 0 ]; then
        echo "RESULT\t SUCCESS"  >> $LOGFILE
else
        echo "RESULT\t FAILURE"  >> $LOGFILE
fi
echo "START \t $start_time" >> $LOGFILE
echo "END   \t $end_time" >> $LOGFILE
echo "-------------------------------------------------------">> $LOGFILE

# DUMP THE REPORT
cat $LOGFILE
echo ""
exit $RESULT
