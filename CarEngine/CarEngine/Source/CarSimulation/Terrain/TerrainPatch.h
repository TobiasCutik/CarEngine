#pragma once

#include <chrono/physics/ChMaterialSurface.h>
#include "../../Core/Transformable.h"
#include "chrono_vehicle/terrain/RigidTerrain.h"

namespace CE
{
    class TerrainPatch {

    friend class TerrainMesh;

    public:
        TerrainPatch();
        virtual ~TerrainPatch() = default;

        Transformable getGlobalTransform() { return globalTransform; }

        float getFriction() const;
        void setFriction(float friction);

    protected:
        void calculateGlobalTransform(glm::mat4 actorTransformationMatrix, glm::mat4 entityTransformationMatrix);

        int terrainPatchId = -1;

    private:
        chrono::ChVector<> getChronoPosition() const;
        chrono::ChVector<> getChronoNormal();
        glm::vec3 getScale() const;

        virtual std::shared_ptr<chrono::ChMaterialSurface> getChronoMaterialPtr() const = 0;

        std::shared_ptr<chrono::vehicle::RigidTerrain::Patch> chronoTerrainPatch;
        Transformable globalTransform;
    };
}