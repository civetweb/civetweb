#!/bin/bash
set -e

workspaceFolder=`pwd`
installFolder=${workspaceFolder}/Install

make -C ./src/web_servlets install installdir=${installFolder}/lib

make clean PREFIX=${installFolder}

make build WITH_WEBSOCKET=1 WITH_IPV6=1 WITH_SERVER_STATS=1 WITH_ZLIB=1 WITH_CPP=1 COPT='-DOPENSSL_API_1_1' WITH_ASHIBA_WEB=1 PREFIX=${installFolder}

make install PREFIX=${installFolder}

