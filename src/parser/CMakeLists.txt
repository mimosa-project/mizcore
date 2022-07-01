add_library(mizcore_parser miz_block_parser.cpp miz_pattern_parser.cpp)
add_library(mizcore::parser ALIAS mizcore_parser)

target_link_libraries(mizcore_parser PUBLIC mizcore::component spdlog::spdlog)
target_include_directories(mizcore_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(mizcore_parser PRIVATE cxx_std_17)
