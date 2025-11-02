#include "Loader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <tiny_gltf/stb_image.h>
#include <glad/glad.h>

namespace CE::Loader
{
    #define GET_STRING(x) #x
    #define GET_DIR(x) GET_STRING(x)

    void checkIfGameAssetRootExists()
    {
        // For shipped games:
        if (std::filesystem::exists(GET_DIR(RELATIVE_GAME_ASSET_ROOT)))
            return;

        // For development:
        if (!std::filesystem::exists(GET_DIR(GAME_ASSET_ROOT)))
            throw std::runtime_error("Provided path 'GAME_ASSET_ROOT' " + std::string(GET_DIR(GAME_ASSET_ROOT)) + " could not be found");
    }

    void checkIfEngineAssetRootExists()
    {
        // For shipped games:
        if (std::filesystem::exists(GET_DIR(RELATIVE_ENGINE_ASSET_ROOT)))
            return;

        // For development:
        if (!std::filesystem::exists(GET_DIR(ENGINE_ASSET_ROOT)))
            throw std::runtime_error("Provided path 'ENGINE_ASSET_ROOT' " + std::string(GET_DIR(ENGINE_ASSET_ROOT)) + " could not be found");
    }

    std::filesystem::path resolve(const std::filesystem::path& relativePath)
    {
        // For shipped games:
        // check if file is in RELATIVE_GAME_ASSET_ROOT
        std::filesystem::path relativeGameAssetRoot = GET_DIR(RELATIVE_GAME_ASSET_ROOT);
        auto relativeGameAssetPath = relativeGameAssetRoot / relativePath;

        if (std::filesystem::exists(relativeGameAssetPath))
            return relativeGameAssetPath;

        // For shipped games:
        // check if file is in RELATIVE_ENGINE_ASSET_ROOT
        std::filesystem::path relativeEngineAssetRoot = GET_DIR(RELATIVE_ENGINE_ASSET_ROOT);
        auto relativeEngineAssetPath = relativeEngineAssetRoot / relativePath;

        if (std::filesystem::exists(relativeEngineAssetPath))
            return relativeEngineAssetPath;

        // For development:
        // check if file is in GAME_ASSET_ROOT
        std::filesystem::path gameAssetRoot = GET_DIR(GAME_ASSET_ROOT);
        auto gameAssetPath = gameAssetRoot / relativePath;

        if (std::filesystem::exists(gameAssetPath))
            return gameAssetPath;

        // For development:
        // check if file is in ENGINE_ASSET_ROOT
        std::filesystem::path engineAssetRoot = GET_DIR(ENGINE_ASSET_ROOT);
        auto engineAssetPath = engineAssetRoot / relativePath;

        if (std::filesystem::exists(engineAssetPath))
            return engineAssetPath;

        throw std::runtime_error("File with relative path of " + relativePath.string() + " could not be found"
                                 + "\n" + "Make sure to put your assets into 'GAME_ASSET_ROOT': " + GET_DIR(GAME_ASSET_ROOT));
    }

    std::string getFilePathExtension(const std::string& filename)
    {
        if (filename.find_last_of('.') != std::string::npos)
            return filename.substr(filename.find_last_of('.') + 1);

        return "";
    }

    std::string loadShader(const std::filesystem::path& filename)
    {
        std::ifstream sourceFile;
        sourceFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            sourceFile.open(filename);
            std::stringstream buffer;
            buffer << sourceFile.rdbuf();
            sourceFile.close();
            return buffer.str();
        }
        catch (std::ifstream::failure& e)
        {
            throw std::runtime_error("Shader with path of " + filename.string() + " could not be read successfully");
        }
    }

    bool loadModel(tinygltf::Model& model, const char* filename)
    {
        tinygltf::TinyGLTF loader;
        std::string error;
        std::string warning;

        bool result;
        std::string fileFormat = getFilePathExtension(filename);
        if (fileFormat == "glb")
        {
            result = loader.LoadBinaryFromFile(&model, &error, &warning, filename);
        }
        else
        {
            result = loader.LoadASCIIFromFile(&model, &error, &warning, filename);
        }

        if (!warning.empty())
            std::cout << "WARNING: " << warning << std::endl;

        if (!error.empty())
            std::cout << "ERROR: " << error << std::endl;

        if (!result)
            std::cout << "Failed to load glTF model: " << filename << std::endl;
        else
            std::cout << "Loaded glTF model: " << filename << std::endl;

        if (model.meshes.empty())
            std::cout << "Loaded glTF model is empty: " << filename << std::endl;

        return result;
    }

    bool loadTexture(const std::filesystem::path& textureAsset)
    {
        int textureWidth, textureHeight, numberColorChannels;

        auto textureAssetString = textureAsset.string();
        const char* textureAssetPtr = textureAssetString.c_str();

        unsigned char* textureData = stbi_load(textureAssetPtr, &textureWidth, &textureHeight, &numberColorChannels, 0);
        if (textureData)
        {
            if (numberColorChannels == 3)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

            stbi_image_free(textureData);

            return true;
        }
        else
        {
            std::cout << "Failed to load texture asset " +  textureAssetString << std::endl;
            stbi_image_free(textureData);

            return false;
        }
    }

    bool loadSkyboxTextures(std::vector<std::filesystem::path> textureAssets)
    {
        int textureWidth, textureHeight, numberColorChannels;
        bool bSuccess = true;

        for (unsigned int i = 0; i < textureAssets.size(); i++)
        {
            auto textureAssetString = textureAssets[i].string();
            const char* textureAssetPtr = textureAssetString.c_str();

            unsigned char* textureData = stbi_load(textureAssetPtr, &textureWidth, &textureHeight, &numberColorChannels, 0);
            if (textureData)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
                stbi_image_free(textureData);
            }
            else
            {
                std::cout << "Failed to load skybox texture asset " +  textureAssetString << std::endl;
                stbi_image_free(textureData);
                bSuccess = false;
            }
        }

        return bSuccess;
    }
}