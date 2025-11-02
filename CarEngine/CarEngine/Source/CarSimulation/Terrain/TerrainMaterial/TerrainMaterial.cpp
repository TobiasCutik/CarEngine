#include "TerrainMaterial.h"

namespace CE
{
    TerrainMaterial::TerrainMaterial()
    {
    }

    float TerrainMaterial::getStaticFriction() const
    {
        if (auto chronoMaterial = getChronoMaterial())
            return chronoMaterial->GetSfriction();

        return -1.f;
    }

    float TerrainMaterial::getKineticFriction() const
    {
        if (auto chronoMaterial = getChronoMaterial())
            return chronoMaterial->GetKfriction();

        return -1.f;
    }

    void TerrainMaterial::setFriction(float friction)
    {
        if (auto chronoMaterial = getChronoMaterial())
            chronoMaterial->SetFriction(friction);
    }

    float TerrainMaterial::getRestitution() const
    {
        if (auto chronoMaterial = getChronoMaterial())
            return chronoMaterial->GetRestitution();

        return -1.f;
    }

    void TerrainMaterial::setRestitution(float restitution)
    {
        if (auto chronoMaterial = getChronoMaterial())
            chronoMaterial->SetRestitution(restitution);
    }
}