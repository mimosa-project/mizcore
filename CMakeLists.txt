cmake_minimum_required(VERSION 3.12)

project(emcore LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, set to default: Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# test
enable_testing()

add_compile_options(-Wall -Wextra -Wpedantic)
add_compile_definitions(SPDLOG_COMPILED_LIB)

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(tests)
