#include "SkyboxShader.h"
#include "../Material/TextureMaterial.h"
#include <utility>

namespace CE
{
    SkyboxShader::SkyboxShader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name)
        : Shader(vertexShaderAsset, fragmentShaderAsset, std::move(name))
    {
        use();
        setUniformInteger(skybox, 0);
    }

    void SkyboxShader::setMeshNodeSpecificUniforms(Transformable* transform, Material* material)
    {
        if (material && material->hasTexture())
        {
            if (auto* textureMaterial = dynamic_cast<TextureMaterial*>(material))
            {
                textureMaterial->useTexture();
            }
        }
    }

    void SkyboxShader::setGlobalEntityMatrix(glm::mat4 globalEntityMatrix)
    {
        use();
        setUniformMatrix(model, globalEntityMatrix);
    }

    void SkyboxShader::setShaderCameraData(ShaderCameraData* shaderCameraData)
    {
        if (shaderCameraData)
        {
            use();
            setUniformMatrix(view, glm::mat4(glm::mat3(shaderCameraData->viewMatrix)));
            setUniformMatrix(projection, shaderCameraData->projectionMatrix);
        }
    }
}