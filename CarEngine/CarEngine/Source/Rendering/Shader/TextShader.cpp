#include "TextShader.h"

namespace CE
{
    TextShader::TextShader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name)
        : Shader(vertexShaderAsset, fragmentShaderAsset, std::move(name))
    {
        use();
        setUniformInteger(textBitmap, 0);
    }

    void TextShader::setGlobalEntityMatrix(glm::mat4 globalEntityMatrix)
    {
        use();
        setUniformMatrix(model, globalEntityMatrix);
    }

    void TextShader::setUiProjectionMatrix(glm::mat4 matrix)
    {
        use();
        setUniformMatrix(uiProjection, matrix);
    }

    void TextShader::setTextColor(glm::vec4 color)
    {
        use();
        setUniformVector4(textColor, color);
    }
}