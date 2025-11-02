#include "Car.h"
#include <utility>

using namespace chrono::vehicle;

namespace CE
{
    Car::Car(std::unique_ptr<CarInputHandler> inputHandler) : carInputHandler(std::move(inputHandler))
    {
    }

    void Car::setDriveMode(DriveMode driveMode)
    {
        if (!chronoPowertrain)
            return;

        switch (driveMode)
        {
            case DriveMode::FORWARD:
                chronoPowertrain->SetDriveMode(ChPowertrain::DriveMode::FORWARD);
                break;
            case DriveMode::NEUTRAL:
                chronoPowertrain->SetDriveMode(ChPowertrain::DriveMode::NEUTRAL);
                break;
            case DriveMode::REVERSE:
                chronoPowertrain->SetDriveMode(ChPowertrain::DriveMode::REVERSE);
                break;
        }
    }

    DriveMode Car::getDriveMode()
    {
        if (!chronoPowertrain)
            return DriveMode::NEUTRAL;

        ChPowertrain::DriveMode driveMode = chronoPowertrain->GetDriveMode();

        switch (driveMode)
        {
            case ChPowertrain::DriveMode::FORWARD:
                return DriveMode::FORWARD;
            case ChPowertrain::DriveMode::NEUTRAL:
                return DriveMode::NEUTRAL;
            case ChPowertrain::DriveMode::REVERSE:
                return DriveMode::REVERSE;
            default:
                return DriveMode::NEUTRAL;
        }
    }

    void Car::activateHandbrake(bool bSetActive)
    {
        if (chronoVehicle)
            chronoVehicle->ApplyParkingBrake(bSetActive);
    }

    float Car::getSpeed()
    {
        if (chronoVehicle)
            return static_cast<float>(chronoVehicle->GetVehicleSpeed());
        else
            return 0.f;
    }

    void Car::setAccelerationMultiplier(float multiplier)
    {
        if (chronoVehicle)
            chronoVehicle->SetAccelerationMultiplier(multiplier);
    }

    void Car::setAccelerationAddend(float addend)
    {
        if (chronoVehicle)
            chronoVehicle->SetAccelerationAddend(addend);
    }

    void Car::passVehicle(chrono::vehicle::ChWheeledVehicle* vehicle)
    {
        chronoVehicle = vehicle;
    }

    void Car::passPowertrain(std::shared_ptr<chrono::vehicle::ChPowertrain> powertrain)
    {
        chronoPowertrain = std::move(powertrain);
    }

    chrono::vehicle::ChTerrain* Car::retrieveChronoTerrain(Terrain* terrain)
    {
        if (!terrain)
            return nullptr;
        else
            return terrain->getChronoTerrain();
    }

    void Car::updateActorTransform()
    {
        applyChronoCarPositionToActor();
        applyChronoCarRotationToActor();
    }
}