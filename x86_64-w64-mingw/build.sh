#!/bin/sh

cmake -DCMAKE_TOOLCHAIN_FILE=/mizcore/x86_64-w64-mingw/x86_64-w64-mingw.cmake -B/mizcore/build/ /mizcore/
cd /mizcore/build
make

cd /opt/llvm-mingw/x86_64-w64-mingw32/bin/
cp libc++.dll libunwind.dll /mizcore/build/tests/parser
cp libc++.dll libunwind.dll /mizcore/build/tests/scanner
cp libc++.dll libunwind.dll /mizcore/build/tests/util
