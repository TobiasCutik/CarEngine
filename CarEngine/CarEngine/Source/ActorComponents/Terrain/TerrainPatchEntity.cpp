#include "TerrainPatchEntity.h"
#include "Core/World.h"

namespace CE
{
    TerrainPatchEntity::TerrainPatchEntity(World* world, glm::mat4 actorTransformationMatrix, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, Material* material,
        bool bUseTextureTiling, std::unique_ptr<TerrainMaterial> terrainMaterial, bool bEntityIsActive, const std::string& name)
        : Entity(Mesh(world, world->getAssetManager()->getModel("CE_Box")), material, position, rotation,
                 {scale.x, scale.y, scale.z}, bUseTextureTiling, bEntityIsActive, name),
        material(std::move(terrainMaterial))
    {
        setComponentType(ActorComponentType::TERRAIN_PATCH_ENTITY);

        bTransformFixed = true;

        calculateGlobalTransform(actorTransformationMatrix, getTransformationMatrix());
    }

    std::shared_ptr<chrono::ChMaterialSurface> TerrainPatchEntity::getChronoMaterialPtr() const
    {
        if (material)
            return material->getChronoMaterial();
        else
            return nullptr;
    }
}