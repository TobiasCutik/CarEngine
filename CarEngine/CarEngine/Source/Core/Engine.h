#pragma once
#include "AbstractGame.h"
#include "CarSimulation/ContactMethod.h"

namespace CE
{
    class Engine {

    public:
        Engine();

        void setGame(AbstractGame* game) { abstractGame = game; }
        void run(ContactMethod physicsContactMethod = ContactMethod::RIGID);

    private:
        AbstractGame* abstractGame = nullptr;
    };
}