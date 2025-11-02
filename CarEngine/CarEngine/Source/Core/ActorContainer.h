#pragma once

#include "Actor.h"

namespace CE
{
    class ActorContainer {

    friend class World;
    friend class LevelManager;

    public:
        ActorContainer();

        template <class T, typename... Args>
        std::weak_ptr<T> addActor(Args&&... args)
        {
            const auto actor = std::make_shared<T>(std::forward<Args>(args)...);

            if (unusedIds.empty())
            {
                actor->id = generateNewId();
            }
            else
            {
                actor->id = unusedIds.at(0);
                unusedIds.erase(unusedIds.begin());
            }

            addedActors.emplace_back(actor);
            onActorAdded.broadcast(actor.get());

            return actor;
        }

        template <class T>
        std::weak_ptr<T> getActorById(int id)
        {
            for (const auto& actor : simulatedActors)
            {
                if (actor && actor->id == id)
                    return std::dynamic_pointer_cast<T>(actor);
            }

            return {};
        }

        template <class T>
        std::weak_ptr<T> getActorByName(const std::string& name)
        {
            for (const auto& actor : simulatedActors)
            {
                if (actor && actor->name == name)
                    return std::dynamic_pointer_cast<T>(actor);
            }

            return {};
        }

        Event<ActorContainer, Actor*> onActorAdded;
        Event<ActorContainer, Actor*> onActorDeleted;

    private:
        void makeActorsSimulated(World* world);

        void deleteFlaggedActors();
        void deleteFlaggedComponents();

        int generateNewId() { return nextId++; }
        std::vector<std::shared_ptr<Actor>>* getAllActors() { return &simulatedActors; }

        int nextId = 0;
        std::vector<int> unusedIds;

        std::vector<std::shared_ptr<Actor>> addedActors;
        std::vector<std::shared_ptr<Actor>> simulatedActors;
    };
}