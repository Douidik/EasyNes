file(GLOB_RECURSE SOURCE_TEST *.hpp *.cpp)

add_executable(NesTest ${SOURCE_TEST})
target_include_directories(NesTest PRIVATE ../Emulator)
target_link_libraries(NesTest NesEmu CONAN_PKG::catch2)
