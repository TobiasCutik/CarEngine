#include "Skybox.h"
#include "ActorComponents/SkyboxEntity.h"

namespace CE
{
    Skybox::Skybox(World* world, SkyboxMaterial* material) : Actor(world)
    {
        Actor::addComponent<SkyboxEntity>(world, material);
    }

    void Skybox::setMaterial(SkyboxMaterial* material)
    {
        auto skyboxEntities = Actor::getComponentsRaw<SkyboxEntity>();

        if (!skyboxEntities.empty())
            skyboxEntities.at(0)->getMesh()->setMaterial(material);
    }
}