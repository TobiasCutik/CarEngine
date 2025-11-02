// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Alesandro Tasora, Radu Serban
// =============================================================================

#include "ChTriangleMeshShape.h"

namespace chrono {

// Register into the object factory, to enable run-time dynamic creation and persistence
CH_FACTORY_REGISTER(ChTriangleMeshShape)

ChTriangleMeshShape::ChTriangleMeshShape()
    : name(""), scale(ChVector<>(1)), wireframe(false), backface_cull(false), fixed_connectivity(false) {
    trimesh = chrono_types::make_shared<geometry::ChTriangleMeshConnected>();
};

}  // end namespace chrono
