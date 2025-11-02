#pragma once

#include <chrono/physics/ChMaterialSurface.h>

namespace CE
{
    class TerrainMaterial {

    public:
        TerrainMaterial();
        virtual ~TerrainMaterial() = default;

        float getStaticFriction() const;
        float getKineticFriction() const;
        void setFriction(float friction);

        float getRestitution() const;
        void setRestitution(float restitution);

        virtual std::shared_ptr<chrono::ChMaterialSurface> getChronoMaterial() const = 0;
    };
}