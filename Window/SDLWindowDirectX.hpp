#pragma once
#include "WindowSDL.hpp"
#include <D3D11.h>

namespace LL::Window {
    class LL_CALL SDLWindowDirectX final : public WindowSDL {
    public:
        SDLWindowDirectX() = default;
        ~SDLWindowDirectX();
        bool Initialize(std::shared_ptr<WindowProperties> properties) override;
        void Clear() override;
        void Swap() override;
        void Destroy();
    private:
        IDXGISwapChain* mSwapChain = nullptr;
        ID3D11Device* mDevice = nullptr;
        ID3D11DeviceContext* mDeviceContext = nullptr;
        ID3D11RenderTargetView* mBackBuffer = nullptr;
        D3D11_VIEWPORT mViewport;
    };
}