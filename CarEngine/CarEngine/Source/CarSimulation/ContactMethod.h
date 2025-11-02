#pragma once

namespace CE
{
    enum class ContactMethod
    {
        RIGID,  // rigid-body (a.k.a. non-smooth, constraint-based) contact
        SOFT   // soft-body (a.k.a. smooth, penalty-based) contact
    };
}