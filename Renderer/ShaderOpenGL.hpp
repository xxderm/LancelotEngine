#pragma once
#include "ShaderMetaDataOpenGL.hpp"
#include "renderer_gl_pch.hpp"
#include "../Core/Log.hpp"
#include "../Core/SessionProfiler.hpp"

namespace LL::Renderer {
    enum LL_CALL ShaderType : int {
        LL_VERT = 0, LL_FRAG,
        LL_TESS_CONT, LL_TESS_EVAL,
        LL_GEOMETRY
    };

    struct LL_CALL ShaderBody final {
        GLuint Id{};
        std::string Source{};
    };

    class LL_CALL ShaderProgramGL {
    public:
        ShaderProgramGL();
        void Compile();
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
        void Set(const ShaderType& type, const std::string& src);
        void Release();
        ~ShaderProgramGL();
    private:
        void CompileErrors();
    private:
        GLuint mId{};
        std::unordered_map<ShaderType, ShaderBody> mShaders{};
    };

    class LL_CALL ShaderBuilderGL {
    protected:
        virtual void ProcessVertex() noexcept;
        virtual void ProcessFragment() noexcept;
        virtual void ProcessTessControl() noexcept;
        virtual void ProcessTessEval() noexcept;
        virtual void ProcessGeometry() noexcept;
    public:
        void SectionFunctions() {
            // Example:
            //  string section = "[text]";
            //  string textBody =
            //        "layout (location=0) in vec3 pos;\n"
            //        "layout (location=0) in vec3 norm;\n";
            //  string shaderContent =
            //        "\n[version]\n"
            //        "\n[text]\n"
            //        "\n[entry]\n";
            //  auto begIndex = shaderContent.find(section);
            //  shaderContent.replace(begIndex, section.size(), textBody);
        }
        // Каждая функция должна принимать необходимые для хранимой функции параметры, кроме стандартных
        // (position, normal, tangent, bitangent, ... )
        void SetBlinnPhong();
        void SetCascadeShadows();
        void SetUpTessellation();
        void SetUpGeometry();
        std::shared_ptr<ShaderProgramGL> Build() const noexcept;
        // section name -> section code
        std::map<std::string, std::string> mSectionCode;
        // TODO:
        // Добавить std::map для отслеживания подключения функций.
        // К примеру, есть несколь видов фукнций тени, можно использовать только одну
        // Хранить к примеру так key => <Shadows> value => true
        // Если будет true, то перезаписать на новую функцию, и убрать старую
    };
}