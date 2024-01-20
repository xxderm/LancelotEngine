cmake_minimum_required(VERSION 3.7)
project(LancelotWindow)

if (WIN32)
    find_package(directxtk CONFIG REQUIRED)
endif()
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

if(WIN32)
    # TODO:
    set(DIRECTX_INCLUDE_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include")
    set(DIRECTX_LIBRARY_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86")
endif()


# Compiler flags
add_compile_definitions(SDL_MAIN_HANDLED)

# Include project directories
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")

add_library(LancelotWindow SHARED ${sources}
        "../Core/EntityComponentSystem.cpp"
        "../Core/Log.cpp"
        "../Core/SessionProfiler.cpp"
        "../Core/Timer.cpp"
        )

if (WIN32)
    target_link_libraries(LancelotWindow PRIVATE d3d11.lib)
    target_link_libraries(LancelotWindow PRIVATE Microsoft::DirectXTK)
endif()
target_link_libraries(LancelotWindow PRIVATE GLEW::GLEW)
target_link_libraries(LancelotWindow PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(LancelotWindow
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        )
if (WIN32)
target_include_directories(LancelotWindow PRIVATE ${DIRECTX_INCLUDE_DIR})
endif()
target_include_directories(LancelotWindow PRIVATE ${OPENGL_INCLUDE_DIR})

set_property(TARGET LancelotWindow PROPERTY CXX_STANDARD 17)