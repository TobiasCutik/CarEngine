#pragma once
#include "Core/Context.h"

namespace CE
{
    class AbstractGame {

    friend class Engine;

    public:
        explicit AbstractGame(int windowWidth, int windowHeight, const char*& windowTitle, int antiAliasingSamplePoints = 2, bool bUseFullscreen = true);
        virtual ~AbstractGame() = default;

    protected:
        virtual void setupGame(World* world) = 0;

    private:
        void run() { context->run(); }

        std::unique_ptr<Context> context;
    };
}