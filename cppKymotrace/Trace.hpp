//
//  Trace.hpp
//  cppKymotrace
//
//  Created by Alex Zwetsloot on 03/03/2018.
//  Copyright © 2018 Alex Zwetsloot. All rights reserved.
//

#ifndef Trace_hpp
#define Trace_hpp

#include <stdio.h>
#include <vector>
#endif /* Trace_hpp */

// Trace.hpp defines an Object called a Trace.
// This is simply a storage device for all t, x, y, intensity, quality of kymograph tracks.
class Point;
class Trace {
public:
    int *t;
    int *x;
    int *y;
    int *quality;
    int *intensity;
    int currentScore;
    std::vector<Point> points;
    Trace(int timepoints = 300) {
        // allocate memory for the pointer;
        t = new int[timepoints];
        x = new int[timepoints];
        y = new int[timepoints];
        quality = new int[timepoints];
        intensity = new int[timepoints];
    }

private:
    int *ptr;
};

class Point {
public:
    int X = 0;
    int Y = 0;
    Point(int x, int y);
    Point operator-(Point subPoint);
    Point operator+(Point addPoint);
    Point operator*=(int m);
    
};
