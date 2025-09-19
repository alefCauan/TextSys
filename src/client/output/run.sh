#!/bin/bash

# Script to run the client application

g++ -Wall -Wextra -g3   ../main.cpp   ../gui.cpp ../http_client.cpp   -o main_gtk  $(pkg-config --cflags --libs gtk+-3.0)     && LD_PRELOAD=/lib/x86_64-linux-gnu/libpthread.so.0 ./main_gtk  ./main_gtk