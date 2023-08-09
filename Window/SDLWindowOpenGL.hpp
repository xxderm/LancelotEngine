#pragma once
#include "WindowSDL.hpp"
#include <GL/glew.h>
#include <GL/GL.h>

namespace LL::Window {
    class LL_CALL SDLWindowOpenGL final : public WindowSDL {
    public:
        SDLWindowOpenGL() = default;
        ~SDLWindowOpenGL();
        void Destroy();
        bool Initialize(WindowProperties properties) override;
        void Clear() override;
        void Swap() override;
    private:
        SDL_GLContext mContext = nullptr;
    };
}