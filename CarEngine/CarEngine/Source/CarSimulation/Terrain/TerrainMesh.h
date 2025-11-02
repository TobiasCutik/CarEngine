#pragma once
#include "Terrain.h"
#include "TerrainPatch.h"
#include "ActorComponents/Terrain/TerrainPatchEntity.h"
#include <chrono_vehicle/terrain/RigidTerrain.h>

namespace CE
{
    class TerrainMesh : public Terrain {

    public:
        explicit TerrainMesh(chrono::ChSystem* carSystem);

        void addTerrainPatch(const std::shared_ptr<TerrainPatchEntity>& terrainPatchEntity);
        void removeTerrainPatch(const std::shared_ptr<TerrainPatchEntity>& terrainPatchEntity);

        void initialize();

        std::vector<TerrainPatchEntity*> getTerrainPatchEntities();
        std::vector<int> getTerrainPatchesCollidingWithCars() { return chronoTerrain.GetPatchesCollidingWithTires(); }

    private:
        chrono::vehicle::ChTerrain* getChronoTerrain() override { return &chronoTerrain; };

        chrono::vehicle::RigidTerrain chronoTerrain;
        std::vector<std::weak_ptr<TerrainPatchEntity>> terrainPatchEntities;
    };
}