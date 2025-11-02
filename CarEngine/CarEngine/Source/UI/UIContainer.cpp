#include "UIContainer.h"
#include <memory>

namespace CE
{
    UIContainer::UIContainer()
    {
    }

    void UIContainer::makeUiPanelsSimulated(World* world)
    {
        for (auto uiPanel : addedUiPanels)
        {
            uiPanel->start(world);
            simulatedUiPanels.push_back(std::move(uiPanel));
        }

        addedUiPanels.clear();
    }

    void UIContainer::deleteFlaggedUIPanels()
    {
        for (const auto& uiPanel : simulatedUiPanels)
        {
            if (uiPanel && uiPanel->bDelete)
                unusedIds.push_back(uiPanel->id);
        }

        simulatedUiPanels.erase(
                std::remove_if(
                        simulatedUiPanels.begin(),
                        simulatedUiPanels.end(),
                        [](const std::shared_ptr<UIPanel>& uiPanel) { return uiPanel->bDelete; }
                ),
                simulatedUiPanels.end()
        );
    }

    void UIContainer::deleteFlaggedUIComponents()
    {
        for (const auto& uiPanel : simulatedUiPanels)
        {
            if (!uiPanel)
                return;

            uiPanel->deleteFlaggedComponents();
        }
    }
}
