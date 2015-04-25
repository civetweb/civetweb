#! /bin/bash
set -e

source .travis/platform.sh

LUA_VERSION=5.2.4
LUAROCKS_VERSION=2.2.2

# directory where this script is located
SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
BUILD_DIR=${TRAVIS_BUILD_DIR:=$SCRIPT_DIR}/build
echo @BUILD_DIR = $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR

LUA_BASE=lua-$LUA_VERSION
rm -rf $LUA_BASE
curl http://www.lua.org/ftp/$LUA_BASE.tar.gz | tar zx

cd $LUA_BASE
make $PLATFORM
make local

LUA_DIR=$BUILD_DIR/lua
rm -rf $LUA_DIR
mv install/ $LUA_DIR

cd $BUILD_DIR

LUAROCKS_BASE=luarocks-$LUAROCKS_VERSION
rm -rf ${LUAROCKS_BASE}
LUAROCKS_URL=http://luarocks.org/releases/${LUAROCKS_BASE}.tar.gz
curl -L $LUAROCKS_URL | tar xzp
cd $LUAROCKS_BASE

./configure --prefix=$LUA_DIR
make build 
make install

cd $BUILD_DIR

rm -rf $LUAROCKS_BASE
rm -rf $LUA_BASE

echo you need to run the following commands to setup the env vars
echo export PATH=\$\($LUA_DIR/bin/luarocks path --bin\):\$PATH
echo export LUA_PATH=\$\(${LUA_DIR}/bin/luarocks path --lr-path\)
echo export LUA_CPATH=\$\(${LUA_DIR}/bin/luarocks path --lr-cpath\)

