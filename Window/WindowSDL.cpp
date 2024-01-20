#pragma once
#include "WindowSDL.hpp"

namespace LL::Window {
    bool WindowSDL::Initialize(WindowProperties properties) {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            LL_LOG(Core::LogLevel::ERR, "SDL Initialize error: ", SDL_GetError());
            return false;
        }
        mWindow = SDL_CreateWindow(properties.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    properties.Width, properties.Height, mFlags);
        if (mWindow == nullptr) {
            LL_LOG(Core::LogLevel::ERR, "SDL Initialize error: ", SDL_GetError());
            return false;
        }
        this->WindowInfoOut();
        return true;
    }

    void WindowSDL::WindowInfoOut() {
        SDL_VERSION(&mSystemInfo.version);
        if (SDL_GetWindowWMInfo(mWindow, &mSystemInfo) == SDL_FALSE ) {
            LL_LOG(Core::LogLevel::WARNING, "System info error: ", SDL_GetError());
            return;
        }
        if (SDL_GetCurrentDisplayMode(0, &mDisplayMode) != 0) {
            LL_LOG(Core::LogLevel::WARNING, "System info error: ", SDL_GetError());
            return;
        }
        LL_LOG(Core::LogLevel::INFO, "Window info:");
        LL_LOG(Core::LogLevel::INFO, "[Platform] ", SDL_GetPlatform());
        LL_LOG(Core::LogLevel::INFO, "[Size] ", mDisplayMode.w, "x", mDisplayMode.h);
        LL_LOG(Core::LogLevel::INFO, "[Hz] ", mDisplayMode.refresh_rate);
        LL_LOG(Core::LogLevel::INFO, "[CPU count] ", SDL_GetCPUCount());
        LL_LOG(Core::LogLevel::INFO, "[CPU cache] ", SDL_GetCPUCacheLineSize());
    }

    void WindowSDL::StartState(std::shared_ptr<Core::AppStateControl> startState) {
        mStates->ChangeState(startState);
    }

    WindowSDL::WindowSDL() {
        mStates = std::make_shared<Core::AppState>();
    }
}
