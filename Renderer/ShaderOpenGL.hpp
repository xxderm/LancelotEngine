#pragma once
#include "ShaderMetaDataOpenGL.hpp"
#include "renderer_gl_pch.hpp"

namespace LL::Renderer {
    class LL_CALL ShaderProgramGL {
    public:
        ShaderProgramGL() = default;
        virtual void Compile(std::string shaderText) = 0;
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetVec2(const std::string& name, const glm::vec2& value) const;
        void SetVec2(const std::string& name, float x, float y) const;
        void SetVec3(const std::string& name, const glm::vec3& value) const;
        void SetVec3(const std::string& name, float x, float y, float z) const;
        void SetVec4(const std::string& name, const glm::vec4& value) const;
        void SetVec4(const std::string& name, float x, float y, float z, float w) const;
        void SetMat2(const std::string& name, const glm::mat2& mat) const;
        void SetMat3(const std::string& name, const glm::mat3& mat) const;
        void SetMat4(const std::string& name, const glm::mat4& mat) const;
        void Bind();
        void Release();
        virtual ~ShaderProgramGL() = default;
    protected:
        void CompileErrors();
        void LinkShader(GLuint linkId, const char* code);
    protected:
        GLuint mId{};
    };

    class LL_CALL VertexProgramGL final : public ShaderProgramGL {
    public:
        void Compile(std::string shaderText) override;
    private:
        GLuint mVertexId{};
    };

    class LL_CALL FragmentProgramGL final : public ShaderProgramGL {
    public:
        void Compile(std::string shaderText) override;
    private:
        GLuint mFragmentId{};
    };

    class LL_CALL TessControlProgramGL final : public ShaderProgramGL {
    public:
        void Compile(std::string shaderText) override;
    private:
        GLuint mTessControlId{};
    };

    class LL_CALL TessEvalProgramGL final : public ShaderProgramGL {
    public:
        void Compile(std::string shaderText) override;
    private:
        GLuint mTessEvalId{};
    };

    class LL_CALL GeometryProgramGL final : public ShaderProgramGL {
    public:
        void Compile(std::string shaderText) override;
    private:
        GLuint mGeomId{};
    };

    class LL_CALL ShaderGL final {
    public:
        void Bind() noexcept;
        void Release() noexcept;

    // bind/release
    // set uniform values
    // set layouts
    // mb UBO?
    // mb TBO?
    };

    class LL_CALL ShaderPipelineGL final {
    public:
        ShaderPipelineGL() = default;
        bool Compile() const noexcept;
        std::shared_ptr<ShaderGL> GetVertex() const noexcept;
        std::shared_ptr<ShaderGL> GetFragment() const noexcept;
        std::shared_ptr<ShaderGL> GetTessControl() const noexcept;      // return mShadersGL[] or nullptr
        std::shared_ptr<ShaderGL> GetTessEval() const noexcept;
        ~ShaderPipelineGL();
    private:
        // Vertex => ShaderTextSrc, Fragment => ShaderTextSrc
        std::map<std::string, std::string> mShadersGL;
    };

    class LL_CALL ShaderBuilderGL final {
    public:
        // Каждая функция должна принимать необходимые для хранимой функции параметры
        void SetBlinnPhong();
        void SetCascadeShadows();
        void SetUpTessellation();
        void SetUpGeometry();
        std::shared_ptr<ShaderPipelineGL> Build() const noexcept;
        // TODO:
        // Добавить std::map для отслеживания подключения функций.
        // К примеру, есть несколь видов фукнций тени, можно использовать только одну
        // Хранить к примеру так key => <Shadows> value => true
        // Если будет true, то перезаписать на новую функцию, и убрать старую

    };
}