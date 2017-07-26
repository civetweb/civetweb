#!/usr/bin/env /bash
set -ev

# this script installs a lua / luarocks environment in .travis/lua
# this is necessary because travis docker architecture (the fast way)
# does not permit sudo, and does not contain a useful lua installation

# After this script is finished, you can configure your environment to
# use it by sourcing lua_env.sh

source ci/travis/platform.sh

# The current versions when this script was written
LUA_VERSION=5.2.4
LUAROCKS_VERSION=2.2.2

# directory where this script is located
SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

# civetweb base dir
PROJECT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )"/../.. && pwd )

# fetch and unpack lua src
cd $SCRIPT_DIR
LUA_BASE=lua-$LUA_VERSION
rm -rf $LUA_BASE
curl http://www.lua.org/ftp/$LUA_BASE.tar.gz | tar zx

# build lua
cd $LUA_BASE
make $PLATFORM
make local

# mv built lua install to target Lua dir
LUA_DIR=$PROJECT_DIR/ci/lua
rm -rf $LUA_DIR
mv $SCRIPT_DIR/$LUA_BASE/install $LUA_DIR

# add to path required by luarocks installer
export PATH=$LUA_DIR/bin:$PATH


# fetch and unpack luarocks
cd $SCRIPT_DIR
LUAROCKS_BASE=luarocks-$LUAROCKS_VERSION
rm -rf ${LUAROCKS_BASE}
LUAROCKS_URL=http://luarocks.org/releases/${LUAROCKS_BASE}.tar.gz
# -L because it's a 302 redirect
curl -L $LUAROCKS_URL | tar xzp
cd $LUAROCKS_BASE

# build luarocks
./configure --prefix=$LUA_DIR
make build
make install

# cleanup source dirs
cd $SCRIPT_DIR
rm -rf $LUAROCKS_BASE
rm -rf $LUA_BASE

