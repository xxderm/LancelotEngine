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
    private:
        std::string mTitle{};
        uint32_t mWinSizeW{};
        uint32_t mWinSizeH{};
    };
}