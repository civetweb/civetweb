#!/bin/bash
set -ev

source .travis/lua_env.sh
busted -o TAP test/ci_tests/


