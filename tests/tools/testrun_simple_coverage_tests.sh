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
#       Content         Count functions in folder $1/src against their
#                       counterparts in $1/test/unit.
#
#       Description     Counting function names against test function names 
#                       using following simple coding convention: 
#               
#                       "each function x_func will have a corresponding 
#                       test function with name test_x_func"
#
#                       In addition a logfile for each run will be created 
#                       at build/test/log/coverage.<time>.log
#
#       Authors         Markus Toepfer
#
#       Usage           ./testrun_simple_coverage_tests.sh /path/to/project
#
#       Note            This simple coverage test just covers the observance 
#                       of the coding convention, nothing else. 
#                       
#       Dependencies	bash, ctags, awk, sed, grep
#   
#       -----------------------------------------------------------------------

echo "-------------------------------------------------------"
echo "              SIMPLE COVERAGE TESTING"
echo "-------------------------------------------------------"

current_time=$(date "+%Y.%m.%d-%H.%M.%S")
PREFIX="test_"

LIBDIR=$1
echo $LIBDIR

logfileName="$LIBDIR/build/test/log/coverage_".$current_time."log"
echo "logfile "$logfileName
echo $LIBDIR

# generate the ctags
cd $LIBDIR/src
if [ $? -ne 0 ]; then
	exit 1
fi
ctags --c-types=f -R
# remove the ctags stuff, to leave just the function lines
sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions
# remove anything but the function names
awk '{print $1 }' $LIBDIR/src/functions > \
        $LIBDIR/src/functionNames
# count the lines
sourceFkt="$(cat functions | wc -l)"

echo "   count source " $sourceFkt >> $logfileName

# generate the ctags
cd $LIBDIR/tests/unit
if [ $? -ne 0 ]; then
	exit 1
fi
ctags --c-types=f -R
# remove the ctags stuff, to leave just the function lines
sed -e '/[ ]*m$/d' -e '/TAG/d' <tags>functions
# remove anything but the function names
awk '{print $1 }' $LIBDIR/tests/unit/functions > \
        $LIBDIR/tests/unit/functionNames
# count the lines
testFkt="$(cat functions | grep -i ^$PREFIX | wc -l)"

echo "   count tests  " $testFkt >> $logfileName

echo "\nUNTESTED: " >> $logfileName
# Found functions:
while read line;
do
        grep -n '^test_'$line'$' $LIBDIR/tests/unit/functionNames > \
        /dev/null || echo $line >> $logfileName
done < $LIBDIR/src/functionNames

if [ $sourceFkt != 0 ]; then
        echo "............................................"  >> $logfileName
        echo "COVERAGE: $sourceFkt $testFkt" | \
        awk '{ printf $1 " %.2f %% \n", $3/$2*100}' >> $logfileName
fi

cat $logfileName
echo "-------------------------------------------------------"
echo ""

# cleanup remove the files we created
rm $LIBDIR/src/tags
rm $LIBDIR/src/functions
rm $LIBDIR/src/functionNames
rm $LIBDIR/tests/unit/tags
rm $LIBDIR/tests/unit/functions
rm $LIBDIR/tests/unit/functionNames