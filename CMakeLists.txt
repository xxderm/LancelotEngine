cmake_minimum_required(VERSION 3.7)
project(Lancelot)

set(DIRECTX_INCLUDE_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include")
set(DIRECTX_LIBRARY_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86")

# Conan deps
include(conanbuildinfo.cmake)
conan_basic_setup()

# Compiler flags
add_compile_definitions(SDL_MAIN_HANDLED)

add_subdirectory(Window/)