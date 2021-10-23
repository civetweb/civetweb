#!/bin/bash
[ ! -e ec_server ] || rm ec_server

echo "Building ec_server"
gcc ec_example.c ../../src/civetweb.c -I ../../include/ -o ec_server -DNO_SSL_DL -DOPENSSL_API_1_1 -lpthread -lssl -lcrypto -DNO_FILES -DNO_FILESYSTEM
[  -e ec_server ] || echo "Failed to build"
[  -e ec_server ] || exit 1

echo "Allowing ec_server to bind to port 443. Requires sudo:"
sudo setcap CAP_NET_BIND_SERVICE=+eip ec_server
