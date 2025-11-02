#include "CarSimulationTransformManager.h"
#include "../Utils/Math.h"

namespace CE
{
    CarSimulationTransformManager::CarSimulationTransformManager()
    {
    }

    void CarSimulationTransformManager::carSimulationTransformUpdate(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        updateCollisionEntityTransforms(actors);
        updateChronoCollisionTransforms(actors);
        updateTerrainPlaneEntityTransforms(actors);
        updateChronoTerrainPlaneTransforms(actors);
        updateTerrainPatchEntityTransforms(actors);
    }

    void CarSimulationTransformManager::updateCollisionEntityTransforms(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        for (auto& actor : actors)
        {
            if (actor == nullptr)
                continue;

            // loop over all collision entities of the current actor
            auto collisionEntities = actor->getComponentsRaw<CollisionEntity>();
            for (auto& collisionEntity : collisionEntities)
            {
                if (!collisionEntity->isCollisionActive() || collisionEntity->isCollisionFixed())
                    continue;

                // set the owning actor position/ rotation to collision (global) position/ rotation - collisionEntity (local) position/ rotation
                auto collisionTransform = collisionEntity->getCollisionTransform();
                actor->position = collisionTransform.position;
                actor->setRotationQ(collisionTransform.getRotationQ());
                actor->position -= collisionEntity->position;
                actor->setRotationQ(actor->getRotationQ() * glm::inverse(collisionEntity->getRotationQ()));
            }
        }
    }

    void CarSimulationTransformManager::updateChronoCollisionTransforms(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        for (auto& actor : actors)
        {
            if (actor == nullptr)
                continue;

            // loop over all collision entities of the current actor
            auto collisionEntities = actor->getComponentsRaw<CollisionEntity>();
            for (auto& collisionEntity : collisionEntities)
            {
                if (!collisionEntity->isCollisionActive() || !collisionEntity->isCollisionFixed())
                    continue;

                if (!actor->transformHasChanged() && !collisionEntity->transformHasChanged())
                    continue;

                // calculate the global transform of the collisionEntity
                glm::mat4 entityGlobalMatrix = Math::calculateGlobalTransformationMatrix(actor->getTransformationMatrix(), collisionEntity->getTransformationMatrix());

                auto position = glm::vec3(1.f);
                auto rotation = glm::quat();
                auto scale = glm::vec3(1.f);
                Math::decompose(entityGlobalMatrix, position, rotation, scale);
                
                collisionEntity->setPosition(Math::openGlToChronoPosition(position));
                collisionEntity->setRotation(Math::openGlToChronoRotationQ(rotation));
            }
        }
    }

    void CarSimulationTransformManager::updateTerrainPlaneEntityTransforms(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        for (auto& actor : actors)
        {
            if (actor == nullptr)
                continue;

            // loop over all terrainPlane entities of the current actor
            auto terrainPlaneEntities = actor->getComponentsRaw<TerrainPlaneEntity>();
            for (auto& terrainPlaneEntity : terrainPlaneEntities)
            {
                if (!actor->transformHasChanged() && !terrainPlaneEntity->transformHasChanged())
                    continue;

                // set the local rotations of the terrainPlaneEntity to zero
                terrainPlaneEntity->rotation = {0.f, 0.f, 0.f};

                // calculate the inverse of the (global) transform of the actor
                // to avoid incorrect calculations in glm::inverse(), each scale value of the actor must be the same
                glm::mat4 actorGlobalMatrixInverse = glm::inverse(actor->getTransformationMatrix());

                auto position = glm::vec3(1.f);
                auto rotation = glm::quat();
                auto scale = glm::vec3(1.f);
                Math::decompose(actorGlobalMatrixInverse, position, rotation, scale);

                // the rotation values of the inverse actor transformation matrix are applied to the terrainPlaneEntity to compensate any rotations of the actor
                terrainPlaneEntity->setRotationQ(rotation);
            }
        }
    }

    void CarSimulationTransformManager::updateChronoTerrainPlaneTransforms(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        for (auto& actor : actors)
        {
            if (actor == nullptr)
                continue;

            // loop over all terrainPlane entities of the current actor
            auto terrainPlaneEntities = actor->getComponentsRaw<TerrainPlaneEntity>();
            for (auto& terrainPlaneEntity : terrainPlaneEntities)
            {
                if (!terrainPlaneEntity->isTerrainActive())
                    continue;

                if (!actor->transformHasChanged() && !terrainPlaneEntity->transformHasChanged())
                    continue;

                // calculate the global transform of the terrainPlaneEntity
                glm::mat4 entityGlobalMatrix = Math::calculateGlobalTransformationMatrix(actor->getTransformationMatrix(), terrainPlaneEntity->getTransformationMatrix());

                auto position = glm::vec3(entityGlobalMatrix[3]);

                terrainPlaneEntity->setYPosition(position.y);
            }
        }
    }

    void CarSimulationTransformManager::updateTerrainPatchEntityTransforms(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        for (auto& actor : actors)
        {
            if (actor == nullptr)
                continue;

            // loop over all terrainPatch entities of the current actor
            auto terrainPatchEntities = actor->getComponentsRaw<TerrainPatchEntity>();
            for (auto& terrainPatchEntity : terrainPatchEntities)
            {
                if (!actor->transformHasChanged())
                    continue;

                // calculate the new local transformation matrix of terrainPatchEntity
                // to avoid incorrect calculations in glm::inverse(), each scale value of the actor must be the same
                auto newEntityTransformationMatrix = glm::inverse(actor->getTransformationMatrix()) * terrainPatchEntity->getGlobalTransform().getTransformationMatrix();

                auto position = glm::vec3(1.f);
                auto rotation = glm::quat();
                auto scale = glm::vec3(1.f);
                Math::decompose(newEntityTransformationMatrix, position, rotation, scale);

                terrainPatchEntity->setTransformationMatrixManually(position, glm::degrees(glm::eulerAngles(rotation)), scale);
            }
        }
    }
}