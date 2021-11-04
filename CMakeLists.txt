cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)

project(tile_cpp)

set(SOURCES
        src/tilecpp_new.cpp
        src/util.hpp
)

add_executable(tile_cpp ${SOURCES})

target_include_directories(tile_cpp
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include
)

add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(tile_cpp ${ImageMagick_LIBRARIES})