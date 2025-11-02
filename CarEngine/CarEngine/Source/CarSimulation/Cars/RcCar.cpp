#include "RcCar.h"
#include "Utils/Math.h"
#include "Input/Car/CarInputHandler.h"
#include <chrono_vehicle/ChSubsysDefs.h>

using namespace chrono::vehicle;

namespace CE
{
    RcCar::RcCar(chrono::ChSystem* simulationSystem, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::unique_ptr<CarInputHandler> inputHandler, CollisionModel collisionModel,
        Tire tire)
        : Car(std::move(inputHandler))
    {
        if (!simulationSystem)
            return;

        chronoCar = rccar::RCCar(simulationSystem);
        setInitialChronoCarTransform(position, rotation);
        setCollisionModel(collisionModel);
        setTire(tire);
        initialize();

        passVehicle(&chronoCar.GetVehicle());
        passPowertrain(chronoCar.GetPowertrain());
    }

    void RcCar::setCollisionModel(CollisionModel collision)
    {
        switch (collision)
        {
            case CollisionModel::NONE:
                chronoCar.SetChassisCollisionType(CollisionType::NONE);
                break;
            case CollisionModel::PRIMITIVES:
                chronoCar.SetChassisCollisionType(CollisionType::PRIMITIVES);
                break;
            case CollisionModel::HULLS:
                chronoCar.SetChassisCollisionType(CollisionType::HULLS);
                break;
            case CollisionModel::MESH:
                chronoCar.SetChassisCollisionType(CollisionType::MESH);
                break;
        }
    }

    void RcCar::setTire(Tire tire)
    {
        switch (tire)
        {
            case Tire::RIGID:
                chronoCar.SetTireType(TireModelType::RIGID);
                break;
            case Tire::TMEASY:
                chronoCar.SetTireType(TireModelType::TMEASY);
                break;
            default:
                break;
        }
    }

    void RcCar::setInitialChronoCarTransform(glm::vec3 position, glm::vec3 rotation)
    {
        chronoCar.SetInitPosition(chrono::ChCoordsys<>(Math::openGlToChronoPosition(position),
                                                       Math::openGlToChronoRotation(rotation)));
    }

    void RcCar::initialize()
    {
        chronoCar.Initialize();
    }

    void RcCar::synchronizeSimulation(const CarInputs& inputs, Terrain* terrain, double simulationTime)
    {
        if (auto* chronoTerrainPtr = retrieveChronoTerrain(terrain))
            chronoCar.Synchronize(simulationTime, (chrono::vehicle::ChDriver::Inputs&&) inputs, *chronoTerrainPtr);
    }

    void RcCar::advanceSimulation(float vehicleFixedDeltaTime)
    {
        chronoCar.Advance(vehicleFixedDeltaTime);
    }
}