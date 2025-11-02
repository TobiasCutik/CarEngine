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
// Authors: Radu Serban, Alessandro Tasora
// =============================================================================
//
// Physical system in which contact is modeled using a smooth (penalty-based)
// method.
//
// =============================================================================

#include <limits>

#include "chrono/physics/ChSystemSMC.h"
#include "chrono/physics/ChContactContainerSMC.h"

#include "chrono/solver/ChSystemDescriptor.h"

#include "chrono/collision/ChCollisionSystemBullet.h"

namespace chrono {

// Register into the object factory, to enable run-time dynamic creation and persistence
CH_FACTORY_REGISTER(ChSystemSMC)

ChSystemSMC::ChSystemSMC(bool use_material_properties)
    : ChSystem(),
      m_use_mat_props(use_material_properties),
      m_contact_model(Hertz),
      m_adhesion_model(AdhesionForceModel::Constant),
      m_tdispl_model(OneStep),
      m_stiff_contact(false) {
    descriptor = chrono_types::make_shared<ChSystemDescriptor>();

    SetSolverType(ChSolver::Type::PSOR);

    collision_system = chrono_types::make_shared<collision::ChCollisionSystemBullet>();
    collision_system->SetNumThreads(nthreads_collision);
    collision_system->SetSystem(this);
    collision_system_type = collision::ChCollisionSystemType::BULLET;

    // For default SMC there is no need to create contacts 'in advance'
    // when models are closer than the safety envelope, so set default envelope to 0
    collision::ChCollisionModel::SetDefaultSuggestedEnvelope(0);

    contact_container = chrono_types::make_shared<ChContactContainerSMC>();
    contact_container->SetSystem(this);

    m_minSlipVelocity = 1e-4;
    m_characteristicVelocity = 1;
}

ChSystemSMC::ChSystemSMC(const ChSystemSMC& other) : ChSystem(other) {}

void ChSystemSMC::SetContactContainer(std::shared_ptr<ChContactContainer> container) {
    if (std::dynamic_pointer_cast<ChContactContainerSMC>(container))
        ChSystem::SetContactContainer(container);
}

void ChSystemSMC::SetSlipVelocityThreshold(double vel) {
    m_minSlipVelocity = std::max(vel, std::numeric_limits<double>::epsilon());
}

// STREAMING - FILE HANDLING

// Trick to avoid putting the following mapper macro inside the class definition in .h file:
// enclose macros in local 'my_enum_mappers', just to avoid avoiding cluttering of the parent class.
class my_enum_mappers : public ChSystemSMC {
  public:
    CH_ENUM_MAPPER_BEGIN(ContactForceModel);
    CH_ENUM_VAL(Hooke);
    CH_ENUM_VAL(Hertz);
    CH_ENUM_VAL(PlainCoulomb);
    CH_ENUM_VAL(Flores)
    CH_ENUM_MAPPER_END(ContactForceModel);

    CH_ENUM_MAPPER_BEGIN(AdhesionForceModel);
    CH_ENUM_VAL(AdhesionForceModel::Constant);
    CH_ENUM_VAL(AdhesionForceModel::DMT);
    CH_ENUM_VAL(AdhesionForceModel::Perko);
    CH_ENUM_MAPPER_END(AdhesionForceModel);

    CH_ENUM_MAPPER_BEGIN(TangentialDisplacementModel);
    CH_ENUM_VAL(None);
    CH_ENUM_VAL(OneStep);
    CH_ENUM_VAL(MultiStep);
    CH_ENUM_MAPPER_END(TangentialDisplacementModel);
};

}  // end namespace chrono
