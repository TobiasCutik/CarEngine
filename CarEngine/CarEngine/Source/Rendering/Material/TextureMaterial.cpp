#include "TextureMaterial.h"
#include "Utils/Loader.h"

namespace CE
{
    TextureMaterial::TextureMaterial(const std::filesystem::path& textureAsset, int textureWrappingMethod, glm::vec4 ambientModifier, glm::vec4 diffuseModifier, glm::vec4 specularModifier,
                                     float shininess, std::string name)
                                     : Material(std::move(name)), ambientModifier(ambientModifier), diffuseModifier(diffuseModifier), specularModifier(specularModifier), shininess(shininess)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        if (Loader::loadTexture(textureAsset))
            setupTexture(textureWrappingMethod);
    }

    void TextureMaterial::useTexture() const
    {
        if (texture)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
        }
    }

    void TextureMaterial::setupTexture(int textureWrappingMethod)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingMethod);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingMethod);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
}