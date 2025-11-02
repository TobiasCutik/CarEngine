#pragma once
#include "Core/Actor.h"
#include "RcCar.h"

namespace CE
{
    class RcCarActor : public RcCar, public Actor {

    public:
        explicit RcCarActor(World* world,
                            glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                            glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                            glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f),
                            std::unique_ptr<CarInputHandler> inputHandler = std::make_unique<AutomaticRcCarInput>(),
                            CollisionModel collisionModel = CollisionModel::PRIMITIVES,
                            Tire tire = Tire::TMEASY);

        void vehicleFixedUpdate(World* world) override;

    private:
        void applyChronoCarPositionToActor() override;
        void applyChronoCarRotationToActor() override;
    };
}