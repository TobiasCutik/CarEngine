#include "TerrainPatch.h"
#include "../../Utils/Math.h"

namespace CE
{
    TerrainPatch::TerrainPatch()
    {
    }

    float TerrainPatch::getFriction() const
    {
        return chronoTerrainPatch->GetFriction();
    }

    void TerrainPatch::setFriction(float friction)
    {
        chronoTerrainPatch->SetFriction(friction);
    }

    void TerrainPatch::calculateGlobalTransform(glm::mat4 actorTransformationMatrix, glm::mat4 entityTransformationMatrix)
    {
        glm::mat4 entityGlobalMatrix = Math::calculateGlobalTransformationMatrix(actorTransformationMatrix, entityTransformationMatrix);

        auto rotation = glm::quat();
        Math::decompose(entityGlobalMatrix, globalTransform.position, rotation, globalTransform.scale);
        globalTransform.setRotationQ(rotation);
    }

    chrono::ChVector<> TerrainPatch::getChronoPosition() const
    {
        auto adjustedPositionForChrono = globalTransform.position;

        // within chrono, the terrain surface position is always the y position of the chrono terrainPatch (no matter its height),
        // so we increase the chrono terrainPatch y position by half our y scale
        adjustedPositionForChrono.y += globalTransform.scale.y / 2;

        return Math::openGlToChronoPosition(adjustedPositionForChrono);
    }

    chrono::ChVector<> TerrainPatch::getChronoNormal()
    {
        return Math::rotateChronoVector(chrono::ChVector<>(0.f, 0.f, 1.f), globalTransform.getRotationQ());
    }

    glm::vec3 TerrainPatch::getScale() const
    {
        // chrono terrainPatches do not have any y rotations (openGl coordinates)
        // when switching the scale's x and z values, a y rotation of -90/ 90 degrees can be simulated
        // this switch is performed whenever it fits more than not doing the switch

        glm::vec3 newScale = globalTransform.scale;
        float yRotationMod180 = std::fmodf(globalTransform.rotation.y, 180.f);

        if (std::abs(yRotationMod180) > 45.f && std::abs(yRotationMod180) <= 135.f)
        {
            newScale.x = globalTransform.scale.z;
            newScale.z = globalTransform.scale.x;
        }

        return newScale;
    }
}