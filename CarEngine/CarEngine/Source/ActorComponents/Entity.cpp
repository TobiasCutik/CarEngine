#include "Entity.h"

namespace CE
{
    Entity::Entity(Mesh mesh, const std::vector<ColorMaterial*>& materials, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool bUseTextureTiling, bool bIsActive, std::string name) :
            entityMesh(std::move(mesh)), bUseTextureTiling(bUseTextureTiling),
            Transformable(position, rotation, scale),
            ActorComponent(bIsActive, std::move(name))
    {
        entityMesh.setMaterials(materials);

        setComponentType(ActorComponentType::ENTITY);
    }

    Entity::Entity(Mesh mesh, const std::vector<TextureMaterial*>& materials, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool bUseTextureTiling, bool bIsActive, std::string name) :
            entityMesh(std::move(mesh)), bUseTextureTiling(bUseTextureTiling),
            Transformable(position, rotation, scale),
            ActorComponent(bIsActive, std::move(name))
    {
        entityMesh.setMaterials(materials);

        setComponentType(ActorComponentType::ENTITY);
    }

    Entity::Entity(Mesh mesh, Material* material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool bUseTextureTiling, bool bIsActive, std::string name) :
            entityMesh(std::move(mesh)), bUseTextureTiling(bUseTextureTiling),
            Transformable(position, rotation, scale),
            ActorComponent(bIsActive, std::move(name))
    {
        entityMesh.setMaterial(material);

        setComponentType(ActorComponentType::ENTITY);
    }
}