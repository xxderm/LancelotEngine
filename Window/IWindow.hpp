#pragma once
#include "../Core/Log.hpp"
#include "AppState.hpp"

namespace LL::Window {
    struct LL_CALL WindowProperties {
        std::string Title = "Window";
        int Vsync = 1;
        int Width = 800;
        int Height = 600;
    };

    class LL_CALL IWindow {
    public:
        virtual bool Initialize(WindowProperties) = 0;
        virtual void StartState(std::shared_ptr<Core::AppStateControl> startState) = 0;
        virtual void Clear() = 0;
        virtual void Swap() = 0;
    };
}