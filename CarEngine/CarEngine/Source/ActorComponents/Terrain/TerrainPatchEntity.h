#pragma once
#include "CarSimulation/Terrain/TerrainPatch.h"
#include "ActorComponents/Entity.h"
#include "CarSimulation/Terrain/TerrainMaterial/TerrainMaterial.h"
#include "CarSimulation/Terrain/TerrainMaterial/TerrainRigidMaterial.h"

namespace CE
{
    class TerrainPatchEntity : public TerrainPatch, public Entity {

    // the used chrono terrainPatch can be only rotated in 90-degree steps on its y-axis
    // it gets rotated to fit the original entity as close as possible

    friend class TerrainMesh;

    public:
        explicit TerrainPatchEntity(World* world,
                                    glm::mat4 actorTransformationMatrix,
                                    glm::vec3 position = {0.f, 0.f, 0.f},
                                    glm::vec3 scale = {75.f, 1.f, 75.f},
                                    glm::vec3 rotation = {0.f, 0.f, 0.f},
                                    Material* material = Materials::yellow(),
                                    bool bUseTextureTiling = false,
                                    std::unique_ptr<TerrainMaterial> terrainMaterial = std::make_unique<TerrainRigidMaterial>(),
                                    bool bEntityIsActive = true,
                                    const std::string& name = "New TerrainPatchEntity");

        std::unique_ptr<TerrainMaterial> material;

        const static ActorComponentType componentType = ActorComponentType::TERRAIN_PATCH_ENTITY;

    private:
        void initializeTerrainPatchId() { terrainPatchId = getId(); }

        std::shared_ptr<chrono::ChMaterialSurface> getChronoMaterialPtr() const override;
    };
}