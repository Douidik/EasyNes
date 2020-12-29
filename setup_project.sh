#!/bin/sh

mkdir cmake && cd cmake
conan install -b=missing ..
cd ../
./compile.sh
