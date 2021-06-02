# emcore

eco-Mizar Core Library

## Environment

- OS
  - Linux (Tested on Ubuntu 20.04 LTS)
- Compiler
  - GCC (Tested on GCC 9.3.0)

## Build

```bash
sudo apt install build-essential flex bison
git clone https://github.com/mimosa-project/emcore.git
cd emcore
bash tools/init_environment.sh
mkdir build
cd build
cmake ..
make
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
- Parse blocks in tokenize process
