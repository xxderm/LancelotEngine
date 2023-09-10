#include "BuffersOpenGL.hpp"

namespace LL::Renderer {
    void VertexBufferGL::CreateBuffer(const std::string &name, GLuint size) {
        VertexBufferObject vertexBufferObject;
        vertexBufferObject.Size = size;
        glGenBuffers(1, &vertexBufferObject.Id);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject.Id);
        mVbos.emplace(name, vertexBufferObject);
    }

    void VertexBufferGL::AddBuffer(const std::string &name, const std::vector<glm::vec3> &vertices) {
        if (!mVbos.contains(name)) {
            this->CreateBuffer(name, vertices.size() * sizeof(glm::vec3));
        }
        VertexBufferObject& vertexBufferObject = mVbos.at(name);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject.Id);
        if (vertexBufferObject.Size < vertices.size() * sizeof(glm::vec3)) {
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
            vertexBufferObject.Size = vertices.size() * sizeof(glm::vec3);
        }
        else {
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data());
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VertexBufferGL::~VertexBufferGL() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        for (auto& vertexBuffer : mVbos) {
            glDeleteBuffers(1, &vertexBuffer.second.Id);
        }
    }

    void VertexBufferGL::SetAttributPointer(const std::string &name, GLuint index, GLuint size, GLenum type,
                                            bool normalized, int stride, int offset) {
        if (mVbos.contains(name)) {
            VertexBufferObject& vertexBufferObject = mVbos.at(name);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject.Id);
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<void*>(offset));
        }
    }

    void IndexBufferGL::SetIndices(const std::vector<GLuint> &indices) {
        glGenBuffers(1, &mId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        this->mCount = indices.size();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    IndexBufferGL::~IndexBufferGL() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &mId);
    }

    VertexArrayObjectGL::VertexArrayObjectGL() {
        glGenVertexArrays(1, &mId);
        glBindVertexArray(mId);
    }

    VertexArrayObjectGL::~VertexArrayObjectGL() {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &mId);
    }

}
