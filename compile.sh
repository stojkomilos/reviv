#!/bin/bash

g++ main.cpp \
    vendor/glad.c \
    -I/home/milos/reviv/src/include \
    -L/home/milos/reviv/src/lib \
    -l:libglfw3.a \
    -lGL \
    -lwayland-client \
    -lpthread -ldl \
    -o izlaz

#-lX11 -lXrandr -lXi \

if [ $? == 0 ]
then
    printf "Finished compiling\nRunning:\n"
    ./izlaz
    exit 0
else
    printf "Compilation failed\n"
    exit 1
fi

#-L/home/milos/reviv/src/       - library directory
#-l:libglfw3.a                  - include library

