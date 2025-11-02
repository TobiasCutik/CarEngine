#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include "Rendering/Material/Material.h"
#include "Rendering/Mesh/Mesh.h"
#include "Audio/AudioClip.h"

namespace CE
{
    class AssetManager {

    public:
        AssetManager();

        template <class T, typename... Args>
        void createMaterial(const std::string& name, Args&&... args)
        {
            auto material = materialList.insert({name, std::make_unique<T>(std::forward<Args>(args)...)});

            // update the material's name if the insertion succeeded
            if (material.second)
                material.first->second.get()->name = name;
        }

        template <class T>
        T* getMaterial(const std::string& name) const
        {
            if (materialList.find(name) != materialList.end())
            {
                return dynamic_cast<T*>(materialList.at(name).get());
            }
            else
            {
                std::cout << "Could not find material \"" << name << "\"" << std::endl;
                return nullptr;
            }
        }

        void createModel(const std::string& name, const std::filesystem::path& assetPath);
        tinygltf::Model getModel(const std::string& name) const;

        template <typename... Args>
        void createAudioClip(const std::string& name, Args&&... args)
        {
            auto audioClip = audioClipList.insert({name, std::make_unique<AudioClip>(std::forward<Args>(args)...)});

            // update the audioClip's name if the insertion succeeded
            if (audioClip.second)
                audioClip.first->second.get()->name = name;
        }

        AudioClip* getAudioClip(const std::string& name) const;

    private:
        void createDefaultAssets();

        std::map<std::string, std::unique_ptr<Material>> materialList;
        std::map<std::string, tinygltf::Model> modelList;
        std::map<std::string, std::unique_ptr<AudioClip>> audioClipList;
    };
}