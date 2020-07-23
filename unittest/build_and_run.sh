#!/bin/sh
rm -rf output
mkdir output
gcc unittest/cgi_test.c -o output/cgi_test.cgi
cd output
cmake ..
make all
make test
cd ..
