#pragma once
#include "SedanCar.h"
#include "Core/Actor.h"

namespace CE
{
    class SedanCarActor : public SedanCar, public Actor {

    public:
        explicit SedanCarActor(World* world,
                               glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                               glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                               glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f),
                               std::unique_ptr<CarInputHandler> inputHandler = std::make_unique<AutomaticCarInput>(),
                               CollisionModel collisionModel = CollisionModel::PRIMITIVES,
                               Tire tire = Tire::TMEASY);

        void vehicleFixedUpdate(World* world) override;

    private:
        void applyChronoCarPositionToActor() override;
        void applyChronoCarRotationToActor() override;
    };
}