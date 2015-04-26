#!/bin/bash
set -ev

export LUAROCKS=.travis/lua/bin/luarocks
export PATH=$($LUAROCKS path --bin):$PATH
export LUA_PATH=$($LUAROCKS path --lr-path)
export LUA_CPATH=$($LUAROCKS path --lr-cpath)


