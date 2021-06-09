flex_target(
  vct_scanner yy_vct_flex_lexer.l
  ${CMAKE_CURRENT_BINARY_DIR}/yy_vct_flex_lexer.cpp COMPILE_FLAGS "-Ca")

flex_target(
  miz_scanner yy_miz_flex_lexer.l
  ${CMAKE_CURRENT_BINARY_DIR}/yy_miz_flex_lexer.cpp COMPILE_FLAGS "-Ca")

add_library(
  mizcore_scanner vct_file_handler.cpp miz_file_handler.cpp miz_flex_lexer.cpp
                  ${FLEX_miz_scanner_OUTPUTS} ${FLEX_vct_scanner_OUTPUTS})

add_library(mizcore::scanner ALIAS mizcore_scanner)

target_link_libraries(mizcore_scanner PUBLIC mizcore::component spdlog::spdlog)
target_include_directories(mizcore_scanner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(mizcore_scanner PRIVATE cxx_std_17)
