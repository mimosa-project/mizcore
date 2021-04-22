cmake_minimum_required(VERSION 3.8)

project(emcore LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, set to default: Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

add_subdirectory(mizar)

# test
enable_testing()
