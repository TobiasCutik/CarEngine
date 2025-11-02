#pragma once
#include "Shader.h"

namespace CE
{
    class TextShader : public Shader {

    public:
        TextShader(const std::filesystem::path& vertexShaderAsset, const std::filesystem::path& fragmentShaderAsset, std::string name = "TextShader");

        void setGlobalEntityMatrix(glm::mat4 globalEntityMatrix) override;
        void setUiProjectionMatrix(glm::mat4 matrix) override;
        void setTextColor(glm::vec4 color);

        void setMeshNodeSpecificUniforms(Transformable* transform, Material* material) override { }

    private:
        const std::string textBitmap = "textBitmap";
        const std::string model = "model";
        const std::string uiProjection = "uiProjection";
        const std::string textColor = "textColor";
    };
}