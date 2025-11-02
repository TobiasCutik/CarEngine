#include "CameraShader.h"
#include "../Material/ColorMaterial.h"
#include "../Material/TextureMaterial.h"
#include <utility>

namespace CE
{
    CameraShader::CameraShader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name)
        : Shader(vertexShaderAsset, fragmentShaderAsset, std::move(name))
    {
        use();
        setUniformInteger(diffuseTexture, 0);
        setUniformInteger(depthMap, 1);
    }

    void CameraShader::setMeshNodeSpecificUniforms(Transformable* transform, Material* material)
    {
        use();
        setUniformMatrix(mesh, transform->getTransformationMatrix());
        setUniformMatrix(modelMeshToWorldNormal, glm::transpose(glm::inverse(modelMatrix * transform->getTransformationMatrix())));

        if (!material)
            return;

        bool bHasTexture = material->hasTexture();
        setUniformInteger(hasTexture, bHasTexture);

        if (auto* textureMaterial = dynamic_cast<TextureMaterial*>(material))
        {
            setUniformVector4(materialAmbientModifier, textureMaterial->ambientModifier);
            setUniformVector4(materialDiffuseModifier, textureMaterial->diffuseModifier);
            setUniformVector4(materialSpecularModifier, textureMaterial->specularModifier);
            setUniformFloat(materialShininess, textureMaterial->shininess);

            if (bHasTexture)
                textureMaterial->useTexture();
        }
        else if (auto* colorMaterial = dynamic_cast<ColorMaterial*>(material))
        {
            setUniformVector4(materialAmbientModifier, colorMaterial->ambientModifier);
            setUniformVector4(materialDiffuseModifier, colorMaterial->diffuseModifier);
            setUniformVector4(materialSpecularModifier, colorMaterial->specularModifier);
            setUniformFloat(materialShininess, colorMaterial->shininess);

            setUniformVector4(entityColor, colorMaterial->color);
        }
    }

    void CameraShader::setGlobalEntityMatrix(glm::mat4 globalEntityMatrix)
    {
        use();
        setUniformMatrix(model, globalEntityMatrix);
        modelMatrix = globalEntityMatrix;
    }

    void CameraShader::setShaderCameraData(ShaderCameraData* shaderCameraData)
    {
        if (shaderCameraData)
        {
            use();
            setUniformMatrix(view, shaderCameraData->viewMatrix);
            setUniformMatrix(projection, shaderCameraData->projectionMatrix);
            setUniformVector4(cameraPosition, shaderCameraData->cameraPosition);
        }
    }

    void CameraShader::setShaderLightData(ShaderLightData* shaderLightData)
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