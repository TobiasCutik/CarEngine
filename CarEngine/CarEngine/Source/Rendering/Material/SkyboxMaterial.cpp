#include "SkyboxMaterial.h"
#include "Utils/Loader.h"
#include <iostream>
#include <utility>
#include <glad/glad.h>

namespace CE
{
    SkyboxMaterial::SkyboxMaterial(std::vector<std::filesystem::path> textureAssets, std::string name) : Material(std::move(name))
    {
        glGenTextures(1, &skyboxTexture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

        if (Loader::loadSkyboxTextures(std::move(textureAssets)))
            setupTextures();
    }

    void SkyboxMaterial::setupTextures()
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void SkyboxMaterial::useTexture() const
    {
        if (skyboxTexture)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        }
    }
}