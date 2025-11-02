
#include "Transformable.h"

namespace CE
{
    Transformable::Transformable(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : position(position), rotation(rotation), scale(scale)
    {
        setRotationQ(glm::radians(rotation));
        updateTransformationMatrix();
    }

    glm::mat4 Transformable::getTransformationMatrix()
    {
        if (bTransformFixed && transformDeviatesFromTransformationMatrix())
        {
            resetTransform();
        }
        else if (!bTransformFixed && transformDeviatesFromTransformationMatrix())
        {
            updateTransformationMatrix();
        }

        return transformationMatrix;
    }

    void Transformable::setRotationQ(glm::quat quaternion)
    {
        rotationQuat = quaternion;
        rotation = glm::degrees(glm::eulerAngles(quaternion));
        rotationQuatInEuler = rotation;
    }

    glm::quat Transformable::getRotationQ()
    {
        if (glm::any(glm::epsilonNotEqual(rotation, rotationQuatInEuler, glm::epsilon<float>())))
        {
            rotationQuat = glm::radians(rotation);
            rotationQuatInEuler = rotation;
        }

        return rotationQuat;
    }

    void Transformable::setTransformationMatrixManually(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;

        updateTransformationMatrix();
    }

    void Transformable::updateTransformationMatrix()
    {
        transformationMatrixPosition = position;
        transformationMatrixRotation = rotation;
        transformationMatrixScale = scale;

        glm::mat4 transformation = glm::mat4(1.0f);
        transformation = glm::translate(transformation, position);
        transformation *= glm::toMat4(getRotationQ());
        transformation = glm::scale(transformation, scale);

        transformationMatrix = transformation;

        bTransformationMatrixHasChanged = true;
    }

    void Transformable::resetTransform()
    {
        position = transformationMatrixPosition;
        rotation = transformationMatrixRotation;
        scale = transformationMatrixScale;
    }

    bool Transformable::transformDeviatesFromTransformationMatrix() const
    {
        if (transformationMatrixPosition != position)
            return true;

        if (transformationMatrixRotation != rotation)
            return true;

        if (transformationMatrixScale != scale)
            return true;

        return false;
    }
}