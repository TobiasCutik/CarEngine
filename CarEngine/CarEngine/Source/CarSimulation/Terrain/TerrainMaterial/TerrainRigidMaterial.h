#pragma once

#include <chrono/physics/ChMaterialSurfaceNSC.h>
#include "TerrainMaterial.h"

namespace CE
{
    class TerrainRigidMaterial : public TerrainMaterial {

    public:
        explicit TerrainRigidMaterial(float friction = 1.f, float restitution = 0.1f);

        std::shared_ptr<chrono::ChMaterialSurface> getChronoMaterial() const override { return chronoMaterial; };

    private:
        std::shared_ptr<chrono::ChMaterialSurfaceNSC> chronoMaterial;
    };
}