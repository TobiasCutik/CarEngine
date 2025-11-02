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
// Authors: Alessandro Tasora, Radu Serban
// =============================================================================

#include <cmath>

#include "chrono/timestepper/ChTimestepper.h"

namespace chrono {

// -----------------------------------------------------------------------------

// Trick to avoid putting the following mapper macro inside the class definition in .h file:
// enclose macros in local 'my_enum_mappers', just to avoid avoiding cluttering of the parent class.
class my_enum_mappers : public ChTimestepper {
  public:
    CH_ENUM_MAPPER_BEGIN(Type);
    CH_ENUM_VAL(Type::EULER_IMPLICIT_LINEARIZED);
    CH_ENUM_VAL(Type::CUSTOM);
    CH_ENUM_MAPPER_END(Type);
};

// -----------------------------------------------------------------------------

// Register into the object factory, to enable run-time dynamic creation and persistence
CH_FACTORY_REGISTER(ChTimestepperEulerImplicitLinearized)

// Performs a step of Euler implicit for II order systems
// using the Anitescu/Stewart/Trinkle single-iteration method,
// that is a bit like an implicit Euler where one performs only
// the first NR corrector iteration.
// If the solver in StateSolveCorrection is a CCP complementarity
// solver, this is the typical Anitescu stabilized timestepper for DVIs.
void ChTimestepperEulerImplicitLinearized::Advance(const double dt) {
    // downcast
    ChIntegrableIIorder* mintegrable = (ChIntegrableIIorder*)this->integrable;

    // setup main vectors
    mintegrable->StateSetup(X, V, A);

    // setup auxiliary vectors
    Dl.setZero(mintegrable->GetNconstr());
    R.setZero(mintegrable->GetNcoords_v());
    Qc.setZero(mintegrable->GetNconstr());
    L.setZero(mintegrable->GetNconstr());

    mintegrable->StateGather(X, V, T);  // state <- system

    mintegrable->StateGatherReactions(L);  // state <- system (may be needed for warm starting StateSolveCorrection)
    L *= dt;                               // because reactions = forces, here L = impulses

    Vold = V;

    // solve only 1st NR step, using v_new = 0, so  Dv = v_new , therefore
    //
    // [ M - dt*dF/dv - dt^2*dF/dx    Cq' ] [ Dv     ] = [ M*(v_old - v_new) + dt*f]
    // [ Cq                           0   ] [ -dt*Dl ] = [ -C/dt - Ct ]
    //
    // becomes the Anitescu/Trinkle timestepper:
    //
    // [ M - dt*dF/dv - dt^2*dF/dx    Cq' ] [ v_new  ] = [ M*(v_old) + dt*f]
    // [ Cq                           0   ] [ -dt*l  ] = [ -C/dt - Ct ]

    mintegrable->LoadResidual_F(R, dt);
    mintegrable->LoadResidual_Mv(R, V, 1.0);
    mintegrable->LoadConstraint_C(Qc, 1.0 / dt, Qc_do_clamp, Qc_clamping);
    mintegrable->LoadConstraint_Ct(Qc, 1.0);

    mintegrable->StateSolveCorrection(  //
        V, L, R, Qc,                    //
        1.0,                            // factor for  M
        -dt,                            // factor for  dF/dv
        -dt * dt,                       // factor for  dF/dx
        X, V, T + dt,                   // not needed
        false,                          // do not scatter update to Xnew Vnew T+dt before computing correction
        false,                          // full update? (not used, since no scatter)
        true                            // force a call to the solver's Setup() function
    );

    L *= (1.0 / dt);  // Note it is not -(1.0/dt) because we assume StateSolveCorrection already flips sign of Dl

    mintegrable->StateScatterAcceleration(
        (V - Vold) * (1 / dt));  // -> system auxiliary data (i.e acceleration as measure, fits DVI/MDI)

    X += V * dt;

    T += dt;

    mintegrable->StateScatter(X, V, T, true);  // state -> system
    mintegrable->StateScatterReactions(L);     // -> system auxiliary data
}

}  // end namespace chrono