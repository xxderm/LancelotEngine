#pragma once
#include "IWindow.hpp"
#include <SDL.h>
#include <SDL_syswm.h>

namespace LL::Window {
    class LL_CALL WindowSDL : public IWindow {
    public:
        WindowSDL();
        bool Initialize(WindowProperties properties) override;
        void StartState(std::shared_ptr<Core::AppStateControl> startState) override;
        void Clear() override = 0;
        void Swap() override = 0;
    private:
        void WindowInfoOut();
    protected:
        virtual void PrepareToUpdateControls() = 0;
    protected:
        SDL_Window* mWindow = nullptr;
        std::shared_ptr<Core::AppState> mStates;
        Uint32 mFlags;
        SDL_SysWMinfo mSystemInfo;
        SDL_DisplayMode  mDisplayMode;
        SDL_Event mEvent{};
    };
}