#pragma once
#include "Texture.hpp"
#include "renderer_gl_pch.hpp"

namespace LL::Renderer {
    // TODO: Create basic gl texture and make their child based on tex. parameters

    class LL_CALL TextureGL final : public Texture {
    public:
        void Create(unsigned char* data, unsigned int width, unsigned int height) override;
        void Destroy() override;
        ~TextureGL() override;
    };
}