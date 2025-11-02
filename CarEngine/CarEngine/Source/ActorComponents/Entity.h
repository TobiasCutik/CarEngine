#pragma once

#include "../Rendering/Shader/Shader.h"
#include "../Rendering/Mesh/Mesh.h"
#include "../Core/Transformable.h"
#include "../Utils/Materials.h"
#include "ActorComponent.h"
#include "Rendering/Material/TextureMaterial.h"

namespace CE
{
    class Entity : public Transformable, public ActorComponent {

    public:
        explicit Entity(Mesh mesh,
                        const std::vector<ColorMaterial*>& materials,
                        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                        glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                        glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f),
                        bool bUseTextureTiling = false,
                        bool bIsActive = true,
                        std::string name = "New Entity");

        explicit Entity(Mesh mesh,
                        const std::vector<TextureMaterial*>& materials,
                        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                        glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                        glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f),
                        bool bUseTextureTiling = false,
                        bool bIsActive = true,
                        std::string name = "New Entity");

        explicit Entity(Mesh mesh,
                        Material* material,
                        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                        glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                        glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f),
                        bool bUseTextureTiling = false,
                        bool bIsActive = true,
                        std::string name = "New Entity");

        Mesh* getMesh() { return &entityMesh; }

        bool bUseTextureTiling;

        const static ActorComponentType componentType = ActorComponentType::ENTITY;

    private:
        Mesh entityMesh;
    };
}