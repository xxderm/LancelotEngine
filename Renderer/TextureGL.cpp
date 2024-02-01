#include "TextureGL.hpp"

namespace LL::Renderer {
    void TextureGL::Create(void* data, unsigned int width, unsigned int height) {
        glGenTextures(1, &Id);
        glBindTexture(GL_TEXTURE_2D, Id);

        this->Width = width;
        this->Height = height;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGB, GL_FLOAT, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    void TextureGL::Destroy() {

    }
    TextureGL::~TextureGL() {
        this->Destroy();
    }
}