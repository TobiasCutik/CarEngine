#include "CarInputHandler.h"
#include "CarSimulation/Cars/Car.h"
#include "Input/ControllerCodes.h"
#include "Input/KeyCodes.h"

namespace CE
{
    void CarInputHandler::handleInput(World* world, Car* car)
    {
        handleSteeringInput(world, car);
        handleHandbrakeInput(world, car);
        handleThrottleInput(world, car);
        handleBrakeInput(world, car);
        handleReverseInput(world, car);
    }

    void CarInputHandler::handleSteeringInput(World* world, Car* car)
    {
        auto axis = Input::getControllerAxis(CE_CONTROLLER_AXIS_LEFT_X);

        if (world->getInput()->isKeyPressed(CE_KEY_D))
        {
            car->inputs.steer = -1.f;
        }
        else if (world->getInput()->isKeyPressed(CE_KEY_A))
        {
            car->inputs.steer = 1.f;
        }
        else if (abs(axis) > 0.f)
        {
            car->inputs.steer = -axis;
        }
        else
        {
            car->inputs.steer = 0.f;
        }
    }

    void CarInputHandler::handleHandbrakeInput(World* world, Car* car)
    {
        if (world->getInput()->isKeyPressed(CE_KEY_SPACE) || Input::isControllerButtonPressed(CE_CONTROLLER_BUTTON_B))
        {
            car->activateHandbrake(true);
        }
        else
        {
            car->activateHandbrake(false);
        }
    }
}
