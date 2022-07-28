set(CMAKE_SYSTEM_NAME darwin)

set(CMAKE_C_COMPILER x86_64-apple-darwin20.4-clang)
set(CMAKE_CXX_COMPILER x86_64-apple-darwin20.4-clang++)

set(CMAKE_SYSROOT /osxcross/target/SDK/MacOSX11.3.sdk/)

set(CMAKE_CXX_FLAGS "-std=c++17 -mmacosx-version-min=10.15")
