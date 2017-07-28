#!/usr/bin/env bash
set -ev

source ci/travis/lua_env.sh

# add any rocks required for ci_tests to this list
# lua-curl depends on a libcurl development package (i.e. libcurl4-openssl-dev)
ROCKS=(lua-curl busted)

for ROCK in ${ROCKS[*]}
do
  $LUAROCKS install $ROCK
done

