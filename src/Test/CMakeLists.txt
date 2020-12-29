file(GLOB_RECURSE SOURCE_TEST *.hpp *.cpp)

add_executable(EasyTest ${SOURCE_TEST})
target_include_directories(EasyTest PRIVATE ../Emulator)
target_link_libraries(EasyTest EasyEmu CONAN_PKG::catch2)
