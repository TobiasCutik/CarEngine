#include "SkyboxEntity.h"
#include "Core/World.h"
#include "Utils/Loader.h"

namespace CE
{
    SkyboxEntity::SkyboxEntity(World* world, SkyboxMaterial* material, bool bSkyboxIsActive, const std::string& name)
        : Entity(Mesh(world, world->getAssetManager()->getModel("CE_Box")), material, {0.f, 0.f, 0.f},
                 {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, false, false, name),
          bSkyboxIsActive(bSkyboxIsActive)
    {
        setComponentType(ActorComponentType::SKYBOX_ENTITY);
    }
}