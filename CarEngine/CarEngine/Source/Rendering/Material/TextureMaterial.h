#pragma once
#include "Material.h"
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <filesystem>

namespace CE
{
    class TextureMaterial : public Material {

    public:
        explicit TextureMaterial(const std::filesystem::path& textureAsset,
                                 int textureWrappingMethod = GL_MIRRORED_REPEAT,
                                 glm::vec4 ambientModifier = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
                                 glm::vec4 diffuseModifier = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
                                 glm::vec4 specularModifier = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
                                 float shininess = 1.f,
                                 std::string name = "New TextureMaterial");

        void useTexture() const;
        bool hasTexture() const override { return texture; }

        glm::vec4 ambientModifier{};
        glm::vec4 diffuseModifier{};
        glm::vec4 specularModifier{};
        float shininess = 1.f;

    private:
        static void setupTexture(int textureWrappingMethod);

        unsigned int texture = 0;
    };
}