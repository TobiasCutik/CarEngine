#include "AssetManager.h"
#include <iostream>
#include "Rendering/Material/SkyboxMaterial.h"

namespace CE
{
    AssetManager::AssetManager()
    {
        createDefaultAssets();
    }

    void AssetManager::createModel(const std::string& name, const std::filesystem::path& assetPath)
    {
        tinygltf::Model model;

        Loader::loadModel(model, assetPath.string().c_str());

        modelList.insert({name, model});
    }

    tinygltf::Model AssetManager::getModel(const std::string& name) const
    {
        if (modelList.find(name) != modelList.end())
        {
            return modelList.at(name);
        }
        else
        {
            std::cout << "Could not find model \"" << name << "\"" << std::endl;
            return {};
        }
    }

    AudioClip* AssetManager::getAudioClip(const std::string& name) const
    {
        if (audioClipList.find(name) != audioClipList.end())
        {
            return audioClipList.at(name).get();
        }
        else
        {
            std::cout << "Could not find audioClip \"" << name << "\"" << std::endl;
            return nullptr;
        }
    }

    void AssetManager::createDefaultAssets()
    {
        createModel("CE_Box", Loader::resolve("Assets/Models/Box.gltf"));
        createModel("CE_Plane", Loader::resolve("Assets/Models/Plane.gltf"));

        createMaterial<SkyboxMaterial>("CE_DefaultSkybox", std::vector {Loader::resolve("Assets/Textures/Skybox/SkyboxPosX.jpg"), Loader::resolve("Assets/Textures/Skybox/SkyboxNegX.jpg"),
                                                                               Loader::resolve("Assets/Textures/Skybox/SkyboxPosY.jpg"), Loader::resolve("Assets/Textures/Skybox/SkyboxNegY.jpg"),
                                                                               Loader::resolve("Assets/Textures/Skybox/SkyboxPosZ.jpg"), Loader::resolve("Assets/Textures/Skybox/SkyboxNegZ.jpg")});

        createMaterial<TextureMaterial>("CE_UIButtonS_Default", Loader::resolve("Assets/Textures/UI/UIButtonS_Default.png"));
        createMaterial<TextureMaterial>("CE_UIButtonS_Hovered", Loader::resolve("Assets/Textures/UI/UIButtonS_Hovered.png"));
        createMaterial<TextureMaterial>("CE_UIButtonS_Pressed", Loader::resolve("Assets/Textures/UI/UIButtonS_Pressed.png"));

        createMaterial<TextureMaterial>("CE_UIButtonS_White_Default", Loader::resolve("Assets/Textures/UI/UIButtonS_White_Default.png"));
        createMaterial<TextureMaterial>("CE_UIButtonS_White_Hovered", Loader::resolve("Assets/Textures/UI/UIButtonS_White_Hovered.png"));
        createMaterial<TextureMaterial>("CE_UIButtonS_White_Pressed", Loader::resolve("Assets/Textures/UI/UIButtonS_White_Pressed.png"));

        createMaterial<TextureMaterial>("CE_UIButtonM_Default", Loader::resolve("Assets/Textures/UI/UIButtonM_Default.png"));
        createMaterial<TextureMaterial>("CE_UIButtonM_Hovered", Loader::resolve("Assets/Textures/UI/UIButtonM_Hovered.png"));
        createMaterial<TextureMaterial>("CE_UIButtonM_Pressed", Loader::resolve("Assets/Textures/UI/UIButtonM_Pressed.png"));

        createMaterial<TextureMaterial>("CE_UIButtonM_White_Default", Loader::resolve("Assets/Textures/UI/UIButtonM_White_Default.png"));
        createMaterial<TextureMaterial>("CE_UIButtonM_White_Hovered", Loader::resolve("Assets/Textures/UI/UIButtonM_White_Hovered.png"));
        createMaterial<TextureMaterial>("CE_UIButtonM_White_Pressed", Loader::resolve("Assets/Textures/UI/UIButtonM_White_Pressed.png"));

        createMaterial<TextureMaterial>("CE_UIButtonL_Default", Loader::resolve("Assets/Textures/UI/UIButtonL_Default.png"));
        createMaterial<TextureMaterial>("CE_UIButtonL_Hovered", Loader::resolve("Assets/Textures/UI/UIButtonL_Hovered.png"));
        createMaterial<TextureMaterial>("CE_UIButtonL_Pressed", Loader::resolve("Assets/Textures/UI/UIButtonL_Pressed.png"));

        createMaterial<TextureMaterial>("CE_UIButtonL_White_Default", Loader::resolve("Assets/Textures/UI/UIButtonL_White_Default.png"));
        createMaterial<TextureMaterial>("CE_UIButtonL_White_Hovered", Loader::resolve("Assets/Textures/UI/UIButtonL_White_Hovered.png"));
        createMaterial<TextureMaterial>("CE_UIButtonL_White_Pressed", Loader::resolve("Assets/Textures/UI/UIButtonL_White_Pressed.png"));
    }
}