#pragma once
#include "Core/Component.h"

namespace CE
{
    enum class ActorComponentType
    {
        ACTOR_COMPONENT,
        CAMERA,
        ENTITY,
        LIGHT_ENTITY,
        SKYBOX_ENTITY,
        COLLISION_ENTITY,
        TERRAIN_PLANE_ENTITY,
        TERRAIN_PATCH_ENTITY
    };

    class ActorComponent : public Component {

    friend class Actor;

    protected:
        explicit ActorComponent(bool bIsActive, std::string name);

        void setComponentType(ActorComponentType type) { componentTypeList.emplace_back(type); }

    public:
        bool isComponentOfType(ActorComponentType type);

        const static ActorComponentType componentType = ActorComponentType::ACTOR_COMPONENT;

    private:
        std::vector<ActorComponentType> componentTypeList;
    };
}