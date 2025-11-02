#include "UIComponent.h"

namespace CE
{
    UIComponent::UIComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool bIsActive, std::string name) : Component(bIsActive, std::move(name)), Transformable(position, rotation, scale)
    {
        setComponentType(UIComponentType::UI_COMPONENT);
    }

    bool UIComponent::isComponentOfType(UIComponentType type)
    {
        for (const auto& componentType : componentTypeList)
        {
            if (componentType == type)
                return true;
        }

        return false;
    }
}