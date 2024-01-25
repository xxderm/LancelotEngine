#include "TextGL.hpp"

namespace LL::Renderer {
    TextGL &LL::Renderer::TextGL::GetInstance() {
        static TextGL instance;
        return instance;
    }

    bool TextGL::AddFont(std::string path) {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            return false;
        }
        FT_Face face;
        if (FT_New_Face(ft, path.c_str(), 0, &face)) {
            return false;
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        FT_GlyphSlot slot = face->glyph;
        for (uint32_t i = 0; i < 255; i++) {
            if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                // error message
                continue;
            }
            auto ftRenderGlyphStatus = FT_Render_Glyph(slot, FT_RENDER_MODE_SDF);
            int width = slot->bitmap.width;
            int height = slot->bitmap.rows;
            unsigned char* buffer = slot->bitmap.buffer;

            // Create texture
            auto glyphTexture = std::make_shared<TextureGL>();
            glyphTexture->Create(buffer, width, height);
            //
            
            Glyph glyph = {
                glyphTexture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (uint32_t)face->glyph->advance.x
            };
            mGlyps.insert(std::pair<uint32_t, Glyph>(i, glyph));
        }
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        s.cr();
        return true;
    }
}