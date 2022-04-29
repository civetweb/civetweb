#!/bin/bash


if test -f "./unittest/build_and_test_default.sh" ; then
  echo "Unit test (default settings) will run on a local Debian system."
  rm -rf output
else
  echo "This script must be started from the civetweb root directory using ./unittest/build_and_test_default.sh"
  exit
fi


# Exit with an error message when any command fails.
set -e
trap 'lastline=$thisline; thisline=$BASH_COMMAND' DEBUG
trap 'echo "ERROR: \"${lastline}\" command failed (error $?)"' EXIT


# Create a test directory and add the CGI test executable
if test -f "./output/" ; then
  echo "Removing old \"output\" folder."
  rm -rf output
fi
mkdir output
gcc unittest/cgi_test.c -o output/cgi_test.cgi
cd output


# Perform build and test steps in "output" directory.
echo "Starting unit test. Write protocol to \"unittest.log\" file."
echo "Starting unit test" > unittest.log
git log -1 >> unittest.log
cmake .. &>> unittest.log
make all &>> unittest.log
make test &>> unittest.log
echo "Unit test completed. See \"unittest.log\" file."
tail -10 unittest.log | grep " tests passed"
cd ..


# Exit with success
trap '' EXIT
exit 0
