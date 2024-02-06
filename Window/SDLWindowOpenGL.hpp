#pragma once
#include "WindowSDL.hpp"
#include <GL/glew.h>
#include <GL/GL.h>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

namespace LL::Window {
    class LL_CALL SDLWindowOpenGL final : public WindowSDL {
    public:
        SDLWindowOpenGL() = default;
        ~SDLWindowOpenGL();
        void Destroy();
        bool Initialize(std::shared_ptr<WindowProperties> properties) override;
        void Clear() override;
        void Swap() override;
    protected:
        virtual void PrepareToUpdateControls();
    private:
        SDL_GLContext mContext = nullptr;
    };
}