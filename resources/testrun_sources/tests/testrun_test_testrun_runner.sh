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
#       Content         TEST of testrun_runner.sh
#                       
#       Authors         Markus Toepfer
#
#       Description     Test the returns of the testrun_runner.sh script
#                       
#       Authors         Markus Toepfer
#   
#       -----------------------------------------------------------------------

SCRIPT_DIR=".."
MOCKUP_DIR_SUCCESS=$SCRIPT_DIR"/tests/mockups/success/"
MOCKUP_DIR_ERROR=$SCRIPT_DIR"/tests/mockups/error/"
MOCKUP_DIR_MIXED=$SCRIPT_DIR"/tests/mockups/mixed/"

# MUST be the same in testrun_runner.sh
LOGFILE="./testrun_tmp_logfile"
touch $LOGFILE
chmod a+w $LOGFILE

#       -----------------------------------------------------------------------
#       CASE 1          NOTHING SPECIFIED
#       -----------------------------------------------------------------------
#       
#       EXPECT          fail because no logfile is provided
#       
#       -----------------------------------------------------------------------

sh ./$SCRIPT_DIR/testrun_runner.sh
if [ $? -ne 1 ]; then
        echo "TEST 1 FAILED"
        exit 1
else
        echo "TEST 1 SUCCESS"
fi

#       -----------------------------------------------------------------------
#       CASE 2          LOGFILE NO FOLDER
#       -----------------------------------------------------------------------
#       
#       EXPECT          fail because no folder is provided
#       
#       -----------------------------------------------------------------------

sh ./$SCRIPT_DIR/testrun_runner.sh $LOGFILE
if [ $? -ne 1 ]; then
        echo "TEST 2 FAILED"
        exit 1
else
        echo "TEST 2 SUCCESS"
fi

#       -----------------------------------------------------------------------
#       CASE 3          LOGFILE AND FOLDER
#       -----------------------------------------------------------------------
#       
#       EXPECT          success
#       
#       -----------------------------------------------------------------------

sh ./$SCRIPT_DIR/testrun_runner.sh $LOGFILE $MOCKUP_DIR_SUCCESS
if [ $? -ne 0 ]; then
        echo "TEST 3 FAILED"
        exit 1
else
        echo "TEST 3 SUCCESS"
fi

RESULT=$(sh ./$SCRIPT_DIR/testrun_runner.sh \
                $LOGFILE $MOCKUP_DIR_SUCCESS | wc -l)
if [ $RESULT != 3 ]; then
        echo "TEST 3 wrong amount of tests"
        exit 1
fi

#       -----------------------------------------------------------------------
#       CASE 4          LOGFILE AND FOLDER      (no tests)
#       -----------------------------------------------------------------------
#       
#       EXPECT          success
#       
#       -----------------------------------------------------------------------

sh ./$SCRIPT_DIR/testrun_runner.sh $LOGFILE $SCRIPT_DIR
if [ $? -ne 0 ]; then
        echo "TEST 4 FAILED"
        exit 1
else
        echo "TEST 4 SUCCESS"
fi

RESULT=$(sh ./$SCRIPT_DIR/testrun_runner.sh \
                $LOGFILE $SCRIPT_DIR | wc -l)
if [ $RESULT != 0 ]; then
        echo "TEST 4 wrong amount of tests"
        exit 1
fi

#       -----------------------------------------------------------------------
#       CASE 5          LOGFILE AND FOLDER      (error)
#       -----------------------------------------------------------------------
#       
#       EXPECT          success
#       
#       -----------------------------------------------------------------------

sh ./$SCRIPT_DIR/testrun_runner.sh $LOGFILE $MOCKUP_DIR_ERROR
if [ $? -ne 0 ]; then
        echo "TEST 5 FAILED"
        exit 1
else
        echo "TEST 5 SUCCESS"
fi

RESULT=$(sh ./$SCRIPT_DIR/testrun_runner.sh \
                $LOGFILE $MOCKUP_DIR_ERROR | wc -l)
if [ $RESULT != 3 ]; then
        echo "TEST 5 wrong amount of tests"
        exit 1
fi

