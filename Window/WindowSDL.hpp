#pragma once
#include "IWindow.hpp"
#include <SDL.h>
#include <SDL_syswm.h>

namespace LL::Window {
    class LL_CALL WindowSDL : public IWindow {
    public:
        bool Initialize(WindowProperties properties) override;
        void Clear() override = 0;
        void Swap() override = 0;
    private:
        void WindowInfoOut();
    protected:
        SDL_Window* mWindow = nullptr;
        Uint32 mFlags;
        SDL_SysWMinfo mSystemInfo;
        SDL_DisplayMode  mDisplayMode;
    };
}