#pragma once
#include "Entity.h"
#include "../Rendering/Material/SkyboxMaterial.h"

namespace CE
{
    class SkyboxEntity : public Entity {

    public:
        explicit SkyboxEntity(World* world,
                              SkyboxMaterial* material,
                              bool bSkyboxIsActive = true,
                              const std::string& name = "New SkyboxEntity");

        bool isSkyboxActive() const { return bSkyboxIsActive; }
        void setSkyboxActive(bool bActive) { bSkyboxIsActive = bActive; }

        const static ActorComponentType componentType = ActorComponentType::SKYBOX_ENTITY;

    private:
        bool bSkyboxIsActive;
    };
}