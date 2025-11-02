#pragma once

#include <glm/gtx/quaternion.hpp>

namespace CE
{
    class World;

    class Transformable {

    friend class CarSimulationTransformManager;

    public:
        explicit Transformable(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                               glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                               glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f));

        glm::mat4 getTransformationMatrix();
        bool transformHasChanged() const { return (!bTransformFixed && transformDeviatesFromTransformationMatrix()) || bTransformationMatrixHasChanged; }
        void resetTransformHasChangedFlag() { bTransformationMatrixHasChanged = false; }

        void setRotationQ(glm::quat quaternion);
        glm::quat getRotationQ();

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        // fixed non-actors do only work well if their owning actor is fixed as well/ not transformed at all
        // meaning the transform of non-actors, that are attached to an actor, gets only fixed locally
        // (terrainPatchEntities are an exception as they are specifically handled in carSimulationTransformManager
        // meaning they are fixed globally, while their owning actor is not fixed/ can still be transformed)
        bool bTransformFixed = false;

    private:
        bool transformDeviatesFromTransformationMatrix() const;
        void updateTransformationMatrix();
        void resetTransform();

        // sets transform and transformationMatrix even when bTransformFixed = true
        void setTransformationMatrixManually(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        bool bTransformationMatrixHasChanged = false;

        glm::mat4 transformationMatrix = glm::mat4(1.f);
        glm::vec3 transformationMatrixPosition;
        glm::vec3 transformationMatrixRotation;
        glm::vec3 transformationMatrixScale;

        glm::quat rotationQuat = glm::quat();
        glm::vec3 rotationQuatInEuler;
    };
}