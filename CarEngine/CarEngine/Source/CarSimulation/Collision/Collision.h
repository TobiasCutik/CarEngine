#pragma once

#include <chrono/physics/ChBody.h>
#include <chrono/physics/ChMaterialSurfaceNSC.h>
#include "../../Core/Transformable.h"

namespace CE
{
    class Collision {

    friend class CarSimulationTransformManager;
    friend class CarSimulationHandler;

    public:
        Collision() = default;
        virtual ~Collision() = default;

        bool isCollisionFixed() const;
        void setCollisionFixed(bool bFixed);
        bool isCollisionActive() const { return bCollisionIsActive; }
        void setCollisionActive(bool bCollisionActive);

    protected:
        void setPosition(const chrono::ChVector<float>& position);
        void setRotation(const chrono::ChQuaternion<float>& rotation);

        void setChronoCollision(const std::shared_ptr<chrono::ChBody>& collision);

        std::shared_ptr<chrono::ChMaterialSurfaceNSC> getRigidMaterial() { return rigidMaterial; }

    private:
        Transformable getCollisionTransform();
        void updateChronoTransform();

        bool bCollisionIsActive = false;

        Transformable chronoTransform;
        std::shared_ptr<chrono::ChBody> chronoCollision;

        std::shared_ptr<chrono::ChMaterialSurfaceNSC> rigidMaterial = chrono_types::make_shared<chrono::ChMaterialSurfaceNSC>();
    };
}