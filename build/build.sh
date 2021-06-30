#!/bin/bash

cd /home/milos/reviv2/build

cmake ../ -B ./ -G "CodeLite - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
