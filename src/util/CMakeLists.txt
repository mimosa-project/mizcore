add_library(
  mizcore_util
  miz_controller.cpp)
add_library(mizcore::util ALIAS mizcore_util)

target_link_libraries(
  mizcore_util PUBLIC mizcore::component mizcore::scanner mizcore::parser)
target_include_directories(mizcore_util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(mizcore_util PRIVATE cxx_std_17)
