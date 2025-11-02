#include "HmmwvCar.h"
#include "Utils/Math.h"
#include "Input/Car/CarInputHandler.h"
#include <chrono_vehicle/ChSubsysDefs.h>

using namespace chrono::vehicle;

namespace CE
{
    HmmwvCar::HmmwvCar(chrono::ChSystem* simulationSystem, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::unique_ptr<CarInputHandler> inputHandler,
        CollisionModel collisionModel, Tire tire, Driveline driveline, Powertrain powertrain)
        : Car(std::move(inputHandler))
    {
        if (!simulationSystem)
            return;

        chronoCar = hmmwv::HMMWV_Reduced(simulationSystem);
        setInitialChronoCarTransform(position, rotation);
        setCollisionModel(collisionModel);
        setTire(tire);
        setDriveline(driveline);
        setPowertrain(powertrain);
        initialize();

        passVehicle(&chronoCar.GetVehicle());
        passPowertrain(chronoCar.GetPowertrain());
    }

    void HmmwvCar::setCollisionModel(CollisionModel collision)
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

    void HmmwvCar::setTire(Tire tire)
    {
        switch (tire)
        {
            case Tire::RIGID:
                chronoCar.SetTireType(TireModelType::RIGID);
                break;
            case Tire::FIALA:
                chronoCar.SetTireType(TireModelType::FIALA);
                break;
            case Tire::PACEJKA:
                chronoCar.SetTireType(TireModelType::PACEJKA);
                break;
            case Tire::PAC02:
                chronoCar.SetTireType(TireModelType::PAC02);
                break;
            case Tire::PAC89:
                chronoCar.SetTireType(TireModelType::PAC89);
                break;
            case Tire::TMEASY:
                chronoCar.SetTireType(TireModelType::TMEASY);
                break;
            case Tire::LUGRE:
                chronoCar.SetTireType(TireModelType::LUGRE);
                break;
        }
    }

    void HmmwvCar::setPowertrain(Powertrain powertrain)
    {
        switch (powertrain)
        {
            case Powertrain::SHAFTS:
                chronoCar.SetPowertrainType(PowertrainModelType::SHAFTS);
                break;
            case Powertrain::SIMPLE_MAP:
                chronoCar.SetPowertrainType(PowertrainModelType::SIMPLE_MAP);
                break;
            case Powertrain::SIMPLE:
                chronoCar.SetPowertrainType(PowertrainModelType::SIMPLE);
                break;
            case Powertrain::SIMPLE_CVT:
                chronoCar.SetPowertrainType(PowertrainModelType::SIMPLE_CVT);
                break;
        }
    }

    void HmmwvCar::setDriveline(Driveline driveline)
    {
        switch (driveline)
        {
            case Driveline::FWD:
                chronoCar.SetDriveType(DrivelineTypeWV::FWD);
                break;
            case Driveline::RWD:
                chronoCar.SetDriveType(DrivelineTypeWV::RWD);
                break;
            case Driveline::AWD:
                chronoCar.SetDriveType(DrivelineTypeWV::AWD);
                break;
            case Driveline::SIMPLE:
                chronoCar.SetDriveType(DrivelineTypeWV::SIMPLE);
                break;
        }
    }

    void HmmwvCar::setInitialChronoCarTransform(glm::vec3 position, glm::vec3 rotation)
    {
        chronoCar.SetInitPosition(chrono::ChCoordsys<>(Math::openGlToChronoPosition(position),
                                                       Math::openGlToChronoRotation(rotation)));
    }

    void HmmwvCar::initialize()
    {
        chronoCar.Initialize();
    }

    void HmmwvCar::synchronizeSimulation(const CarInputs& inputs, Terrain* terrain, double simulationTime)
    {
        if (auto* chronoTerrainPtr = retrieveChronoTerrain(terrain))
            chronoCar.Synchronize(simulationTime, (chrono::vehicle::ChDriver::Inputs&&) inputs, *chronoTerrainPtr);
    }

    void HmmwvCar::advanceSimulation(float vehicleFixedDeltaTime)
    {
        chronoCar.Advance(vehicleFixedDeltaTime);
    }
}