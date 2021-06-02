add_test(
  NAME emcore_scanner_test
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/emcore_scanner_test.out
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/)

add_executable(
  emcore_scanner_test.out symbol_table_test.cpp vct_file_handler_test.cpp
                          miz_file_handler_test.cpp main.cpp)

target_link_libraries(emcore_scanner_test.out PRIVATE doctest::doctest
                                                      emcore::scanner)
target_compile_features(emcore_scanner_test.out PRIVATE cxx_std_17)
