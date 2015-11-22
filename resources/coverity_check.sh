#! /bin/sh

ls src/civetweb.c
if [ "$?" = "0" ]; then
	echo "Building files for coverity check ..."
else
	echo "Run this script from the root directory of project!" 1>&2
	echo "username@hostname:/somewhere/civetweb$ resources/coverity_check.sh" 1>&2
	exit 1
fi

rm -rf cov_int/
make clean

../cov-analysis-linux64-7.6.0/bin/cov-build  --dir cov-int make WITH_IPV6=1 WITH_WEBSOCKET=1 WITH_LUA_SHARED=1

rm coverity_check.tgz
tar czvf coverity_check.tgz cov-int

ls coverity_check.tgz

if [ "$?" = "0" ]; then
	echo "... done"
else
	echo "No coverity_check.tgz file" 1>&2
	exit 1
fi

exit 0

