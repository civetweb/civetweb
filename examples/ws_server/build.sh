#!/bin/sh
[ -e ws_server ] && rm ws_server

gcc ../../src/civetweb.c ws_server.c -I../../include/ -lpthread -o ws_server -DNO_SSL -DNO_CGI -DNO_FILESYSTEM -DUSE_WEBSOCKET -Wall
