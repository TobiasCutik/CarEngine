#include "Math.h"
#include "../Core/Transformable.h"

namespace CE::Math
{
    glm::mat4 calculateGlobalTransformationMatrix(glm::mat4 owningActorMatrix, glm::mat4 transformableLocalMatrix)
    {
        return owningActorMatrix * transformableLocalMatrix;
    }

    chrono::ChVector<> rotateChronoVector(const chrono::ChVector<>& vectorToRotate, glm::quat quaternion)
    {
        chrono::ChQuaternion<> chronoRotation = Math::openGlToChronoRotationQ(quaternion);
        return chronoRotation.Rotate(vectorToRotate);
    }

    glm::vec3 chronoToOpenGlPosition(const chrono::ChVector<double>& position)
    {
        glm::vec3 openGlPosition;

        openGlPosition.x = (float) position.x();
        openGlPosition.y = (float) position.y();
        openGlPosition.z = (float) position.z();

        float tempPositionX = openGlPosition.x;
        openGlPosition.x = -openGlPosition.y;
        openGlPosition.y = openGlPosition.z;
        openGlPosition.z = -tempPositionX;

        return openGlPosition;
    }

    glm::vec3 chronoToOpenGlRotation(const chrono::ChVector<double>& rotation)
    {
        glm::vec3 openGlRotation;

        openGlRotation.x = (float) rotation.x();
        openGlRotation.y = (float) rotation.y();
        openGlRotation.z = (float) rotation.z();
        openGlRotation *= chrono::CH_C_RAD_TO_DEG;

        float tempRotationX = openGlRotation.x;
        openGlRotation.x = -openGlRotation.y;
        openGlRotation.y = openGlRotation.z;
        openGlRotation.z = -tempRotationX;

        return openGlRotation;
    }

    glm::quat chronoToOpenGlRotationQ(const chrono::ChQuaternion<double>& quaternion)
    {
        glm::quat openGlRotationQuat;

        openGlRotationQuat.w = (float) quaternion.e0();
        openGlRotationQuat.x = (float) -quaternion.e2();
        openGlRotationQuat.y = (float) quaternion.e3();
        openGlRotationQuat.z = (float) -quaternion.e1();

        return openGlRotationQuat;
    }

    glm::vec3 chronoToOpenGlScale(glm::vec3 scale)
    {
        float tempScaleX = scale.x;
        scale.x = -scale.y;
        scale.y = scale.z;
        scale.z = -tempScaleX;

        return scale;
    }

    chrono::ChVector<double> openGlToChronoPosition (glm::vec3 position)
    {
        float tempPositionX = position.x;
        float tempPositionY = position.y;
        position.x = -position.z;
        position.y = -tempPositionX;
        position.z = tempPositionY;

        return {position.x, position.y, position.z};
    }

    chrono::ChQuaternion<double> openGlToChronoRotation(glm::vec3 rotation)
    {
        float tempRotationX = rotation.x;
        float tempRotationY = rotation.y;
        rotation.x = -rotation.z;
        rotation.y = -tempRotationX;
        rotation.z = tempRotationY;

        chrono::ChQuaternion<double> chronoRotationX = chrono::Q_from_AngX(rotation.x * chrono::CH_C_DEG_TO_RAD);
        chrono::ChQuaternion<double> chronoRotationY = chrono::Q_from_AngY(rotation.y * chrono::CH_C_DEG_TO_RAD);
        chrono::ChQuaternion<double> chronoRotationZ = chrono::Q_from_AngZ(rotation.z * chrono::CH_C_DEG_TO_RAD);

        return chronoRotationX * chronoRotationY * chronoRotationZ;
    }

    chrono::ChQuaternion<double> openGlToChronoRotationQ(glm::quat quaternion)
    {
        chrono::ChQuaternion<double> chronoRotation;

        chronoRotation.e0() = quaternion.w;
        chronoRotation.e1() = -quaternion.z;
        chronoRotation.e2() = -quaternion.x;
        chronoRotation.e3() = quaternion.y;

        return chronoRotation;
    }

    glm::vec3 openGlToChronoScale(glm::vec3 scale)
    {
        float tempScaleX = scale.x;
        float tempScaleY = scale.y;
        scale.x = scale.z;
        scale.y = tempScaleX;
        scale.z = tempScaleY;

        return scale;
    }

    glm::vec2 rotatePointAroundPivotPoint(glm::vec2 pointToRotate, glm::vec2 pivotPoint, float angleInDegrees)
    {
        float angleInRadians = glm::radians(angleInDegrees);
        float sine = sin(angleInRadians);
        float cosine = cos(angleInRadians);

        // translate point back to origin
        pointToRotate.x -= pivotPoint.x;
        pointToRotate.y -= pivotPoint.y;

        // rotate point
        float xNew = pointToRotate.x * cosine + pointToRotate.y * sine;
        float yNew = -pointToRotate.x * sine + pointToRotate.y * cosine;

        // translate point back
        pointToRotate.x = xNew + pivotPoint.x;
        pointToRotate.y = yNew + pivotPoint.y;

        return pointToRotate;
    }

    float lerp(float lowerBound, float upperBound, float value)
    {
        return lowerBound + (upperBound - lowerBound) * value;
    }

    float inverseLerp(float lowerBoundOfValue, float upperBoundOfValue, float value)
    {
        return (value - lowerBoundOfValue) / (upperBoundOfValue - lowerBoundOfValue);
    }
}