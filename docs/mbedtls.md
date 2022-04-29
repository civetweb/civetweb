#### Use MbedTLS instead of  OpenSSL
=====

1 [Build mbedtls](https://github.com/ARMmbed/mbedtls)

 - 1.1 git clone https://github.com/ARMmbed/mbedtls.git -b mbedtls-2.24.0
 - 1.2 Use Arm Mbed TLS in threaded environments, enable `MBEDTLS_THREADING_C` and `MBEDTLS_THREADING_PTHREAD` in config.h, [more](https://tls.mbed.org/kb/development/thread-safety-and-multi-threading)
 - 1.3 make SHARED=1 && make install

2 Build civetweb

 - make build WITH_MBEDTLS=1

3 Run civetweb
 - export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH
 - ./civetweb  -listening_ports 8443s  -ssl_certificate resources/cert/server.pem  -document_root ./test/htmldir/
