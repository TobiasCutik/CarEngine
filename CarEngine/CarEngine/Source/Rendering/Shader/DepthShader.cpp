#include "DepthShader.h"
#include <utility>

namespace CE
{
    DepthShader::DepthShader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name)
        : Shader(vertexShaderAsset, fragmentShaderAsset, std::move(name))
    {
    }

    void DepthShader::setMeshNodeSpecificUniforms(Transformable* transform, Material* material)
    {
        use();
        setUniformMatrix(mesh, transform->getTransformationMatrix());
    }

    void DepthShader::setGlobalEntityMatrix(glm::mat4 globalEntityMatrix)
    {
        use();
        setUniformMatrix(model, globalEntityMatrix);
    }

    void DepthShader::setShaderLightData(ShaderLightData* shaderLightData)
    {
        if (shaderLightData)
        {
            use();
            setUniformMatrix(lightView, shaderLightData->lightViewMatrix);
            setUniformMatrix(lightProjection, shaderLightData->lightProjectionMatrix);
        }
    }
}