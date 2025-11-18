// ============================================================================
// File: AddDisplacedPoints.cpp
//
// Version: 3.0
// Author:  Luciano Ristori
//
// Description:
//   This program reads a list of 3D points from a CSV file, applies a set of
//   geometric displacements to each point, and writes the results to a new CSV
//   file.  Each input point consists of:
//
//       label, X, Y, Z
//
//   The program extracts the *numeric* portion of each label (e.g. “C12” → 12)
//   and uses this number to determine whether the point belongs to a
//   **BLUE** or **RED** displacement category.
//
//   The displacement geometry (small diagonals + 3 large radial offsets) is
//   defined in Extensions.h.  The integer ranges that map label numbers to
//   BLUE or RED sets are defined in this file.
//
//   OUTPUT FILE:
//       A CSV file containing:
//         - The original point      (unless --no-original is used)
//         - Several displaced points per original, with suffixes "_1", "_2", ...
//
//   PLOTTING:
//       Version 3 adds a ROOT 2D (X,Y) scatter plot with:
//         - BLUE original points → blue marker (larger size)
//         - RED  original points → red marker (larger size)
//         - All displaced points → black markers (smaller size)
//       This plot is:
//         (1) shown interactively on screen
//         (2) saved as AddDisplacedPoints.root
//         (3) saved as AddDisplacedPoints.png
//
//   NOTE:
//       Plotting is ALWAYS performed, even if --no-original is given.
//       The “no-original” option only affects the CSV file output.
//
// ----------------------------------------------------------------------------
//
// Usage:
//     AddDisplacedPoints input.csv output.csv
//     AddDisplacedPoints input.csv output.csv --no-original
//
// Example:
//     ./AddDisplacedPoints BoltCenters.csv BoltCenters_ext.csv
//
// Required files:
//     - Points.h / Points.cpp     (for reading the input CSV)
//     - Extensions.h              (defines displacement geometry)
//
// Dependencies:
//     Requires ROOT (https://root.cern/).
//
// ============================================================================

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>

#include "Points.h"
#include "Extensions.h"

// ROOT includes
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TFile.h"

// ============================================================================
// Extract numeric part from a label
//   Example: "C12" → 12, "P015" → 15, "ABC3" → 3
//   Returns -1 if no digits found.
// ============================================================================
int extractLabelNumber(const std::string& label) {
    std::string digits;
    for (char c : label) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            digits += c;
        }
    }
    if (digits.empty()) return -1;
    return std::stoi(digits);
}

// ============================================================================
// Simple inclusive integer range
// ============================================================================
struct Range { int lo; int hi; };

// ============================================================================
// BLUE label-number ranges  (editable)
// ============================================================================
const Range rangesBlue[] = {
    {  1,  8 },
    { 16, 21 },
    { 28, 32 },
    { 37, 39 },
    { 42, 42 }
};
const int numBlueRanges = sizeof(rangesBlue) / sizeof(rangesBlue[0]);

// ============================================================================
// RED label-number ranges  (editable)
// ============================================================================
const Range rangesRed[] = {
    {  9, 15 },
    { 22, 27 },
    { 33, 36 },
    { 40, 41 }
};
const int numRedRanges = sizeof(rangesRed) / sizeof(rangesRed[0]);

