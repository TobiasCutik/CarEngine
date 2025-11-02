#pragma once

#include "Shader.h"

namespace CE
{
    class DepthShader : public Shader {

    public:
        DepthShader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name = "DepthShader");

        void setMeshNodeSpecificUniforms(Transformable* transform, Material* material) override;
        void setGlobalEntityMatrix(glm::mat4 globalEntityMatrix) override;

        void setShaderLightData(ShaderLightData* shaderLightData) override;

    private:
        const std::string mesh = "mesh";
        const std::string model = "model";

        const std::string lightView = "lightView";
        const std::string lightProjection = "lightProjection";
    };
}