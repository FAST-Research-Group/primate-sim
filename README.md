# PrimateSIM

# Requirements

- CMake
- g++ Compiler
- boost

## WSL Quick Commands

build-essentials contain g++ and libboost-all-dev contains boost
```bash
sudo apt-get update
sudo apt-get install cmake build-essential libboost-all-dev
```

# Building

Project uses cmake

To build the files, follow these steps

- `mkdir build`
- `cd build`
- `cmake ../`
- executable should now be in `build/src`