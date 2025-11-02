#include "ActorContainer.h"

namespace CE
{
    ActorContainer::ActorContainer()
    {
    }

    void ActorContainer::makeActorsSimulated(World* world)
    {
        for (auto actor : addedActors)
        {
            actor->start(world);
            simulatedActors.push_back(std::move(actor));
        }

        addedActors.clear();
    }

    void ActorContainer::deleteFlaggedActors()
    {
        for (const auto& actor : simulatedActors)
        {
            if (actor && actor->bDelete)
            {
                unusedIds.push_back(actor->id);
                onActorDeleted.broadcast(actor.get());
            }
        }

        simulatedActors.erase(
                std::remove_if(
                        simulatedActors.begin(),
                        simulatedActors.end(),
                        [](const std::shared_ptr<Actor>& actor) { return actor->bDelete; }
                ),
                simulatedActors.end()
        );
    }

    void ActorContainer::deleteFlaggedComponents()
    {
        for (const auto& actor : simulatedActors)
        {
            if (!actor)
                return;

            actor->deleteFlaggedComponents();
        }
    }
}