#pragma once

#include <glm/gtx/matrix_decompose.hpp>
#include <chrono/core/ChVector.h>
#include <chrono/core/ChQuaternion.h>

namespace CE::Math
{
    glm::mat4 calculateGlobalTransformationMatrix(glm::mat4 owningActorMatrix, glm::mat4 transformableLocalMatrix);

    chrono::ChVector<> rotateChronoVector(const chrono::ChVector<>& vectorToRotate, glm::quat quaternion);

    glm::vec3 chronoToOpenGlPosition(const chrono::ChVector<double>& position);
    glm::vec3 chronoToOpenGlRotation(const chrono::ChVector<double>& rotation);
    glm::quat chronoToOpenGlRotationQ(const chrono::ChQuaternion<double>& quaternion);
    glm::vec3 chronoToOpenGlScale(glm::vec3 scale);

    chrono::ChVector<double> openGlToChronoPosition(glm::vec3 position);
    chrono::ChQuaternion<double> openGlToChronoRotation(glm::vec3 rotation);
    chrono::ChQuaternion<double> openGlToChronoRotationQ(glm::quat quaternion);
    glm::vec3 openGlToChronoScale(glm::vec3 scale);

    glm::vec2 rotatePointAroundPivotPoint(glm::vec2 pointToRotate, glm::vec2 pivotPoint, float angleInDegrees);

    // returns the linear interpolation between lowerBound and upperBound, if value is inside [0, 1] (the linear extrapolation otherwise)
    float lerp(float lowerBound, float upperBound, float value);

    // returns a fraction, based on value between lowerBoundOfValue and upperBoundOfValue
    float inverseLerp(float lowerBoundOfValue, float upperBoundOfValue, float value);

    // Matrix decompose; modified to calculate position, rotation, scale only
    // from <glm/gtx/matrix_decompose.hpp>
    // http://www.opensource.apple.com/source/WebCore/WebCore-514/platform/graphics/transforms/TransformationMatrix.cpp
    // Decomposes a model matrix to translations, rotation and scale components
    template<typename T, glm::qualifier Q>
    bool decompose(glm::mat<4, 4, T, Q> const& transformationMatrix, glm::vec<3, T, Q> & position, glm::qua<T, Q> & rotation, glm::vec<3, T, Q> & scale)
    {
        glm::mat<4, 4, T, Q> localMatrix(transformationMatrix);

        // Normalize the matrix.
        if(glm::epsilonEqual(localMatrix[3][3], static_cast<T>(0), glm::epsilon<T>()))
            return false;

        for(glm::length_t i = 0; i < 4; ++i)
            for(glm::length_t j = 0; j < 4; ++j)
                localMatrix[i][j] /= localMatrix[3][3];

        // perspectiveMatrix is used to solve for perspective, but it also provides
        // an easy way to test for singularity of the upper 3x3 component.
        glm::mat<4, 4, T, Q> perspectiveMatrix(localMatrix);

        for(glm::length_t i = 0; i < 3; i++)
            perspectiveMatrix[i][3] = static_cast<T>(0);
        perspectiveMatrix[3][3] = static_cast<T>(1);

        if(glm::epsilonEqual(glm::determinant(perspectiveMatrix), static_cast<T>(0), glm::epsilon<T>()))
            return false;

        // Next take care of translation (easy).
        position = glm::vec<3, T, Q>(localMatrix[3]);
        localMatrix[3] = glm::vec<4, T, Q>(0, 0, 0, localMatrix[3].w);

        glm::vec<3, T, Q> row[3], pdum3;

        // Now get scale and shear.
        for(glm::length_t i = 0; i < 3; ++i)
            for(glm::length_t j = 0; j < 3; ++j)
                row[i][j] = localMatrix[i][j];

        // Compute X scale factor and normalize first row.
        scale.x = glm::length(row[0]);// v3Length(Row[0]);

        row[0] = glm::detail::scale(row[0], static_cast<T>(1));

        auto skew = glm::vec3(1.f);
        // Compute XY shear factor and make 2nd row orthogonal to 1st.
        skew.z = glm::dot(row[0], row[1]);
        row[1] = glm::detail::combine(row[1], row[0], static_cast<T>(1), -skew.z);

        // Now, compute Y scale and normalize 2nd row.
        scale.y = glm::length(row[1]);
        row[1] = glm::detail::scale(row[1], static_cast<T>(1));

        // Compute XZ and YZ shears, orthogonalize 3rd row.
        skew.y = glm::dot(row[0], row[2]);
        row[2] = glm::detail::combine(row[2], row[0], static_cast<T>(1), -skew.y);
        skew.x = glm::dot(row[1], row[2]);
        row[2] = glm::detail::combine(row[2], row[1], static_cast<T>(1), -skew.x);

        // Next, get Z scale and normalize 3rd row.
        scale.z = glm::length(row[2]);
        row[2] = glm::detail::scale(row[2], static_cast<T>(1));

        // At this point, the matrix (in rows[]) is orthonormal.
        // Check for a coordinate system flip.  If the determinant
        // is -1, then negate the matrix and the scaling factors.
        pdum3 = glm::cross(row[1], row[2]); // v3Cross(row[1], row[2], Pdum3);
        if(glm::dot(row[0], pdum3) < 0)
        {
            for(glm::length_t i = 0; i < 3; i++)
            {
                scale[i] *= static_cast<T>(-1);
                row[i] *= static_cast<T>(-1);
            }
        }

        // Now, get the rotations out, as described in the gem.
        rotation = glm::quat();
        int i, j, k = 0;
        T root, trace = row[0].x + row[1].y + row[2].z;
        if(trace > static_cast<T>(0))
        {
            root = glm::sqrt(trace + static_cast<T>(1.0));
            rotation.w = static_cast<T>(0.5) * root;
            root = static_cast<T>(0.5) / root;
            rotation.x = root * (row[1].z - row[2].y);
            rotation.y = root * (row[2].x - row[0].z);
            rotation.z = root * (row[0].y - row[1].x);
        } // End if > 0
        else
        {
            static int next[3] = {1, 2, 0};
            i = 0;
            if(row[1].y > row[0].x) i = 1;
            if(row[2].z > row[i][i]) i = 2;
            j = next[i];
            k = next[j];

            root = glm::sqrt(row[i][i] - row[j][j] - row[k][k] + static_cast<T>(1.0));

            rotation[i] = static_cast<T>(0.5) * root;
            root = static_cast<T>(0.5) / root;
            rotation[j] = root * (row[i][j] + row[j][i]);
            rotation[k] = root * (row[i][k] + row[k][i]);
            rotation.w = root * (row[j][k] - row[k][j]);
        } // End if <= 0

        return true;
    }
}