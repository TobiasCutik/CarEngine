#include "EntityCollection.h"
#include <utility>
#include "Core/Actor.h"
#include "ActorComponents/Collision/CollisionBox.h"
#include "Utils/Math.h"
#include "CarSimulation/Terrain/TerrainMaterial/TerrainSoftMaterial.h"

namespace CE
{
    EntityCollection::EntityCollection(Mesh mesh, const std::vector<ColorMaterial*>& materials, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool bUseTextureTiling, bool bIsActive, std::string name)
            : Entity(std::move(mesh), materials, position, rotation, scale, bUseTextureTiling, bIsActive, std::move(name))
    {
    }

    EntityCollection::EntityCollection(Mesh mesh, const std::vector<TextureMaterial*>& materials, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool bUseTextureTiling, bool bIsActive, std::string name)
            : Entity(std::move(mesh), materials, position, rotation, scale, bUseTextureTiling, bIsActive, std::move(name))
    {
    }

    void EntityCollection::createCollisionEntities(World* world, Actor* owner)
    {
        for (auto& node : getMesh()->meshNodes)
        {
            if (node.getName().find("CollBox_") != std::string::npos)
            {
                if (node.getName().find("_Vis_") == std::string::npos)
                {
                    // meshNode should be invisible, so we set this meshNode inactive
                    node.setActive(false);
                }

                auto entityMatrix = Math::calculateGlobalTransformationMatrix(getTransformationMatrix(), node.getTransform().getTransformationMatrix());
                auto position = glm::vec3(1.f);
                auto rotation = glm::quat();
                auto scale = glm::vec3(1.f);
                Math::decompose(entityMatrix, position, rotation, scale);

                auto collisionEntityWeak = owner->addComponent<CollisionBox>(world, position, scale.operator*=(scaleAdjustment), glm::degrees(glm::eulerAngles(rotation)),
                                                                                                  true, false, true, Materials::grey(), false, node.getName());

                if (auto collisionEntityShared = collisionEntityWeak.lock())
                    collisionEntityShared->setActive(false);
            }
        }
    }

    void EntityCollection::createTerrainPatchEntities(World* world, Actor* owner)
    {
        for (auto& node : getMesh()->meshNodes)
        {
            if (node.getName().find("TerrPatch_") != std::string::npos)
            {
                if (node.getName().find("_Vis_") == std::string::npos)
                {
                    // meshNode should be invisible, so we set this meshNode inactive
                    node.setActive(false);
                }

                auto entityMatrix = Math::calculateGlobalTransformationMatrix(getTransformationMatrix(), node.getTransform().getTransformationMatrix());
                auto position = glm::vec3(1.f);
                auto rotation = glm::quat();
                auto scale = glm::vec3(1.f);
                Math::decompose(entityMatrix, position, rotation, scale);

                if (node.getName().find("_SOFT_") != std::string::npos)
                {
                    // use TerrainSoftMaterial
                    owner->addComponent<TerrainPatchEntity>(world, owner->getTransformationMatrix(), position, scale.operator*=(scaleAdjustment), glm::degrees(glm::eulerAngles(rotation)),
                                                            Materials::grey(), false, std::make_unique<TerrainSoftMaterial>(), false, node.getName());
                }
                else
                {
                    // use TerrainRigidMaterial
                    owner->addComponent<TerrainPatchEntity>(world, owner->getTransformationMatrix(), position, scale.operator*=(scaleAdjustment), glm::degrees(glm::eulerAngles(rotation)),
                                                            Materials::grey(), false, std::make_unique<TerrainRigidMaterial>(), false, node.getName());
                }
            }
        }
    }
}