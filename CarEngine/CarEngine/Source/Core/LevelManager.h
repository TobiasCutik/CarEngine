#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Level.h"
#include "UI/UIContainer.h"

namespace CE
{
    class LevelManager {

    friend class World;

    public:
        explicit LevelManager(std::weak_ptr<ActorContainer> actorContainer, std::weak_ptr<UIContainer> uiContainer);

        template <class T, typename... Args>
        void addLevel(Args&&... args)
        {
            auto& level = levelList.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        void setStartingLevelIndex(int index) { startingLevelIndex = index; }

        bool loadLevel(const std::string& name);
        bool loadLevel(int index);
        bool loadNextLevel();
        bool loadPreviousLevel();

        std::string getNameOfCurrentLevel();
        int getIndexOfCurrentLevel();

    private:
        void setupStartingLevel(World* world);
        void updateLevel(World* world);
        bool loadLevelInternal(int index, World* world);
        void unloadCurrentLevel();
        void unloadCurrentActors();
        void unloadCurrentUiPanels();
        void resetIsActiveForAllLevels();
        int getLowestLevelIndex();

        int startingLevelIndex = 0;
        std::vector<std::unique_ptr<Level>> levelList;

        std::weak_ptr<ActorContainer> actorContainer;
        std::weak_ptr<UIContainer> uiContainer;
    };
}