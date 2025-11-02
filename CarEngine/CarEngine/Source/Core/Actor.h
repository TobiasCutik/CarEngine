#pragma once

#include "../ActorComponents/Collision/CollisionEntity.h"
#include "../ActorComponents/Terrain/TerrainPlaneEntity.h"
#include "../CarSimulation/Terrain/TerrainMesh.h"
#include "ActorComponents/Terrain/TerrainPatchEntity.h"
#include "CarSimulation/CarSimulationHandler.h"

namespace CE
{
    class Actor : public Transformable {

        friend class ActorContainer;

    public:
        explicit Actor(World* world,
                       glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
                       glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f),
                       glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f),
                       std::string name = "New Actor");

        virtual void start(World* world);
        virtual void vehicleFixedUpdate(World* world);
        virtual void update(World* world, float deltaTime);

        void setName(const std::string& name) { this->name = name; }
        std::string& getName() { return name; }

        void setKeepOnLevelChange(bool keepOnLevelChange) { bKeepOnLevelChange = keepOnLevelChange; }
        bool isKeptOnLevelChange() const { return bKeepOnLevelChange; }

        int getId() const { return id; }

        std::vector<Entity*> getTerrainPatchEntitiesCollidingWithCars();

        template <class T, typename... Args>
        std::weak_ptr<T> addComponent(Args&&... args)
        {
            const auto component = std::make_shared<T>(std::forward<Args>(args)...);

            if (unusedComponentIds.empty())
            {
                component->id = generateNewIdForComponent();
            }
            else
            {
                component->id = unusedComponentIds.at(0);
                unusedComponentIds.erase(unusedComponentIds.begin());
            }

            if (component->isComponentOfType(ActorComponentType::TERRAIN_PLANE_ENTITY))
            {
                if (const auto terrainPlaneEntity = dynamic_cast<TerrainPlaneEntity*>(component.get()))
                    carSimulationHandler->addTerrain(terrainPlaneEntity);
            }
            else if (component->isComponentOfType(ActorComponentType::TERRAIN_PATCH_ENTITY))
            {
                setupTerrainMesh();

                if (terrainMesh)
                {
                    if (const auto terrainPatchEntity = std::dynamic_pointer_cast<TerrainPatchEntity>(component))
                        terrainMesh->addTerrainPatch(terrainPatchEntity);

                    terrainMesh->initialize();
                }
            }
            else if (component->isComponentOfType(ActorComponentType::COLLISION_ENTITY))
            {
                if (const auto collisionEntity = dynamic_cast<CollisionEntity*>(component.get()))
                    carSimulationHandler->addCollisionToSimulation(collisionEntity);
            }

            this->componentList.emplace_back(component);

            return component;
        }

        template <class T>
        std::vector<T*> getComponentsRaw()
        {
            std::vector<T*> components;

            for (const auto& component : componentList)
            {
                if (!component)
                    continue;

                if (component->isComponentOfType(T::componentType))
                    components.emplace_back(static_cast<T*>(component.get()));
            }

            return components;
        }

        template <class T>
        std::vector<std::weak_ptr<T>> getComponentsWeak()
        {
            std::vector<std::weak_ptr<T>> components;

            for (const auto& component : componentList)
            {
                if (!component)
                    continue;

                if (component->isComponentOfType(T::componentType))
                    components.emplace_back(std::static_pointer_cast<T>(component));
            }

            return components;
        }

        template <class T>
        std::weak_ptr<T> getComponentById(int id)
        {
            for (const auto& component : componentList)
            {
                if (!component)
                    continue;

                if (component->id == id && component->isComponentOfType(T::componentType))
                    return std::static_pointer_cast<T>(component);
            }

            return {};
        }

        template <class T>
        std::weak_ptr<T> getComponentByName(const std::string& name)
        {
            for (const auto& component : componentList)
            {
                if (!component)
                    continue;

                if (component->name == name && component->isComponentOfType(T::componentType))
                    return std::static_pointer_cast<T>(component);
            }

            return {};
        }

        static void destroy(Actor* actorRawPtr);
        static void destroy(const std::weak_ptr<Actor>& actorWeakPtr);
        static void destroy(const std::vector<std::weak_ptr<Actor>>& actorWeakPtrList);

        static void destroy(ActorComponent* componentRawPtr);
        static void destroy(const std::weak_ptr<ActorComponent>& componentWeakPtr);
        static void destroy(const std::vector<std::weak_ptr<ActorComponent>>& componentWeakPtrList);

        ~Actor();

    protected:
        std::shared_ptr<CarSimulationHandler> carSimulationHandler;

    private:
        void deleteFlaggedComponents();

        void setupTerrainMesh();
        int generateNewIdForComponent() { return nextComponentId++; }

        std::string name;
        bool bDelete = false;
        bool bKeepOnLevelChange = false;
        int id = -1;
        int nextComponentId = 0;
        std::vector<int> unusedComponentIds;

        std::vector<std::shared_ptr<ActorComponent>> componentList;
        std::unique_ptr<TerrainMesh> terrainMesh;
    };
}