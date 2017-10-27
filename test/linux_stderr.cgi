#!/bin/sh

printf "Content-Type: text/plain\r\n"
printf "\r\n"

echo "This is a shell script called by CGI:"
tree / 1>&2
echo
set

