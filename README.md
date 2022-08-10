# mizcore

Mizar Core Library

## Environment

- OS
  - Linux (Tested on Ubuntu 20.04 LTS)
- Compiler
  - GCC (Tested on GCC 9.3.0)

## Build

```bash
sudo apt install build-essential cmake flex bison clang-tidy clang-format
git clone https://github.com/mimosa-project/mizcore.git --recursive
cd mizcore
mkdir build
cd build
cmake ..
make
```
### Build with clang
```
cmake -C ../clang-cmakeinit.cmake ..
make
```
### Cross compile for windows
```bash
docker build -t llvm-mingw ./x86_64-w64-mingw/
docker run --rm -v $(pwd):/mizcore llvm-mingw sh /mizcore/x86_64-w64-mingw/build.sh
```
Note: You need to put `libc++.dll`, `libunwind.dll` in the same directory as the executable. Refer to `/x86_64-w64-mingw/build.sh`
### Cross compile for mac
```bash
docker build -t osxcross ./x86_64-apple-darwin/
docker run --rm -v $(pwd):/mizcore osxcross sh /mizcore/x86_64-apple-darwin/build.sh
```
## Test

```bash
cd build
ctest
```

## Development

### Installation

All developers are supposed to inspect the source code with clang-tidy and clang-format before committing.

```bash
sudo apt install clang-tidy clang-format
```

It is also recommended that VSCode users install the following extensions for on-the-fly inspection.

- [Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format)
- [Clang-Tidy](https://marketplace.visualstudio.com/items?itemName=notskm.clang-tidy)

## TODO

- Python binding (in another project).
- JavaScript binding (in another project).
- Support GitHub Actions.
