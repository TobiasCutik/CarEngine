#pragma once
#include "Rendering/Light.h"
#include "PlayerCar.h"

namespace CD
{
    class DirectionalLight : public Light {

    public:
        DirectionalLight(World* world, glm::vec3 position, PlayerCar* playerCar);

        void update(World* world, float deltaTime) override;

    private:
        PlayerCar* car;
    };
}