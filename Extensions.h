/*------------------------------------------------------------------------------
 * File: Extensions.h
 *
 * Description:
 *   This header defines the compile-time displacement pattern used by
 *   AddDisplacedPoints.cpp to generate additional points based on each
 *   input point (label, X, Y, Z).
 *
 *   Each displacement entry has the form:
 *
 *       { ext, dx, dy, dz }
 *
 *   where:
 *       ext  - string appended to the original label to form a new label
 *       dx   - X offset added to the original X coordinate
 *       dy   - Y offset added to the original Y coordinate
 *       dz   - Z offset added to the original Z coordinate
 *
 *   For an input point:
 *
 *       P = (label, X, Y, Z)
 *
 *   AddDisplacedPoints writes a new point:
 *
 *       (label + ext, X + dx, Y + dy, Z + dz)
 *
 *   The list of displacement entries is defined in the array `extList[]`
 *   and its length is given by `numExtensions`.  You may edit, reorder,
 *   remove, or add entries to `extList[]` as needed.
 *
 *   Constants such as R, D, or angular steps may be defined here as well.
 *   These values do not need to be constexpr; simple const variables are
 *   sufficient.
 *
 * Usage:
 *   - Edit the extList[] array to define the displacement pattern you need.
 *   - All numeric values are in millimeters unless otherwise noted.
 *   - The Makefile for AddDisplacedPoints includes Extensions.h as an
 *     explicit dependency, ensuring that any changes to this file will
 *     trigger a rebuild.
 *
 * Example entry:
 *
 *       constexpr Extension extList[] = {
 *           {".A", +1.000, +1.000, 0.000},
 *           {".B", -1.000, +1.000, -1.000},
 *       };
 *
 * Notes:
 *   - Changes to this file require recompilation of AddDisplacedPoints.cpp.
 *   - The struct `Extension` is defined below and must match expectations
 *     in AddDisplacedPoints.cpp.
 *
 *------------------------------------------------------------------------------
 */
	

#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <cmath>

struct Extension {
    const char* ext;
    double dx;
    double dy;
    double dz;
};

// --------------------------------------------------
// Constants (const, NOT constexpr because sqrt is NOT constexpr on macOS)
// --------------------------------------------------
const double R = 5.0;                   // mm
const double D = R / std::sqrt(2.0);    // mm projected at 45 degrees

// --------------------------------------------------
// Extension list
// --------------------------------------------------
//
//     2       1
//       \   /
//         *
//       /   \
//     3       4
//
const Extension extList[] = {
    {".1", +D, +D, 0.000},  // up right
    {".2", -D, +D, 0.000},  // up left
    {".3", -D, -D, 0.000},  // down left  
    {".4", +D, -D, 0.000}   // down right
};

const int numExtensions = sizeof(extList) / sizeof(extList[0]);

#endif
