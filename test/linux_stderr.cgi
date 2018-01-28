#!/bin/bash

printf "Content-Type: text/plain\r\n"
printf "\r\n"

echo "This is a shell script called by CGI"

for ((number=1;number < 5000;number++))
{
echo "Write $number to stdout"
echo "Write $number to stderr" 1>&2
}

echo "done"


