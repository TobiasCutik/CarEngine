#pragma once

#include "../../CarSimulation/Collision/Collision.h"
#include "../Entity.h"
#include "Core/Event.h"

namespace CE
{
    class Car;

    class CollisionEntity : public Collision, public Entity {

    public:
        explicit CollisionEntity(Collision collision, Entity entity, bool bIsTrigger = false);

        void setIsTrigger(bool bTrigger);
        void resetTrigger();

        bool isTrigger() const { return bIsTrigger; }
        bool wasTriggered() const { return bWasTriggered; }

        ~CollisionEntity();

        Event<CarSimulationHandler, CollisionEntity*> onCollision;
        Event<CarSimulationHandler, Car*> onCollisionWithCar;

        const static ActorComponentType componentType = ActorComponentType::COLLISION_ENTITY;

    private:
        bool bIsTrigger = false;
        bool bWasTriggered = false;

        std::list<std::function<void(CollisionEntity*)>>::iterator onCollisionCallbackId;
        std::list<std::function<void(Car*)>>::iterator onCollisionWithCarCallbackId;
    };
}