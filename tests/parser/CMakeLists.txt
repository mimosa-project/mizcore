add_test(
  NAME mizcore_parser_test
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/mizcore_parser_test.out
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/)

add_executable(mizcore_parser_test.out miz_block_parser_test.cpp main.cpp)

target_link_libraries(
  mizcore_parser_test.out PRIVATE doctest::doctest mizcore::scanner
                                  mizcore::parser mizcore::test_util)
target_compile_features(mizcore_parser_test.out PRIVATE cxx_std_17)
