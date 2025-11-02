#pragma once
#include "AutomaticCarInput.h"

namespace CE
{
    class AutomaticRcCarInput : public AutomaticCarInput {

    public:
        AutomaticRcCarInput() = default;

    protected:
        // slowed down reversing acceleration in comparison to AutomaticCarInput; this avoids that the RcCar spins around heavily
        void handleReverseInput(World* world, Car* car) override;
    };
}