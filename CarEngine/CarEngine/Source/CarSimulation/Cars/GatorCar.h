#pragma once
#include "Car.h"
#include <chrono_models/gator/Gator.h>
#include "Input/Car/AutomaticCarInput.h"

namespace CE
{
    class GatorCar : public Car {

    public:
        explicit GatorCar(chrono::ChSystem* simulationSystem,
                          glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                          glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                          glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f),
                          std::unique_ptr<CarInputHandler> inputHandler = std::make_unique<AutomaticCarInput>(),
                          CollisionModel collisionModel = CollisionModel::PRIMITIVES,
                          Tire tire = Tire::TMEASY);

        void setCollisionModel(CollisionModel collision);
        void setTire(Tire tire);

    protected:
        void setInitialChronoCarTransform(glm::vec3 position, glm::vec3 rotation);
        void initialize();
        void synchronizeSimulation(const CarInputs& inputs, Terrain* terrain, double simulationTime) override;
        void advanceSimulation(float vehicleFixedDeltaTime) override;

        chrono::vehicle::gator::Gator chronoCar;
    };
}
