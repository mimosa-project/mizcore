#!/bin/sh

mkdir /mizcore/build
cmake -DCMAKE_TOOLCHAIN_FILE=/mizcore/x86_64-apple-darwin/x86_64-apple-darwin.cmake -B/mizcore/build/ /mizcore/
cd /mizcore/build
make
