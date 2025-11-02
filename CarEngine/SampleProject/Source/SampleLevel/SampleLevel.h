#pragma once
#include "Core/Level.h"

namespace CD
{
    class SampleLevel : public CE::Level {

    public:
        explicit SampleLevel();

    protected:
        void setupLevel(CE::World* world) override;
    };
}