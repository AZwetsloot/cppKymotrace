//
//  main.cpp
//  cppKymotrace
//
//  Created by Alex Zwetsloot on 03/03/2018.
//  Copyright © 2018 Alex Zwetsloot. All rights reserved.
//

#include <iostream>
#define cimg_display 0
#include "CImg.h"
#include <cstring>
#include <string>
#include "Linker.hpp"
using namespace cimg_library;



int main(int argc, const char * argv[]) {
    
    // Ignore below
    int noise = 20;
    int window_size = 3; // the number of pixels you get the local maxima over
    int travel = 2; // how far left/right you look for local maxima

    
    CImg<uint> image("/Users/alex/Desktop/Example6.bmp");
    CImg<uint> output(image.width(), image.height(),1,1);
    CImg<uint> detection(image.width(),image.height(),1,1);
    for (int y = 0; y < image.height(); y++){
        for (int x = window_size; x < image.width()-window_size; x++) {
            if (image(x,y) < noise) {
                continue;
            }
            // Is X a local maxima on the scale of window_size pix left window_size pix right?
            bool maxima = true;
            int sum = 0;
            int search = floor(window_size/2);

            for (int count = x - search; count <= x+search; count++) {
                sum += image(count,y);  // Sum of the maxima right here.
            }

            for (int slide = -travel; slide <= travel; slide++) {
                int testSum = 0;
                for (int count = x + slide - search; count <= x+slide+search; count++) {
                    testSum += image(count,y);  // Sum of the maxima right here.
                }
                if (testSum > sum) {
                    maxima = false;
                }
            }
            if (maxima == true) {
                output(x,y) = 255;
                detection(x,y) = 1;
            }
        }
    }
    std::string loc = "/Users/alex/Desktop/Example_out_original.bmp";
    output.save_bmp(loc.c_str());
    Linker trackTracer = Linker(image, detection);
    trackTracer.Link(1,2,6,100);
    trackTracer.Draw();
    return 0;
}
