file(GLOB_RECURSE SOURCE_APP *.hpp *.cpp)

add_executable(NesApp ${SOURCE_APP})
target_include_directories(NesApp PRIVATE ../Emulator)
target_link_libraries(NesApp NesEmu)
