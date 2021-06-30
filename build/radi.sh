#!/bin/bash

cd /home/reviv2/build/

rm -r CMakeCache.txt CMakeFiles cmake_install.cmake Makefile target_link_library reviv

printf "===> Beggining Cmake\n"
cmake ../

printf "===> Beggining make\n"
make

