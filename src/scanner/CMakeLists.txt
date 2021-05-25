flex_target(vct_scanner yy_vct_flex_lexer.l
            ${CMAKE_CURRENT_BINARY_DIR}/yy_vct_flex_lexer.cpp)

flex_target(miz_scanner yy_miz_flex_lexer.l
            ${CMAKE_CURRENT_BINARY_DIR}/yy_miz_flex_lexer.cpp)

add_library(
  emcore_scanner vct_file_handler.cpp miz_file_handler.cpp miz_flex_lexer.cpp
                 ${FLEX_miz_scanner_OUTPUTS} ${FLEX_vct_scanner_OUTPUTS})

add_library(emcore::scanner ALIAS emcore_scanner)

target_link_libraries(emcore_scanner PUBLIC emcore::component)
target_include_directories(emcore_scanner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(emcore_scanner PRIVATE cxx_std_17)
