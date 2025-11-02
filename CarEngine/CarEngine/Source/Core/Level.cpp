#include "Level.h"
#include "Actor.h"
#include <utility>
#include "Core/World.h"
#include "Rendering/Skybox.h"

namespace CE
{
    Level::Level(int index, std::string name) : index(index), name(std::move(name))
    {
    }

    void Level::setupLevel(World* world)
    {
        skybox = world->getActorContainer()->addActor<Skybox>(world, world->getAssetManager()->getMaterial<SkyboxMaterial>("CE_DefaultSkybox"));
        light = world->getActorContainer()->addActor<Light>(world, glm::vec3 {0.f, 25.f, 0.f});

        cameraActor = world->getActorContainer()->addActor<Actor>(world);
        cameraActor.lock()->addComponent<Camera>(glm::vec3(0.f, 5.f, 0.f), glm::vec3(0.f, 0.f, 0.f), true);
    }
}