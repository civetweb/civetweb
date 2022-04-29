#!/bin/sh

#################
# call build.sh
$(dirname $0)/build.sh
#################

echo ""
echo "====================="
echo "== run fuzz test 1 =="
echo "====================="
echo ""

./civetweb_fuzz1 -max_total_time=60 -max_len=2048 fuzztest/url/

echo ""
echo "====================="
echo "== run fuzz test 2 =="
echo "====================="
echo ""

./civetweb_fuzz2 -max_total_time=60 -max_len=2048 -dict=fuzztest/http1.dict fuzztest/http1/

echo ""
echo "====================="
echo "== run fuzz test 3 =="
echo "====================="
echo ""

./civetweb_fuzz3 -max_total_time=60 -max_len=2048 -dict=fuzztest/http1.dict fuzztest/http1c/

echo ""
echo "====================="
echo "== fuzz tests done =="
echo "====================="
echo ""
