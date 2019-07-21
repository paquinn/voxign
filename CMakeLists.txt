cmake_minimum_required(VERSION 3.5)

project(voxign)
set(VOXIGN_VERSION "0.1")

set(NANOGUI_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
add_subdirectory(ext/nanogui)
add_subdirectory(ext/tinyformat)
add_subdirectory(ext/pybind11)
add_subdirectory(ext/lodepng)
add_subdirectory(ext/args)

include_directories(include)
include_directories(ext/nanogui/include)
include_directories(${NANOGUI_EXTRA_INCS})
include_directories(ext/lodepng/lodepng/include)
include_directories(ext/tinyformat)
include_directories(ext/args)

set(VOXIGN_DEFINITIONS -DVOXIGN_VERSION="${VOXIGN_VERSION}")
add_definitions(${VOXIGN_DEFINITIONS} ${NANOGUI_EXTRA_DEFS})

add_executable(voxign
        include/viewer.h
        include/common.h
        include/voxels.h

        src/main.cpp
        src/viewer.cpp
        src/voxels.cpp
        include/compiler.h src/compiler.cpp include/pixels.h src/pixels.cpp src/common.cpp)

target_link_libraries(voxign 
    nanogui ${NANOGUI_EXTRA_LIBS} 
    lodepng pybind11::embed)
