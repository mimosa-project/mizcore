add_library(emcore_component symbol_table.cpp token_array.cpp token.cpp)
add_library(emcore::component ALIAS emcore_component)

target_link_libraries(emcore_component PUBLIC tsl::hat_trie)
target_include_directories(emcore_component PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(emcore_component PRIVATE "$<$<CONFIG:RELEASE>:-o3>")
target_compile_options(emcore_component PRIVATE "$<$<CONFIG:DEBUG>:-o0 -g>")
target_compile_features(emcore_component PRIVATE cxx_std_17)
target_compile_definitions(emcore_component
                           PRIVATE "$<$<CONFIG:RELEASE>:NDEBUG>")
