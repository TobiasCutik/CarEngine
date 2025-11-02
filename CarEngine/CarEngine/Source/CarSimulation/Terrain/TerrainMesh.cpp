#include "TerrainMesh.h"

namespace CE
{
    TerrainMesh::TerrainMesh(chrono::ChSystem* carSystem) : chronoTerrain(chrono::vehicle::RigidTerrain(carSystem))
    {
    }

    void TerrainMesh::addTerrainPatch(const std::shared_ptr<TerrainPatchEntity>& terrainPatchEntity)
    {
        if (auto chronoMaterialPtr = terrainPatchEntity->getChronoMaterialPtr())
        {
            terrainPatchEntity->initializeTerrainPatchId();
            terrainPatchEntity->chronoTerrainPatch = std::move(chronoTerrain.AddPatch(terrainPatchEntity->terrainPatchId, chronoMaterialPtr, terrainPatchEntity->getChronoPosition(),
                                                                                      terrainPatchEntity->getChronoNormal(), terrainPatchEntity->getScale().z, terrainPatchEntity->getScale().x,
                                                                                      terrainPatchEntity->getScale().y));

            terrainPatchEntities.emplace_back(terrainPatchEntity);
        }
    }

    void TerrainMesh::removeTerrainPatch(const std::shared_ptr<TerrainPatchEntity>& terrainPatchEntity)
    {
        if (auto chronoTerrainPatchPtr = terrainPatchEntity->chronoTerrainPatch)
            chronoTerrain.RemovePatch(chronoTerrainPatchPtr);
    }

    void TerrainMesh::initialize()
    {
        chronoTerrain.Initialize();
    }

    std::vector<TerrainPatchEntity*> TerrainMesh::getTerrainPatchEntities()
    {
        std::vector<TerrainPatchEntity*> terrainPatchEntitiesRaw;

        for (const auto& terrainPatchEntityWeak : terrainPatchEntities)
        {
            if (const auto& terrainPatchEntityShared = terrainPatchEntityWeak.lock())
            {
                terrainPatchEntitiesRaw.emplace_back(terrainPatchEntityShared.get());
            }
        }

        return terrainPatchEntitiesRaw;
    }
}