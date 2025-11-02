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

#ifndef CHTIMESTEPPER_H
#define CHTIMESTEPPER_H

#include <cstdlib>
#include "chrono/core/ChApiCE.h"
#include "chrono/core/ChMath.h"
#include "chrono/serialization/ChArchive.h"
#include "chrono/timestepper/ChIntegrable.h"
#include "chrono/timestepper/ChState.h"

namespace chrono {

/// @addtogroup chrono_timestepper
/// @{

/// Base class for timesteppers, i.e., time integrators that can advance a system state.
/// It operates on systems inherited from ChIntegrable.
class ChApi ChTimestepper {
  public:
    /// Available methods for time integration (time steppers).
    enum class Type {
        EULER_IMPLICIT_LINEARIZED = 0,
        CUSTOM = 10
    };

    /// Constructor
    ChTimestepper(ChIntegrable* intgr = nullptr)
        : integrable(intgr), T(0), verbose(false), Qc_do_clamp(false), Qc_clamping(0) {}

    /// Destructor
    virtual ~ChTimestepper() {}

    /// Return type of the integration method.
    /// Default is CUSTOM. Derived classes should override this function.
    virtual Type GetType() const { return Type::CUSTOM; }

    /// Performs an integration timestep
    virtual void Advance(const double dt  ///< timestep to advance
                         ) = 0;

    /// Access the lagrangian multipliers, if any.
    virtual ChVectorDynamic<>& get_L() { return L; }

    /// Set the integrable object.
    virtual void SetIntegrable(ChIntegrable* intgr) { integrable = intgr; }

    /// Get the integrable object.
    ChIntegrable* GetIntegrable() { return integrable; }

    /// Get the current time.
    virtual double GetTime() const { return T; }

    /// Set the current time.
    virtual void SetTime(double mt) { T = mt; }

    /// Turn on/off logging of messages.
    void SetVerbose(bool verb) { verbose = verb; }

    /// Turn on/off clamping on the Qcterm.
    void SetQcDoClamp(bool dc) { Qc_do_clamp = dc; }

    /// Turn on/off clamping on the Qcterm.
    void SetQcClamping(double cl) { Qc_clamping = cl; }

  protected:
    ChIntegrable* integrable;
    double T;

    ChVectorDynamic<> L;

    bool verbose;

    bool Qc_do_clamp;
    double Qc_clamping;
};

/// Base class for 2nd order timesteppers, i.e., a time integrator for a ChIntegrableIIorder.
/// A ChIntegrableIIorder is a special subclass of integrable objects that have a state comprised
/// of position and velocity y={x,v}, and state derivative dy/dt={v,a}, where a=acceleration.
class ChApi ChTimestepperIIorder : public ChTimestepper {
  protected:
    ChState X;
    ChStateDelta V;
    ChStateDelta A;

  public:
    /// Constructor
    ChTimestepperIIorder(ChIntegrableIIorder* intgr = nullptr) : ChTimestepper(intgr) { SetIntegrable(intgr); }

    /// Destructor
    virtual ~ChTimestepperIIorder() {}

    /// Access the state, position part, at current time
    virtual ChState& get_X() { return X; }

    /// Access the state, speed part, at current time
    virtual ChStateDelta& get_V() { return V; }

    /// Access the acceleration, at current time
    virtual ChStateDelta& get_A() { return A; }

    /// Set the integrable object
    virtual void SetIntegrable(ChIntegrableIIorder* intgr) {
        ChTimestepper::SetIntegrable(intgr);
        X.setZero(1, intgr);
        V.setZero(1, intgr);
        A.setZero(1, intgr);
    }

  private:
    using ChTimestepper::SetIntegrable;
};

/// Base class for implicit solvers (double inheritance)
class ChApi ChImplicitTimestepper {};

/// Performs a step of Euler implicit for II order systems using the Anitescu/Stewart/Trinkle
/// single-iteration method, that is a bit like an implicit Euler where one performs only the
/// first Newton corrector iteration.
/// If using an underlying CCP complementarity solver, this is the typical Anitescu stabilized
/// timestepper for DVIs.
class ChApi ChTimestepperEulerImplicitLinearized : public ChTimestepperIIorder, public ChImplicitTimestepper {
  protected:
    ChStateDelta Vold;
    ChVectorDynamic<> Dl;
    ChVectorDynamic<> R;
    ChVectorDynamic<> Qc;

  public:
    /// Constructors (default empty)
    ChTimestepperEulerImplicitLinearized(ChIntegrableIIorder* intgr = nullptr)
        : ChTimestepperIIorder(intgr), ChImplicitTimestepper() {}

    virtual Type GetType() const override { return Type::EULER_IMPLICIT_LINEARIZED; }

    /// Performs an integration timestep
    virtual void Advance(const double dt  ///< timestep to advance
                         ) override;
};

/// @} chrono_timestepper

}  // end namespace chrono

#endif
