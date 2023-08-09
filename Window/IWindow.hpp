#pragma once
#include "../impexp.hpp"
#include "../Core/Log.hpp"

namespace LL::Window {
    struct LL_CALL WindowProperties {
        char* Title = "Window";
        int Vsync = 1;
        int Width = 800;
        int Height = 600;
    };

    class LL_CALL IWindow {
    public:
        virtual bool Initialize(const WindowProperties) = 0;
        virtual void Clear() = 0;
        virtual void Swap() = 0;
    };
}