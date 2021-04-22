find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

flex_target(vct_scanner
    vct_scanner.l
    ${CMAKE_CURRENT_BINARY_DIR}/vct_scanner.cpp)

add_library(
    emcore_mizar
    STATIC
    ${FLEX_vct_scanner_OUTPUTS}
)

target_include_directories(
    emcore_mizar
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(emcore_mizar PUBLIC "$<$<CONFIG:RELEASE>:-o3>")
target_compile_options(emcore_mizar PUBLIC "$<$<CONFIG:DEBUG>:-o0 -g>")
target_compile_features(emcore_mizar PUBLIC cxx_std_17)
target_compile_definitions(emcore_mizar PUBLIC "$<$<CONFIG:RELEASE>:NDEBUG>")
