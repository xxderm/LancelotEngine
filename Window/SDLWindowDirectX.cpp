#pragma once
#include "SDLWindowDirectX.hpp"

namespace LL::Window {
    bool SDLWindowDirectX::Initialize(WindowProperties properties) {
        mFlags = SDL_WINDOW_SHOWN;
        if (!WindowSDL::Initialize(properties)) {
            LL_LOG(Core::LogLevel::ERR, "SDLWindowDirectX SDL initialize error");
            return false;
        }

        D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
        D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                0, &featureLevel, 1, D3D11_SDK_VERSION, &mDevice, nullptr, &mDeviceContext);

        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Width = properties.Width;
        swapChainDesc.BufferDesc.Height = properties.Height;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = this->mSystemInfo.info.win.window;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Windowed = TRUE;

        D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
                nullptr, 0, nullptr, 0,
                D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, nullptr, &mDeviceContext);

        ID3D11Texture2D* backBufferTexture;
        mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTexture));
        mDevice->CreateRenderTargetView(backBufferTexture, nullptr, &mBackBuffer);

        mViewport.Width = properties.Width;
        mViewport.Height = properties.Height;
        mViewport.TopLeftX = 0;
        mViewport.TopLeftY = 0;
        mViewport.MinDepth = 0.0f;
        mViewport.MaxDepth = 1.0f;
        mDeviceContext->RSSetViewports(1, &mViewport);

        return true;
    }

    void SDLWindowDirectX::Clear() {
        const float clearColor[] = { 0.f, 0.f, 0.f, 1.f };
        mDeviceContext->ClearRenderTargetView(mBackBuffer, clearColor);
    }

    void SDLWindowDirectX::Swap() {
        mSwapChain->Present(0, 0);
    }

    void SDLWindowDirectX::Destroy() {
        mBackBuffer->Release();
        mSwapChain->Release();
        mDeviceContext->Release();
        mDevice->Release();
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
    }

    SDLWindowDirectX::~SDLWindowDirectX() {
        this->Destroy();
    }
}