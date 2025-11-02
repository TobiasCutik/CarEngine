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
// Authors: Radu Serban
// =============================================================================
//
// Template for a rigid-body chassis vehicle subsystem.
//
// =============================================================================

#include "chrono/utils/ChUtilsCreators.h"

#include "chrono_vehicle/ChVehicleModelData.h"
#include "chrono_vehicle/chassis/ChRigidChassis.h"

namespace chrono {
namespace vehicle {

// -----------------------------------------------------------------------------

ChRigidChassis::ChRigidChassis(const std::string& name, bool fixed) : ChChassis(name, fixed) {}

void ChRigidChassis::Initialize(ChSystem* system,
                                const ChCoordsys<>& chassisPos,
                                double chassisFwdVel,
                                int collision_family) {
    // Invoke the base class method to construct the frame body.
    ChChassis::Initialize(system, chassisPos, chassisFwdVel);

    // If collision shapes were defined, create the contact geometry and enable contact
    // for the chassis's rigid body.
    // NOTE: setting the collision family is deferred to the containing vehicle system
    // (which can also disable contact between the chassis and certain vehicle subsystems).
    if (m_geometry.m_has_collision) {
        CreateContactMaterials(system->GetContactMethod());
        m_geometry.AddCollisionShapes(m_body, collision_family);
    }
}

void ChRigidChassis::Output(ChVehicleOutput& database) const {
    if (!m_output)
        return;

    std::vector<std::shared_ptr<ChBodyAuxRef>> bodies;
    bodies.push_back(m_body);
    database.WriteAuxRefBodies(bodies);

    database.WriteMarkers(m_markers);
}

// -----------------------------------------------------------------------------

ChRigidChassisRear::ChRigidChassisRear(const std::string& name) : ChChassisRear(name) {}

void ChRigidChassisRear::Initialize(std::shared_ptr<ChChassis> chassis,
                                    int collision_family) {
    // Invoke the base class method to construct the frame body.
    ChChassisRear::Initialize(chassis, collision_family);

    // If collision shapes were defined, create the contact geometry and enable contact
    // for the chassis's rigid body.
    // NOTE: setting the collision family is deferred to the containing vehicle system
    // (which can also disable contact between the chassis and certain vehicle subsystems).
    if (m_geometry.m_has_collision) {
        CreateContactMaterials(m_body->GetSystem()->GetContactMethod());
        m_geometry.AddCollisionShapes(m_body, collision_family);
    }
}

void ChRigidChassisRear::Output(ChVehicleOutput& database) const {
    if (!m_output)
        return;

    std::vector<std::shared_ptr<ChBodyAuxRef>> bodies;
    bodies.push_back(m_body);
    database.WriteAuxRefBodies(bodies);

    database.WriteMarkers(m_markers);
}

}  // end namespace vehicle
}  // end namespace chrono
