#include "LightEntity.h"
#include "Core/World.h"
#include "Utils/Loader.h"

namespace CE
{
    LightEntity::LightEntity(World* world, glm::vec3 position, glm::vec4 color, float ambientLightStrength, float diffuseLightStrength, float specularLightStrength, float shadowStrength,
                             glm::vec3 lookAtPosition, bool bIsActive, bool bLightPositionVisible, const std::string& name)
        : Entity(Mesh(world, world->getAssetManager()->getModel("CE_Box")), Materials::red(), position,
                 {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f}, false, bLightPositionVisible, name),
          ambientLightStrength(ambientLightStrength), diffuseLightStrength(diffuseLightStrength), specularLightStrength(specularLightStrength), shadowStrength(shadowStrength),
          lookAtPosition(lookAtPosition), bLightIsActive(bIsActive), color(color)
    {
        setComponentType(ActorComponentType::LIGHT_ENTITY);
    }
}