#pragma once

#include <chrono/physics/ChBodyEasy.h>
#include <glm/vec3.hpp>
#include "CollisionEntity.h"

namespace CE
{
    class CollisionBox : public CollisionEntity {

    public:
        CollisionBox(World* world, glm::vec3 position, glm::vec3 scale = {10.f, 10.f, 10.f}, glm::vec3 rotation = {0.f, 0.f, 0.f}, bool bIsFixed = true,
                     bool bIsTrigger = false, bool bActive = true, Material* material = Materials::blue(), bool bUseTextureTiling = false, const std::string& name = "New CollisionBox");
    };
}