#pragma once
#include "../Core/pch.hpp"

namespace LL::Renderer {
    // TODO: combine with resource manager
    struct LL_CALL Texture {
        virtual void Create(unsigned char* data, unsigned int width, unsigned int height) = 0;
        virtual void Destroy() = 0;
        virtual ~Texture() = default;
    public:
        unsigned int Id{};
        unsigned int Width{};
        unsigned int Height{};
    };
}