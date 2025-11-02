#pragma once
#include "Terrain.h"
#include <chrono_vehicle/terrain/FlatTerrain.h>

namespace CE
{
    class TerrainPlane : public Terrain {

    friend class CarSimulationTransformManager;

    public:
        explicit TerrainPlane(float yPosition = 0.f, float friction = 0.9f);

        float getFriction() const;
        void setFriction(float friction);

    private:
        void setYPosition(float yPosition);

        chrono::vehicle::ChTerrain* getChronoTerrain() override { return &chronoTerrain; };

        chrono::vehicle::FlatTerrain chronoTerrain;
    };
}