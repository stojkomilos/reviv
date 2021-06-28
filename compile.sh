#!/bin/bash


#project_dir=$(pwd)
project_dir=/home/milos/reviv2
printf "Project directory: $project_dir\n"

g++ "$project_dir"/src/main.cpp \
    -I"$project_dir"/src/ \
      "$project_dir"/dependencies/glad.c \
      "$project_dir"/dependencies/stb_image.cpp \
    -I"$project_dir"/dependencies/include \
    -L"$project_dir"/dependencies/lib \
      "$project_dir"/src/core/*.cpp \
      "$project_dir"/src/entity/*.cpp \
      "$project_dir"/src/physics/*.cpp \
      "$project_dir"/src/renderer/*.cpp \
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
else
    printf "Compilation failed\n"
    exit 1
fi

exit 0
