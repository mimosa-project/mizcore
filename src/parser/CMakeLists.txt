project(emcore_parser CXX)

include_directories(
    ${EMCORE_SOURCE_DIR}
    ${VENDOR_INCLUDE_DIRS})

file(GLOB PARSER_SOURCES "*.cpp")
add_library(emcore_parser STATIC ${PARSER_SOURCES})
