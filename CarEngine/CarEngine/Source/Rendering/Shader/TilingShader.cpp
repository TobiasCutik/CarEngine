#include "TilingShader.h"
#include "../Material/TextureMaterial.h"
#include <utility>

namespace CE
{
    TilingShader::TilingShader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name)
            : Shader(vertexShaderAsset, fragmentShaderAsset, std::move(name))
    {
        use();
        setUniformInteger(diffuseTexture, 0);
        setUniformInteger(depthMap, 1);
    }

    void TilingShader::setMeshNodeSpecificUniforms(Transformable* transform, Material* material)
    {
        use();
        setUniformMatrix(mesh, transform->getTransformationMatrix());
        setUniformVector3(scale, transform->scale);

        if (!material)
            return;

        if (auto* textureMaterial = dynamic_cast<TextureMaterial*>(material))
        {
            setUniformVector4(materialAmbientModifier, textureMaterial->ambientModifier);
            setUniformVector4(materialDiffuseModifier, textureMaterial->diffuseModifier);
            setUniformVector4(materialSpecularModifier, textureMaterial->specularModifier);
            setUniformFloat(materialShininess, textureMaterial->shininess);
            textureMaterial->useTexture();
        }
    }

    void TilingShader::setGlobalEntityMatrix(glm::mat4 globalEntityMatrix)
    {
        use();
        setUniformMatrix(model, globalEntityMatrix);
        setUniformMatrix(modelToWorldNormal, glm::transpose(glm::inverse(globalEntityMatrix)));
    }

    void TilingShader::setShaderCameraData(ShaderCameraData* shaderCameraData)
    {
        if (shaderCameraData)
        {
            use();
            setUniformMatrix(view, shaderCameraData->viewMatrix);
            setUniformMatrix(projection, shaderCameraData->projectionMatrix);
            setUniformVector4(cameraPosition, shaderCameraData->cameraPosition);
        }
    }

    void TilingShader::setShaderLightData(ShaderLightData* shaderLightData)
    {
        if (shaderLightData)
        {
            use();
            setUniformMatrix(lightView, shaderLightData->lightViewMatrix);
            setUniformMatrix(lightProjection, shaderLightData->lightProjectionMatrix);
            setUniformVector4(lightPosition, shaderLightData->lightPosition);
            setUniformVector4(lightColor, shaderLightData->lightColor);
            setUniformFloat(ambientLightStrength, shaderLightData->ambientLightStrength);
            setUniformFloat(diffuseLightStrength, shaderLightData->diffuseLightStrength);
            setUniformFloat(specularLightStrength, shaderLightData->specularLightStrength);
            setUniformFloat(shadowStrength, shaderLightData->shadowStrength);
        }
    }
}