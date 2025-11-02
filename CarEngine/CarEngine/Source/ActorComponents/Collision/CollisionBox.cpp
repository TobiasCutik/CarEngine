#include "CollisionBox.h"
#include "Core/World.h"
#include "Utils/Loader.h"
#include "Utils/Math.h"

namespace CE
{
    CollisionBox::CollisionBox(World* world, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, bool bIsFixed, bool bIsTrigger, bool bActive, Material* material, bool bUseTextureTiling,
                               const std::string& name)
        : CollisionEntity(Collision(),
        std::move(Entity(Mesh(world, world->getAssetManager()->getModel("CE_Box")), material, position, rotation,
            {scale.x, scale.y, scale.z}, bUseTextureTiling, bActive, name)),
            bIsTrigger)
    {
        auto chronoScale = Math::openGlToChronoScale(scale);

        setChronoCollision(std::make_shared<chrono::ChBodyEasyBox>(chronoScale.x, chronoScale.y, chronoScale.z, 1.f, false, true, getRigidMaterial()));
        setPosition(Math::openGlToChronoPosition(position));
        setRotation(Math::openGlToChronoRotation(rotation));
        setCollisionFixed(bIsFixed);
        setCollisionActive(bActive);
    }
}