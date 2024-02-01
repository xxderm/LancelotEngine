#pragma once
#include "renderer_gl_pch.hpp"
#include "../Core/pch.hpp"
#include "TextureGL.hpp"
#include "ShaderMetaDataOpenGL.hpp"
#include "../Core/Log.hpp"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "msdfgen.h"
#include "msdfgen-ext.h"
namespace LL::Renderer {
    class Shader
    {
    public:
        Shader() {}
        void cr() {
            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            auto shaders = ShaderStorageGL::GetInstance();
            std::string vCode = shaders.FontSDFVertex();
            std::string fCode = shaders.FontSDFFrag();
            auto vCodeChar = vCode.c_str();
            auto fCodeChar = fCode.c_str();

            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vCodeChar, NULL);
            glCompileShader(vertex);
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                LL_LOG(Core::ERR, infoLog);
                std::cout << infoLog << std::endl;
            };

            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fCodeChar, NULL);
            glCompileShader(fragment);
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                std::cout << infoLog << std::endl;

            };


            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
            }

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }
        unsigned int ID;
        void use() {
            glUseProgram(ID);
        }
        void setBool(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(const std::string &name, int value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
    };

    // TODO: make multiple fonts
    // TODO: add glyph range selection
    class LL_CALL TextGL final {
    public:
        static TextGL& GetInstance();
        bool AddFont(std::string path);
        void RenderText(std::string text, float x, float y, int size, glm::vec3 color)
        {
            float pxRange = (static_cast<float>(size) / 32.f) * 3.85f;

            // activate corresponding render state
            s.use();
            glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

            glUniformMatrix4fv(glGetUniformLocation(s.ID, "projection"), 1, false, &projection[0][0]);
            glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(VAO);
            s.setInt("glyphTexture", 0);
            s.setFloat("pxRange", pxRange);

            // iterate through all characters
            std::string::const_iterator c;
            float spacing = 3;
            for (c = text.begin(); c != text.end(); c++)
            {
                if (*c == ' ') {
                    x += (13.f * (static_cast<float>(size) / 32.f));
                    continue;
                }

                Glyph ch = mGlyps[*c];
                float scale = static_cast<float>(size) / static_cast<float>(ch.PixelSize);

                float xpos = x;
                float ypos = y - ( (ch.PixelSize - ch.Bot) * scale );

                float w = (ch.PixelSize) * scale;
                float h = (ch.PixelSize) * scale;
                // update VBO for each character
                float vertices[6][4] = {
                        { xpos,     ypos,   0.0f, 0.0f },
                        { xpos,     ypos + h,       0.0f, 1.0f },
                        { xpos + w, ypos + h,       1.0f, 1.0f },

                        { xpos,     ypos,   0.0f, 0.0f },
                        { xpos + w, ypos + h,       1.0f, 1.0f },
                        { xpos + w, ypos,   1.0f, 0.0f }
                };
                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.Texture->Id);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                x += ((ch.Right) * scale) + (spacing - ch.Left); // bitshift by 6 to get value in pixels (2^6 = 64)
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void RenderText(std::wstring text, float x, float y, int size, glm::vec3 color)
        {
            float pxRange = (static_cast<float>(size) / 32.f) * 3.85f;

            // activate corresponding render state
            s.use();
            glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

            glUniformMatrix4fv(glGetUniformLocation(s.ID, "projection"), 1, false, &projection[0][0]);
            glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(VAO);
            s.setInt("glyphTexture", 0);
            s.setFloat("pxRange", pxRange);

            // iterate through all characters
            std::wstring::const_iterator c;
            float spacing = 3;
            for (c = text.begin(); c != text.end(); c++)
            {
                if (*c == ' ') {
                    x += (13.f * (static_cast<float>(size) / 32.f));
                    continue;
                }

                Glyph ch = mGlyps[*c];
                float scale = static_cast<float>(size) / static_cast<float>(ch.PixelSize);

                float xpos = x;
                float ypos = y - ( (ch.PixelSize - ch.Bot) * scale );

                float w = (ch.PixelSize) * scale;
                float h = (ch.PixelSize) * scale;
                // update VBO for each character
                float vertices[6][4] = {
                        { xpos,     ypos,   0.0f, 0.0f },
                        { xpos,     ypos + h,       0.0f, 1.0f },
                        { xpos + w, ypos + h,       1.0f, 1.0f },

                        { xpos,     ypos,   0.0f, 0.0f },
                        { xpos + w, ypos + h,       1.0f, 1.0f },
                        { xpos + w, ypos,   1.0f, 0.0f }
                };
                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.Texture->Id);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                x += ((ch.Right) * scale) + (spacing - ch.Left); // bitshift by 6 to get value in pixels (2^6 = 64)
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    public:
        struct Glyph {
            std::shared_ptr<TextureGL> Texture;
            double TopPoint; // Точка начала сверху
            double Right; // Пикселей снизу-вверх
            double Top; // Пикселей слева-вправо
            double Bot;
            double Left;
            int PixelSize;
        };

        TextGL() = default;
        std::map<uint32_t, Glyph> mGlyps;
        unsigned int VAO, VBO;
        Shader s;
    };
}