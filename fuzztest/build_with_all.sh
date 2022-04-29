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
md5sum civetweb_fuzz*

echo ""
echo "====================="
echo ""
