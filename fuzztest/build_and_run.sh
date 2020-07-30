#!/bin/sh

make clean
rm civetweb_fuzz?

make WITH_ALL=1 TEST_FUZZ=1
mv civetweb civetweb_fuzz1
make WITH_ALL=1 TEST_FUZZ=2
mv civetweb civetweb_fuzz2
make WITH_ALL=1 TEST_FUZZ=3
mv civetweb civetweb_fuzz3

echo ""
echo "====================="
echo "== Build completed =="
echo "====================="
echo ""

ls -halt civetweb*
echo ""
md5sum civetweb_fuzz*

echo ""
echo "====================="
echo "== run fuzz test 1 =="
echo "====================="
echo ""

./civetweb_fuzz1 -max_total_time=600 -max_len=2048 fuzztest/url/

echo ""
echo "====================="
echo "== run fuzz test 2 =="
echo "====================="
echo ""

./civetweb_fuzz2 -max_total_time=600 -max_len=2048 -dict=fuzztest/http1.dict fuzztest/http1/

echo ""
echo "====================="
echo "== run fuzz test 3 =="
echo "====================="
echo ""

./civetweb_fuzz3 -max_total_time=600 -max_len=2048 -dict=fuzztest/http1.dict fuzztest/http1c/

echo ""
echo "====================="
echo "== fuzz tests done =="
echo "====================="
echo ""
