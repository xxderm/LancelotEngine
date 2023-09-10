#pragma once
#include <string>

namespace LL::Renderer {

    std::string SHADER_GLSL_TEMPLATE =
            "[VERSION]\n"
            "[LAYOUTS]\n"
            "[UNIFORMS]\n"
            "[INPUTS]\n"
            "[OUTPUTS]\n"
            "[FUNCTIONS]\n"
            "[ENTRY]\n";

    class LL_CALL ShaderFunctionsStoreGL {
    public:
        virtual std::string BlinnPhong() noexcept {
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
        virtual std::string CascadeShadows() noexcept {
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

    std::string SHADER_TYPE_STORE[] = {
            "float", "int",
            "vec2", "vec3", "vec4",
            "mat3", "mat4",
            "sampler2D"
    };

    static inline const std::string GetTypeGLSL(ShaderTypesGLSL type) noexcept {
        return SHADER_TYPE_STORE[type];
    }

    static inline const std::string UniformGLSL(ShaderTypesGLSL type) noexcept {
        return "uniform " + SHADER_TYPE_STORE[type];
    }

    static inline const std::string InGLSL(ShaderTypesGLSL type) noexcept {
        return "in " + SHADER_TYPE_STORE[type];
    }

    static inline const std::string OutGLSL(ShaderTypesGLSL type) noexcept {
        return "out " + SHADER_TYPE_STORE[type];
    }
}