#include "World.h"
#include <utility>
#include "Utils/GeneralTransformManager.h"

namespace CE
{
    World::World(std::shared_ptr<Input> input) : worldInput(std::move(input))
    {
        uiContainer = std::make_shared<UIContainer>();
        actorContainer = std::make_shared<ActorContainer>();
        levelManager = std::make_shared<LevelManager>(actorContainer, uiContainer);
        carSimulationHandler = std::make_shared<CarSimulationHandler>(actorContainer);
        collisionTransformManager = std::make_unique<CarSimulationTransformManager>();
        uiButtonHandler = std::make_unique<UIButtonHandler>(this->worldInput);

        renderer = std::make_unique<Renderer>(Loader::resolve("Shaders/CameraShader/CameraVertexShader.glsl"), Loader::resolve("Shaders/CameraShader/CameraFragmentShader.glsl"),
                                              Loader::resolve("Shaders/SkyboxShader/SkyboxVertexShader.glsl"), Loader::resolve("Shaders/SkyboxShader/SkyboxFragmentShader.glsl"),
                                              Loader::resolve("Shaders/DepthShader/DepthVertexShader.glsl"), Loader::resolve("Shaders/DepthShader/DepthFragmentShader.glsl"),
                                              Loader::resolve("Shaders/UIShader/UIVertexShader.glsl"), Loader::resolve("Shaders/UIShader/UIFragmentShader.glsl"),
                                              Loader::resolve("Shaders/TextShader/TextVertexShader.glsl"), Loader::resolve("Shaders/TextShader/TextFragmentShader.glsl"),
                                              Loader::resolve("Shaders/TilingShader/TilingVertexShader.glsl"), Loader::resolve("Shaders/TilingShader/TilingFragmentShader.glsl"),
                                              12288, 12288);

        assetManager = std::make_unique<AssetManager>();
        audioPlayer = std::make_shared<AudioPlayer>();
    }

    void World::start()
    {
        levelManager->setupStartingLevel(this);

        elapsedTimeInVehicleFixedUpdate = (float) glfwGetTime();
    }

    void World::tick(int windowWidth, int windowHeight)
    {
        updateVehicleFixedDeltaTime();
        manageFrameRate(false);

        // Chrono Vehicle Physics Update
        vehicleFixedUpdate();

        // Logic Update
        update();

        uiButtonHandler->calculateUiButtonGlobalTransforms(*uiContainer->getAllUIPanels(), renderer->getWindowSize());

        collisionTransformManager->carSimulationTransformUpdate(*actorContainer->getAllActors());

        // Rendering
        draw(windowWidth, windowHeight);

        cleanUpActors();
        cleanUpUI();

        levelManager->updateLevel(this);

        actorContainer->makeActorsSimulated(this);
        uiContainer->makeUiPanelsSimulated(this);
    }

    void World::updateVehicleFixedDeltaTime()
    {
        vehicleFixedDeltaTime = carSimulationHandler->getCarSimulationStepSize();

        #ifndef NDEBUG // if debug mode
            vehicleFixedDeltaTime *= carSimulationHandler->getDebugDeltaTimeModifier();
        #endif
    }

    void World::manageFrameRate(bool bLogFrameRate, bool bLogAverageFrameRate, float averageFrameRateMeasurementTimeInSeconds)
    {
        auto currentTime = (float) glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        if (bLogFrameRate)
            std::cout << "FPS: " << 1.f / deltaTime << std::endl;

        if (bLogAverageFrameRate && currentTime < averageFrameRateMeasurementTimeInSeconds)
        {
            fpsSum += (1.f / deltaTime);
            totalFramesRendered++;
        }
        else if (bLogAverageFrameRate)
        {
            float averageFrameRate = fpsSum / totalFramesRendered;
            std::cout << "AVERAGE FPS AFTER " << averageFrameRateMeasurementTimeInSeconds << " SECONDS: " << averageFrameRate << std::endl;
        }
    }

    void World::vehicleFixedUpdate()
    {
        bool bHasUpdatedOnce = false;

        while (elapsedTimeInVehicleFixedUpdate < (float) glfwGetTime() || !bHasUpdatedOnce)
        {
            elapsedTimeInVehicleFixedUpdate += vehicleFixedDeltaTime;

            carSimulationHandler->updateTerrains();

            for (auto& actor : *actorContainer->getAllActors())
            {
                actor->vehicleFixedUpdate(this);
            }

            if (auto simulationSystem = carSimulationHandler->getSimulationSystem())
                simulationSystem->DoStepDynamics(carSimulationHandler->getCarSimulationStepSize());

            carSimulationHandler->updateCollisionEntityTriggers();

            bHasUpdatedOnce = true;
        }
    }

    void World::update()
    {
        for (auto& actor : *actorContainer->getAllActors())
        {
            actor->update(this, deltaTime);
        }

        for (auto& uiPanel : *uiContainer->getAllUIPanels())
        {
            uiPanel->update(this, deltaTime);
        }
    }

    void World::draw(int windowWidth, int windowHeight)
    {
        // Clear the render buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer->updateWindowSize(windowWidth, windowHeight);

        renderer->updateUiProjectionMatrix();

        renderer->updateLight(*actorContainer->getAllActors());

        renderer->updateCamera(*actorContainer->getAllActors());

        renderer->passDataToShaders();

        renderer->renderDepthMap(*actorContainer->getAllActors());

        // Adjust actual viewport to window size
        glViewport(0, 0, windowWidth, windowHeight);

        // Clear the render buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer->draw(*actorContainer->getAllActors());

        renderer->drawSkybox(*actorContainer->getAllActors());

        renderer->drawUI(*uiContainer->getAllUIPanels());
    }

    void World::cleanUpActors()
    {
        actorContainer->deleteFlaggedComponents();
        actorContainer->deleteFlaggedActors();
        GeneralTransformManager::resetHasChangedFlagInAllTransforms(*actorContainer->getAllActors());
    }

    void World::cleanUpUI()
    {
        uiContainer->deleteFlaggedUIComponents();
        uiContainer->deleteFlaggedUIPanels();
    }

    void World::shutdown()
    {
        assetManager.reset();
    }
}