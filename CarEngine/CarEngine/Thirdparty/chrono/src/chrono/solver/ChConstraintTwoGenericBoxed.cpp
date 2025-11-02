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

#include "chrono/solver/ChConstraintTwoGenericBoxed.h"

namespace chrono {

// Register into the object factory, to enable run-time dynamic creation and persistence
CH_FACTORY_REGISTER(ChConstraintTwoGenericBoxed)

ChConstraintTwoGenericBoxed::ChConstraintTwoGenericBoxed(ChVariables* mvariables_a, ChVariables* mvariables_b)
    : ChConstraintTwoGeneric(mvariables_a, mvariables_b), l_min(-1), l_max(1) {}

ChConstraintTwoGenericBoxed::ChConstraintTwoGenericBoxed(const ChConstraintTwoGenericBoxed& other)
    : ChConstraintTwoGeneric(other) {
    l_min = other.l_min;
    l_max = other.l_max;
}

double ChConstraintTwoGenericBoxed::Violation(double mc_i) {
    if ((l_i - 10e-5 < l_min) || (l_i + 10e-5 > l_max))
        return 0;
    return mc_i;
}

ChConstraintTwoGenericBoxed& ChConstraintTwoGenericBoxed::operator=(const ChConstraintTwoGenericBoxed& other) {
    if (&other == this)
        return *this;

    // copy parent class data
    ChConstraintTwoGeneric::operator=(other);

    l_min = other.l_min;
    l_max = other.l_max;

    return *this;
}

void ChConstraintTwoGenericBoxed::Project() {
    if (l_i < l_min)
        l_i = l_min;
    if (l_i > l_max)
        l_i = l_max;
}

void ChConstraintTwoGenericBoxed::SetBoxedMinMax(double mmin, double mmax) {
    assert(mmin <= mmax);
    l_min = mmin;
    l_max = mmax;
}

}  // end namespace chrono
