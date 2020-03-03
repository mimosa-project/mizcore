cmake_minimum_required(VERSION 3.0)
project(emcore CXX)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -s -DNDEBUG -march=native")

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Catch2/include)
set(HAT_TRIE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/hat-trie/include)
set(RAPID_JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/rapidjson/include)
set(SPDLOG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include)
set(VENDOR_INCLUDE_DIRS
    ${CATCH_INCLUDE_DIR}
    ${HAT_TRIE_INCLUDE_DIR}
    ${RAPID_JSON_INCLUDE_DIR}
    ${SPDLOG_INCLUDE_DIR})

set(EMCORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EMCORE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(vendor/Catch2)
add_subdirectory(src)
add_subdirectory(test)
