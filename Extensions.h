/*==============================================================================
 * File: Extensions.h
 *
 * Version: 3.0
 * Author : Luciano Ristori
 *
 * Description:
 *   Defines the geometry of the BLUE and RED displacement sets used by
 *   AddDisplacedPoints.  Each displacement consists of:
 *
 *       ext   - string appended to the original label
 *       dx
 *       dy
 *       dz    - applied offsets in millimeters
 *
 *   Geometry:
 *     - Four small "diagonal" displacements at radius r, using dx=±D, dy=±D
 *       where D = r * sqrt(2).
 *
 *     - Three large radial displacements at radius R:
 *           angle a1 = -30°
 *           angle a2 = +90°
 *           angle a3 = -150°
 *
 *     - BLUE set uses these angles directly.
 *     - RED  set uses the same magnitudes, but reverses the sign of dy
 *       (mirror symmetry in Y).
 *
 *   The user may add/remove displacement entries or modify r, R, a1, a2, a3.
 *   All displacements are applied in AddDisplacedPoints.cpp after a BLUE/RED
 *   selection based on the numeric part of the label.
 *
 *============================================================================*/

#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <cmath>

constexpr double PI = 3.14159265358979323846;

// Each displacement entry
struct Extension {
    const char* ext;
    double dx;
    double dy;
    double dz;
};

// -----------------------------------------------------------------------------
// Parameters controlling the displacement geometry
// -----------------------------------------------------------------------------

// Small radial offset (mm)
const double r = 2.0;

// 45° diagonal displacement magnitude (mm)
const double D = r * std::sqrt(2.0);

// Large radial offset
const double R = 6.0;

// Degrees → radians conversion factor
const double deg = PI / 180.0;

// Angles for large radial BLUE displacements
const double a1 =  -30 * deg;
const double a2 =  +90 * deg;
const double a3 = -150 * deg;

// -----------------------------------------------------------------------------
// BLUE displacement set
// -----------------------------------------------------------------------------
const Extension extListBlue[] = {

    // Small diagonals
    {"_1", +D, +D, 0.000},
    {"_2", -D, +D, 0.000},
    {"_3", -D, -D, 0.000},
    {"_4", +D, -D, 0.000},

    // Large radial displacements
    {"_5", R*std::cos(a1), R*std::sin(a1), 0.000},
    {"_6", R*std::cos(a2), R*std::sin(a2), 0.000},
    {"_7", R*std::cos(a3), R*std::sin(a3), 0.000}
};

const int numExtBlue = sizeof(extListBlue) / sizeof(extListBlue[0]);

// -----------------------------------------------------------------------------
// RED displacement set
//   Same geometry as BLUE, but Y is mirrored (sin → -sin)
// -----------------------------------------------------------------------------
const Extension extListRed[] = {

    // Small diagonals
    {"_1", +D, +D, 0.000},
    {"_2", -D, +D, 0.000},
    {"_3", -D, -D, 0.000},
    {"_4", +D, -D, 0.000},

    // Large radial displacements (mirrored in Y)
    {"_5", R*std::cos(a1), -R*std::sin(a1), 0.000},
    {"_6", R*std::cos(a2), -R*std::sin(a2), 0.000},
    {"_7", R*std::cos(a3), -R*std::sin(a3), 0.000}
};

const int numExtRed = sizeof(extListRed) / sizeof(extListRed[0]);

#endif // EXTENSIONS_H
