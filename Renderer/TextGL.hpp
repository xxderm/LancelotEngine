#pragma once
#include "../Core/pch.hpp"
#include "TextureGL.hpp"
#include "../Core/Log.hpp"
#include "ShaderOpenGL.hpp"

namespace LL::Renderer {
    class LL_CALL TextGL;
    template<typename T>
    concept GlyphList = std::same_as<T, std::string> || std::same_as<T, std::wstring>;

    class LL_CALL FontGL final {
    public:
        FontGL() = default;
        FontGL(const FontGL&) = delete;
        ~FontGL();
        friend class TextGL;
    public:
        void Initialize(const std::string& path);
        //template<GlyphList T>
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
        //template<GlyphList T>
        void AddFont(std::string fontKey, std::string path, std::wstring glyphs);
        //template<GlyphList T>
        void Render(std::string fontKey, std::wstring text, float x, float y, int size, glm::vec3 color);
    private:
        unsigned int mGlyphBufferArrayId{};
        unsigned  int mGlyphBufferObjectId{};
        std::shared_ptr<Renderer::ShaderProgramGL> mGlyphShader{};
        std::map<std::string, std::shared_ptr<FontGL>> mFonts{};
    };
}