file(GLOB_RECURSE SOURCE_EMU *.hpp *.cpp)
add_library(NesEmu STATIC ${SOURCE_EMU})
