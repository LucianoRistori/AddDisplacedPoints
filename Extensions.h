/*------------------------------------------------------------------------------
 * File: Extensions.h
 *
 * Description:
 *   Defines two displacement sets used by AddDisplacedPoints:
 *
 *      BLUE  (Blue set)
 *      RED   (Red set)
 *
 *   Each displacement is:
 *       { ext, dx, dy, dz }
 *
 *   where:
 *       ext  = string appended to the original label
 *       dx   dy   dz   = offsets in mm
 *
 *   Version 2 of AddDisplacedPoints chooses between BLUE and RED for each
 *   point based on the numeric part extracted from the point's label.
 *
 *   Edit these displacement sets freely to match your geometry.
 *
 *----------------------------------------------------------------------------*/

#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <cmath>
constexpr double PI = 3.14159265358979323846;


struct Extension {
    const char* ext;   // appended to label
    double dx;
    double dy;
    double dz;
};

// -----------------------------------------------------------------------------
// Parameters you can change
// -----------------------------------------------------------------------------
const double r = 2.0; // small radial offset (mm)                       
const double D = r * std::sqrt(2.0); // 45° diagonal offset (mm)

const double R = 6.0;	// large radial offset (mm)

const double deg = PI/180.;

const double a1 =  -30*deg; // angles for blue large radial extensions
const double a2 =  +90*deg;
const double a3 = -150*deg;


// -----------------------------------------------------------------------------
// BLUE displacement set (example: 4 diagonal offsets)
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Displacement set Blue
// -----------------------------------------------------------------------------
const Extension extListBlue[] = {
    {"_1", +D, +D, 0.000},  // up-right
    {"_2", -D, +D, 0.000},  // up-left
    {"_3", -D, -D, 0.000},  // down-left
    {"_4", +D, -D, 0.000},   // down-right   
    
    {"_5", R*cos(a1), R*sin(a1), 0.000},  // down-right
    {"_6", R*cos(a2), R*sin(a2), 0.000},  // up-center
    {"_7", R*cos(a3), R*sin(a3), 0.000}  // down-left
   
    
};
const int numExtBlue = static_cast<int>(sizeof(extListBlue) / sizeof(extListBlue[0]));

// -----------------------------------------------------------------------------
// Displacement set Red
// -----------------------------------------------------------------------------
const Extension extListRed[] = {
    {"_1", +D, +D, 0.000},  // up-right
    {"_2", -D, +D, 0.000},  // up-left
    {"_3", -D, -D, 0.000},  // down-left
    {"_4", +D, -D, 0.000},  // down-right
    
    // reverse Y for red points
          
    {"_5", R*cos(a1), -R*sin(a1), 0.000},  // down-right
    {"_6", R*cos(a2), -R*sin(a2), 0.000},  // up-center
    {"_7", R*cos(a3), -R*sin(a3), 0.000}  // down-left
  
};
const int numExtRed = static_cast<int>(sizeof(extListRed) / sizeof(extListRed[0]));

#endif // EXTENSIONS_H
