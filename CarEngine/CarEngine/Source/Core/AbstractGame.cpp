#include "AbstractGame.h"

namespace CE
{
    AbstractGame::AbstractGame(int windowWidth, int windowHeight, const char*& windowTitle, int antiAliasingSamplePoints, bool bUseFullscreen)
    {
        Loader::checkIfGameAssetRootExists();

        context = std::make_unique<Context>(windowWidth, windowHeight, windowTitle, antiAliasingSamplePoints, bUseFullscreen);
    }
}