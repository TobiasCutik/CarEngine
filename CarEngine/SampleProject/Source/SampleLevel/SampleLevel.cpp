#include "SampleLevel.h"
#include "SampleLevelGeometry.h"
#include "PlayerCar.h"
#include "DirectionalLight.h"

namespace CD
{
    SampleLevel::SampleLevel() : Level(0, std::move("SampleLevel"))
    {
    }

    void SampleLevel::setupLevel(World* world)
    {
        Level::setupLevel(world);

        if (auto lightShared = light.lock())
            lightShared->getLightEntity()->setLightActive(false);

        if (auto cameraActorShared = cameraActor.lock())
            cameraActorShared->getComponentsRaw<Camera>().at(0)->setActive(false);

        auto entityCollection = world->getActorContainer()->addActor<SampleLevelGeometry>(world);

        auto playerCar = world->getActorContainer()->addActor<PlayerCar>(world);
        world->getActorContainer()->addActor<DirectionalLight>(world, glm::vec3 {19.f, 50.f, -15.f}, playerCar.lock().get());
    }
}