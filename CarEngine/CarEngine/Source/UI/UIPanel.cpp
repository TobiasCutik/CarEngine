#include "UIPanel.h"

namespace CE
{
    UIPanel::UIPanel(World* world, glm::vec2 anchorPositionInPercent, float zRotation, glm::vec2 scale, std::string name)
        : Transformable(glm::vec3(0.f), glm::vec3 {0.f, 0.f, zRotation}, glm::vec3 {scale.x, scale.y, 1.f}), name(std::move(name)),
          anchorPosition(glm::vec2 {anchorPositionInPercent.x, -anchorPositionInPercent.y})
    {
    }

    void UIPanel::start(World* world)
    {
        // called once when this UIPanel is registered
    }

    void UIPanel::update(World* world, float deltaTime)
    {
        // update logic
    }

    void UIPanel::destroy(UIPanel* uiPanelRawPtr)
    {
        destroy(uiPanelRawPtr->getComponentsWeak<UIComponent>());
        uiPanelRawPtr->bDelete = true;
    }

    void UIPanel::destroy(const std::weak_ptr<UIPanel>& uiPanelWeakPtr)
    {
        if (auto uiPanelSharedPtr = uiPanelWeakPtr.lock())
        {
            destroy(uiPanelSharedPtr->getComponentsWeak<UIComponent>());
            uiPanelSharedPtr->bDelete = true;
        }
    }

    void UIPanel::destroy(const std::vector<std::weak_ptr<UIPanel>>& uiPanelWeakPtrList)
    {
        for (const auto& uiPanelWeakPtr : uiPanelWeakPtrList)
        {
            if (auto uiPanelSharedPtr = uiPanelWeakPtr.lock())
            {
                destroy(uiPanelSharedPtr->getComponentsWeak<UIComponent>());
                uiPanelSharedPtr->bDelete = true;
            }
        }
    }

    void UIPanel::destroy(UIComponent* componentRawPtr)
    {
        componentRawPtr->bDelete = true;
    }

    void UIPanel::destroy(const std::weak_ptr<UIComponent>& componentWeakPtr)
    {
        if (auto componentSharedPtr = componentWeakPtr.lock())
            componentSharedPtr->bDelete = true;
    }

    void UIPanel::destroy(const std::vector<std::weak_ptr<UIComponent>>& componentWeakPtrList)
    {
        for (const auto& componentWeakPtr : componentWeakPtrList)
        {
            if (auto componentSharedPtr = componentWeakPtr.lock())
                componentSharedPtr->bDelete = true;
        }
    }

    UIPanel::~UIPanel()
    {
        deleteFlaggedComponents();
    }

    void UIPanel::deleteFlaggedComponents()
    {
        for (const auto& component : componentList)
        {
            if (component && component->bDelete)
                unusedComponentIds.push_back(component->id);
        }

        componentList.erase(
                std::remove_if(
                        componentList.begin(),
                        componentList.end(),
                        [](const std::shared_ptr<UIComponent>& component) { return component->bDelete; }
                ),
                componentList.end()
        );
    }
}