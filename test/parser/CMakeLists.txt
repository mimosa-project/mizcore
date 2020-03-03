project(emcore_parser_test)

# Make test executable
include_directories(
    ${EMCORE_SOURCE_DIR}
    ${VENDOR_INCLUDE_DIRS})

file(GLOB TEST_SOURCES "*.cpp")
add_executable(emcore_parser_test.out ${TEST_SOURCES})
target_link_libraries(emcore_parser_test.out emcore_parser)
target_link_libraries(emcore_parser_test.out Catch2::Catch2)
