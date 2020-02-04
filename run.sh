#!/bin/bash
killall -9 civetweb
make clean
make build COPT="-g  -DNO_CGI  -DNO_SSL  -DMG_MBEDTLS -I./" LDFLAGS="-L./mbedlib"
export LD_LIBRARY_PATH=./mbedlib:$LD_LIBRARY_PATH

valgrind  --tool=memcheck --leak-check=full --show-mismatched-frees=yes  ./civetweb -listening_ports 8888,4433s -document_root ./webs
