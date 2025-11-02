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
// Authors: Radu Serban, Justin Madsen
// =============================================================================
//
// Base class for a vehicle wheel.
// A wheel subsystem does not own a body. Instead, when attached to the spindle 
// of a suspension subsystem, the wheel's mass properties are used to update
// those of the spindle body owned by the suspension.
// A concrete wheel subsystem can optionally carry its own visualization assets
// (which are associated with the suspension's spindle body).
//
// =============================================================================

#include <algorithm>

#include "chrono/core/ChGlobal.h"

#include "chrono_vehicle/ChVehicleModelData.h"
#include "chrono_vehicle/wheeled_vehicle/ChWheel.h"
#include "chrono_vehicle/wheeled_vehicle/ChTire.h"

namespace chrono {
namespace vehicle {

ChWheel::ChWheel(const std::string& name) : ChPart(name), m_offset(0), m_side(LEFT) {}

// Initialize this wheel by associating it to the specified suspension spindle body.
// Increment the mass and inertia of the spindle body to account for the wheel mass and inertia.
void ChWheel::Initialize(std::shared_ptr<ChBody> spindle, VehicleSide side, double offset) {
    m_spindle = spindle;
    m_side = side;
    m_offset = (side == LEFT) ? offset : -offset;

    //// RADU
    //// Todo:  Properly account for offset in adjusting inertia.
    ////        This requires changing the spindle to a ChBodyAuxRef.
    m_spindle->SetMass(m_spindle->GetMass() + GetMass());
    m_spindle->SetInertiaXX(m_spindle->GetInertiaXX() + GetInertia());
}

void ChWheel::Synchronize() {
    if (!m_tire)
        return;
    auto tire_force = m_tire->GetTireForce();
    m_spindle->Accumulate_force(tire_force.force, tire_force.point, false);
    m_spindle->Accumulate_torque(tire_force.moment, false);
}

ChVector<> ChWheel::GetPos() const {
    return m_spindle->TransformPointLocalToParent(ChVector<>(0, m_offset, 0));
}

WheelState ChWheel::GetState() const {
    WheelState state;

    ChFrameMoving<> wheel_loc(ChVector<>(0, m_offset, 0), QUNIT);
    ChFrameMoving<> wheel_abs;
    m_spindle->TransformLocalToParent(wheel_loc, wheel_abs);
    state.pos = wheel_abs.GetPos();
    state.rot = wheel_abs.GetRot();
    state.lin_vel = wheel_abs.GetPos_dt();
    state.ang_vel = wheel_abs.GetWvel_par();

    ChVector<> ang_vel_loc = state.rot.RotateBack(state.ang_vel);
    state.omega = ang_vel_loc.y();

    return state;
}

}  // end namespace vehicle
}  // end namespace chrono
