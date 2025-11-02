#include "Actor.h"
#include "Core/World.h"

namespace CE
{
    Actor::Actor(World* world, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string name) : Transformable(position, rotation, scale), name(std::move(name))
    {
        carSimulationHandler = world->getCarSimulationHandler();
    }

    void Actor::start(World* world)
    {
        // called once before update() at game start
    }

    void Actor::vehicleFixedUpdate(World* world)
    {
        // update chrono vehicle physics
    }

    void Actor::update(World* world, float deltaTime)
    {
        // update logic
    }

    std::vector<Entity*> Actor::getTerrainPatchEntitiesCollidingWithCars()
    {
        std::vector<Entity*> terrainPatchEntities;

        if (!terrainMesh)
            return terrainPatchEntities;

        std::vector<int> terrainPatchesCollidingWithCars = terrainMesh->getTerrainPatchesCollidingWithCars();

        for (const auto& component : componentList)
        {
            if (!component)
                continue;

            if (component->isComponentOfType(ActorComponentType::TERRAIN_PATCH_ENTITY))
            {
                for (const auto terrainPatchId : terrainPatchesCollidingWithCars)
                {
                    if (component->getId() == terrainPatchId)
                        terrainPatchEntities.emplace_back(static_cast<Entity*>(component.get()));
                }
            }
        }

        return terrainPatchEntities;
    }

    void Actor::destroy(Actor* actorRawPtr)
    {
        destroy(actorRawPtr->getComponentsWeak<ActorComponent>());
        actorRawPtr->bDelete = true;
    }

    void Actor::destroy(const std::weak_ptr<Actor>& actorWeakPtr)
    {
        if (auto actorSharedPtr = actorWeakPtr.lock())
        {
            destroy(actorSharedPtr->getComponentsWeak<ActorComponent>());
            actorSharedPtr->bDelete = true;
        }
    }

    void Actor::destroy(const std::vector<std::weak_ptr<Actor>>& actorWeakPtrList)
    {
        for (const auto& actorWeakPtr : actorWeakPtrList)
        {
            if (auto actorSharedPtr = actorWeakPtr.lock())
            {
                destroy(actorSharedPtr->getComponentsWeak<ActorComponent>());
                actorSharedPtr->bDelete = true;
            }
        }
    }

    void Actor::destroy(ActorComponent* componentRawPtr)
    {
        componentRawPtr->bDelete = true;
    }

    void Actor::destroy(const std::weak_ptr<ActorComponent>& componentWeakPtr)
    {
        if (auto componentSharedPtr = componentWeakPtr.lock())
            componentSharedPtr->bDelete = true;
    }

    void Actor::destroy(const std::vector<std::weak_ptr<ActorComponent>>& componentWeakPtrList)
    {
        for (const auto& componentWeakPtr : componentWeakPtrList)
        {
            if (auto componentSharedPtr = componentWeakPtr.lock())
                componentSharedPtr->bDelete = true;
        }
    }

    Actor::~Actor()
    {
        deleteFlaggedComponents();

        if (terrainMesh)
            carSimulationHandler->removeTerrain(terrainMesh.get());
    }

    void Actor::deleteFlaggedComponents()
    {
        for (const auto& component : componentList)
        {
            if (!component || !component->bDelete)
                continue;

            unusedComponentIds.push_back(component->id);

            if (component->isComponentOfType(ActorComponentType::TERRAIN_PLANE_ENTITY))
                carSimulationHandler->removeTerrain(static_cast<TerrainPlaneEntity*>(component.get()));

            if (terrainMesh && component->isComponentOfType(ActorComponentType::TERRAIN_PATCH_ENTITY))
                terrainMesh->removeTerrainPatch(std::static_pointer_cast<TerrainPatchEntity>(component));

            if (component->isComponentOfType(ActorComponentType::COLLISION_ENTITY))
                carSimulationHandler->removeCollisionFromSimulation(static_cast<CollisionEntity*>(component.get()));
        }

        componentList.erase(
                std::remove_if(
                        componentList.begin(),
                        componentList.end(),
                        [](const std::shared_ptr<ActorComponent>& component) { return component->bDelete; }
                ),
                componentList.end()
        );
    }

    void Actor::setupTerrainMesh()
    {
        if (terrainMesh)
            return;

        if (auto carSystem = carSimulationHandler->getSimulationSystem())
        {
            terrainMesh = std::make_unique<TerrainMesh>(carSystem);
            carSimulationHandler->addTerrain(terrainMesh.get());
        }
    }
}