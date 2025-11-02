#pragma once
#include "Core/AbstractGame.h"

namespace CD
{
    class Game : public CE::AbstractGame {

    public:
        Game(int width, int height, const char*& title, int antiAliasingSamplePoints = 8, bool bUseFullscreen = true);

        void setupGame(CE::World* world) override;
    };
}