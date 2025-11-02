#include "ColorMaterial.h"

namespace CE
{
    ColorMaterial::ColorMaterial(glm::vec4 color, glm::vec4 ambientModifier, glm::vec4 diffuseModifier, glm::vec4 specularModifier, float shininess, std::string name)
        : Material(std::move(name)), color(color), ambientModifier(ambientModifier), diffuseModifier(diffuseModifier), specularModifier(specularModifier), shininess(shininess)
    {
    }
}