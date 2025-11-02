#include "Light.h"

namespace CE
{
    Light::Light(World* world, glm::vec3 position) : Actor(world, position)
    {
        Actor::addComponent<LightEntity>(world);
    }

    LightEntity* Light::getLightEntity()
    {
        auto lightEntities = Actor::getComponentsRaw<LightEntity>();

        if (!lightEntities.empty())
            return lightEntities.at(0);

        return nullptr;
    }
}