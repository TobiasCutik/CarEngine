#include "Engine.h"

namespace CE
{
    Engine::Engine()
    {
        Loader::checkIfEngineAssetRootExists();
    }

    void Engine::run(ContactMethod physicsContactMethod)
    {
        auto world = abstractGame->context->getWorld();
        world->getCarSimulationHandler()->setContactMethod(physicsContactMethod);
        abstractGame->setupGame(world);
        abstractGame->run();
    }
}