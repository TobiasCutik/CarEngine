#pragma once
#include <chrono_vehicle/ChPowertrain.h>
#include <chrono_vehicle/wheeled_vehicle/ChWheeledVehicle.h>
#include "CarSimulation/Terrain/Terrain.h"
#include "CarEnums.h"
#include "Input/Car/CarInputHandler.h"

namespace CE
{
    struct CarInputs
    {
        double steer = 0;
        double throttle = 0;
        double brake = 0;
    };

    class Car {

        friend class CarSimulationHandler;

    public:
        explicit Car(std::unique_ptr<CarInputHandler> inputHandler);
        virtual ~Car() = default;

        void setDriveMode(DriveMode driveMode);
        DriveMode getDriveMode();
        void activateHandbrake(bool bSetActive);

        float getSpeed();
        void setAccelerationMultiplier(float multiplier);
        void setAccelerationAddend(float addend);

        CarInputs inputs = {0, 0, 0};

        Event<CarSimulationHandler, CollisionEntity*> onCollision;
        Event<CarSimulationHandler, Car*> onCollisionWithCar;

    protected:
        virtual void synchronizeSimulation(const CarInputs& inputs, Terrain* terrain, double simulationTime) = 0;
        virtual void advanceSimulation(float vehicleFixedDeltaTime) = 0;

        void passVehicle(chrono::vehicle::ChWheeledVehicle* vehicle);
        void passPowertrain(std::shared_ptr<chrono::vehicle::ChPowertrain> powertrain);

        static chrono::vehicle::ChTerrain* retrieveChronoTerrain(Terrain* terrain);

        // update this actor transform with the chrono car transform
        void updateActorTransform();

        std::unique_ptr<CarInputHandler> carInputHandler;

    private:
        virtual void applyChronoCarPositionToActor() = 0;
        virtual void applyChronoCarRotationToActor() = 0;

        chrono::vehicle::ChWheeledVehicle* chronoVehicle = nullptr;
        std::shared_ptr<chrono::vehicle::ChPowertrain> chronoPowertrain;
    };
}