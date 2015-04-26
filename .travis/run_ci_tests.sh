#!/bin/bash
set -ev

source .travis/lua_env.sh

CI_TEST_DIRS=$(ls -d test/ci_test*)

for DIR in $CI_TEST_DIRS
do
  echo starting tests in $DIR
  SCRIPTS=$(ls -f $DIR/*test*.lua)
  for SCRIPT in $SCRIPTS
  do
    echo starting test script $SCRIPT
    lunit.sh $SCRIPT
    #lua $SCRIPT
  done
done


