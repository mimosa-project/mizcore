cmake_minimum_required(VERSION 3.12)

project(mizcore LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, set to default: Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_CLANG_TIDY clang-tidy; -header-filter=.; -checks=*;)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# test
enable_testing()

add_compile_options(-Wall -Wextra -Wpedantic -fPIC)

set(JSON_BuildTests
    OFF
    CACHE INTERNAL "")

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(py_binding)