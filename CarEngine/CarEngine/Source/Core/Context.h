#pragma once
#include <memory>
#include "World.h"

namespace CE
{
    class Context {

    public:
        Context(int windowWidth, int windowHeight, const char*& windowTitle, int antiAliasingSamplePoints, bool bUseFullscreen);
        ~Context();

        void run();

        World* getWorld() const
        {
            return world.get();
        }

    private:
        GLFWwindow* window;
        std::unique_ptr<World> world;
        std::shared_ptr<Input> input;
    };
}