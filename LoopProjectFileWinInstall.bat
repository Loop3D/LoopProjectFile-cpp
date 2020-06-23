@echo off

mkdir build-win
cd build-win
cmake ..
cmake --build . --config Release --target TestingLoopProjectFile --verbose
cmake --build . --config Release --target install --verbose

cd ..

@echo on