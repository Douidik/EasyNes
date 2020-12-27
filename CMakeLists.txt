project(EasyNes)
cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
# This project can only compile with clang++
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Binaries/${PROJECT_NAME})

add_subdirectory(Source)
