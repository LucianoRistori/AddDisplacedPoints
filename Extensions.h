/*------------------------------------------------------------------------------
 * File: Extensions.h
 *
 * Version 4.0 (candidate)
 * ------------------------
 * Defines ALL user-editable geometric configuration for AddDisplacedPoints.
 *
 * Contents:
 *   • Mathematical constants (PI, deg)
 *   • Small radial displacement (r) and diagonal offset (D)
 *   • Large radial displacement (R) and angles for BLUE/RED sets
 *   • BLUE/RED numeric ranges (which point IDs use which displacement set)
 *   • BLUE displacement list (extListBlue + numExtBlue)
 *   • RED displacement list  (extListRed  + numExtRed)
 *
 * This header provides ALL configuration and geometry used by the program.
 * AddDisplacedPoints.cpp contains the logic only.
 *
 * All values in this file are meant to be user-editable.
 *
 *------------------------------------------------------------------------------*/

#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <cmath>
#include <string>

/*------------------------------------------------------------------------------
 * Mathematical constants
 *------------------------------------------------------------------------------*/
constexpr double PI  = 3.14159265358979323846;
constexpr double deg = PI / 180.0;

/*------------------------------------------------------------------------------
 * Small radial displacement (mm)
 *------------------------------------------------------------------------------*/
constexpr double r = 2.0;

/*------------------------------------------------------------------------------
 * Diagonal offset for 45° displacements (mm)
 *   D = r / sqrt(2)
 *------------------------------------------------------------------------------*/
const double D = r / std::sqrt(2.0);

/*------------------------------------------------------------------------------
 * Large radial displacement (mm)
 *------------------------------------------------------------------------------*/
constexpr double R = 6.0;

/*------------------------------------------------------------------------------
 * Angles (in radians) used for large radial offsets
 *  a1 = -30°,  a2 = +90°,  a3 = -150°
 *------------------------------------------------------------------------------*/
constexpr double a1 = -30.0 * deg;
constexpr double a2 = +90.0 * deg;
constexpr double a3 = -150.0 * deg;

/*------------------------------------------------------------------------------
 * Range struct (integer inclusive)
 *------------------------------------------------------------------------------*/
struct Range {
    int lo;
    int hi;
};

/*------------------------------------------------------------------------------
 * BLUE numeric ranges
 *   If a point's label number falls within ANY of these ranges,
 *   AddDisplacedPoints uses extListBlue for that point.
 *------------------------------------------------------------------------------*/
const Range rangesBlue[] = {
    {  1,  8 },
    { 16, 21 },
    { 28, 32 },
    { 37, 39 },
    { 42, 42 }
};
const int numBlueRanges = sizeof(rangesBlue) / sizeof(rangesBlue[0]);

/*------------------------------------------------------------------------------
 * RED numeric ranges
 *   If a point's label number falls within ANY of these ranges,
 *   AddDisplacedPoints uses extListRed for that point.
 *------------------------------------------------------------------------------*/
const Range rangesRed[] = {
    {  9, 15 },
    { 22, 27 },
    { 33, 36 },
    { 40, 41 }
};
const int numRedRanges = sizeof(rangesRed) / sizeof(rangesRed[0]);

/*------------------------------------------------------------------------------
 * Displacement extension definition
 *   ext → string appended to original label
 *   dx, dy, dz → displacement offsets in mm
 *------------------------------------------------------------------------------*/
struct Extension {
    const char* ext;
    double dx;
    double dy;
    double dz;
};

/*------------------------------------------------------------------------------
 * BLUE displacement set
 *   First 4: 45° diagonals
 *   Next 3:  Large radial offsets (angles a1, a2, a3)
 *------------------------------------------------------------------------------*/
const Extension extListBlue[] = {

    // 45° diagonal offsets
    {"_1", +D, +D, 0.0},      // up-right
    {"_2", -D, +D, 0.0},      // up-left
    {"_3", -D, -D, 0.0},      // down-left
    {"_4", +D, -D, 0.0},      // down-right

    // Large radial offsets (BLUE)
    {"_5", R*std::cos(a1), R*std::sin(a1), -6.},
    {"_6", R*std::cos(a2), R*std::sin(a2), -6.},
    {"_7", R*std::cos(a3), R*std::sin(a3), -6.}
};
const int numExtBlue = sizeof(extListBlue) / sizeof(extListBlue[0]);

/*------------------------------------------------------------------------------
 * RED displacement set
 *   First 4: 45° diagonals (same as BLUE)
 *   Next 3:  Large radial offsets with Y reversed (mirror)
 *------------------------------------------------------------------------------*/
const Extension extListRed[] = {

    // 45° diagonal offsets (same as BLUE)
    {"_1", +D, +D, 0.0},
    {"_2", -D, +D, 0.0},
    {"_3", -D, -D, 0.0},
    {"_4", +D, -D, 0.0},

    // Large radial offsets (RED mirrors in Y)
    {"_5", R*std::cos(a1), -R*std::sin(a1), -6.},
    {"_6", R*std::cos(a2), -R*std::sin(a2), -6.},
    {"_7", R*std::cos(a3), -R*std::sin(a3), -6.}
};
const int numExtRed = sizeof(extListRed) / sizeof(extListRed[0]);

#endif // EXTENSIONS_H
