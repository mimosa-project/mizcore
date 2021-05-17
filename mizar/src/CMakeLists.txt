find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

flex_target(vct_scanner
    yy_vct_flex_lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/yy_vct_flex_lexer.cpp)

flex_target(miz_scanner
    yy_miz_flex_lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/yy_miz_flex_lexer.cpp)

add_library(emcore_mizar
    STATIC
    symbol_table.cpp
    token.cpp
    token_array.cpp
    token_factory.cpp
    vct_file_handler.cpp
    miz_file_handler.cpp
    miz_flex_lexer.cpp
    ${FLEX_miz_scanner_OUTPUTS}
    ${FLEX_vct_scanner_OUTPUTS}
)

target_include_directories(emcore_mizar
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/common/include
)

target_link_libraries(emcore_mizar
    PUBLIC
    spdlog::spdlog
)

target_compile_options(emcore_mizar PUBLIC "$<$<CONFIG:RELEASE>:-o3>")
target_compile_options(emcore_mizar PUBLIC "$<$<CONFIG:DEBUG>:-o0 -g>")
target_compile_features(emcore_mizar PUBLIC cxx_std_17)
target_compile_definitions(emcore_mizar PUBLIC SPDLOG_COMPILED_LIB)
target_compile_definitions(emcore_mizar PUBLIC "$<$<CONFIG:RELEASE>:NDEBUG>")
