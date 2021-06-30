#!/bin/bash


#project_dir=$(pwd)
project_dir=/home/milos/reviv2
printf "Project directory: $project_dir\n"
mkdir "$project_dir"/bin

#g++ "$project_dir"/src/main.cpp \
#    -I"$project_dir"/src/ \
#      "$project_dir"/dependencies/glad.c \
#      "$project_dir"/**.cpp
#      "$project_dir"/dependencies/stb_image.cpp \
#    -I"$project_dir"/dependencies/include \
#    -L"$project_dir"/dependencies/lib \
#    -l:libglfw3.a \
#    -lGL \
#    -lwayland-client \
#    -lpthread -ldl \
#    -o izlaz
#    #-o "$project_dir"/izlaz


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
    #-o "$project_dir"/izlaz

##-lX11 -lXrandr -lXi \

if [ $? == 0 ]
then
    printf "Finished compiling\nRunning:\n"
    #cd "$project_dir"
    ./izlaz
else
    printf "Compilation failed\n"
    exit 1
fi

exit 0
