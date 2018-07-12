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
#       File            testrun_gcov.sh
#       Authors         Markus Toepfer
#       Authors         Michael Beer
#       Date            2018-02-09
#
#       Project         testrun.info
#
#       Description     Run gprof based analysis tests on all test cases.
#
#       Usage           ./testrun_gcov.sh /path/to/project
#
#       Dependencies    bash, gprof
#
#       Last changed    2018-07-11
#       ------------------------------------------------------------------------

start_time=$(date "+%Y.%m.%d-%H.%M.%S.%N")

FOLDER_TEST_EXEC="build/tests/unit"
FOLDER_LOGFILE="build/tests/log"
TEST_EXEC_SUFFIX=".test"

# SET A LOGFILE
LOGFILE="$FOLDER_LOGFILE/gprof.$start_time.log"
touch $LOGFILE
chmod a+w $LOGFILE
echo " (log)   $start_time" > $LOGFILE

echo "-------------------------------------------------------" >> $LOGFILE
echo "               GPROF RUNNER" >> $LOGFILE
echo "-------------------------------------------------------" >> $LOGFILE

# Execute the test once and profile the execution
for test in $FOLDER_TEST_EXEC/*$TEST_EXEC_SUFFIX; do
        name=${test##*/}        echo "Profiling" $name
    $test
    gprof $test gmon.out > $name.profile
done

# move profile to build/tests/logs
mv *.profile $FOLDER_LOGFILE
exit 0
