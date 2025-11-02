#pragma once
#include "Shader.h"

namespace CE
{
    class TilingShader : public Shader {

    public:
        TilingShader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name = "TilingShader");

        void setMeshNodeSpecificUniforms(Transformable* transform, Material* material) override;
        void setGlobalEntityMatrix(glm::mat4 globalEntityMatrix) override;

        void setShaderCameraData(ShaderCameraData* shaderCameraData) override;
        void setShaderLightData(ShaderLightData* shaderLightData) override;

    private:
        const std::string diffuseTexture = "diffuseTexture";
        const std::string depthMap = "depthMap";

        const std::string mesh = "mesh";
        const std::string scale = "scale";
        const std::string materialAmbientModifier = "material.ambientModifier";
        const std::string materialDiffuseModifier = "material.diffuseModifier";
        const std::string materialSpecularModifier = "material.specularModifier";
        const std::string materialShininess = "material.shininess";

        const std::string model = "model";
        const std::string modelToWorldNormal = "modelToWorldNormal";
        const std::string view = "view";
        const std::string projection = "projection";
        const std::string cameraPosition = "cameraPosition";

        const std::string lightView = "lightView";
        const std::string lightProjection = "lightProjection";
        const std::string lightPosition = "lightPosition";
        const std::string lightColor = "lightColor";
        const std::string ambientLightStrength = "ambientLightStrength";
        const std::string diffuseLightStrength = "diffuseLightStrength";
        const std::string specularLightStrength = "specularLightStrength";
        const std::string shadowStrength = "shadowStrength";
    };
}