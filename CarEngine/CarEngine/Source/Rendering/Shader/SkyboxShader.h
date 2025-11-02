#pragma once

#include "Shader.h"

namespace CE
{
    class SkyboxShader : public Shader {

    public:
        SkyboxShader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name = "SkyboxShader");

        void setMeshNodeSpecificUniforms(Transformable* transform, Material* material) override;
        void setGlobalEntityMatrix(glm::mat4 globalEntityMatrix) override;

        void setShaderCameraData(ShaderCameraData* shaderCameraData) override;

    private:
        const std::string skybox = "skybox";

        const std::string model = "model";
        const std::string view = "view";
        const std::string projection = "projection";
    };
}