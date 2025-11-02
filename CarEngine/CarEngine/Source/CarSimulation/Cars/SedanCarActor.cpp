#include "SedanCarActor.h"
#include "Utils/Math.h"

namespace CE
{
    SedanCarActor::SedanCarActor(World* world, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::unique_ptr<CarInputHandler> inputHandler, CollisionModel collisionModel, Tire tire)
            : SedanCar(world->getCarSimulationHandler()->getSimulationSystem(), position, rotation, scale, std::move(inputHandler), collisionModel, tire), Actor(world, position, rotation, scale)
    {
        carSimulationHandler->setDebugDeltaTimeModifier(4.f);
    }

    void SedanCarActor::vehicleFixedUpdate(World* world)
    {
        carInputHandler->handleInput(world, this);

        // Update modules (process inputs from other modules)
        if (auto activeTerrain = carSimulationHandler->getActiveTerrain())
            synchronizeSimulation(inputs, activeTerrain, carSimulationHandler->getSimulationTime());

        // Advance simulation for one timestep for all modules
        advanceSimulation(carSimulationHandler->getCarSimulationStepSize());

        updateActorTransform();
    }

    void SedanCarActor::applyChronoCarPositionToActor()
    {
        const auto carPosition = chronoCar.GetVehicle().GetChassisBody()->GetFrame_REF_to_abs().GetPos();
        position = Math::chronoToOpenGlPosition(carPosition);
    }

    void SedanCarActor::applyChronoCarRotationToActor()
    {
        const auto carRotationQuaternion = chronoCar.GetVehicle().GetChassisBody()->GetFrame_REF_to_abs().GetRot();
        setRotationQ(Math::chronoToOpenGlRotationQ(carRotationQuaternion));
    }
}