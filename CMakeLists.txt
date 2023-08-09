cmake_minimum_required(VERSION 3.7)
project(Lancelot)

# Compiler flags
add_compile_definitions(SDL_MAIN_HANDLED)
set(CMAKE_TOOLCHAIN_FILE Lancelot/vcpkg/scripts/buildsystems/vcpkg.cmake)

add_subdirectory(Window/)