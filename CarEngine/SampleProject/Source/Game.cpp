#include "Game.h"
#include "SampleLevel/SampleLevel.h"

using namespace CE;

namespace CD
{
    Game::Game(int width, int height, const char*& title, int antiAliasingSamplePoints, bool bUseFullscreen) : AbstractGame(width, height, title, antiAliasingSamplePoints, bUseFullscreen)
    {
    }

    void Game::setupGame(World* world)
    {
        auto assetManager = world->getAssetManager();
        assetManager->createMaterial<TextureMaterial>("Car_ScratchedAluminiumMatt", Loader::resolve("RallyCar/ScratchedAluminium.jpg"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f});
        assetManager->createMaterial<TextureMaterial>("Car_ScratchedAluminium", Loader::resolve("RallyCar/ScratchedAluminium.jpg"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.35f, 0.35f, 0.35f, 0.35f}, glm::vec4 {0.35f, 0.35f, 0.35f, 0.35f}, glm::vec4 {0.2f, 0.2f, 0.2f, 0.2f}, 1.9f);
        assetManager->createMaterial<TextureMaterial>("Car_Headlight", Loader::resolve("RallyCar/Headlight.jpg"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f});
        assetManager->createMaterial<TextureMaterial>("Car_Lights", Loader::resolve("RallyCar/Lights.png"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f});
        assetManager->createMaterial<TextureMaterial>("Car_Grill", Loader::resolve("RallyCar/Grills.png"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f});
        assetManager->createMaterial<TextureMaterial>("Car_Wheel", Loader::resolve("RallyCar/Tire.png"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f});

        assetManager->createMaterial<TextureMaterial>("Car_Black", Loader::resolve("RallyCar/Black.jpg"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f});
        assetManager->createMaterial<TextureMaterial>("Car_DarkGrey", Loader::resolve("RallyCar/DarkGrey.jpg"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f});
        assetManager->createMaterial<TextureMaterial>("Car_VeryDarkGrey", Loader::resolve("RallyCar/VeryDarkGrey.jpg"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f}, glm::vec4 {0.5f, 0.5f, 0.5f, 0.5f});

        assetManager->createMaterial<TextureMaterial>("Car_WhitePaint", Loader::resolve("RallyCar/WhitePaint.jpg"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.35f, 0.35f, 0.35f, 0.35f}, glm::vec4 {0.35f, 0.35f, 0.35f, 0.35f}, glm::vec4 {0.2f, 0.2f, 0.2f, 0.2f}, 1.9f);
        assetManager->createMaterial<TextureMaterial>("Car_BlackPaint", Loader::resolve("RallyCar/BlackPaint.jpg"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.35f, 0.35f, 0.35f, 0.35f}, glm::vec4 {0.35f, 0.35f, 0.35f, 0.35f}, glm::vec4 {0.2f, 0.2f, 0.2f, 0.2f}, 1.9f);
        assetManager->createMaterial<TextureMaterial>("Car_RedPaint", Loader::resolve("RallyCar/RedPaint.jpg"), GL_MIRRORED_REPEAT,
                                                      glm::vec4 {0.35f, 0.35f, 0.35f, 0.35f}, glm::vec4 {0.35f, 0.35f, 0.35f, 0.35f}, glm::vec4 {0.2f, 0.2f, 0.2f, 0.2f}, 1.9f);

        assetManager->createModel("Car", Loader::resolve("RallyCar/RallyCar.gltf"));

        assetManager->createModel("GroundTerrain", Loader::resolve("Ground/GroundTerrain.gltf"));

        world->getLevelManager()->addLevel<SampleLevel>();
    }
}