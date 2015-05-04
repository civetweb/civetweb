#!/bin/bash
set -ev

source ci/travis/lua_env.sh
busted -o TAP ci/test/


