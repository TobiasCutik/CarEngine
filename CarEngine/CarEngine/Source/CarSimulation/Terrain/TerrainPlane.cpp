#include "TerrainPlane.h"

namespace CE
{
    TerrainPlane::TerrainPlane(float yPosition, float friction) : chronoTerrain(chrono::vehicle::FlatTerrain(yPosition, friction))
    {
    }

    float TerrainPlane::getFriction() const
    {
        return chronoTerrain.GetCoefficientFriction(0.f);
    }

    void TerrainPlane::setFriction(float friction)
    {
        chronoTerrain.SetFriction(friction);
    }

    void TerrainPlane::setYPosition(float yPosition)
    {
        chronoTerrain.SetHeight(yPosition);
    }
}