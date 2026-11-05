#!/bin/sh

server_name="localhost"
cert_subject="/C=XX/ST=ExampleState/L=ExampleCity/O=ExampleCorp/OU=ExampleDepartment/CN=$server_name"

echo "Creating new certificates"
rm server.* client.* rootCA.* server_bkup.*
echo "Using 'pass' for every password"


echo "Generating a root CA ..."

openssl genrsa -passout pass:pass -out rootCA.key 2048
openssl req -passout pass:pass -new -key rootCA.key -out rootCA.csr -subj $cert_subject
# For a test certificate, use "AA" as "user assigned" language code: https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2#AA

cp rootCA.key rootCA.key.orig
openssl rsa -in rootCA.key.orig -out rootCA.key
openssl x509 -req -days 3651 -in rootCA.csr -signkey rootCA.key -out rootCA.crt
cp rootCA.crt rootCA.pem
cat rootCA.key >> rootCA.pem


echo "Generating client certificate ..."

openssl genrsa -passout pass:pass -out client.key 2048
openssl req -passout pass:pass -new -key client.key -out client.csr -subj $cert_subject

cp client.key client.key.orig

openssl rsa -in client.key.orig -out client.key

openssl x509 -req -days 3650 -in client.csr -signkey client.key -out client.crt

cp client.crt client.pem
cat client.key >> client.pem

openssl pkcs12 -passout pass:pass -export -inkey client.key -in client.pem -name ClientName -out client.pfx


echo "Generating first server certificate ..."

openssl genrsa -passout pass:pass -out server.key 2048
openssl req -passout pass:pass -new -key server.key -out server.csr -subj $cert_subject

cp server.key server.key.orig

openssl rsa -in server.key.orig -out server.key

echo "authorityKeyIdentifier=keyid,issuer" > server.ext
echo "basicConstraints=critical,CA:FALSE" >> server.ext
echo "keyUsage=digitalSignature,nonRepudiation,keyEncipherment,dataEncipherment" >> server.ext
echo "subjectAltName=DNS:$server_name" >> server.ext

openssl x509 -req -days 3650 -sha256 -CA rootCA.pem -CAkey rootCA.key -CAcreateserial -extfile server.ext -in server.csr -out server.crt
#openssl x509 -req -days 3650 -sha256 -CA rootCA.pem -CAkey rootCA.key -CAcreateserial -extfile server.ext -in server.csr -signkey server.key -out server.crt
#openssl x509 -req -days 3650 -sha256 -extfile server.ext -in server.csr -signkey server.key -out server.crt

cp server.crt server.pem
cat server.key >> server.pem
cat rootCA.crt >> server.pem

openssl pkcs12 -passout pass:pass -export -inkey server.key -in server.pem -name ServerName -out server.pfx

echo "First server certificate hash for Public-Key-Pins header:"

openssl x509 -pubkey < server.crt | openssl pkey -pubin -outform der | openssl dgst -sha256 -binary | base64 > server.pin

cat server.pin

echo "Generating backup server certificate ..."

openssl genrsa -passout pass:pass -out server_bkup.key 2048
openssl req -passout pass:pass -new -key server_bkup.key -out server_bkup.csr -subj $cert_subject

cp server_bkup.key server_bkup.key.orig

openssl rsa -in server_bkup.key.orig -out server_bkup.key

openssl x509 -req -days 3650 -in server_bkup.csr -signkey server_bkup.key -out server_bkup.crt

cp server_bkup.crt server_bkup.pem
cat server_bkup.key >> server_bkup.pem

openssl pkcs12 -passout pass:pass -export -inkey server_bkup.key -in server_bkup.pem -name ServerName -out server_bkup.pfx

echo "Backup server certificate hash for Public-Key-Pins header:"

openssl x509 -pubkey < server_bkup.crt | openssl pkey -pubin -outform der | openssl dgst -sha256 -binary | base64 > server_bkup.pin

cat server_bkup.pin

