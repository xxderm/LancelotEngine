# LancelotEngine

# Build
1. Download and install [Conan](https://conan.io/), cmake, vcpkg
2. Open main dir and use:
```shell script
conan install .
```
#### Install directxtk
```shell script
vcpkg install directxtk
vcpkg install directxtk:x64-windows
```
3. Set cmake option CMAKE_TOOLCHAIN_FILE to your "vcpkg\scripts\buildsystems\vcpkg.cmake"
4. Build CMakeLists with MSVC