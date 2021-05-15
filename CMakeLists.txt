
# specify the minimum version required for CMake

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_FLAGS "-g -Wall")
# Project name

project(hydra)



option(UseConan "whether conan is used when building" ON)

# Set output output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# The following folder will be included
include_directories(${PROJECT_SOURCE_DIR}/include/ ${PROJECT_SOURCE_DIR}/include/libs)

if (UseConan)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lglapi")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
endif()

# This prevents linking errors when compiling with MSVC: 
# maybe use non-boost ASIO??
IF(MSVC)
    ADD_DEFINITIONS("/EHsc")
ENDIF(MSVC)

add_subdirectory(src)

