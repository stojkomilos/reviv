#!/bin/bash

cd /home/milos/dev/reviv2/ && mkdir build

cd /home/milos/dev/reviv2/build && rm -rf ./*

cmake ../

make

make install
