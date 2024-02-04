#pragma once
#include "../Core/pch.hpp"
#include "TextureGL.hpp"
#include "../Core/Log.hpp"
#include "ShaderOpenGL.hpp"

namespace LL::Renderer {
    // TODO: Make string renderer

    class LL_CALL TextGL;

    class LL_CALL FontGL final {
    public:
        FontGL() = default;
        FontGL(const FontGL&) = delete;
        ~FontGL();
        friend class TextGL;
    public:
        void Initialize(const std::string& path);
        void AddGlyphs(std::wstring glyphs);
    private:
        struct Glyph {
            std::shared_ptr<TextureGL> Texture;
            double TopPoint;
            double Right;
            double Top;
            double Bot;
            double Left;
            int PixelSize;
        };
    private:
        msdfgen::FreetypeHandle* mFtHandle{};
        msdfgen::FontHandle* mFontHandle{};
        std::string mFontPath{};
        std::map<uint32_t, Glyph> mGlyphs;
    };

    class LL_CALL TextGL final {
    public:
        TextGL() = default;
        static TextGL& GetInstance();
        void Initialize();
        void AddFont(std::string fontKey, std::string path, std::wstring glyphs);
        void Render(std::string fontKey, std::wstring text, float x, float y, int size, glm::vec3 color);
    private:
        unsigned int mGlyphBufferArrayId{};
        unsigned  int mGlyphBufferObjectId{};
        std::shared_ptr<Renderer::ShaderProgramGL> mGlyphShader{};
        std::map<std::string, std::shared_ptr<FontGL>> mFonts{};
        bool mAlreadyInit = false;
    };
}