file(GLOB_RECURSE SOURCE_APP *.hpp *.cpp)

add_executable(EasyApp ${SOURCE_APP})
target_include_directories(EasyApp PRIVATE ../Emulator)
target_link_libraries(EasyApp EasyEmu)
