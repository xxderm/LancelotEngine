#pragma once
#include "renderer_gl_pch.hpp"
#include "../Core/pch.hpp"

namespace LL::Renderer {
    class LL_CALL VertexArrayObjectGL;

    class LL_CALL VertexBufferGL final {
    public:
        // TODO:
        // Add overload vertices (void*, float, vec2, vec4)
        void AddBuffer(
                const std::string& name,
                const std::vector<glm::vec3>& vertices);
        void SetAttributPointer(const std::string& name, GLuint index, GLuint size, GLenum type,
                bool normalized, int stride, int offset);
        ~VertexBufferGL();
    private:
        void CreateBuffer(const std::string& name, GLuint size);
    private:
        friend class VertexArrayObjectGL;
        struct VertexBufferObject final {
            GLuint Id{};
            GLuint Size{};
        };
        std::unordered_map<std::string, VertexBufferObject> mVbos;
    };

    class LL_CALL IndexBufferGL final {
    public:
        void SetIndices(const std::vector<GLuint>& indices);
        ~IndexBufferGL();
    private:
        friend class VertexArrayObjectGL;
        GLuint mCount{};
        GLuint mId{};
    };

    // TODO:
    class LL_CALL UniformBufferModel {
    protected:
        void InitializeStructure() {

        }
        void Update() {

        }
    protected:
        std::unordered_map<std::string, std::string> mStructure;
    private:
        GLuint mSize{};
        GLuint mId{};
    };
    #define Record(type, name) mStructure.emplace(#type, #name)


    // TODO: Сделать класс - пайплайн рендеринга, который будет биндить VAO и рендерить
    //       зависимости от нужного примитива/патча
    class LL_CALL VertexArrayObjectGL final {
    public:
        VertexArrayObjectGL();
        ~VertexArrayObjectGL();
    private:
        GLuint mId{};
        std::shared_ptr<VertexBufferGL> mBuffers;
        std::shared_ptr<IndexBufferGL> mIndices;
    };

}