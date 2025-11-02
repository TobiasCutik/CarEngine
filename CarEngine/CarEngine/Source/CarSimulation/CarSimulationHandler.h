#pragma once
#include <chrono/physics/ChSystem.h>
#include "Terrain/Terrain.h"
#include "ContactMethod.h"
#include "ActorComponents/Collision/CollisionEntity.h"

namespace CE
{
    class ActorContainer;
    class Car;

    class CarSimulationHandler {
        
    friend class Actor;
    friend class Engine;
    friend class World;

    public:
        explicit CarSimulationHandler(std::weak_ptr<ActorContainer> actorContainer, int maxSimulationIterations = 60, float carSimulationStepSize = 2e-3f);

        CarSimulationHandler(const CarSimulationHandler& other) = delete;

        CarSimulationHandler(CarSimulationHandler&& other) noexcept {
            std::swap(this->simulationSystem, other.simulationSystem);
            std::swap(this->maxSimulationIterations, other.maxSimulationIterations);
            std::swap(this->carSimulationStepSize, other.carSimulationStepSize);
            std::swap(this->debugDeltaTimeModifier, other.debugDeltaTimeModifier);
            std::swap(this->terrainList, other.terrainList);
            std::swap(this->collisionEntityList, other.collisionEntityList);
            std::swap(this->carList, other.carList);
            std::swap(this->actorContainer, other.actorContainer);
            std::swap(this->registerCarCallbackId, other.registerCarCallbackId);
            std::swap(this->unregisterCarCallbackId, other.unregisterCarCallbackId);
        }

        ~CarSimulationHandler();

        void setMaxSimulationIterations(int maxIterations);
        int getMaxSimulationIterations() const { return maxSimulationIterations; }

        void setCarSimulationStepSize(float stepSize) { carSimulationStepSize = stepSize; }
        float getCarSimulationStepSize() const { return carSimulationStepSize; }

        void setDebugDeltaTimeModifier(float modifier) { debugDeltaTimeModifier = modifier; }
        float getDebugDeltaTimeModifier() const { return debugDeltaTimeModifier; }

        double getSimulationTime();

        Terrain* getActiveTerrain();

        chrono::ChSystem* getSimulationSystem() const { return simulationSystem.get(); }
        void removeAllChronoCollisions();

    private:
        void setContactMethod(ContactMethod contactMethod);

        void registerCar(Actor* actor);
        void unregisterCar(Actor* actor);

        void onCollision(const chrono::collision::ChCollisionInfo& contactInfo);

        void addCollisionToSimulation(CollisionEntity* collisionEntity);
        void removeCollisionFromSimulation(CollisionEntity* collisionEntity);

        void updateCollisionEntityTriggers();

        void addTerrain(Terrain* terrain);
        void removeTerrain(Terrain* terrain);

        void updateTerrains();

        std::unique_ptr<chrono::ChSystem> simulationSystem = nullptr;
        int maxSimulationIterations;
        float carSimulationStepSize;
        float debugDeltaTimeModifier = 2.f;

        std::vector<Terrain*> terrainList;
        std::vector<CollisionEntity*> collisionEntityList;
        std::vector<Car*> carList;

        std::weak_ptr<ActorContainer> actorContainer;

        std::list<std::function<void(Actor*)>>::iterator registerCarCallbackId;
        std::list<std::function<void(Actor*)>>::iterator unregisterCarCallbackId;

        class CeCollisionCallback : public chrono::ChContactContainer::AddContactCallback {

        public:
            explicit CeCollisionCallback(CarSimulationHandler* simulationHandler) : simulationHandler(simulationHandler)
            {
            }

            void OnAddContact(const chrono::collision::ChCollisionInfo& contactinfo, chrono::ChMaterialComposite* const material) override;

            CarSimulationHandler* simulationHandler;
        };
    };
}