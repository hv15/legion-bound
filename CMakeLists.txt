cmake_minimum_required (VERSION 3.10)

project (legion-bound C)

# add CMake modules
set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# make sure we have SDL2
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

add_executable (legion-bound legion-bound.c)
target_link_libraries(legion-bound ${SDL2_LIBRARY})
