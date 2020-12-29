#!/bin/sh
set -e

# $1: build configuration, defaulted to debug
configs=${1:-Debug}
# $2: number of working threads: defaulted to 12
threads=${2:-12}

cmake -DCMAKE_BUILD_TYPE=$configs .
make -j$threads
bin/EasyTest
