add_library(mizcore_test_util file_handling_tools.cpp)

add_library(mizcore::test_util ALIAS mizcore_test_util)

target_link_libraries(mizcore_test_util PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(mizcore_test_util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(mizcore_test_util PRIVATE cxx_std_17)
