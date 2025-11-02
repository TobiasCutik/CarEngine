#pragma once
#include "Core/Actor.h"
#include "Rendering/Material/SkyboxMaterial.h"

namespace CE
{
    class Skybox : public Actor {

    public:
        explicit Skybox(World* world, SkyboxMaterial* material);

        void setMaterial(SkyboxMaterial* material);
    };
}