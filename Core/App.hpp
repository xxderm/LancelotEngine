#pragma once
#include <utility>

#include "pch.hpp"
#include "../impexp.hpp"

namespace LL::Core {
    class LL_CALL App {
    public:
        void SetTitle(std::string title) { this->mTitle = std::move(title); }
        void Resize(uint32_t w, uint32_t h) { mWinSizeW = w; mWinSizeH = h; }
        virtual ~App() = default;
        std::string GetTitle() { return mTitle; }
        uint32_t GetWinSizeW() { return mWinSizeW; }
        uint32_t GetWinSizeH() { return mWinSizeH; }
    private:
        std::string mTitle = "Window";
        uint32_t mWinSizeW = 800;
        uint32_t mWinSizeH = 600;
    };
}