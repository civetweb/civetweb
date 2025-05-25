# Use GnuTLS instead of OpenSSL

1 [Build libgmp](https://gmplib.org)

 - 1.1 [Download source](https://gmplib.org/#DOWNLOAD)
 - 1.2 ./configure && make && make install

2 [Build libhogweed and libnettle](https://www.lysator.liu.se/~nisse/nettle/)

 - 2.1 [Download source](https://ftp.gnu.org/gnu/nettle/)
 - 2.2 ./configure && make && make install

3 Build civetweb

 - make build WITH_GNUTLS=1

4 Run civetweb
 - export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH
 - ./civetweb  -listening_ports 8443s  -ssl_certificate resources/cert/server.pem  -document_root ./test/htmldir/
