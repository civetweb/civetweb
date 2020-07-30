For fuzz testing civetweb, perform the following steps:

- Switch to civetweb root directory
- make clean

First fuzz target: vary URI for HTTP1 server
- make WITH_ALL=1 TEST_FUZZ=1
- mv civetweb civetweb_fuzz1
- sudo ./civetweb_fuzz1 -max_len=2048 fuzztest/url/

Second fuzz target: vary HTTP1 request for HTTP1 server
- make WITH_ALL=1 TEST_FUZZ=2
- mv civetweb civetweb_fuzz2
- sudo ./civetweb_fuzz2 -max_len=2048 -dict=fuzztest/http1.dict fuzztest/http1/

Third fuzz target: vary HTTP1 response for HTTP1 client API
- make WITH_ALL=1 TEST_FUZZ=3
- mv civetweb civetweb_fuzz3
- sudo ./civetweb_fuzz3 -max_len=2048 -dict=fuzztest/http1.dict fuzztest/http1c/



Open issues:
 * Need "sudo" for container? (ASAN seems to needs it on WSL test)
 * let "make" create "civetweb_fuzz#" instead of "mv"
 * useful initial corpus and directory
 * Planned additional fuzz test: 
  * vary HTTP2 request for HTTP2 server (in HTTP2 feature branch)
  * use internal function to bypass socket (bottleneck)
 * where to put fuzz corpus?
