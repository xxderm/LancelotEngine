#pragma once
#include "IWindow.hpp"
#include <SDL.h>
#include <SDL_syswm.h>

namespace LL::Window {
    class LL_CALL ProcessHandler;
    class LL_CALL WindowSDL : public IWindow {
    public:
        WindowSDL();
        bool Initialize(std::shared_ptr<WindowProperties> properties) override;
        void StartState(std::shared_ptr<Core::AppStateControl> startState) override;
        void Clear() override = 0;
        void Swap() override = 0;
        void Focus();
        bool HasFocMouse() const noexcept { return mFocMouse; }
        bool HasFocKey() const noexcept { return mFocKey; }
        bool IsMin() noexcept { return mMin; }
        bool IsShown() noexcept { return mShown; }
    private:
        void WindowInfoOut();
    protected:
        virtual void PrepareToUpdateControls() = 0;
        void HandleFocus(SDL_Event& e);
    protected:
        SDL_Window* mWindow = nullptr;
        std::shared_ptr<Core::AppState> mStates;
        Uint32 mFlags;
        SDL_SysWMinfo mSystemInfo;
        SDL_DisplayMode  mDisplayMode;
        SDL_Event mEvent{};
        uint32_t mId;
        uint32_t mWidth;
        uint32_t mHeight;
        bool mFocMouse;
        bool mFocKey;
        bool mFullS;
        bool mMin;
        bool mShown;
    };
}