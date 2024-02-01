#include "TextGL.hpp"

namespace LL::Renderer {
    TextGL &LL::Renderer::TextGL::GetInstance() {
        static TextGL instance;
        return instance;
    }

    const auto specialGlyphs = {
            '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
            '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
            '"', ';', '%',':', '?',
    };

    bool TextGL::AddFont(std::string path) {
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        msdfgen::FreetypeHandle *ftm = msdfgen::initializeFreetype();
        if (ftm) {
            msdfgen::FontHandle *font = loadFont(ftm, path.c_str());
            if (font) {
                msdfgen::Shape shape;
                for (auto i = L'а'; i <= L'я'; i++) {
                    if (msdfgen::loadGlyph(shape, font, i)) {
                        shape.normalize();
                        //                      max. angle
                        msdfgen::edgeColoringSimple(shape, 3.0);
                        //           image width, height
                        msdfgen::Bitmap<float, 3> msdf(32, 32);
                        auto bounds = shape.getBounds();
                        //                     range, scale, translation
                        generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(0.0, abs(bounds.b - 2)));

                        // Create texture
                        auto glyphTexture = std::make_shared<TextureGL>();
                        glyphTexture->Create(msdf.operator()(0, 0), msdf.width(), msdf.height());
                        //

                        //msdfgen::savePng(msdf, "TEST.png");
                        //break;

                        Glyph glyph = {
                                glyphTexture,
                                32 - bounds.t,
                                bounds.r + bounds.l,
                                bounds.t,
                                bounds.b,
                                bounds.l,
                                32
                        };
                        mGlyps.insert(std::pair<uint32_t, Glyph>(i, glyph));
                    }
                }
                destroyFont(font);
            }
            deinitializeFreetype(ftm);
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        s.cr();
        return true;
    }
}