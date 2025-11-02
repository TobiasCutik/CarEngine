#include "Collision.h"
#include "Utils/Math.h"

namespace CE
{
    bool Collision::isCollisionFixed() const
    {
        if (chronoCollision)
            return chronoCollision->GetBodyFixed();

        return false;
    }

    void Collision::setCollisionFixed(bool bFixed)
    {
        if (chronoCollision)
            chronoCollision->SetBodyFixed(bFixed);
    }

    void Collision::setCollisionActive(bool bCollisionActive)
    {
        if (chronoCollision)
        {
            chronoCollision->SetCollide(bCollisionActive);
            bCollisionIsActive = bCollisionActive;
        }
    }

    void Collision::setPosition(const chrono::ChVector<float>& position)
    {
        if (chronoCollision)
        {
            chronoCollision->SetPos(position);
        }
    }

    void Collision::setRotation(const chrono::ChQuaternion<float>& rotation)
    {
        if (chronoCollision)
        {
            chronoCollision->SetRot(rotation);
        }
    }

    Transformable Collision::getCollisionTransform()
    {
        updateChronoTransform();

        return chronoTransform;
    }

    void Collision::setChronoCollision(const std::shared_ptr<chrono::ChBody>& collision)
    {
        if (collision)
            chronoCollision = collision;
    }

    void Collision::updateChronoTransform()
    {
        if (chronoCollision)
        {
            chronoTransform.position = Math::chronoToOpenGlPosition(chronoCollision->GetPos());
            chronoTransform.setRotationQ(Math::chronoToOpenGlRotationQ(chronoCollision->GetRot()));
        }
    }
}