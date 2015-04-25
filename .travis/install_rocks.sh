#!/bin/bash

set -e

LUAROCKS=$TRAVIS_BUILD_DIR/build/lua/bin/luarocks
PATH=$($LUAROCKS path --bin):$PATH
LUA_PATH=$($LUAROCKS path --lr-path)
LUA_CPATH=$($LUAROCKS path --lr-cpath)

$LUAROCKS install lunitx
$LUAROCKS install lua-curl


