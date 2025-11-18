// File: AddDisplacedPoints.cpp
//
// Version 2: Blue/Red displacement sets selected per point by numeric label.
//
// Example label → number extraction:
//      "C12" → 12
//      "P015" → 15
//      "ABC3" → 3
//
// Range rules map the number to either BLUE or RED displacement sets.
//
// Usage:
//      AddDisplacedPoints input.csv output.csv [--no-original]
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>

#include "Points.h"
#include "Extensions.h"

//------------------------------------------------------------------------------
// Extract numeric part from label: "C12" → 12
//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Simple inclusive integer range
//------------------------------------------------------------------------------
struct Range { int lo; int hi; };

//------------------------------------------------------------------------------
// BLUE ranges (EDIT THESE)
//------------------------------------------------------------------------------
const Range rangesBlue[] = {
    {  1,  8 },
    { 16, 21 },
    { 28, 32 },
    { 37, 39},
    { 42, 42}
};
const int numBlueRanges = sizeof(rangesBlue) / sizeof(rangesBlue[0]);

//------------------------------------------------------------------------------
// RED ranges (EDIT THESE)
//------------------------------------------------------------------------------
const Range rangesRed[] = {
    { 9, 15 },
    { 22, 27},
    { 33, 36},
    { 40, 41}
};
const int numRedRanges = sizeof(rangesRed) / sizeof(rangesRed[0]);

//------------------------------------------------------------------------------
bool inAnyRange(const Range* ranges, int nRanges, int value) {
    for (int i = 0; i < nRanges; ++i) {
        if (value >= ranges[i].lo && value <= ranges[i].hi) {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
// Choose BLUE or RED depending on label number.
// If the number matches no range, choose RED (you may change fallback behavior).
//------------------------------------------------------------------------------
const Extension* chooseSet(int number, int& count) {
    if (number >= 0 && inAnyRange(rangesBlue, numBlueRanges, number)) {
        count = numExtBlue;
        return extListBlue;
    }

    if (number >= 0 && inAnyRange(rangesRed, numRedRanges, number)) {
        count = numExtRed;
        return extListRed;
    }

    // Fallback behavior:
    // Choose RED for labels outside the defined ranges.
    count = numExtRed;
    return extListRed;
}

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    bool writeOriginal = true;

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

    // Read points
    std::vector<Point> points = readPoints(inputFile);

    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cerr << "Error opening output file.\n";
        return 1;
    }

    out.setf(std::ios::fixed);
    out << std::setprecision(3);

    for (const auto& p : points) {

        const double x = p.coords[0];
        const double y = p.coords[1];
        const double z = p.coords[2];

        int number = extractLabelNumber(p.label);

        int nExt = 0;
        const Extension* extList = chooseSet(number, nExt);

        // Write original
        if (writeOriginal) {
            out << p.label << "," << x << "," << y << "," << z << "\n";
        }

        // Write displaced
        for (int i = 0; i < nExt; ++i) {
            const Extension& e = extList[i];
            out << p.label << e.ext << ","
                << (x + e.dx) << ","
                << (y + e.dy) << ","
                << (z + e.dz) << "\n";
        }
    }

    out.close();
    std::cout << "Wrote " << outputFile << "\n";
    return 0;
}
