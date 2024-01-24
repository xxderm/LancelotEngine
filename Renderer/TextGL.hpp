#pragma once
#include "renderer_gl_pch.hpp"
#include "../Core/pch.hpp"

namespace LL::Renderer {
    // TODO: make multiple fonts
    // TODO: add glyph range selection
    class LL_CALL TextGL final {
    public:
        static TextGL& GetInstance();
        bool AddFont(std::string path);
    public:
        struct Glyph {
            uint32_t Texture;
            glm::ivec2 Size;
            glm::ivec2 Bearing;
            uint32_t Advance;
        };
    private:
        TextGL() = default;
        TextGL(const TextGL&);
        std::map<uint32_t, Glyph> mGlyps;
    };
}