#include "Terrain.h"

namespace CE
{
    Terrain::Terrain()
    {
    }

    void Terrain::updateChronoTerrain(double simulationTime)
    {
        getChronoTerrain()->Synchronize(simulationTime);
    }
}