#pragma once

#include "../Core/Actor.h"

namespace CE
{
    class CarSimulationTransformManager {

    public:
        CarSimulationTransformManager();

        static void carSimulationTransformUpdate(const std::vector<std::shared_ptr<Actor>>& actors);

    private:
        // update the collisionEntity (entity)/ owning actor transforms to align with their collisions
        static void updateCollisionEntityTransforms(const std::vector<std::shared_ptr<Actor>>& actors);

        // update the collision transforms to align with their entities/ owning actors
        static void updateChronoCollisionTransforms(const std::vector<std::shared_ptr<Actor>>& actors);

        // update the terrainPlaneEntity (entity) transforms to align with their terrainPlanes, which global rotations are fixed to zero
        static void updateTerrainPlaneEntityTransforms(const std::vector<std::shared_ptr<Actor>>& actors);

        // update the terrainPlane transforms to align with their entities/ owning actors
        static void updateChronoTerrainPlaneTransforms(const std::vector<std::shared_ptr<Actor>>& actors);

        // update the terrainPatchEntity (entity) transforms to align with their terrainPatches, which global transforms are fixed
        static void updateTerrainPatchEntityTransforms(const std::vector<std::shared_ptr<Actor>>& actors);
    };
}