#!/bin/bash

export PICO_SDK_PATH=$PWD/../pico-sdk

cd BadWDSD

#rm -rf build
mkdir build

cd build

cmake ..

make clean
make BadWDSD -j12
