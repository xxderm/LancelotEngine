#pragma once
#include "SDLWindowOpenGL.hpp"

namespace LL::Window {
    bool SDLWindowOpenGL::Initialize(WindowProperties properties) {
        this->mFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        if (!WindowSDL::Initialize(properties)) {
            LL_LOG(Core::LogLevel::ERR, "SDLWindowOpenGL SDL initialize error");
            return false;
        }
        mContext = SDL_GL_CreateContext(mWindow);
        if (!mContext) {
            LL_LOG(Core::ERR, "Failed to create context: ", SDL_GetError());
            return false;
        }
        GLenum glewInitResult = glewInit();
        if (glewInitResult != GLEW_OK) {
            LL_LOG(Core::ERR, "Failed to initialize glew", glewGetErrorString(glewInitResult));
            return false;
        }
        SDL_GL_SetSwapInterval(properties.Vsync);
        LL_LOG(Core::INFO, "SDLWindowOpenGL initialization success");
        return true;
    }

    void SDLWindowOpenGL::Swap() {
        mStates->OnRender();
        SDL_GL_SwapWindow(mWindow);
        LL_LOG(LL::Core::LogLevel::ERR, "Убрать хуйню SDLWindowOpenGLm");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    void SDLWindowOpenGL::Clear() {
        PrepareToUpdateControls();
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    SDLWindowOpenGL::~SDLWindowOpenGL() {
        this->Destroy();
    }

    void SDLWindowOpenGL::Destroy() {
        if (mContext) {
            SDL_GL_DeleteContext(mContext);
            mContext = nullptr;
        }
        if (mWindow) {
            SDL_DestroyWindow(mWindow);
            mWindow = nullptr;
        }
        SDL_Quit();
    }

    void SDLWindowOpenGL::PrepareToUpdateControls() {
        while (SDL_PollEvent(&mEvent) != 0) {
            if (mEvent.type == SDL_QUIT) {
            }
        }
        mStates->OnUpdate();
    }
}