project(EasyApp)
project(EasyEmu)
project(EasyTest)

cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
# This project can only compile with clang++
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Libraries/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Libraries/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Binaries/${PROJECT_NAME})

# Setup conan
include(cmake/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(src/App)
add_subdirectory(src/Emulator)
add_subdirectory(src/Test)
