#pragma once
#include "../Core/pch.hpp"

namespace LL::Renderer {
    constexpr char* GLSL_VERSION = (char*)"#version 400 core\n";

    constexpr char* SHADER_TEMPLATE_GLSL = (char*)
            "[VERSION]"
            "[LAYOUT]"
            "[UNIFORM]"
            "[INPUT]"
            "[OUTPUT]"
            "[FUNCTION]"
            "[ENTRY]";

    constexpr char* SHADER_SECTIONS_GLSL[] = {
            (char*)"[VERSION]",
            (char*)"[LAYOUT]",
            (char*)"[UNIFORM]",
            (char*)"[INPUT]",
            (char*)"[OUTPUT]",
            (char*)"[FUNCTION]",
            (char*)"[ENTRY]"
    };

    enum LL_CALL ShaderSectionGLSL : int {
        LL_VERSION = 0,
        LL_LAYOUT,
        LL_UNIFORM,
        LL_INPUT,
        LL_OUTPUT,
        LL_FUNCTION,
        LL_ENTRY
    };

    class LL_CALL ShaderStorageGL {
    public:
        ShaderStorageGL() = default;

        static ShaderStorageGL& GetInstance() {
            static ShaderStorageGL instance;
            return instance;
        }

        virtual std::string FontSDFVertex() noexcept {
            return std::string(GLSL_VERSION) +
                   "layout (location = 0) in vec4 vertex;\n"
                   "out vec2 TexCoords;\n"
                   "uniform mat4 projection;\n"
                   "void main()\n"
                   "{\n"
                   "    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
                   "    TexCoords = vertex.zw;\n"
                   "}";
        }

        virtual std::string FontSDFFrag() noexcept {
            return std::string(GLSL_VERSION) +
                    "in vec2 TexCoords;\n"
                    "out vec4 fragColor;\n"
                    "uniform sampler2D glyphTexture;\n"
                    "uniform vec3 textColor;\n"
                    "void main()\n"
                    "{\n"
                    "    float sdfValue = texture(glyphTexture, TexCoords).r;\n"
                    "    float alpha = smoothstep(0.455, 0.475, sdfValue);\n"
                    "\n"
                    "    fragColor = vec4(textColor, alpha);\n"
                    "}";
        }

        virtual std::string BlinnPhongFn() noexcept {
            return "\nvec3 BlinnPhong(vec3 ambientColor, vec3 diffuseColor, \n"
                   "   vec3 specularColor, vec3 normal, vec3 viewDir, Light light) {\n"
                   "   vec3 lightDir = normalize(light.position - gl_FragCoord.xyz);\n"
                   "   vec3 halfVector = normalize(viewDir + lightDir);\n"
                   "   float specularIntensity = pow(max(dot(normal, halfVector), 0.0), 32.0);\n"
                   "   float diffuseIntensity = max(dot(normal, lightDir), 0.0);\n"
                   "   vec3 result = ambientColor * light.ambient + \n"
                   "   diffuseColor * light.diffuse * diffuseIntensity + \n"
                   "   specularColor * light.specular * specularIntensity;\n"
                   "   return result;\n"
                   "}\n";
        }
        virtual std::string CascadeShadowsFn() noexcept {
            return
                // @position: position of the point for which the shadow needs to be calculated
                // @lightDirection: direction of the light source
                // @shadowMap: shadow map storing illuminance values
                // @cascadeIndex: shadow cascade index for the current point
                // @cascadeCount: total number of shadow cascades
                // @nearPlane: camera's near plane
                // @farPlane: far plane of the camera
                // @bias: offset for smoothing shadows
                // @return: returns the light value for a point, taking into account soft shadows
                    "\nfloat CascadeSoftShadows(vec3 position, vec3 lightDirection,\n"
                    " sampler2D shadowMap, int cascadeIndex, int cascadeCount, float nearPlane,\n"
                    " float farPlane, float bias) {\n"
                    "   vec2 texCoords = (position.xy / position.z) * 0.5 + 0.5;\n"
                    "   float currentDepth = texture(shadowMap, texCoords).r\n"
                    "   float previousDepth = currentDepth;\n"
                    "   if (cascadeIndex > 0) {\n"
                    "       float cascadeDepth = float(cascadeIndex) /\n"
                    "       float(cascadeCount) * (farPlane - nearPlane) + nearPlane;\n"
                    "       vec2 previousTexCoords = (position.xy / position.z) * 0.5 * previousDepth / cascadeDepth + 0.5;\n"
                    "       previousDepth = texture(shadowMap, previousTexCoords).r;\n"
                    "   }\n"
                    "   float shadowAmount = 0.0;\n"
                    "   for (int i = 0; i < 4; i++) {\n"
                    "       float depth = currentDepth + float(i) * (previousDepth - currentDepth) / 4.0;\n"
                    "       shadowAmount += step(depth + bias, position.z);\n"
                    "   }\n"
                    "   shadowAmount /= 4.0;\n"
                    "   return shadowAmount;\n"
                    "}\n";
        }
    };

    enum LL_CALL ShaderTypesGLSL : int {
        LL_FLOAT = 0, LL_INT,
        LL_VEC2, LL_VEC3, LL_VEC4,
        LL_MAT3, LL_MAT4,
        LL_SAMPLER2D,
    };

    static const std::string SHADER_TYPE_STORE[] = {
            "float", "int",
            "vec2", "vec3", "vec4",
            "mat3", "mat4",
            "sampler2D"
    };

    static inline const std::string GetSection(ShaderSectionGLSL section) noexcept {
        return SHADER_SECTIONS_GLSL[section];
    }

    static inline const std::string GetTypeGLSL(ShaderTypesGLSL type) noexcept {
        return SHADER_TYPE_STORE[type];
    }

    static inline const std::string UniformGLSL(const std::string& name, ShaderTypesGLSL type) noexcept {
        return "uniform " + GetTypeGLSL(type) + " " + name + ";\n";
    }

    static inline const std::string InGLSL(const std::string& name, ShaderTypesGLSL type) noexcept {
        return "in " + GetTypeGLSL(type) + " " + name + ";\n";
    }

    static inline const std::string OutGLSL(const std::string& name, ShaderTypesGLSL type) noexcept {
        return "out " + GetTypeGLSL(type) + " " + name + ";\n";
    }

    static inline const std::string GetLayoutString(const std::string& name, ShaderTypesGLSL type, unsigned locNum = 0) noexcept {
        return "layout (location = " + std::to_string(locNum) + ") in " + GetTypeGLSL(type) + " " + name + ";\n";
    }
}