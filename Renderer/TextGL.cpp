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

            //
            
            Glyph glyph = {
                0,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (uint32_t)face->glyph->advance.x
            };
            mGlyps.insert(std::pair<uint32_t, Glyph>(i, glyph));
        }
        return true;
    }
}