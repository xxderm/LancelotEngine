#include "ShaderOpenGL.hpp"

namespace LL::Renderer {
    void ShaderProgramGL::SetBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(mId, name.c_str()), (int)value);
    }

    void ShaderProgramGL::SetInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(mId, name.c_str()), value);
    }

    void ShaderProgramGL::SetFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(mId, name.c_str()), value);
    }

    void ShaderProgramGL::SetVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(mId, name.c_str()), 1, &value[0]);
    }

    void ShaderProgramGL::SetVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(mId, name.c_str()), x, y);
    }

    void ShaderProgramGL::SetVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(mId, name.c_str()), 1, &value[0]);
    }

    void ShaderProgramGL::SetVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(mId, name.c_str()), x, y, z);
    }

    void ShaderProgramGL::SetVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(mId, name.c_str()), 1, &value[0]);
    }

    void ShaderProgramGL::SetVec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(mId, name.c_str()), x, y, z, w);
    }

    void ShaderProgramGL::SetMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void ShaderProgramGL::SetMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void ShaderProgramGL::SetMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void ShaderProgramGL::Bind() {
        glUseProgram(mId);
    }

    void ShaderProgramGL::Release() {
        glUseProgram(0);
    }

    void ShaderProgramGL::CompileErrors() {
        GLint success;
        GLchar infoLog[1024];

        glGetShaderiv(mId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(mId, 1024, NULL, infoLog);
            LL_LOG(LL::Core::ERR, "ERROR::SHADER_COMPILATION_ERROR of type: ", infoLog);
        }

        glGetProgramiv(mId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(mId, 1024, NULL, infoLog);
            LL_LOG(LL::Core::ERR, "ERROR::PROGRAM_LINKING_ERROR of type: ", infoLog);
        }
    }

    void ShaderProgramGL::LinkShader(GLuint linkId, const char* code) {
        glShaderSource(linkId, 1, &code, NULL);
        glCompileShader(linkId);
        this->CompileErrors();
        mId = glCreateProgram();
        glAttachShader(mId, linkId);
        glLinkProgram(mId);
        this->CompileErrors();
        glDeleteShader(linkId);
    }

    void VertexProgramGL::Compile(std::string shaderText) {
        mVertexId = glCreateShader(GL_VERTEX_SHADER);
        const char* shaderCode = shaderText.c_str();
        this->LinkShader(mVertexId, shaderCode);
    }

    void FragmentProgramGL::Compile(std::string shaderText) {
        mFragmentId = glCreateShader(GL_FRAGMENT_SHADER);
        const char* shaderCode = shaderText.c_str();
        this->LinkShader(mFragmentId, shaderCode);
    }

    void TessControlProgramGL::Compile(std::string shaderText) {
        mTessControlId = glCreateShader(GL_TESS_CONTROL_SHADER);
        const char* shaderCode = shaderText.c_str();
        this->LinkShader(mTessControlId, shaderCode);
    }

    void TessEvalProgramGL::Compile(std::string shaderText) {
        mTessEvalId = glCreateShader(GL_TESS_EVALUATION_SHADER);
        const char* shaderCode = shaderText.c_str();
        this->LinkShader(mTessEvalId, shaderCode);
    }

    void GeometryProgramGL::Compile(std::string shaderText) {
        mGeomId = glCreateShader(GL_GEOMETRY_SHADER);
        const char* shaderCode = shaderText.c_str();
        this->LinkShader(mGeomId, shaderCode);
    }
}