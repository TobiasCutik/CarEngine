#pragma once
#include "Material.h"
#include <filesystem>

namespace CE
{
    class SkyboxMaterial : public Material {

    public:
        explicit SkyboxMaterial(std::vector<std::filesystem::path> textureAssets, std::string name = "New SkyboxMaterial");

        void useTexture() const;
        bool hasTexture() const override { return skyboxTexture; }

    private:
        static void setupTextures();

        unsigned int skyboxTexture = 0;
    };
}