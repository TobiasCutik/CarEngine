#include "TerrainSoftMaterial.h"

namespace CE
{
    TerrainSoftMaterial::TerrainSoftMaterial(float friction, float restitution)
    {
        chronoMaterial = std::make_shared<chrono::ChMaterialSurfaceSMC>();
        setFriction(friction);
        setRestitution(restitution);
    }
}