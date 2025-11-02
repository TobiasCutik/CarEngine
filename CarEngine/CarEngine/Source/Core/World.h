#pragma once
#include "ActorContainer.h"
#include "Rendering/Renderer.h"
#include "Input/Input.h"
#include "../CarSimulation/CarSimulationTransformManager.h"
#include "LevelManager.h"
#include "UI/UIContainer.h"
#include "Utils/AssetManager.h"
#include "UI/UIButtonHandler.h"
#include "Audio/AudioPlayer.h"
#include "Utils/SaveGameManager.h"

namespace CE
{
    class World {

    friend class Context;

    public:
        explicit World(std::shared_ptr<Input> input);

        std::shared_ptr<LevelManager> getLevelManager() const
        {
            return levelManager;
        }

        std::shared_ptr<Input> getInput() const
        {
            return worldInput;
        }

        std::shared_ptr<ActorContainer> getActorContainer() const
        {
            return actorContainer;
        }

        std::shared_ptr<UIContainer> getUIContainer() const
        {
            return uiContainer;
        }

        std::shared_ptr<CarSimulationHandler> getCarSimulationHandler() const
        {
            return carSimulationHandler;
        }

        AssetManager* getAssetManager() const
        {
            return assetManager.get();
        }

        std::shared_ptr<AudioPlayer> getAudioPlayer() const
        {
            return audioPlayer;
        }

        Shader::VertexAttributes& getShaderVertexAttributes() const { return renderer->getShaderVertexAttributes(); }
        glm::ivec2 getWindowSize() const { return renderer->getWindowSize(); }

    private:
        void start();

        void tick(int windowWidth, int windowHeight);

        void updateVehicleFixedDeltaTime();
        void manageFrameRate(bool bLogFrameRate = false, bool bLogAverageFrameRate = false, float averageFrameRateMeasurementTimeInSeconds = 30.f);
        void vehicleFixedUpdate();
        void update();
        void draw(int windowWidth, int windowHeight);
        void cleanUpActors();
        void cleanUpUI();
        void shutdown();

        float deltaTime = 0.f;
        float vehicleFixedDeltaTime = 0.f;
        float elapsedTimeInVehicleFixedUpdate = 0.f;

        float lastFrameTime = 0.f;
        float fpsSum = 0.f;
        int totalFramesRendered = 0;

        std::shared_ptr<LevelManager> levelManager;
        std::shared_ptr<Input> worldInput;
        std::shared_ptr<UIContainer> uiContainer;
        std::shared_ptr<ActorContainer> actorContainer;
        std::unique_ptr<Renderer> renderer;
        std::shared_ptr<CarSimulationHandler> carSimulationHandler;
        std::unique_ptr<CarSimulationTransformManager> collisionTransformManager;
        std::unique_ptr<UIButtonHandler> uiButtonHandler;
        std::unique_ptr<AssetManager> assetManager;
        std::shared_ptr<AudioPlayer> audioPlayer;
    };
}