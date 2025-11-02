#pragma once
#include "Car.h"
#include <chrono_models/hmmwv/HMMWV.h>
#include "Input/Car/AutomaticCarInput.h"

namespace CE
{
    class HmmwvCar : public Car {

    public:
        explicit HmmwvCar(chrono::ChSystem* simulationSystem,
                          glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                          glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                          glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f),
                          std::unique_ptr<CarInputHandler> inputHandler = std::make_unique<AutomaticCarInput>(),
                          CollisionModel collisionModel = CollisionModel::PRIMITIVES,
                          Tire tire = Tire::TMEASY,
                          Driveline driveline = Driveline::AWD,
                          Powertrain powertrain = Powertrain::SIMPLE);

        void setCollisionModel(CollisionModel collision);
        void setTire(Tire tire);
        void setPowertrain(Powertrain powertrain);
        void setDriveline(Driveline driveline);

    protected:
        void setInitialChronoCarTransform(glm::vec3 position, glm::vec3 rotation);
        void initialize();
        void synchronizeSimulation(const CarInputs& inputs, Terrain* terrain, double simulationTime) override;
        void advanceSimulation(float vehicleFixedDeltaTime) override;

        chrono::vehicle::hmmwv::HMMWV_Reduced chronoCar;
    };
}
