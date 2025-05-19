#!/bin/bash

export PICO_SDK_PATH=$PWD/pico_sdk

cd BadWDSD

#rm -rf build
mkdir build

cd build

cmake ..

make clean
make BadWDSD -j12
