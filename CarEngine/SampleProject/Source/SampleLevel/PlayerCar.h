#pragma once

#include "CarSimulation/Cars/SedanCarActor.h"

using namespace CE;

namespace CD
{
    class PlayerCar : public SedanCarActor {

    public:
        explicit PlayerCar(World* world,
                           glm::vec3 position = {0.f, 0.8f, 0.f},
                           glm::vec3 rotation = {0.f, 0.f, 0.f});
    };
}