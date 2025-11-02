#pragma once
#include "Core/World.h"

namespace CE
{
    class Car;

    class CarInputHandler {

    public:
        CarInputHandler() = default;
        virtual ~CarInputHandler() = default;

        virtual void handleInput(World* world, Car* car);

    protected:
        virtual void handleSteeringInput(World* world, Car* car);
        virtual void handleHandbrakeInput(World* world, Car* car);

        virtual void handleThrottleInput(World* world, Car* car) {}
        virtual void handleBrakeInput(World* world, Car* car) {}
        virtual void handleReverseInput(World* world, Car* car) {}
    };
}