// ============================================================================
// Check whether an integer value lies inside any of a list of ranges
// ============================================================================
bool inAnyRange(const Range* ranges, int nRanges, int value) {
    for (int i = 0; i < nRanges; ++i) {
        if (value >= ranges[i].lo && value <= ranges[i].hi) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// Choose whether the point uses the BLUE or RED displacement set.
// If the label number falls into a BLUE range → BLUE
// If into a RED range → RED
// Otherwise fallback → RED
//
// Returns: pointer to the chosen Extension array
//          and sets `count` to the number of entries.
// ============================================================================
const Extension* chooseSet(int number, int& count) {
    if (number >= 0 && inAnyRange(rangesBlue, numBlueRanges, number)) {
        count = numExtBlue;
        return extListBlue;
    }

    if (number >= 0 && inAnyRange(rangesRed, numRedRanges, number)) {
        count = numExtRed;
        return extListRed;
    }

    // Fallback behavior (choose RED)
    count = numExtRed;
    return extListRed;
}

// ============================================================================
// MAIN
// ============================================================================
int main(int argc, char* argv[]) {

    bool writeOriginal = true;

    // Parse command-line arguments
    if (argc == 4) {
        if (std::string(argv[3]) == "--no-original") {
            writeOriginal = false;
        } else {
            std::cerr << "Unknown option: " << argv[3] << "\n"
                      << "Usage: " << argv[0]
                      << " inputFile outputFile [--no-original]\n";
            return 1;
        }
    } else if (argc != 3) {
        std::cerr << "Usage: " << argv[0]
                  << " inputFile outputFile [--no-original]\n";
        return 1;
    }

    const std::string inputFile  = argv[1];
    const std::string outputFile = argv[2];

    // Read input points
    std::vector<Point> points = readPoints(inputFile);

    // Open output CSV
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Error opening output file.\n";
        return 1;
    }

    out.setf(std::ios::fixed);
    out << std::setprecision(3);

    // -------------------------------------------------------------------------
    // Prepare vectors for ROOT plotting
    // -------------------------------------------------------------------------
    std::vector<double> xBlueOrig, yBlueOrig;
    std::vector<double> xRedOrig,  yRedOrig;
    std::vector<double> xDisp,     yDisp;

    // -------------------------------------------------------------------------
    // Process each input point
    // -------------------------------------------------------------------------
    for (const auto& p : points) {

        const double x = p.coords[0];
        const double y = p.coords[1];
        const double z = p.coords[2];

        // Determine BLUE or RED based on label number
        int number = extractLabelNumber(p.label);
        int nExt = 0;
        const Extension* extList = chooseSet(number, nExt);

        // Classify original point for plotting
        bool isBlue = (extList == extListBlue);

        if (isBlue) {
            xBlueOrig.push_back(x);
            yBlueOrig.push_back(y);
        } else {
            xRedOrig.push_back(x);
            yRedOrig.push_back(y);
        }

        // Write original point to CSV unless suppressed
        if (writeOriginal) {
            out << p.label << "," << x << "," << y << "," << z << "\n";
        }

        // Generate and write displaced points
        for (int i = 0; i < nExt; ++i) {
            const Extension& e = extList[i];

            double xNew = x + e.dx;
            double yNew = y + e.dy;
            double zNew = z + e.dz;

            // CSV output
            out << p.label << e.ext << ","
                << xNew << "," << yNew << "," << zNew << "\n";

            // For plotting (displaced points are black)
            xDisp.push_back(xNew);
            yDisp.push_back(yNew);
        }
    }

    out.close();
    std::cout << "Wrote " << outputFile << "\n";

    // -------------------------------------------------------------------------
    // ROOT plotting
    // -------------------------------------------------------------------------
    TApplication app("AddDisplacedPointsApp", &argc, argv);

    TCanvas* c1 = new TCanvas("c1", "AddDisplacedPoints XY Plot", 800, 800);

    TGraph* grDisp = nullptr;
    TGraph* grBlue = nullptr;
    TGraph* grRed  = nullptr;
    bool firstDrawn = false;   // The first graph defines axes

    // Displaced points (black, small)
    if (!xDisp.empty()) {
        grDisp = new TGraph(xDisp.size(), xDisp.data(), yDisp.data());
        grDisp->SetName("grDisplaced");
        grDisp->SetTitle("AddDisplacedPoints;X [mm];Y [mm]");
        grDisp->SetMarkerStyle(20);
        grDisp->SetMarkerSize(0.6);
        grDisp->SetMarkerColor(1);
        grDisp->Draw("AP");
        firstDrawn = true;
    }

    // Blue originals (blue, larger)
    if (!xBlueOrig.empty()) {
        grBlue = new TGraph(xBlueOrig.size(), xBlueOrig.data(), yBlueOrig.data());
        grBlue->SetName("grBlueOriginal");
        grBlue->SetMarkerStyle(20);
        grBlue->SetMarkerSize(1.2);
        grBlue->SetMarkerColor(4);
        grBlue->Draw(firstDrawn ? "P SAME" : "AP");
        firstDrawn = true;
    }

    // Red originals (red, larger)
    if (!xRedOrig.empty()) {
        grRed = new TGraph(xRedOrig.size(), xRedOrig.data(), yRedOrig.data());
        grRed->SetName("grRedOriginal");
        grRed->SetMarkerStyle(20);
        grRed->SetMarkerSize(1.2);
        grRed->SetMarkerColor(2);
        grRed->Draw(firstDrawn ? "P SAME" : "AP");
        firstDrawn = true;
    }

    c1->Update();

    // Save ROOT output
    TFile outFile("AddDisplacedPoints.root", "RECREATE");
    c1->Write("AddDisplacedPointsCanvas");
    if (grDisp) grDisp->Write();
    if (grBlue) grBlue->Write();
    if (grRed)  grRed->Write();
    outFile.Close();

    // Save PNG picture
    c1->SaveAs("AddDisplacedPoints.png");

    // Enter ROOT event loop
    app.Run();

    return 0;
}
