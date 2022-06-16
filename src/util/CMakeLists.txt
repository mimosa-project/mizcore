add_library(mizcore_all_in_one all_in_one.cpp)
add_library(mizcore::AllInOne ALIAS mizcore_all_in_one)

target_link_libraries(
  mizcore_all_in_one PUBLIC mizcore::component mizcore::scanner mizcore::parser mizcore::test_util)
target_include_directories(mizcore_all_in_one PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(mizcore_all_in_one PRIVATE cxx_std_17)
