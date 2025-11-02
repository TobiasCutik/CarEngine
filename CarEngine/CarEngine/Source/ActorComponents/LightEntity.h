#pragma once

#include "Entity.h"

namespace CE
{
    class LightEntity : public Entity {

    public:
        explicit LightEntity(World* world, glm::vec3 position = {0.f, 0.f, 0.f}, glm::vec4 color = {1.f, 1.f, 1.f, 1.f},
                             float ambientLightStrength = 0.75f, float diffuseLightStrength = 0.75f, float specularLightStrength = 0.5f, float shadowStrength = 0.75f, glm::vec3 lookAtPosition = {0.f, 0.f, 0.f}, bool bIsActive = true,
                             bool bLightPositionVisible = false, const std::string& name = "New LightEntity");

        bool isLightActive() const { return bLightIsActive; }
        void setLightActive(bool bActive) { bLightIsActive = bActive; }

        const static ActorComponentType componentType = ActorComponentType::LIGHT_ENTITY;

        glm::vec4 color;
        float ambientLightStrength;
        float diffuseLightStrength;
        float specularLightStrength;
        float shadowStrength;
        glm::vec3 lookAtPosition;

    private:
        bool bLightIsActive;
    };
}