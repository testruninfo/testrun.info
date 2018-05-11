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
#       Authors         Markus Toepfer, Michael Beer
#       Date            2018-02-09
#
#       Project         testrun_gcov.sh
#
#       Description     Run gcov based coverage tests on all test cases.
#
#       Usage           ./testrun_gcov.sh /path/to/project
#
#       Dependencies    bash, gcov
#
#       Last changed    2018-02-09
#       ------------------------------------------------------------------------

#       ------------------------------------------------------------------------
#       CONFIGURE SCRIPT BASED ON PRESET VARIBALES OR ON INPUT
#       ------------------------------------------------------------------------

start_time=$(date "+%Y.%m.%d-%H.%M.%S")

LOGFOLDER="build/tests/log"

# SET A LOGFILE
LOGFILE=$LOGFOLDER"/gcov_".$start_time."log"
echo " (log)   $start_time" > $LOGFILE
touch $LOGFILE
chmod a+w $LOGFILE

# SET THE FOLDER
FOLDER="build/tests/unit"
SRC_FOLDER="tests/unit"

echo "-------------------------------------------------------"
echo "               GCOV RUNNER"
echo "-------------------------------------------------------"

# RUN THE RUNNER
#sh ./tests/./tools/testrun_runner.sh  $LOGFILE $FOLDER FAIL_ON_ERROR
#RESULT=$?

for test in $FOLDER/*.test; do
    $test
done

FILES=`ls  $FOLDER/ | grep ".test" | wc -l`
if [ $? -ne 0 ]; then
        echo "ERROR ... could not count files of $FOLDER"
        exit 1
fi
c=0

if [ $FILES -eq 0 ]; then
        exit 0
fi

for i in $SRC_FOLDER/*_tests.c
do
        # RUN GCOV
        echo $i
    	gcov $i
done

# move coverage to build/tests/logs
mv *.gcov $LOGFOLDER
exit 0

echo "-------------------------------------------------------"
echo ""
