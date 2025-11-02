#pragma once

#include "../../CarSimulation/Terrain/TerrainPlane.h"
#include "../Entity.h"

namespace CE
{
    class TerrainPlaneEntity : public TerrainPlane, public Entity {

    public:
        explicit TerrainPlaneEntity(World* world,
                                    float yPosition = 0.f,
                                    float friction = 0.9f,
                                    Material* material = Materials::yellow(),
                                    bool bUseTextureTiling = false,
                                    bool bEntityIsActive = true,
                                    const std::string& name = "New TerrainPlaneEntity");

        const static ActorComponentType componentType = ActorComponentType::TERRAIN_PLANE_ENTITY;
    };
}