cmake_minimum_required(VERSION 3.7)
project(Lancelot)

add_compile_definitions(SDL_MAIN_HANDLED)

add_subdirectory(Window)
add_subdirectory(Renderer)