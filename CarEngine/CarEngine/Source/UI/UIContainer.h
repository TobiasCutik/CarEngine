#pragma once
#include <memory>
#include "UIPanel.h"

namespace CE
{
    class UIContainer {

    friend class World;
    friend class LevelManager;

    public:
        UIContainer();

        template <class T, typename... Args>
        std::weak_ptr<T> addUIPanel(Args&&... args)
        {
            const auto uiPanel = std::make_shared<T>(std::forward<Args>(args)...);

            if (unusedIds.empty())
            {
                uiPanel->id = generateNewId();
            }
            else
            {
                uiPanel->id = unusedIds.at(0);
                unusedIds.erase(unusedIds.begin());
            }

            addedUiPanels.emplace_back(uiPanel);

            return uiPanel;
        }

        template <class T>
        std::weak_ptr<T> getUIPanelById(int id)
        {
            for (const auto& uiPanel : simulatedUiPanels)
            {
                if (uiPanel && uiPanel->id == id)
                    return std::dynamic_pointer_cast<T>(uiPanel);
            }

            return {};
        }

        template <class T>
        std::weak_ptr<T> getUIPanelByName(const std::string& name)
        {
            for (const auto& uiPanel : simulatedUiPanels)
            {
                if (uiPanel && uiPanel->getName() == name)
                    return std::dynamic_pointer_cast<T>(uiPanel);
            }

            return {};
        }

    private:
        void deleteFlaggedUIPanels();
        void deleteFlaggedUIComponents();

        void makeUiPanelsSimulated(World* world);

        int generateNewId() { return nextId++; }
        std::vector<std::shared_ptr<UIPanel>>* getAllUIPanels() { return &simulatedUiPanels; }

        int nextId = 0;
        std::vector<int> unusedIds;

        std::vector<std::shared_ptr<UIPanel>> addedUiPanels;
        std::vector<std::shared_ptr<UIPanel>> simulatedUiPanels;
    };
}