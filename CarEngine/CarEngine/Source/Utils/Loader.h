#pragma once
#include "tiny_gltf/tiny_gltf.h"
#include <filesystem>

namespace CE::Loader
{
    void checkIfGameAssetRootExists();
    void checkIfEngineAssetRootExists();

    std::filesystem::path resolve(const std::filesystem::path& relativePath);

    std::string getFilePathExtension(const std::string& filename);

    std::string loadShader(const std::filesystem::path& filename);

    bool loadModel(tinygltf::Model& model, const char* filename);

    bool loadTexture(const std::filesystem::path& textureAsset);

    bool loadSkyboxTextures(std::vector<std::filesystem::path> textureAssets);
}