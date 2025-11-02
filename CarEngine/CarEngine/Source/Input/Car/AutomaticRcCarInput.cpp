#include "AutomaticRcCarInput.h"
#include "CarSimulation/Cars/Car.h"
#include "Input/ControllerCodes.h"
#include "Input/KeyCodes.h"

namespace CE
{
    void AutomaticRcCarInput::handleReverseInput(World* world, Car* car)
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
                car->inputs.throttle = 0.25f;
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
                car->inputs.throttle = axis * 0.25f;
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
