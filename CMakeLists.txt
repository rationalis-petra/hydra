
# specify the minimum version required for CMake

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_FLAGS "-g -Wall")
# Project name

project(hydra)

# Set output output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# The following folder will be included
include_directories(${PROJECT_SOURCE_DIR}/include/ ${PROJECT_SOURCE_DIR}/include/libs)

add_subdirectory(src)

add_executable(hydra src/main.cpp)


target_link_libraries(hydra expressions operations memory build)
target_link_libraries(hydra ffi ltdl)
