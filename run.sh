#!/bin/bash
export LD_LIBRARY_PATH=./mbedlib:$LD_LIBRARY_PATH

./civetweb -listening_ports 2222,3333s -document_root ./webs
