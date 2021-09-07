@echo off
cd ../
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
cmake --build ./ -j24
echo [32mBuilding complete. Your executable path is: ../build/bin/Release-Windows/sandbox.exe [0m
pause
