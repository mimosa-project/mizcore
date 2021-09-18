add_library(mizcore_component error_def.cpp symbol.cpp symbol_table.cpp token_table.cpp
                              token.cpp)
add_library(mizcore::component ALIAS mizcore_component)

target_link_libraries(mizcore_component PUBLIC tsl::hat_trie
                                               nlohmann_json::nlohmann_json)
target_include_directories(mizcore_component PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(mizcore_component PRIVATE cxx_std_17)
