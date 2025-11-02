#include "GeneralTransformManager.h"

namespace CE::GeneralTransformManager
{
    void resetHasChangedFlagInAllTransforms(const std::vector<std::shared_ptr<Actor>>& actors)
    {
        for (auto& actor : actors)
        {
            if (actor == nullptr)
                continue;

            actor->resetTransformHasChangedFlag();

            // loop over all entities of the current actor
            auto entities = actor->getComponentsRaw<Entity>();
            for (auto& entity : entities)
                entity->resetTransformHasChangedFlag();
        }
    }
}