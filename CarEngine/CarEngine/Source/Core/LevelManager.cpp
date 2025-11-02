#include "LevelManager.h"
#include <utility>
#include "World.h"

namespace CE
{
    LevelManager::LevelManager(std::weak_ptr<ActorContainer> actorContainer, std::weak_ptr<UIContainer> uiContainer) : actorContainer(std::move(actorContainer)), uiContainer(std::move(uiContainer))
    {
    }

    bool LevelManager::loadLevel(const std::string& name)
    {
        for (const auto& level : levelList)
        {
            if (!level)
                continue;

            if (level->name == name)
            {
                level->bLoad = true;
                unloadCurrentLevel();
                return true;
            }
        }

        return false;
    }

    bool LevelManager::loadLevel(int index)
    {
        for (const auto& level : levelList)
        {
            if (!level)
                continue;

            if (level->index == index)
            {
                level->bLoad = true;
                unloadCurrentLevel();
                return true;
            }
        }

        return false;
    }

    bool LevelManager::loadNextLevel()
    {
        int nextLevelIndex = getIndexOfCurrentLevel() + 1;
        return loadLevel(nextLevelIndex);
    }

    bool LevelManager::loadPreviousLevel()
    {
        int previousLevelIndex = getIndexOfCurrentLevel() - 1;
        return loadLevel(previousLevelIndex);
    }

    std::string LevelManager::getNameOfCurrentLevel()
    {
        for (const auto& level : levelList)
        {
            if (!level)
                continue;

            if (level->bIsActive)
                return level->name;
        }

        return "";
    }

    int LevelManager::getIndexOfCurrentLevel()
    {
        for (const auto& level : levelList)
        {
            if (!level)
                continue;

            if (level->bIsActive)
                return level->index;
        }

        return -std::numeric_limits<int>::max();
    }

    void LevelManager::setupStartingLevel(World* world)
    {
        if (loadLevelInternal(startingLevelIndex, world))
            return;

        // fallback 1: no level with the startingLevelIndex exists
        // load the level with the lowest index
        if (loadLevelInternal(getLowestLevelIndex(), world))
            return;

        // fallback 2: no levels exist at all
        // load default level
        addLevel<Level>(0);
        loadLevelInternal(0, world);
    }

    void LevelManager::updateLevel(World* world)
    {
        for (const auto& level : levelList)
        {
            if (!level)
                continue;

            if (level->bLoad)
                loadLevelInternal(level->index, world);
        }
    }

    bool LevelManager::loadLevelInternal(int index, World* world)
    {
        for (const auto& level : levelList)
        {
            if (!level)
                continue;

            if (level->index == index)
            {
                world->getCarSimulationHandler()->removeAllChronoCollisions();
                resetIsActiveForAllLevels();
                level->bIsActive = true;
                level->setupLevel(world);
                level->bLoad = false;
                return true;
            }
        }

        return false;
    }

    void LevelManager::unloadCurrentLevel()
    {
        unloadCurrentActors();
        unloadCurrentUiPanels();
    }

    void LevelManager::unloadCurrentActors()
    {
        auto actorContainerShared = actorContainer.lock();
        if (!actorContainerShared)
            return;

        auto actors = actorContainerShared->getAllActors();
        for (auto& actor : *actors)
        {
            if (!actor)
                continue;

            if (!actor->isKeptOnLevelChange())
                Actor::destroy(actor);
        }
    }

    void LevelManager::unloadCurrentUiPanels()
    {
        auto uiContainerShared = uiContainer.lock();
        if (!uiContainerShared)
            return;

        auto uiPanels = uiContainerShared->getAllUIPanels();
        for (auto& uiPanel : *uiPanels)
        {
            if (!uiPanel)
                continue;

            if (!uiPanel->isKeptOnLevelChange())
                UIPanel::destroy(uiPanel);
        }
    }

    void LevelManager::resetIsActiveForAllLevels()
    {
        for (const auto& level : levelList)
        {
            if (!level)
                continue;

            level->bIsActive = false;
        }
    }

    int LevelManager::getLowestLevelIndex()
    {
        int lowestLevelIndex = std::numeric_limits<int>::max();

        for (const auto& level : levelList)
        {
            if (!level)
                continue;

            if (level->index < lowestLevelIndex)
                lowestLevelIndex = level->index;
        }

        return lowestLevelIndex;
    }
}