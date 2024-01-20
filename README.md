## LancelotEngine
[![CMake](https://github.com/xxderm/LancelotEngine/actions/workflows/cmake.yml/badge.svg)](https://github.com/xxderm/LancelotEngine/actions/workflows/cmake.yml)
### Build
#### Update submodule
```shell script
git submodule init
git submodule update
```
#### Vcpkg build
````shell script
vcpkg\bootstrap-vcpkg.bat
vcpkg install --triplet x64-windows
````
##### Single installation dependencies
```shell script
vcpkg install directxtk
vcpkg install directxtk:x64-windows
vcpkg install opengl
vcpkg install glew
vcpkg install sdl2 --triplet x64-windows
```
1) Set cmake option CMAKE_TOOLCHAIN_FILE
2) Build CMakeLists 
