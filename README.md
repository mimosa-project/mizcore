# emcore

eco-Mizar Core Library

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
