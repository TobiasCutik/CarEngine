#pragma once

namespace CE
{
    enum class CollisionModel
    {
        // these may not ever work with my implementation, if so remove them
        // for now they should stay here for completeness (1:1 copy of chrono CollisionType enum)
        NONE,        // no contact shapes
        PRIMITIVES,  // contact model composed of primitives
        HULLS,       // contact model composed of convex hulls
        MESH         // contact model composed of trimeshes
    };

    enum class Tire
    {
        RIGID,       // rigid tire (cylindrical)
        FIALA,       // Fiala tire
        PACEJKA,     // Pacejka (magic formula) tire
        PAC02,       // Pacejka 02 (magic formula) tire, redesign of PACEJKA
        PAC89,       // Pacejka 89 (magic formula) tire
        TMEASY,      // Tire Model Made Easy tire (G. Rill)
        LUGRE,       // Lugre frition model tire
    };

    enum class Powertrain
    {
        SHAFTS,      // powertrain based on chrono ChShaft elements
        SIMPLE_MAP,  // simple powertrain model (based on engine-map)
        SIMPLE,      // simple powertrain model (similar to a DC motor)
        SIMPLE_CVT   // simple cvt powertrain model (like a DC motor / CVT gearbox)
    };

    enum class Driveline
    {
        FWD,    // front-wheel drive
        RWD,    // rear-wheel drive
        AWD,    // all-wheel drive
        SIMPLE  // simple kinematic driveline
    };

    enum class DriveMode
    {
        FORWARD,    // car drives forward when accelerating
        NEUTRAL,    // car can not drive forward or backward
        REVERSE     // car drives backward when accelerating
    };
}