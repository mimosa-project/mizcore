add_library(
  mizcore_component
  ast_block.cpp
  ast_component.cpp
  ast_element.cpp
  ast_statement.cpp
  ast_token.cpp
  ast_type.cpp
  error_def.cpp
  symbol.cpp
  symbol_table.cpp
  token_table.cpp)
add_library(mizcore::component ALIAS mizcore_component)

target_link_libraries(mizcore_component PUBLIC tsl::hat_trie
                                               nlohmann_json::nlohmann_json)
target_include_directories(mizcore_component PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(mizcore_component PRIVATE cxx_std_17)
