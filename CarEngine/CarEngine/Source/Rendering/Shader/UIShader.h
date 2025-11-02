#pragma once

#include "Shader.h"

namespace CE
{
    class UIShader : public Shader {

    public:
        UIShader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name = "UIShader");

        void setMeshNodeSpecificUniforms(Transformable* transform, Material* material) override;
        void setGlobalEntityMatrix(glm::mat4 globalEntityMatrix) override;
        void setUiProjectionMatrix(glm::mat4 matrix) override;

    private:
        const std::string diffuseTexture = "diffuseTexture";

        const std::string mesh = "mesh";
        const std::string hasTexture = "bHasTexture";
        const std::string entityColor = "entityColor";

        const std::string model = "model";
        const std::string uiProjection = "uiProjection";
    };
}