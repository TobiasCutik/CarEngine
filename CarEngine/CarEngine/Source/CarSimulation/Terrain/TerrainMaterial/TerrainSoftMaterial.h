#pragma once

#include <chrono/physics/ChMaterialSurfaceSMC.h>
#include "TerrainMaterial.h"

namespace CE
{
    class TerrainSoftMaterial : public TerrainMaterial {

    public:
        explicit TerrainSoftMaterial(float friction = 1.f, float restitution = 0.1f);

        std::shared_ptr<chrono::ChMaterialSurface> getChronoMaterial() const override { return chronoMaterial; };

    private:
        std::shared_ptr<chrono::ChMaterialSurfaceSMC> chronoMaterial;
    };
}