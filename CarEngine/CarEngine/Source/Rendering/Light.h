#pragma once
#include "Core/Actor.h"
#include "ActorComponents/LightEntity.h"

namespace CE
{
    class Light : public Actor {

    public:
        Light(World* world, glm::vec3 position);

        LightEntity* getLightEntity();
    };
}