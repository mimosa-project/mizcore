add_library(mizcore_test_util file_handling_tools.cpp)

add_library(mizcore::test_util ALIAS mizcore_test_util)

target_link_libraries(mizcore_test_util PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(mizcore_test_util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(mizcore_test_util PRIVATE cxx_std_17)

add_test(
  NAME mizcore_util_test
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/mizcore_util_test.out
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/)

add_executable(mizcore_util_test.out miz_controller_test.cpp main.cpp)

target_link_libraries(
  mizcore_util_test.out PRIVATE doctest::doctest mizcore::scanner
                                  mizcore::parser mizcore::util mizcore::test_util)
target_compile_features(mizcore_util_test.out PRIVATE cxx_std_17)
