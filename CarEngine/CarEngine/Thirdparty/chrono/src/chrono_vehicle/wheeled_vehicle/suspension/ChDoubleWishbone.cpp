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
// Authors: Radu Serban, Justin Madsen, Daniel Melanz
// =============================================================================
//
// Base class for a double-A arm suspension modeled with bodies and constraints.
//
// The suspension subsystem is modeled with respect to a right-handed frame,
// with X pointing towards the front, Y to the left, and Z up (ISO standard).
// The suspension reference frame is assumed to be always aligned with that of
// the vehicle.  When attached to a chassis, only an offset is provided.
//
// All point locations are assumed to be given for the left half of the
// suspension and will be mirrored (reflecting the y coordinates) to construct
// the right side.
//
// =============================================================================

#include <algorithm>
#include "chrono_vehicle/wheeled_vehicle/suspension/ChDoubleWishbone.h"

namespace chrono {
namespace vehicle {

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------
const std::string ChDoubleWishbone::m_pointNames[] = {
    "SPINDLE ", "UPRIGHT ", "UCA_F   ", "UCA_B   ", "UCA_U   ", "UCA_CM  ", "LCA_F   ", "LCA_B   ",
    "LCA_U   ", "LCA_CM  ", "SHOCK_C ", "SHOCK_A ", "SPRING_C", "SPRING_A", "TIEROD_C", "TIEROD_U"};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
ChDoubleWishbone::ChDoubleWishbone(const std::string& name, bool vehicle_frame_inertia)
    : ChSuspension(name), m_vehicle_frame_inertia(vehicle_frame_inertia) {}

ChDoubleWishbone::~ChDoubleWishbone() {
    auto sys = m_upright[0]->GetSystem();
    if (sys) {
        for (int i = 0; i < 2; i++) {
            sys->Remove(m_upright[i]);
            sys->Remove(m_UCA[i]);
            sys->Remove(m_LCA[i]);

            ChChassis::RemoveJoint(m_revoluteUCA[i]);
            ChChassis::RemoveJoint(m_sphericalUCA[i]);
            ChChassis::RemoveJoint(m_revoluteLCA[i]);
            ChChassis::RemoveJoint(m_sphericalLCA[i]);

            if (m_tierod[i]) {
                sys->Remove(m_tierod[i]);
                ChChassis::RemoveJoint(m_sphericalTierod[i]);
                ChChassis::RemoveJoint(m_universalTierod[i]);
            }
            if (m_distTierod[i]) {
                sys->Remove(m_distTierod[i]);
            }

            sys->Remove(m_shock[i]);
            sys->Remove(m_spring[i]);
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void ChDoubleWishbone::Initialize(std::shared_ptr<ChChassis> chassis,
                                  std::shared_ptr<ChSubchassis> subchassis,
                                  std::shared_ptr<ChSteering> steering,
                                  const ChVector<>& location,
                                  double left_ang_vel,
                                  double right_ang_vel) {
    m_location = location;

    // Express the suspension reference frame in the absolute coordinate system.
    ChFrame<> suspension_to_abs(location);
    suspension_to_abs.ConcatenatePreTransformation(chassis->GetBody()->GetFrame_REF_to_abs());

    // Transform all hardpoints to absolute frame.
    m_pointsL.resize(NUM_POINTS);
    m_pointsR.resize(NUM_POINTS);
    for (int i = 0; i < NUM_POINTS; i++) {
        ChVector<> rel_pos = getLocation(static_cast<PointId>(i));
        m_pointsL[i] = suspension_to_abs.TransformLocalToParent(rel_pos);
        rel_pos.y() = -rel_pos.y();
        m_pointsR[i] = suspension_to_abs.TransformLocalToParent(rel_pos);
    }

    // Initialize left and right sides.
    std::shared_ptr<ChBody> tierod_body = (steering == nullptr) ? chassis->GetBody() : steering->GetSteeringLink();
    InitializeSide(LEFT, chassis, tierod_body, m_pointsL, left_ang_vel);
    InitializeSide(RIGHT, chassis, tierod_body, m_pointsR, right_ang_vel);
}

void ChDoubleWishbone::InitializeSide(VehicleSide side,
                                      std::shared_ptr<ChChassis> chassis,
                                      std::shared_ptr<ChBody> tierod_body,
                                      const std::vector<ChVector<>>& points,
                                      double ang_vel) {
    std::string suffix = (side == LEFT) ? "_L" : "_R";

    // Chassis orientation (expressed in absolute frame)
    // Recall that the suspension reference frame is aligned with the chassis.
    ChQuaternion<> chassisRot = chassis->GetBody()->GetFrame_REF_to_abs().GetRot();

    // Unit vectors for orientation matrices.
    ChVector<> u;
    ChVector<> v;
    ChVector<> w;
    ChMatrix33<> rot;

    // Create and initialize spindle body (same orientation as the chassis)
    m_spindle[side] = std::shared_ptr<ChBody>(chassis->GetSystem()->NewBody());
    m_spindle[side]->SetNameString(m_name + "_spindle" + suffix);
    m_spindle[side]->SetPos(points[SPINDLE]);
    m_spindle[side]->SetRot(chassisRot);
    m_spindle[side]->SetWvel_loc(ChVector<>(0, ang_vel, 0));
    m_spindle[side]->SetMass(getSpindleMass());
    m_spindle[side]->SetInertiaXX(getSpindleInertia());
    chassis->GetSystem()->AddBody(m_spindle[side]);

    // Create and initialize upright body (same orientation as the chassis)
    m_upright[side] = std::shared_ptr<ChBody>(chassis->GetSystem()->NewBody());
    m_upright[side]->SetNameString(m_name + "_upright" + suffix);
    m_upright[side]->SetPos(points[UPRIGHT]);
    m_upright[side]->SetRot(chassisRot);
    m_upright[side]->SetMass(getUprightMass());
    if (m_vehicle_frame_inertia) {
        ChMatrix33<> inertia =
            TransformInertiaMatrix(getUprightInertiaMoments(), getUprightInertiaProducts(), chassisRot, chassisRot);
        m_upright[side]->SetInertia(inertia);
    } else {
        m_upright[side]->SetInertiaXX(getUprightInertiaMoments());
        m_upright[side]->SetInertiaXY(getUprightInertiaProducts());
    }
    chassis->GetSystem()->AddBody(m_upright[side]);

    // Create and initialize Upper Control Arm body.
    // Determine the rotation matrix of the UCA based on the plane of the hard points
    // (z axis normal to the plane of the UCA)
    w = Vcross(points[UCA_B] - points[UCA_U], points[UCA_F] - points[UCA_U]);
    w.Normalize();
    u = points[UCA_F] - points[UCA_B];
    u.Normalize();
    v = Vcross(w, u);
    rot.Set_A_axis(u, v, w);

    m_UCA[side] = std::shared_ptr<ChBody>(chassis->GetSystem()->NewBody());
    m_UCA[side]->SetNameString(m_name + "_UCA" + suffix);
    m_UCA[side]->SetPos(points[UCA_CM]);
    m_UCA[side]->SetRot(rot);
    m_UCA[side]->SetMass(getUCAMass());
    if (m_vehicle_frame_inertia) {
        ChMatrix33<> inertia = TransformInertiaMatrix(getUCAInertiaMoments(), getUCAInertiaProducts(), chassisRot, rot);
        m_UCA[side]->SetInertia(inertia);
    } else {
        m_UCA[side]->SetInertiaXX(getUCAInertiaMoments());
        m_UCA[side]->SetInertiaXY(getUCAInertiaProducts());
    }
    chassis->GetSystem()->AddBody(m_UCA[side]);

    // Create and initialize Lower Control Arm body.
    // Determine the rotation matrix of the LCA, based on the plane of the hard points
    // (z axis normal to the plane of the LCA)
    w = Vcross(points[LCA_B] - points[LCA_U], points[LCA_F] - points[LCA_U]);
    w.Normalize();
    u = points[LCA_F] - points[LCA_B];
    u.Normalize();
    v = Vcross(w, u);
    rot.Set_A_axis(u, v, w);

    m_LCA[side] = std::shared_ptr<ChBody>(chassis->GetSystem()->NewBody());
    m_LCA[side]->SetNameString(m_name + "_LCA" + suffix);
    m_LCA[side]->SetPos(points[LCA_CM]);
    m_LCA[side]->SetRot(rot);
    m_LCA[side]->SetMass(getLCAMass());
    if (m_vehicle_frame_inertia) {
        ChMatrix33<> inertia = TransformInertiaMatrix(getLCAInertiaMoments(), getLCAInertiaProducts(), chassisRot, rot);
        m_LCA[side]->SetInertia(inertia);
    } else {
        m_LCA[side]->SetInertiaXX(getLCAInertiaMoments());
        m_LCA[side]->SetInertiaXY(getLCAInertiaProducts());
    }
    chassis->GetSystem()->AddBody(m_LCA[side]);

    // Create and initialize the revolute joint between upright and spindle.
    ChCoordsys<> rev_csys(points[SPINDLE], chassisRot * Q_from_AngAxis(CH_C_PI / 2.0, VECT_X));
    m_revolute[side] = chrono_types::make_shared<ChLinkLockRevolute>();
    m_revolute[side]->SetNameString(m_name + "_revolute" + suffix);
    m_revolute[side]->Initialize(m_spindle[side], m_upright[side], rev_csys);
    chassis->GetSystem()->AddLink(m_revolute[side]);

    // Create and initialize the revolute joint between chassis and UCA.
    // Determine the joint orientation matrix from the hardpoint locations by
    // constructing a rotation matrix with the z axis along the joint direction
    // and the y axis normal to the plane of the UCA.
    v = Vcross(points[UCA_B] - points[UCA_U], points[UCA_F] - points[UCA_U]);
    v.Normalize();
    w = points[UCA_F] - points[UCA_B];
    w.Normalize();
    u = Vcross(v, w);
    rot.Set_A_axis(u, v, w);

    m_revoluteUCA[side] = chrono_types::make_shared<ChVehicleJoint>(
        ChVehicleJoint::Type::REVOLUTE, m_name + "_revoluteUCA" + suffix, chassis->GetBody(), m_UCA[side],
        ChCoordsys<>((points[UCA_F] + points[UCA_B]) / 2, rot.Get_A_quaternion()), getUCABushingData());
    chassis->AddJoint(m_revoluteUCA[side]);

    // Create and initialize the spherical joint between upright and UCA.
    m_sphericalUCA[side] =
        chrono_types::make_shared<ChVehicleJoint>(ChVehicleJoint::Type::SPHERICAL, m_name + "_sphericalUCA" + suffix,
                                                  m_UCA[side], m_upright[side], ChCoordsys<>(points[UCA_U], QUNIT));
    chassis->AddJoint(m_sphericalUCA[side]);

    // Create and initialize the revolute joint between chassis and LCA.
    // Determine the joint orientation matrix from the hardpoint locations by
    // constructing a rotation matrix with the z axis along the joint direction
    // and the y axis normal to the plane of the LCA.
    v = Vcross(points[LCA_B] - points[LCA_U], points[LCA_F] - points[LCA_U]);
    v.Normalize();
    w = points[LCA_F] - points[LCA_B];
    w.Normalize();
    u = Vcross(v, w);
    rot.Set_A_axis(u, v, w);

    m_revoluteLCA[side] = chrono_types::make_shared<ChVehicleJoint>(
        ChVehicleJoint::Type::REVOLUTE, m_name + "_revoluteLCA" + suffix, chassis->GetBody(), m_LCA[side],
        ChCoordsys<>((points[LCA_F] + points[LCA_B]) / 2, rot.Get_A_quaternion()), getLCABushingData());
    chassis->AddJoint(m_revoluteLCA[side]);

    // Create and initialize the spherical joint between upright and LCA.
    m_sphericalLCA[side] =
        chrono_types::make_shared<ChVehicleJoint>(ChVehicleJoint::Type::SPHERICAL, m_name + "_sphericalLCA" + suffix,
                                                  m_LCA[side], m_upright[side], ChCoordsys<>(points[LCA_U], QUNIT));
    chassis->AddJoint(m_sphericalLCA[side]);

    if (UseTierodBodies()) {
        // Orientation of tierod body
        w = (points[TIEROD_U] - points[TIEROD_C]).GetNormalized();
        u = chassisRot.GetXaxis();
        v = Vcross(w, u).GetNormalized();
        u = Vcross(v, w);
        rot.Set_A_axis(u, v, w);

        // Create the tierod body
        m_tierod[side] = std::shared_ptr<ChBody>(chassis->GetBody()->GetSystem()->NewBody());
        m_tierod[side]->SetNameString(m_name + "_tierodBody" + suffix);
        m_tierod[side]->SetPos((points[TIEROD_U] + points[TIEROD_C]) / 2);
        m_tierod[side]->SetRot(rot.Get_A_quaternion());
        m_tierod[side]->SetMass(getTierodMass());
        m_tierod[side]->SetInertiaXX(getTierodInertia());
        chassis->GetBody()->GetSystem()->AddBody(m_tierod[side]);

        // Connect tierod body to upright (spherical) and chassis (universal)
        m_sphericalTierod[side] = chrono_types::make_shared<ChVehicleJoint>(
            ChVehicleJoint::Type::SPHERICAL, m_name + "_sphericalTierod" + suffix, m_upright[side], m_tierod[side],
            ChCoordsys<>(points[TIEROD_U], QUNIT), getTierodBushingData());
        chassis->AddJoint(m_sphericalTierod[side]);
        m_universalTierod[side] = chrono_types::make_shared<ChVehicleJoint>(
            ChVehicleJoint::Type::UNIVERSAL, m_name + "_universalTierod" + suffix, tierod_body, m_tierod[side],
            ChCoordsys<>(points[TIEROD_C], rot.Get_A_quaternion()), getTierodBushingData());
        chassis->AddJoint(m_universalTierod[side]);
    } else {
        // Create and initialize the tierod distance constraint between chassis and upright.
        m_distTierod[side] = chrono_types::make_shared<ChLinkDistance>();
        m_distTierod[side]->SetNameString(m_name + "_distTierod" + suffix);
        m_distTierod[side]->Initialize(tierod_body, m_upright[side], false, points[TIEROD_C], points[TIEROD_U]);
        chassis->GetSystem()->AddLink(m_distTierod[side]);
    }

    // Create and initialize the spring/damper
    m_shock[side] = chrono_types::make_shared<ChLinkTSDA>();
    m_shock[side]->SetNameString(m_name + "_shock" + suffix);
    m_shock[side]->Initialize(chassis->GetBody(), m_LCA[side], false, points[SHOCK_C], points[SHOCK_A]);
    m_shock[side]->RegisterForceFunctor(getShockForceFunctor());
    chassis->GetSystem()->AddLink(m_shock[side]);

    m_spring[side] = chrono_types::make_shared<ChLinkTSDA>();
    m_spring[side]->SetNameString(m_name + "_spring" + suffix);
    m_spring[side]->Initialize(chassis->GetBody(), m_LCA[side], false, points[SPRING_C], points[SPRING_A], false,
                               getSpringRestLength());
    m_spring[side]->RegisterForceFunctor(getSpringForceFunctor());
    chassis->GetSystem()->AddLink(m_spring[side]);

    // Create and initialize the axle shaft and its connection to the spindle. Note that the
    // spindle rotates about the Y axis.
    m_axle[side] = chrono_types::make_shared<ChShaft>();
    m_axle[side]->SetNameString(m_name + "_axle" + suffix);
    m_axle[side]->SetInertia(getAxleInertia());
    m_axle[side]->SetPos_dt(-ang_vel);
    chassis->GetSystem()->Add(m_axle[side]);

    m_axle_to_spindle[side] = chrono_types::make_shared<ChShaftsBody>();
    m_axle_to_spindle[side]->SetNameString(m_name + "_axle_to_spindle" + suffix);
    m_axle_to_spindle[side]->Initialize(m_axle[side], m_spindle[side], ChVector<>(0, -1, 0));
    chassis->GetSystem()->Add(m_axle_to_spindle[side]);
}

// -----------------------------------------------------------------------------
// Get the total mass of the suspension subsystem.
// -----------------------------------------------------------------------------
double ChDoubleWishbone::GetMass() const {
    return 2 * (getSpindleMass() + getUCAMass() + getLCAMass() + getUprightMass());
}

// -----------------------------------------------------------------------------
// Get the current COM location of the suspension subsystem.
// -----------------------------------------------------------------------------
ChVector<> ChDoubleWishbone::GetCOMPos() const {
    ChVector<> com(0, 0, 0);

    com += getSpindleMass() * m_spindle[LEFT]->GetPos();
    com += getSpindleMass() * m_spindle[RIGHT]->GetPos();

    com += getUCAMass() * m_UCA[LEFT]->GetPos();
    com += getUCAMass() * m_UCA[RIGHT]->GetPos();

    com += getLCAMass() * m_LCA[LEFT]->GetPos();
    com += getLCAMass() * m_LCA[RIGHT]->GetPos();

    com += getUprightMass() * m_upright[LEFT]->GetPos();
    com += getUprightMass() * m_upright[RIGHT]->GetPos();

    return com / GetMass();
}

// -----------------------------------------------------------------------------
// Get the wheel track using the spindle local position.
// -----------------------------------------------------------------------------
double ChDoubleWishbone::GetTrack() {
    return 2 * getLocation(SPINDLE).y();
}

// -----------------------------------------------------------------------------
// Return current suspension forces
// -----------------------------------------------------------------------------
ChSuspension::Force ChDoubleWishbone::ReportSuspensionForce(VehicleSide side) const {
    ChSuspension::Force force;

    force.spring_force = m_spring[side]->GetForce();
    force.spring_length = m_spring[side]->GetLength();
    force.spring_velocity = m_spring[side]->GetVelocity();

    force.shock_force = m_shock[side]->GetForce();
    force.shock_length = m_shock[side]->GetLength();
    force.shock_velocity = m_shock[side]->GetVelocity();

    return force;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void ChDoubleWishbone::LogHardpointLocations(const ChVector<>& ref, bool inches) {
    double unit = inches ? 1 / 0.0254 : 1.0;

    for (int i = 0; i < NUM_POINTS; i++) {
        ChVector<> pos = ref + unit * getLocation(static_cast<PointId>(i));

        GetLog() << "   " << m_pointNames[i].c_str() << "  " << pos.x() << "  " << pos.y() << "  " << pos.z() << "\n";
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void ChDoubleWishbone::LogConstraintViolations(VehicleSide side) {
    // Revolute joints
    {
        const auto& C = m_revoluteLCA[side]->GetConstraintViolation();
        GetLog() << "LCA revolute          ";
        GetLog() << "  " << C(0) << "  ";
        GetLog() << "  " << C(1) << "  ";
        GetLog() << "  " << C(2) << "  ";
        GetLog() << "  " << C(3) << "  ";
        GetLog() << "  " << C(4) << "\n";
    }
    {
        const auto& C = m_revoluteUCA[side]->GetConstraintViolation();
        GetLog() << "UCA revolute          ";
        GetLog() << "  " << C(0) << "  ";
        GetLog() << "  " << C(1) << "  ";
        GetLog() << "  " << C(2) << "  ";
        GetLog() << "  " << C(3) << "  ";
        GetLog() << "  " << C(4) << "\n";
    }
    {
        const auto& C = m_revolute[side]->GetConstraintViolation();
        GetLog() << "Spindle revolute      ";
        GetLog() << "  " << C(0) << "  ";
        GetLog() << "  " << C(1) << "  ";
        GetLog() << "  " << C(2) << "  ";
        GetLog() << "  " << C(3) << "  ";
        GetLog() << "  " << C(4) << "\n";
    }

    // Spherical joints
    {
        const auto& C = m_sphericalLCA[side]->GetConstraintViolation();
        GetLog() << "LCA spherical         ";
        GetLog() << "  " << C(0) << "  ";
        GetLog() << "  " << C(1) << "  ";
        GetLog() << "  " << C(2) << "\n";
    }
    {
        const auto& C = m_sphericalUCA[side]->GetConstraintViolation();
        GetLog() << "UCA spherical         ";
        GetLog() << "  " << C(0) << "  ";
        GetLog() << "  " << C(1) << "  ";
        GetLog() << "  " << C(2) << "\n";
    }

    // Tierod constraint
    if (UseTierodBodies()) {
        {
            const auto& C = m_sphericalTierod[side]->GetConstraintViolation();
            GetLog() << "Tierod spherical      ";
            GetLog() << "  " << C(0) << "  ";
            GetLog() << "  " << C(1) << "  ";
            GetLog() << "  " << C(2) << "\n";
        }
        {
            const auto& C = m_universalTierod[side]->GetConstraintViolation();
            GetLog() << "Tierod universal      ";
            GetLog() << "  " << C(0) << "  ";
            GetLog() << "  " << C(1) << "  ";
            GetLog() << "  " << C(2) << "\n";
            GetLog() << "  " << C(3) << "\n";
        }
    } else {
        GetLog() << "Tierod distance       ";
        GetLog() << "  " << m_distTierod[side]->GetCurrentDistance() - m_distTierod[side]->GetImposedDistance() << "\n";
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChDoubleWishbone::Output(ChVehicleOutput& database) const {
    if (!m_output)
        return;

    std::vector<std::shared_ptr<ChBody>> bodies;
    bodies.push_back(m_spindle[0]);
    bodies.push_back(m_spindle[1]);
    bodies.push_back(m_upright[0]);
    bodies.push_back(m_upright[1]);
    bodies.push_back(m_UCA[0]);
    bodies.push_back(m_UCA[1]);
    bodies.push_back(m_LCA[0]);
    bodies.push_back(m_LCA[1]);
    if (UseTierodBodies()) {
        bodies.push_back(m_tierod[0]);
        bodies.push_back(m_tierod[1]);
    }
    database.WriteBodies(bodies);

    std::vector<std::shared_ptr<ChShaft>> shafts;
    shafts.push_back(m_axle[0]);
    shafts.push_back(m_axle[1]);
    database.WriteShafts(shafts);

    std::vector<std::shared_ptr<ChLink>> joints;
    std::vector<std::shared_ptr<ChLoadBodyBody>> bushings;
    joints.push_back(m_revolute[0]);
    joints.push_back(m_revolute[1]);
    m_revoluteUCA[0]->IsKinematic() ? joints.push_back(m_revoluteUCA[0]->GetAsLink())
                                    : bushings.push_back(m_revoluteUCA[0]->GetAsBushing());
    m_revoluteUCA[1]->IsKinematic() ? joints.push_back(m_revoluteUCA[1]->GetAsLink())
                                    : bushings.push_back(m_revoluteUCA[1]->GetAsBushing());
    m_sphericalUCA[0]->IsKinematic() ? joints.push_back(m_sphericalUCA[0]->GetAsLink())
                                     : bushings.push_back(m_sphericalUCA[0]->GetAsBushing());
    m_sphericalUCA[1]->IsKinematic() ? joints.push_back(m_sphericalUCA[1]->GetAsLink())
                                     : bushings.push_back(m_sphericalUCA[1]->GetAsBushing());
    m_revoluteLCA[0]->IsKinematic() ? joints.push_back(m_revoluteLCA[0]->GetAsLink())
                                    : bushings.push_back(m_revoluteLCA[0]->GetAsBushing());
    m_revoluteLCA[1]->IsKinematic() ? joints.push_back(m_revoluteLCA[1]->GetAsLink())
                                    : bushings.push_back(m_revoluteUCA[1]->GetAsBushing());
    m_sphericalLCA[0]->IsKinematic() ? joints.push_back(m_sphericalLCA[0]->GetAsLink())
                                     : bushings.push_back(m_sphericalLCA[0]->GetAsBushing());
    m_sphericalLCA[1]->IsKinematic() ? joints.push_back(m_sphericalLCA[1]->GetAsLink())
                                     : bushings.push_back(m_sphericalLCA[1]->GetAsBushing());
    if (UseTierodBodies()) {
        m_sphericalTierod[0]->IsKinematic() ? joints.push_back(m_sphericalTierod[0]->GetAsLink())
                                            : bushings.push_back(m_sphericalTierod[0]->GetAsBushing());
        m_sphericalTierod[1]->IsKinematic() ? joints.push_back(m_sphericalTierod[1]->GetAsLink())
                                            : bushings.push_back(m_sphericalTierod[1]->GetAsBushing());
        m_universalTierod[0]->IsKinematic() ? joints.push_back(m_universalTierod[0]->GetAsLink())
                                            : bushings.push_back(m_universalTierod[0]->GetAsBushing());
        m_universalTierod[1]->IsKinematic() ? joints.push_back(m_universalTierod[1]->GetAsLink())
                                            : bushings.push_back(m_universalTierod[1]->GetAsBushing());
    } else {
        joints.push_back(m_distTierod[0]);
        joints.push_back(m_distTierod[1]);
    }
    database.WriteJoints(joints);
    database.WriteBodyLoads(bushings);

    std::vector<std::shared_ptr<ChLinkTSDA>> springs;
    springs.push_back(m_spring[0]);
    springs.push_back(m_spring[1]);
    springs.push_back(m_shock[0]);
    springs.push_back(m_shock[1]);
    database.WriteLinSprings(springs);
}

}  // end namespace vehicle
}  // end namespace chrono
