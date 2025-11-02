#pragma once

#include "Entity.h"
#include "Rendering/Material/TextureMaterial.h"

namespace CE
{
    class EntityCollection : public Entity {

    // can create collisionEntities and terrainPatchEntities from a loaded mesh if mesh node names have specific prefixes

    public:
        explicit EntityCollection(Mesh mesh,
                                  const std::vector<ColorMaterial*>& materials,
                                  glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                                  glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                                  glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f),
                                  bool bUseTextureTiling = false,
                                  bool bIsActive = true,
                                  std::string name = "New EntityCollection");

        explicit EntityCollection(Mesh mesh,
                                  const std::vector<TextureMaterial*>& materials,
                                  glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                                  glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                                  glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f),
                                  bool bUseTextureTiling = false,
                                  bool bIsActive = true,
                                  std::string name = "New EntityCollection");

        virtual void createCollisionEntities(World* world, Actor* owner);
        virtual void createTerrainPatchEntities(World* world, Actor* owner);

        // scale difference adjustment, that is applied to collisionEntities and terrainPatchEntities to match the scale of any loaded mesh
        // when using Blender, the scale difference is 2
        // (meshes created in Blender are twice as large as meshes that are created in CarEngine when using the same scale values)
        float scaleAdjustment = 2.f;
    };
}