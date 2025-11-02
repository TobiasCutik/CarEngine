#include "TerrainRigidMaterial.h"

namespace CE
{
    TerrainRigidMaterial::TerrainRigidMaterial(float friction, float restitution)
    {
        chronoMaterial = std::make_shared<chrono::ChMaterialSurfaceNSC>();
        setFriction(friction);
        setRestitution(restitution);
    }
}