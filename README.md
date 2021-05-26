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

## TODO

- Support rapidjson
- Parse blocks in tokenize process
