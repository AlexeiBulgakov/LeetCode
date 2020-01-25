#!/bin/bash

BUILD_DIR=build

if [ $1 == "--clean" ]; then
    if [ -d ./${BUILD_DIR} ]; then
        echo "[INFO] target build directory was cleaned"
        rm -r -r ./${BUILD_DIR}
    else
        echo "[INFO] target build directory is clean"
    fi
fi

mkdir ./${BUILD_DIR}
cd ./${BUILD_DIR}
cmake .. -A "x64" -G "Visual Studio 16 2019" -T v142
cmake --build .
