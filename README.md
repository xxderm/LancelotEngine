## LancelotEngine

### Build

#### Install dependencies
```shell script
vcpkg install directxtk
vcpkg install directxtk:x64-windows
vcpkg install opengl
vcpkg install glew
vcpkg install sdl2 --triplet x64-windows
```
1) Set cmake option CMAKE_TOOLCHAIN_FILE
2) Build CMakeLists 