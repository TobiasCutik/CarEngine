#pragma once

#include "../Core/Actor.h"

namespace CE::GeneralTransformManager
{
    void resetHasChangedFlagInAllTransforms(const std::vector<std::shared_ptr<Actor>>& actors);
}