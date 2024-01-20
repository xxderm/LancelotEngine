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

    ShaderProgramGL::ShaderProgramGL() {
        this->mId = glCreateProgram();
    }

    ShaderProgramGL::~ShaderProgramGL() {
        glDeleteProgram(this->mId);
    }

    void ShaderProgramGL::Compile() {
        LL_PROFILE
        LL_LOG(Core::INFO, "Shader compile process");
        for (auto& shader : mShaders) {
            auto &body = shader.second;
            const char* sCode = body.Source.c_str();
            switch (shader.first) {
                case ShaderType::LL_VERT: {
                    body.Id = glCreateShader(GL_VERTEX_SHADER);
                    break;
                }
                case LL_FRAG: {
                    body.Id = glCreateShader(GL_FRAGMENT_SHADER);
                    break;
                }
                case LL_TESS_CONT: {
                    body.Id = glCreateShader(GL_TESS_CONTROL_SHADER);
                    break;
                }
                case LL_TESS_EVAL: {
                    body.Id = glCreateShader(GL_TESS_EVALUATION_SHADER);
                    break;
                }
                case LL_GEOMETRY: {
                    body.Id = glCreateShader(GL_GEOMETRY_SHADER);
                    break;
                }
            }
            glShaderSource(body.Id, 1, &sCode, NULL);
            glCompileShader(body.Id);
            this->CompileErrors();
        }

        for (auto& shader : mShaders) {
            auto& body = shader.second;
            glAttachShader(this->mId, body.Id);
        }

        glLinkProgram(this->mId);
        this->CompileErrors();

        for (auto& shader : mShaders) {
            auto& body = shader.second;
            glDeleteShader(body.Id);
        }
    }

    void ShaderProgramGL::Set(const ShaderType &type, const std::string &src) {
        LL_PROFILE
        if (mShaders.count(type) > 0) {
            LL_LOG(Core::INFO, "Update shader with code: ", (int)type);
            auto& sBody = mShaders.at(type);
            sBody.Source = src;
            return;
        }
        LL_LOG(Core::INFO, "Shader add with code: ", (int)type);
        ShaderBody shaderBody;
        shaderBody.Source = src;
        mShaders.insert({ type, shaderBody });
    }

    void ShaderBuilderGL::ProcessVertex() noexcept {

    }

    void ShaderBuilderGL::ProcessFragment() noexcept {

    }

    void ShaderBuilderGL::ProcessTessControl() noexcept {

    }

    void ShaderBuilderGL::ProcessTessEval() noexcept {

    }

    void ShaderBuilderGL::ProcessGeometry() noexcept {

    }
}