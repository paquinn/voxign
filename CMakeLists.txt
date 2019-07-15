cmake_minimum_required(VERSION 3.5)

project(voxign)

set(NANOGUI_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
add_subdirectory(ext/nanogui)
add_subdirectory(ext/tinyformat)
add_subdirectory(ext/pybind11)
add_subdirectory(ext/lodepng)

include_directories(include)
include_directories(ext/nanogui/include)
include_directories(${NANOGUI_EXTRA_INCS})
include_directories(ext/lodepng/lodepng/include)
include_directories(ext/tinyformat)

add_definitions(${NANOGUI_EXTRA_DEFS})

add_executable(voxign
        include/viewer.h
        include/common.h
        include/voxels.h
        
        src/main.cpp
        src/viewer.cpp
        src/voxels.cpp
        )

target_link_libraries(voxign 
    nanogui ${NANOGUI_EXTRA_LIBS} 
    lodepng pybind11::embed)
