#include "SampleLevelGeometry.h"
#include "ActorComponents/EntityCollection.h"
#include "Core/World.h"

using namespace CE;

namespace CD
{
    SampleLevelGeometry::SampleLevelGeometry(World* world) : Actor(world)
    {
        auto colorEntityCollectionCollisions =
                addComponent<EntityCollection>(std::move(Mesh(world, world->getAssetManager()->getModel("GroundTerrain"))),
                                               std::vector {Materials::white()});
        colorEntityCollectionCollisions.lock()->createCollisionEntities(world, this);
        colorEntityCollectionCollisions.lock()->createTerrainPatchEntities(world, this);
    }
}