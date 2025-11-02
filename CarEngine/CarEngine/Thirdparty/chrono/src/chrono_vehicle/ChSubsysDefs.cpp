// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2020 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban
// =============================================================================
//
// Various utility classes for vehicle subsystems.
//
// =============================================================================

#include "chrono_vehicle/ChVehicleModelData.h"
#include "chrono_vehicle/ChSubsysDefs.h"

#include "chrono/assets/ChAssetLevel.h"
#include "chrono/utils/ChUtilsCreators.h"

#include "chrono/assets/ChTriangleMeshShape.h"

namespace chrono {
namespace vehicle {

ChVehicleGeometry::ChVehicleGeometry()
    : m_has_primitives(false), m_has_mesh(false), m_has_collision(false), m_has_colors(false) {}

void ChVehicleGeometry::AddCollisionShapes(std::shared_ptr<ChBody> body, int collision_family) {
    body->SetCollide(true);

    body->GetCollisionModel()->ClearModel();

    body->GetCollisionModel()->SetFamily(collision_family);

    for (auto& sphere : m_coll_spheres) {
        assert(m_materials[sphere.m_matID] &&
               m_materials[sphere.m_matID]->GetContactMethod() == body->GetSystem()->GetContactMethod());
        body->GetCollisionModel()->AddSphere(m_materials[sphere.m_matID], sphere.m_radius, sphere.m_pos);
    }
    for (auto& box : m_coll_boxes) {
        assert(m_materials[box.m_matID] &&
               m_materials[box.m_matID]->GetContactMethod() == body->GetSystem()->GetContactMethod());
        ChVector<> hdims = box.m_dims / 2;
        body->GetCollisionModel()->AddBox(m_materials[box.m_matID], hdims.x(), hdims.y(), hdims.z(), box.m_pos,
                                          box.m_rot);
    }
    for (auto& cyl : m_coll_cylinders) {
        assert(m_materials[cyl.m_matID] &&
               m_materials[cyl.m_matID]->GetContactMethod() == body->GetSystem()->GetContactMethod());
        body->GetCollisionModel()->AddCylinder(m_materials[cyl.m_matID], cyl.m_radius, cyl.m_radius, cyl.m_length / 2,
                                               cyl.m_pos, cyl.m_rot);
    }
    for (auto& hulls_group : m_coll_hulls) {
        assert(m_materials[hulls_group.m_matID] &&
               m_materials[hulls_group.m_matID]->GetContactMethod() == body->GetSystem()->GetContactMethod());
        geometry::ChTriangleMeshConnected mesh;
        std::vector<std::vector<ChVector<>>> hulls;
        utils::LoadConvexHulls(vehicle::GetDataFile(hulls_group.m_filename), mesh, hulls);
        for (int c = 0; c < hulls.size(); c++) {
            body->GetCollisionModel()->AddConvexHull(m_materials[hulls_group.m_matID], hulls[c]);
        }
    }
    for (auto& mesh : m_coll_meshes) {
        auto trimesh = chrono_types::make_shared<geometry::ChTriangleMeshConnected>();
        trimesh->LoadWavefrontMesh(mesh.m_filename, true, false);
        // Hack: explicitly offset vertices
        for (auto& v : trimesh->m_vertices)
            v += mesh.m_pos;
        body->GetCollisionModel()->AddTriangleMesh(m_materials[mesh.m_matID], trimesh, false, false, ChVector<>(0),
                                                   ChMatrix33<>(1), mesh.m_radius);
    }

    body->GetCollisionModel()->BuildModel();
}

}  // end namespace vehicle
}  // end namespace chrono
