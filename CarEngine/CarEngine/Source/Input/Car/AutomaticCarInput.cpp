#include "AutomaticCarInput.h"
#include "CarSimulation/Cars/Car.h"
#include "Input/ControllerCodes.h"
#include "Input/KeyCodes.h"

namespace CE
{
    void AutomaticCarInput::handleThrottleInput(World* world, Car* car)
    {
        auto axis = Input::getControllerTriggerAxis(CE_CONTROLLER_AXIS_RIGHT_TRIGGER);

        if (world->getInput()->isKeyPressed(CE_KEY_W))
        {
            car->inputs.throttle = 1.f;
            car->setDriveMode(DriveMode::FORWARD);
        }
        else if (axis > 0.f)
        {
            car->inputs.throttle = axis;
            car->setDriveMode(DriveMode::FORWARD);
        }
        else if (car->getDriveMode() == DriveMode::FORWARD)
        {
            car->inputs.throttle = 0.f;
        }
    }

    void AutomaticCarInput::handleBrakeInput(World* world, Car* car)
    {
        auto axis = Input::getControllerTriggerAxis(CE_CONTROLLER_AXIS_LEFT_TRIGGER);

        if (world->getInput()->isKeyPressed(CE_KEY_S))
        {
            if (car->getDriveMode() != DriveMode::REVERSE)
            {
                car->inputs.brake = 1.f;
            }
        }
        else if (axis > 0.f)
        {
            if (car->getDriveMode() != DriveMode::REVERSE)
            {
                car->inputs.brake = axis;
            }
        }
        else if (car->getDriveMode() != DriveMode::REVERSE)
        {
            car->inputs.brake = 0.f;
        }
    }

    void AutomaticCarInput::handleReverseInput(World* world, Car* car)
    {
        bool bCarIsStandingStill;

        auto axis = Input::getControllerTriggerAxis(CE_CONTROLLER_AXIS_LEFT_TRIGGER);

        if (std::abs(car->getSpeed()) < 0.4f)
        {
            bCarIsStandingStill = true;
        }
        else
        {
            bCarIsStandingStill = false;
        }
        if (world->getInput()->isKeyPressed(CE_KEY_S))
        {
            if (bCarIsStandingStill)
            {
                car->setDriveMode(DriveMode::REVERSE);
            }

            if (car->getDriveMode() == DriveMode::REVERSE)
            {
                car->inputs.throttle = 1.f;
                car->inputs.brake = 0.f;
            }
        }
        else if (axis > 0.f)
        {
            if (bCarIsStandingStill)
            {
                car->setDriveMode(DriveMode::REVERSE);
            }

            if (car->getDriveMode() == DriveMode::REVERSE)
            {
                car->inputs.throttle = axis;
                car->inputs.brake = 0.f;
            }
        }
        else if (car->getDriveMode() == DriveMode::REVERSE)
        {
            car->inputs.throttle = 0.f;
            car->inputs.brake = 0.02f;
        }
    }
}
