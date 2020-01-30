#!/bin/bash
killall -9 civetweb
make clean
make build COPT="-g  -DNO_CGI  -DNO_SSL  -DMG_MBEDTLS -I./" LDFLAGS="-L./mbedlib"
export LD_LIBRARY_PATH=./mbedlib:$LD_LIBRARY_PATH

./civetweb -listening_ports 2222,2288s -document_root ./webs  -access_log_file logi&
