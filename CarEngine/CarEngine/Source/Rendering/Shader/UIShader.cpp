#include "UIShader.h"
#include "../Material/ColorMaterial.h"
#include "../Material/TextureMaterial.h"
#include <utility>

namespace CE
{
    UIShader::UIShader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name)
            : Shader(vertexShaderAsset, fragmentShaderAsset, std::move(name))
    {
        use();
        setUniformInteger(diffuseTexture, 0);
    }

    void UIShader::setMeshNodeSpecificUniforms(Transformable* transform, Material* material)
    {
        use();
        setUniformMatrix(mesh, transform->getTransformationMatrix());

        if (!material)
            return;

        bool bHasTexture = material->hasTexture();
        setUniformInteger(hasTexture, bHasTexture);

        if (auto* textureMaterial = dynamic_cast<TextureMaterial*>(material))
        {
            if (bHasTexture)
                textureMaterial->useTexture();
        }
        else if (auto* colorMaterial = dynamic_cast<ColorMaterial*>(material))
        {
            setUniformVector4(entityColor, colorMaterial->color);
        }
    }

    void UIShader::setGlobalEntityMatrix(glm::mat4 globalEntityMatrix)
    {
        use();
        setUniformMatrix(model, globalEntityMatrix);
    }

    void UIShader::setUiProjectionMatrix(glm::mat4 matrix)
    {
        use();
        setUniformMatrix(uiProjection, matrix);
    }
}