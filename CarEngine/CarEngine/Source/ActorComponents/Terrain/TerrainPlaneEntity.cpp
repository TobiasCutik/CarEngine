#include "TerrainPlaneEntity.h"
#include "Core/World.h"
#include "Utils/Loader.h"

namespace CE
{
    TerrainPlaneEntity::TerrainPlaneEntity(World* world, float yPosition, float friction, Material* material, bool bUseTextureTiling, bool bEntityIsActive, const std::string& name) :
        TerrainPlane(yPosition, friction),
        Entity(Mesh(world, world->getAssetManager()->getModel("CE_Plane")),
                 material,
                 {0.f, yPosition, 0.f},
                 {0.f, 0.f, 0.f},
                 {75.f, 1.f, 75.f},
                 bUseTextureTiling,
                 bEntityIsActive,
                 name)
    {
        setComponentType(ActorComponentType::TERRAIN_PLANE_ENTITY);
    }
}