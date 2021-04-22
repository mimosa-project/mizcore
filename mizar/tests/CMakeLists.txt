add_test(NAME emcore_mizar_test
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/emcore_mizar_test.out
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/
)

add_executable(
    emcore_mizar_test.out
    vct_file_handler_test.cpp
    main.cpp
)

target_link_libraries(
    emcore_mizar_test.out
    PRIVATE
    emcore_mizar
)

target_include_directories(
    emcore_mizar_test.out
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_compile_options(emcore_mizar_test.out PUBLIC "$<$<CONFIG:RELEASE>:-o3>")
target_compile_options(emcore_mizar_test.out PUBLIC "$<$<CONFIG:DEBUG>:-o0 -g>")
target_compile_features(emcore_mizar_test.out PUBLIC cxx_std_17)
target_compile_definitions(emcore_mizar_test.out PUBLIC "$<$<CONFIG:RELEASE>:NDEBUG>")

