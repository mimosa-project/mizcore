set(CMAKE_SYSTEM_NAME mingw)

set(triple x86_64-w64-mingw32)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})
set(CMAKE_CXX_FLAGS "-std=c++17 -stdlib=libc++ --rtlib=compiler-rt -fuse-ld=lld -unwindlib=libunwind -Qunused-arguments")

set(CMAKE_SYSROOT /opt/llvm-mingw/x86_64-w64-mingw32)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
