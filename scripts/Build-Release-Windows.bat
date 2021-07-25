@echo off
cd ../
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
cmake --build ./ -j24