#pragma once
#include "Input/Car/CarInputHandler.h"

namespace CE
{
    class AutomaticCarInput : public CarInputHandler {

    public:
        AutomaticCarInput() = default;

    protected:
        void handleThrottleInput(World* world, Car* car) override;
        void handleBrakeInput(World* world, Car* car) override;
        void handleReverseInput(World* world, Car* car) override;
    };
}