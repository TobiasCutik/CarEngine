#include "CollisionEntity.h"
#include <utility>

namespace CE
{
    CollisionEntity::CollisionEntity(Collision collision, Entity entity, bool bIsTrigger) : Collision(std::move(collision)), Entity(std::move(entity))
    {
        setComponentType(ActorComponentType::COLLISION_ENTITY);
        setIsTrigger(bIsTrigger);
    }

    void CollisionEntity::setIsTrigger(bool bTrigger)
    {
        if (bIsTrigger == bTrigger)
            return;

        if (bTrigger)
        {
            onCollisionWithCarCallbackId = onCollisionWithCar.addCallback([this] (Car* car) { bWasTriggered = true; });
            onCollisionCallbackId = onCollision.addCallback([this] (CollisionEntity* collisionEntity) { bWasTriggered = true; });
        }
        else
        {
            onCollisionWithCar.removeCallback(onCollisionWithCarCallbackId);
            onCollision.removeCallback(onCollisionCallbackId);
        }

        bIsTrigger = bTrigger;
        bWasTriggered = false;
    }

    void CollisionEntity::resetTrigger()
    {
        if (!bIsTrigger)
            return;

        setCollisionActive(true);
        bWasTriggered = false;
    }

    CollisionEntity::~CollisionEntity()
    {
        setIsTrigger(false);
    }
}