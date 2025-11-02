#pragma once
#include "Material.h"
#include <glm/vec4.hpp>

namespace CE
{
    class ColorMaterial : public Material {

    public:
        explicit ColorMaterial(glm::vec4 color,
                               glm::vec4 ambientModifier = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
                               glm::vec4 diffuseModifier = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
                               glm::vec4 specularModifier = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
                               float shininess = 1.f,
                               std::string name = "New ColorMaterial");

        bool hasTexture() const override { return false; }

        glm::vec4 color{};

        glm::vec4 ambientModifier{};
        glm::vec4 diffuseModifier{};
        glm::vec4 specularModifier{};
        float shininess = 1.f;
    };
}