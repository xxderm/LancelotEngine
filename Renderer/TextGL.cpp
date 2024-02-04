#include "TextGL.hpp"

namespace LL::Renderer {
    TextGL &LL::Renderer::TextGL::GetInstance() {
        static TextGL instance;
        return instance;
    }

    // TODO: placeholder, use Renderer/BuffersOpenGL
    void TextGL::Initialize() {
        LL_PROFILE
        mAlreadyInit = true;
        glGenVertexArrays(1, &mGlyphBufferArrayId);
        glGenBuffers(1, &mGlyphBufferObjectId);
        glBindVertexArray(mGlyphBufferArrayId);
        glBindBuffer(GL_ARRAY_BUFFER, mGlyphBufferObjectId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        mGlyphShader = std::make_shared<ShaderProgramGL>();
        auto fontVertex = ShaderStorageGL::GetInstance().FontSDFVertex();
        auto fontFrag = ShaderStorageGL::GetInstance().FontSDFFrag();
        mGlyphShader->Set(ShaderType::LL_VERT, fontVertex);
        mGlyphShader->Set(ShaderType::LL_FRAG, fontFrag);
        mGlyphShader->Compile();
    }

    void TextGL::AddFont(std::string fontKey, std::string path, std::wstring glyphs) {
        LL_PROFILE
        if (!mAlreadyInit) this->Initialize();
        mFonts.insert({fontKey, std::make_shared<FontGL>()});
        mFonts.at(fontKey)->Initialize(path);
        mFonts.at(fontKey)->AddGlyphs(glyphs);
    }

    void TextGL::Render(std::string fontKey, std::wstring text, float x, float y, int size, glm::vec3 color) {
        float pxRange = (static_cast<float>(size) / 32.f) * 3.85f;

        mGlyphShader->Bind();
        // TODO: fix
        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

        mGlyphShader->SetMat4("projection", projection);
        mGlyphShader->SetVec3("textColor", color);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(mGlyphBufferArrayId);

        mGlyphShader->SetInt("glyphTexture", 0);
        mGlyphShader->SetFloat("pxRange", pxRange);

        float spacing = 3;
        for (const auto& c : text) {
            if (c == ' ') {
                x += (13.f * (static_cast<float>(size) / 32.f));
                continue;
            }

            const auto& ch = mFonts.at(fontKey)->mGlyphs.at(c);
            float scale = static_cast<float>(size) / static_cast<float>(ch.PixelSize);

            float xpos = x;
            float ypos = y - ( (ch.PixelSize - ch.Bot) * scale );

            float w = (ch.PixelSize) * scale;
            float h = (ch.PixelSize) * scale;

            float vertices[6][4] = {
                    { xpos,     ypos,   0.0f, 0.0f },
                    { xpos,     ypos + h,       0.0f, 1.0f },
                    { xpos + w, ypos + h,       1.0f, 1.0f },

                    { xpos,     ypos,   0.0f, 0.0f },
                    { xpos + w, ypos + h,       1.0f, 1.0f },
                    { xpos + w, ypos,   1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, ch.Texture->Id);
            glBindBuffer(GL_ARRAY_BUFFER, mGlyphBufferObjectId);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);
            x += ((ch.Right) * scale) + (spacing - ch.Left);
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void FontGL::Initialize(const std::string& path) {
        LL_PROFILE
        mFontPath = path;
        mFtHandle = msdfgen::initializeFreetype();
        if (!mFtHandle)
            LL_LOG(Core::LogLevel::ERR, "Error when trying to initialize freetype");
        mFontHandle = loadFont(mFtHandle, path.c_str());
        if (!mFontHandle)
            LL_LOG(Core::LogLevel::ERR, "Error when trying to load font: ", path);
    }

    FontGL::~FontGL() {
        if (mFontHandle)
            msdfgen::destroyFont(mFontHandle);
        if (mFtHandle)
            msdfgen::deinitializeFreetype(mFtHandle);
    }

    void FontGL::AddGlyphs(const std::wstring glyphs) {
        LL_PROFILE
        msdfgen::Shape currentShape;
        for (const auto& glyph : glyphs) {
            if (!msdfgen::loadGlyph(currentShape, mFontHandle, glyph)) {
                //LL_LOG(Core::LogLevel::ERR, "Error when trying load glyph: ", glyph, " font: ", mFontPath);
                continue;
            }

            currentShape.normalize();
            msdfgen::edgeColoringSimple(currentShape, 3.0);
            msdfgen::Bitmap<float, 3> msdf(32, 32);

            auto bounds = currentShape.getBounds();
            generateMSDF(msdf, currentShape, 4.0, 1.0,
                         msdfgen::Vector2(0.0, abs(bounds.b - 2)));

            auto glyphTexture = std::make_shared<TextureGL>();
            glyphTexture->Create(msdf.operator()(0, 0), msdf.width(), msdf.height());

            Glyph currentGlyph = {
                    glyphTexture,
                    32 - bounds.t,
                    bounds.r + bounds.l,
                    bounds.t,
                    bounds.b,
                    bounds.l,
                    32
            };
            mGlyphs.insert(std::pair<uint32_t, Glyph>(glyph, currentGlyph));
        }
    }
}