#       -----------------------------------------------------------------------
#       CASE 6          LOGFILE AND FOLDER      (mixed)
#       -----------------------------------------------------------------------
#       
#       EXPECT          success
#       
#       -----------------------------------------------------------------------

sh ./$SCRIPT_DIR/testrun_runner.sh $LOGFILE $MOCKUP_DIR_MIXED
if [ $? -ne 0 ]; then
        echo "TEST 6 FAILED"
        exit 1
else
        echo "TEST 6 SUCCESS"
fi

RESULT=$(sh ./$SCRIPT_DIR/testrun_runner.sh \
                $LOGFILE $MOCKUP_DIR_MIXED | wc -l)
if [ $RESULT != 5 ]; then
        echo "TEST 6 wrong amount of tests"
        exit 1
fi



#       -----------------------------------------------------------------------
#       CASE 7          LOGFILE AND FOLDER (success) FAIL_ON_ERROR
#       -----------------------------------------------------------------------
#       
#       EXPECT          success
#       
#       -----------------------------------------------------------------------

sh ./$SCRIPT_DIR/testrun_runner.sh $LOGFILE $MOCKUP_DIR_SUCCESS FAIL_ON_ERROR
if [ $? -ne 0 ]; then
        echo "TEST 7 FAILED"
        exit 1
else
        echo "TEST 7 SUCCESS"
fi

RESULT=$(sh ./$SCRIPT_DIR/testrun_runner.sh \
                $LOGFILE $MOCKUP_DIR_SUCCESS FAIL_ON_ERROR | wc -l)
if [ $RESULT != 3 ]; then
        echo "TEST 7 wrong amount of tests"
        exit 1
fi

#       -----------------------------------------------------------------------
#       CASE 8          LOGFILE AND FOLDER      (no tests) FAIL_ON_ERROR
#       -----------------------------------------------------------------------
#       
#       EXPECT          success
#       
#       -----------------------------------------------------------------------

sh ./$SCRIPT_DIR/testrun_runner.sh $LOGFILE $SCRIPT_DIR
if [ $? -ne 0 ]; then
        echo "TEST 8 FAILED"
        exit 1
else
        echo "TEST 8 SUCCESS"
fi

RESULT=$(sh ./$SCRIPT_DIR/testrun_runner.sh \
                $LOGFILE $SCRIPT_DIR FAIL_ON_ERROR | wc -l)
if [ $RESULT != 0 ]; then
        echo "TEST 8 wrong amount of tests"
        exit 1
fi

#       -----------------------------------------------------------------------
#       CASE 9          LOGFILE AND FOLDER      (error) FAIL_ON_ERROR
#       -----------------------------------------------------------------------
#       
#       EXPECT          error
#       
#       -----------------------------------------------------------------------

sh ./$SCRIPT_DIR/testrun_runner.sh $LOGFILE $MOCKUP_DIR_ERROR 1
if [ $? -ne 1 ]; then
        echo "TEST 9 FAILED"
        exit 1
else
        echo "TEST 9 SUCCESS"
fi

RESULT=$(sh ./$SCRIPT_DIR/testrun_runner.sh \
                $LOGFILE $MOCKUP_DIR_ERROR 1 | wc -l)
echo $RESULT
if [ $RESULT != 1 ]; then
        echo "TEST 9 wrong amount of tests"
        exit 1
fi

#       -----------------------------------------------------------------------
#       CASE 10          LOGFILE AND FOLDER      (mixed) FAIL_ON_ERROR
#       -----------------------------------------------------------------------
#       
#       EXPECT          error
#       
#       -----------------------------------------------------------------------

sh ./$SCRIPT_DIR/testrun_runner.sh $LOGFILE $MOCKUP_DIR_MIXED FAIL_ON_ERROR
if [ $? -ne 1 ]; then
        echo "TEST 10 FAILED"
        exit 1
else
        echo "TEST 10 SUCCESS"
fi

RESULT=$(sh ./$SCRIPT_DIR/testrun_runner.sh \
                $LOGFILE $MOCKUP_DIR_MIXED 1 | wc -l)
if [ $RESULT != 3 ]; then
        echo "TEST 10 wrong amount of tests"
        exit 1
fi