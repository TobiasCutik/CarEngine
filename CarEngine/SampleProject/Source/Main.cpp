#include "Game.h"
#include "Core/Engine.h"

const int W_WIDTH = 1920;
const int W_HEIGHT = 1080;
const char* W_TITLE = "SampleProject";

int main()
{
    CE::Engine engine;
    CD::Game game(W_WIDTH, W_HEIGHT, W_TITLE, 8, true);
    engine.setGame(&game);
    engine.run(CE::ContactMethod::RIGID);

    return 0;
}