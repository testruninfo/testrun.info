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
#       Content         Run all *.test executables at $1
#
#       Description     Run each test.test of a folder and log 
#                       OK or NOK for each executed test with a counter. 
#                       
#                       e.g. 
#                       
#                       [OK]  1/5 filename.test
#                       [NOK] 2/5 filename.test
#                       
#                       The testrun may be used in 3 MODES
#                       
#       MODE 1          RUN ALL TESTS (log but ignore errors)
#                       invoke script using 2 parameter 
#                       
#                       **testrun_runner.sh logfile dir**
#                       exit with 0
#                       
#       MODE 2          FAIL_ON_ERROR (Fail on first error)
#                       invoke script using 3 parameter 
#                       
#                       **testrun_runner.sh logfile dir 1**
#                       exit with 1
#                       
#       Authors         Markus Toepfer
#
#       Usage           ./testrun_run.sh /path/logfile /path/testdir
#                       
#       Dependencies    bash, tail, ls, grep, wc
#   
#       -----------------------------------------------------------------------
#       CONFIGURE SCRIPT BASED ON PRESET VARIBALES OR ON INPUT 
#       -----------------------------------------------------------------------

if [ -z $1 ]; then
        echo "INPUT ERROR ... NO LOGFILE INPUT TO SRCIPT"
        exit 1
fi
LOGFILE=$1

if [ -z $2 ]; then
        echo "INPUT ERROR ... FOLDER INPUT TO SRCIPT"
        exit 1
fi
FOLDER=$2

FAIL_ON_ERROR=0
if [ ! -z $3 ]; then
        FAIL_ON_ERROR=1  
fi

if [ ! -w $LOGFILE ]; then
        echo "INPUT ERROR ... LOGFILE NOT WRITABLE"
        exit 1
fi

#       -----------------------------------------------------------------------
#       PERFORM TESTRUN
#       -----------------------------------------------------------------------

FILES=`ls  $FOLDER/ | grep "\.test" | wc -l`
if [ $? -ne 0 ]; then
        echo "INPUT ERROR ... could not count files of $FOLDER"
        exit 1
fi
c=0

if [ $FILES -eq 0 ]; then
        exit 0
fi

for i in $FOLDER/*.test
do
        c=$((c+1))

        # RUN EXECUTABLE
        $i 2>&1 >> $LOGFILE
        
        # CHECK RETURN OF EXECUTABLE
        if [ $? -ne 0 ]; then

                echo "NOK\t("$c"/"$FILES")\t"$i

                if [ $FAIL_ON_ERROR -eq 1 ]; then
                        exit 1  
                fi 
        else 
                echo "OK\t("$c"/"$FILES")\t"$i
        fi
done
exit 0