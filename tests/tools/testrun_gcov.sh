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
#       Description     Run gcov based coverage tests on all test cases.
#
#       Usage           ./testrun_gcov.sh /path/to/project
#
#       Dependencies    bash, gcov
#
#       Last changed    2018-07-11
#       ------------------------------------------------------------------------

start_time=$(date "+%Y.%m.%d-%H.%M.%S.%N")

FOLDER_TEST_EXEC="build/tests/unit"
FOLDER_TEST_SRC="tests/unit"
FOLDER_LOGFILE="build/tests/log"
TEST_EXEC_SUFFIX=".test"
TEST_SRC_SUFFIX="_tests"

# SET A LOGFILE
LOGFILE="$FOLDER_LOGFILE/gcov.$start_time.log"
touch $LOGFILE
chmod a+w $LOGFILE
echo " (log)   $start_time" > $LOGFILE

echo "-------------------------------------------------------" >> $LOGFILE
echo "               GCOV RUNNER" >> $LOGFILE
echo "-------------------------------------------------------" >> $LOGFILE

for test in $FOLDER_TEST_EXEC/*$TEST_EXEC_SUFFIX; do
    $test
done

FILES=`ls  $FOLDER_TEST_EXEC/ | grep $TEST_EXEC_SUFFIX | wc -l`
if [ $? -ne 0 ]; then
        echo "ERROR ... could not count files of $FOLDER_TEST_EXEC"
        exit 1
fi
c=0

if [ $FILES -eq 0 ]; then
        exit 0
fi

for i in $FOLDER_TEST_SRC/*$TEST_SRC_SUFFIX.c
do
        # RUN GCOV
        echo $i
        gcov $i
done

# move coverage output to log folder
mv *.gcov $FOLDER_LOGFILE
exit 0

echo "-------------------------------------------------------"
echo ""
