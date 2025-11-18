# AddDisplacedPoints — Version 3.0

A geometry-based point expander with visualization.

This tool reads a CSV file containing points of the form:

    label, X, Y, Z

For each input point, the program:
1. Extracts the numeric part of its label (e.g., C12 → 12, P015 → 15)
2. Assigns the point to either the BLUE or RED displacement set, based on integer ranges
3. Writes the original point (unless --no-original is used)
4. Writes several displaced points, each with updated coordinates and label suffixes
5. Produces an interactive ROOT 2D (X,Y) scatter plot:
     - BLUE originals → blue markers (larger)
     - RED originals  → red markers  (larger)
     - All displaced  → black markers (smaller)
6. Saves:
     - AddDisplacedPoints.root — ROOT file with the canvas and graphs
     - AddDisplacedPoints.png  — exported image
     - <output>.csv            — expanded point list

---

## Version History

### v1.0
- Basic displacement of all points using a single displacement set.

### v2.0
- Added BLUE and RED displacement sets.
- Added label-number range selection logic.
- Added --no-original option.

### v3.0
- Added ROOT 2D (X,Y) plotting.
- Original points shown in color and double marker size.
- Displaced points shown in black.
- Added PNG image export.
- Full documentation and comments.

---

## Building

Requires ROOT (https://root.cern/).

Example installation on macOS:

    brew install root

To build:

    make clean
    make

This produces the executable:

    AddDisplacedPoints

---

## Running

    ./AddDisplacedPoints input.csv output.csv

Or, to suppress the original points in the CSV file:

    ./AddDisplacedPoints input.csv output.csv --no-original

The program always shows the originals in the ROOT plot, regardless of --no-original.

---

## Output Files

- output.csv             — expanded list of points
- AddDisplacedPoints.root — ROOT canvas + graphs
- AddDisplacedPoints.png  — exported plot image

---

## Editing Displacement Geometry

Displacement sets are in:

    Extensions.h

### Small radial offsets
Controlled by:

    const double r = 2.0;
    const double D = r * sqrt(2.0);

### Large radial offsets
Magnitude:

    const double R = 6.0;

Angles (degrees):

    a1 = -30°
    a2 = +90°
    a3 = -150°

BLUE set uses these angles directly.  
RED set mirrors the Y-component (sin → -sin).

---

## Editing BLUE/RED Ranges

Ranges are in AddDisplacedPoints.cpp:

    const Range rangesBlue[] = { {1,8}, {16,21}, ... };
    const Range rangesRed[]  = { {9,15}, {22,27}, ... };

A point is BLUE if its numeric label is in a BLUE range.  
Otherwise it is RED.

---

## Plot Customization

You can change in AddDisplacedPoints.cpp:

- Marker sizes  
- Colors  
- Canvas size  
- Axis labels  

---

## Notes

- The numeric part of the label is extracted from all digits.
  Example: ABC015Z9 → 159
- If no digits are found, the point defaults to RED.
- The program uses one canvas with three TGraphs.

---

## License

This code is part of private research and geometry work by Luciano Ristori.
