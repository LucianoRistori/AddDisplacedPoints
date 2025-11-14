/*------------------------------------------------------------------------------
 * File: AddDisplacedPoints.cpp
 *
 * Description:
 *   This utility reads a file of 3D points (label, X, Y, Z) and generates
 *   a new output file in which each input point is expanded into one or more
 *   "displaced" points.  The displacements are specified at compile time in
 *   the file Extensions.h.
 *
 *   For every input point P = (label, X, Y, Z):
 *
 *        1) The program normally writes the original point:
 *
 *               label, X, Y, Z
 *
 *        2) For each displacement entry defined in Extensions.h:
 *
 *               { ext, dx, dy, dz }
 *
 *           a new point is written with:
 *
 *               newLabel = label + ext
 *               newX     = X + dx
 *               newY     = Y + dy
 *               newZ     = Z + dz
 *
 *           The output is always comma-separated (CSV) with exactly
 *           3 decimal digits of precision:
 *
 *               newLabel,newX,newY,newZ
 *
 *   Displacements are determined at compile time by editing Extensions.h.
 *   No ROOT libraries are used; this is a simple standalone C++17 program.
 *
 *------------------------------------------------------------------------------
 * Command-line usage:
 *
 *      AddDisplacedPoints  inputFile  outputFile  [--no-original]
 *
 *   Required arguments:
 *      inputFile   - Path to a text or CSV file containing points:
 *                        label X Y Z
 *                     or   label,X,Y,Z
 *
 *      outputFile  - Path to write the expanded output CSV file.
 *
 *   Optional argument:
 *      --no-original
 *          When present, the original input point is NOT copied to the
 *          output. Only the displaced points are written.
 *
 *   Examples:
 *
 *      # Default: include original point and all displaced points
 *      AddDisplacedPoints points.csv expanded.csv
 *
 *      # Write ONLY the displaced points (skip the originals)
 *      AddDisplacedPoints points.csv expanded.csv --no-original
 *
 *------------------------------------------------------------------------------
 * Input format:
 *      - Handled by Points.h / Points.cpp
 *      - Accepts space- or comma-separated values
 *      - First field is a label (string), followed by X, Y, Z in mm
 *
 * Output format:
 *      - Strict CSV (comma-separated, no spaces)
 *      - Fixed 3-digit precision for all numeric fields
 *      - One point per line
 *
 *------------------------------------------------------------------------------
 * Notes:
 *      - All displacements are defined in Extensions.h using the struct:
 *
 *              struct Extension {
 *                  const char* ext;   // string appended to label
 *                  double dx, dy, dz; // coordinate offsets
 *              };
 *
 *      - The list 'extList[]' and the constant 'numExtensions' determine
 *        how many displaced points are generated for each input point.
 *
 *      - The Makefile lists Extensions.h as a dependency so that any change
 *        to the displacement definitions triggers a rebuild.
 *
 *------------------------------------------------------------------------------
 */


#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>

#include "Points.h"
#include "Extensions.h"

int main(int argc, char* argv[]) {
    // --------------------------------------------------------------
    // Parse command-line arguments
    // --------------------------------------------------------------
    bool writeOriginal = true;

	if (argc == 4) {
		std::string opt = argv[3];
		if (opt == "--no-original") {
			writeOriginal = false;
		} else {
			std::cerr << "Unknown option: " << opt << "\n";
			return 1;
		}
	} else if (argc != 3) {
		std::cerr << "Usage: " << argv[0]
				  << " inputFile outputFile [--no-original]\n";
		return 1;
	}
    const std::string inputFile  = argv[1];
    const std::string outputFile = argv[2];

    // --------------------------------------------------------------
    // Read points using your common reader
    // --------------------------------------------------------------
    std::vector<Point> points = readPoints(inputFile);
    if (points.empty()) {
        std::cerr << "Warning: no points read from " << inputFile << "\n";
    }

    // --------------------------------------------------------------
    // Open output CSV file
    // --------------------------------------------------------------
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Error: cannot open output file " << outputFile << "\n";
        return 1;
    }

    // We want fixed with 3 decimal digits for all numeric fields
    out.setf(std::ios::fixed);
    out << std::setprecision(3);

    // --------------------------------------------------------------
    // Write original and displaced points
    // --------------------------------------------------------------
    for (const auto& p : points) {
        const double x = p.coords[0];
        const double y = p.coords[1];
        const double z = p.coords[2];

        // 1) Original point      
        if (writeOriginal) {
			out << p.label << ","
				<< x << ","
				<< y << ","
				<< z << "\n";
		}		
        // 2) Displaced points for each extension
        for (int i = 0; i < numExtensions; ++i) {
            const Extension& e = extList[i];

            std::string newLabel = p.label + e.ext;
            double xNew = x + e.dx;
            double yNew = y + e.dy;
            double zNew = z + e.dz;

            out << newLabel << ","
                << xNew << ","
                << yNew << ","
                << zNew << "\n";
        }
    }

    out.close();
    std::cout << "Wrote " << outputFile << " with ";
    	if(writeOriginal){
              std::cout << points.size()
              << " original points and ";
         }     
              std::cout << points.size() * numExtensions
              << " displaced points.\n";

    return 0;
}
