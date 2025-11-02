#include "DirectionalLight.h"

namespace CD
{
    DirectionalLight::DirectionalLight(World* world, glm::vec3 position, PlayerCar* playerCar) : Light(world, glm::vec3 {0.f, 0.f, 0.f})
    {
        getLightEntity()->position = position;
        car = playerCar;
    }

    void DirectionalLight::update(World* world, float deltaTime)
    {
        if (!car)
            return;

        position = car->position;
        getLightEntity()->lookAtPosition = car->position;
    }
}