#!/bin/sh
set -e

mkdir -p cmake
cd cmake
conan install .. --build=missing
cd ../
./compile.sh
