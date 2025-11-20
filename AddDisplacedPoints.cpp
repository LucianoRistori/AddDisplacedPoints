//------------------------------------------------------------------------------
// File: AddDisplacedPoints.cpp
//
// Version 4.0
// ------------------------
// Reads a CSV file containing labeled 3D points (label,X,Y,Z).
//
// For each input point:
//   • Writes the original point (unless --no-original is used)
//   • Selects a displacement set (BLUE or RED) based on the point number
//   • Writes all displaced points by appending extList[i].ext and applying dx,dy,dz
//
// Additionally:
//   • Produces a ROOT plot showing:
//        - Original BLUE points   (large blue markers)
//        - Original RED points    (large red markers)
//        - Displaced points       (small black markers)
//   • Draws the point number above (blue) or below (red) each original point
//   • Saves the plot to AddDisplacedPoints.png and AddDisplacedPoints.root
//
// Usage:
//      ./AddDisplacedPoints input.csv output.csv [--no-original]
//
//------------------------------------------------------------------------------

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
#include "TLatex.h"
#include "TStyle.h"
#include "TColor.h"
#include "TROOT.h"
#include "TFile.h"

//------------------------------------------------------------------------------
// Extract numeric part from the label:  "C12" → 12,  "P015" → 15
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
// Check if a number lies within ANY range in a list
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
// Choose BLUE or RED displacement set
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
    // Fallback: RED
    count = numExtRed;
    return extListRed;
}

//------------------------------------------------------------------------------
// MAIN
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    bool writeOriginal = true;

    if (argc == 4) {
        if (std::string(argv[3]) == "--no-original") {
            writeOriginal = false;
        } else {
            std::cerr << "Unknown option: " << argv[3] << "\n";
            return 1;
        }
    } else if (argc != 3) {
        std::cerr << "Usage: " << argv[0]
                  << " input.csv output.csv [--no-original]\n";
        return 1;
    }

    const std::string inputFile  = argv[1];
    const std::string outputFile = argv[2];

    // Read all points
    std::vector<Point> points = readPoints(inputFile);

    // Open output file
    std::ofstream out(outputFile);
    if (!out) {
        std::cerr << "Error opening output file " << outputFile << "\n";
        return 1;
    }

    out.setf(std::ios::fixed);
    out << std::setprecision(3);

    //--------------------------------------------------------------------------
    // TGraph containers for plotting
    //--------------------------------------------------------------------------
    std::vector<double> xb, yb;  // blue originals
    std::vector<double> xr, yr;  // red originals
    std::vector<double> xd, yd;  // displaced points

    //--------------------------------------------------------------------------
    // Process all points
    //--------------------------------------------------------------------------
    for (const auto& p : points) {

        double x = p.coords[0];
        double y = p.coords[1];
        double z = p.coords[2];

        int number = extractLabelNumber(p.label);

        int nExt = 0;
        const Extension* extList = chooseSet(number, nExt);

        bool isBlue = (number >= 0 && inAnyRange(rangesBlue, numBlueRanges, number));
        bool isRed  = (number >= 0 && inAnyRange(rangesRed,  numRedRanges,  number));

        // Save original to CSV
        if (writeOriginal) {
            out << p.label << "," << x << "," << y << "," << z << "\n";
        }

        // Save original to graph containers
        if (isBlue) {
            xb.push_back(x);
            yb.push_back(y);
        } else if (isRed) {
            xr.push_back(x);
            yr.push_back(y);
        }

        // Save displaced points
        for (int i = 0; i < nExt; ++i) {
            const Extension& e = extList[i];

            double xp = x + e.dx;
            double yp = y + e.dy;
            double zp = z + e.dz;

            out << p.label << e.ext << ","
                << xp << "," << yp << "," << zp << "\n";

            xd.push_back(xp);
            yd.push_back(yp);
        }
    }

    out.close();
    std::cout << "Wrote " << outputFile << "\n";

    //--------------------------------------------------------------------------
    // ROOT application
    //--------------------------------------------------------------------------
    int dummy = 0;
    TApplication app("app", &dummy, nullptr);

    TCanvas* c = new TCanvas("c", "AddDisplacedPoints", 900, 900);
    c->SetGrid();

    // All original points in one frame graph (for axes)
    std::vector<double> xa_all = xb;
    xa_all.insert(xa_all.end(), xr.begin(), xr.end());
    std::vector<double> ya_all = yb;
    ya_all.insert(ya_all.end(), yr.begin(), yr.end());

    TGraph* gAll = new TGraph(xa_all.size(), xa_all.data(), ya_all.data());
    gAll->SetMarkerSize(0); // hidden
    gAll->Draw("AP");       // draws axes

    // Blue originals
    TGraph* gBlue = new TGraph(xb.size(), xb.data(), yb.data());
    gBlue->SetMarkerColor(kBlue+1);
    gBlue->SetMarkerStyle(20);
    gBlue->SetMarkerSize(2.5);
    gBlue->Draw("P SAME");

    // Red originals
    TGraph* gRed = new TGraph(xr.size(), xr.data(), yr.data());
    gRed->SetMarkerColor(kRed+1);
    gRed->SetMarkerStyle(20);
    gRed->SetMarkerSize(2.5);
    gRed->Draw("P SAME");

    // Displaced points
    TGraph* gDis = new TGraph(xd.size(), xd.data(), yd.data());
    gDis->SetMarkerColor(kBlack);
    gDis->SetMarkerStyle(20);
    gDis->SetMarkerSize(0.8);
    gDis->Draw("P SAME");

    //--------------------------------------------------------------------------
    // Draw labels for original points
    //--------------------------------------------------------------------------
    for (size_t i = 0; i < points.size(); ++i) {

        int number = extractLabelNumber(points[i].label);
        if (number < 0) continue;

        double x = points[i].coords[0];
        double y = points[i].coords[1];

        bool isBlue = inAnyRange(rangesBlue, numBlueRanges, number);
        bool isRed  = inAnyRange(rangesRed,  numRedRanges,  number);

        double yLabel = y;
        int align = 21; // center horizontally

        if (isBlue) {
            yLabel = y + 30.0; // above
            align = 21;
        } else if (isRed) {
            yLabel = y - 30.0; // below
            align = 23;
        } else {
            continue;
        }

        TLatex* tl = new TLatex(x, yLabel, Form("%d", number));
        tl->SetTextColor(kBlack);
        tl->SetTextSize(0.015);
        tl->SetTextAlign(align);
        tl->Draw("SAME");
    }

    c->Modified();
    c->Update();

    // Save outputs
    c->Print("AddDisplacedPoints.png");
    TFile f("AddDisplacedPoints.root", "RECREATE");
    c->Write();
    f.Close();

    app.Run();
    return 0;
}
