//
//  Linker.cpp
//  cppKymotrace
//
//  Created by Alex Zwetsloot on 03/03/2018.
//  Copyright © 2018 Alex Zwetsloot. All rights reserved.
//

#include "Linker.hpp"
#include <list>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
using namespace cimg_library;


void Linker::Link(int lookForward, int max_dX, int max_dT, int max_dIntensity) {
    // Link detection. We have two images. "img" is the original, with intensities etc.
    // "detection" is the same image but peak fit and made into 1s and 0s.
    // We will use:
    //      lookForward: How many Ys to look forward before making your deicision whether you can link the spot at y(n) and y(n+1)
    //      max_dX: Maximum delta X you can change between y slices. (in pix)
    //      max_dT: Maximum gap you can link over if we haven't seen anything for the last few timepoints
    //      max_dIntensity: maximum intensity difference between y(n) and y(n+1)
    
    // Do the first line simply:
    for (int x = 0 ; x < img.width(); x++ ){
        int y = 0;
        if (detection(x,y) == 1) {
            Trace newPath = Trace(detection.height());
            newPath.x[0] = x;
            newPath.y[0] = y;
            newPath.intensity[0] = img(x,y);
            newPath.t[0] = 0;
            newPath.quality[0] = 100;
            newPath.points.emplace_back(Point(x,y));
            tracks.emplace_back(newPath);
        }
    }
    for (int y = 1; y < img.height(); y++) {
        // For each scanline, see who is the best Trace to accept the point.
        // Look forward a little bit and see if it is still good.
        for (int x = 0; x < img.width(); x++) {
            if (detection(x,y) == 1) {
                // Linking logic goes here.
                
                int maxScore = 0;
                int maxScoreLocation = 0;
                int fuckedupcount = 0;
                // First check if we have any viable options to extend current paths?
                for (int path = 0; path < tracks.size(); path++ ) {
                    std::vector<Point>& points = tracks[path].points;   // For convenience, copy the address of the points vector to a friendly name.
                    Point newPeak = Point(x,y); // The peak we are considering
                    Point lastPoint = points[points.size() - 1];
                    Point diff = newPeak - lastPoint;   // Location difference between two.
                    int* intensities = tracks[path].intensity;
                    int score = 0;
                    
                    // If diff.Y (i.e. same timepoint) and there are
                    // two very close peaks, then it's possibly just a thick line.
                    // This are forced into being linked (+1000 points) regardless of other factors.
                    if (diff.Y == 0 && diff.X <= 2) {
                        diff.Y = 1; // make the maths later not crash.
                        score += 1000;
                    } else if (diff.Y == 0 && diff.X > 2) {
                        continue;
                    }
                    // Most importantly, is it within a reasonable X and t of our last saved point?
                    if (abs(diff.X)/diff.Y <= max_dX && diff.Y <= max_dT) {
                        //printf("+100 for being within max_dX and max_dT of last point in trace");
                        score += 100;
                        score += max_dT - diff.Y;   // Prefer shorter hops in T
                    }
                    // How does the intensity difference look? This is not as important; score less.
                    int deltaIntensity = abs(int(img(x,y) - intensities[lastPoint.Y]));
                    if (deltaIntensity < max_dIntensity) {
                        float scf = (max_dIntensity - deltaIntensity)/float(max_dIntensity);
                        float add = scf*50;

                        score += int(add);     // Closer intensity = more points. Maximum score = 20;
                    }
                    
                    // Speed? Other factors?
                    
                    // Write the score to the tracks for us to inspect after considering everything.
                    tracks[path].currentScore = score;
                    if (score > maxScore) {
                        maxScore = score;
                        maxScoreLocation = path;
                    }
                    
                } // End of iterate over currently saved paths
                printf("I should tell you that I fucked up and found diff.Y = 0 %i times\n", fuckedupcount);
                // Ok, so have we got a worthwhile allocation?
                if (maxScore >= 100) {
                    tracks[maxScoreLocation].points.emplace_back(Point(x,y));
                    tracks[maxScoreLocation].intensity[y] = img(x,y);
                } else {
                    Trace newPath = Trace(img.height());
                    newPath.points.emplace_back(Point(x,y));
                    newPath.intensity[y] = img(x,y);
                    tracks.emplace_back(newPath);
                }

                
                // We can deal with filtering only long or short paths after the fact, we should concentrate
                // instead on making the paths properly.
                
            } //end of  detection(x,y) == 1
            
        } // end of all x
        
    } // end of all y
    
}

void Linker::Draw() {
    CImg<uint> output(img.width(), img.height(),1,3); // Make a colour image.
    
    for (int countTraces = 0; countTraces < tracks.size(); countTraces++) {
        //if (tracks[countTraces].points.size() < 25) { continue; }
        uint r = rand() % 255;
        uint g = rand() % 255;
        uint b = rand() % 255;
        uint col[] = {r,g,b};
        for (int num_point = 0; num_point < tracks[countTraces].points.size()-1; num_point++ ) {
            
            output.draw_line(tracks[countTraces].points[num_point].X,
                             tracks[countTraces].points[num_point].Y,
                             tracks[countTraces].points[num_point+1].X,
                             tracks[countTraces].points[num_point+1].Y,
                             col);
            /*printf("Drawing from (%i, %i) to (%i, %i)\n",tracks[countTraces].points[num_point].X,
                                                          tracks[countTraces].points[num_point].Y,
                                                          tracks[countTraces].points[num_point+1].X,
                                                          tracks[countTraces].points[num_point+1].Y);*/
        }


    }
    std::string outputname = "/Users/alex/Desktop/Example_out.bmp";
    output.save_bmp(outputname.c_str());
}
