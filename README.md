# AddDisplacedPoints (v4.0)

AddDisplacedPoints is a C++/ROOT utility that reads a list of measured 3D points (X, Y, Z) and generates additional displaced copies of each point. These displaced points are placed at predefined offsets in the XY plane, and the program can optionally display a ROOT plot showing original points, displaced copies, and labels identifying each displacement.

This tool is useful for:
- simulating measurement perturbations,
- generating synthetic calibration grids,
- studying tolerance effects,
- creating visual overlays for flatness or scan-alignment studies.

All common geometric definitions are centralized in common/Extensions.h and common/Points.h, allowing multiple programs (FlatnessScan, CompareScan, OptimizePath, etc.) to share displacement conventions and point-handling logic.

------------------------------------------------------------
## Features
------------------------------------------------------------

Reads 3D coordinate files of the form:
    <label>  X  Y  Z

Generates displaced points using radii r and angles θ defined in Extensions.h:
    dx = r * cos(θ)
    dy = r * sin(θ)
resulting in:
    (X + dx, Y + dy, Z)

Centralized geometry and displacement configuration located only in:
    common/Extensions.h

Optional ROOT visualization of:
- original points,
- displaced points,
- text labels for each displaced copy.

Output file includes both original and displaced points.

------------------------------------------------------------
## File Structure
------------------------------------------------------------

    AddDisplacedPoints/
        AddDisplacedPoints.cpp
        Extensions.h
        common/
            Points.h
            Points.cpp

------------------------------------------------------------
## Building
------------------------------------------------------------

Build with:
    make

ROOT must be initialized before building:
    source ~/Dropbox/Documents/initRoot

------------------------------------------------------------
## Running
------------------------------------------------------------

Basic usage:
    ./AddDisplacedPoints inputPoints.txt outputPoints.txt

With visualization:
    ./AddDisplacedPoints input.txt output.txt --show

------------------------------------------------------------
## Displacement Definitions (Extensions.h)
------------------------------------------------------------

All displacement radii and angles are defined in Extensions.h. Example:

    radii  = {0.0, 5.0, 10.0}
    thetas = {0, π/2, π, 3π/2}

Displacement:
    dx = r * cos(θ)
    dy = r * sin(θ)

Changing displacement geometry requires editing only Extensions.h.

------------------------------------------------------------
## Output Format
------------------------------------------------------------

Output preserves the input format:
    <label>  X  Y  Z

Displaced points typically use suffixes like:
    A1_d0
    A1_d1
    A1_d2

------------------------------------------------------------
## Example
------------------------------------------------------------

Input:
    A1   100.0   200.0   3.0
    A2   110.0   205.0   2.7

If four displacements are defined, output will include:
    A1
    A1_d0
    A1_d1
    A1_d2
    A1_d3
    A2
    A2_d0
    A2_d1
    A2_d2
    A2_d3

------------------------------------------------------------
## Versioning
------------------------------------------------------------

v4.0
- All displacement logic moved to Extensions.h
- Bug fixes in displacement geometry
- Updated label drawing
- README rewritten

Create and push tag:
    git tag -a v4.0 -m "Stable v4.0 release"
    git push origin v4.0

------------------------------------------------------------
## Future Enhancements
------------------------------------------------------------

- More visualization modes
- Command-line-selectable displacement sets
- ROOT file / image exports

All geometry changes are made exclusively in Extensions.h.
