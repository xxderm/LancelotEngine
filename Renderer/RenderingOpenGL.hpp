#pragma once
#include "BuffersOpenGL.hpp"
#include "ShaderOpenGL.hpp"
#include "../Core/EntityComponentSystem.hpp"

namespace LL::Renderer {
    struct Object3DGL final : public Core::Component {
        ShaderProgramGL Shader;
        // ... Render needs
        // TODO: LoadFromFile using Assimp
    };

    class RenderingSystemGL final : public Core::System {
    public:

    };

}