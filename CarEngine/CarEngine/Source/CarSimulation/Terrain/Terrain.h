#pragma once

#include <chrono_vehicle/ChTerrain.h>

namespace CE
{
    class Terrain {

        friend class Car;

    public:
        Terrain();
        virtual ~Terrain() = default;

        bool isTerrainActive() const { return bIsActive; }
        void setTerrainActive(bool bActive) { bIsActive = bActive; }

        void updateChronoTerrain(double simulationTime);

    private:
        virtual chrono::vehicle::ChTerrain* getChronoTerrain() = 0;

        bool bIsActive = true;
    };
}