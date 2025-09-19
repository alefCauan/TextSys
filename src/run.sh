#!/bin/bash

# Script to run the client application

# Cria diretório de saída se não existir
mkdir -p client/output

g++ -Wall -Wextra -g3   client/main.cpp   client/gui.cpp client/http_client.cpp   -o client/output/main_gtk  $(pkg-config --cflags --libs gtk+-3.0)     && LD_PRELOAD=/lib/x86_64-linux-gnu/libpthread.so.0 ./client/output/main_gtk  ./client/output/main_gtk