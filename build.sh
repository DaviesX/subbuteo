#!/bin/bash

mkdir -p build
pushd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
popd
