#include "ActorComponent.h"

namespace CE
{
    ActorComponent::ActorComponent(bool bIsActive, std::string name) : Component(bIsActive, std::move(name))
    {
        setComponentType(ActorComponentType::ACTOR_COMPONENT);
    }

    bool ActorComponent::isComponentOfType(ActorComponentType type)
    {
        for (const auto& componentType : componentTypeList)
        {
            if (componentType == type)
                return true;
        }

        return false;
    }
}