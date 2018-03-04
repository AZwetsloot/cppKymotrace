//
//  Trace.cpp
//  cppKymotrace
//
//  Created by Alex Zwetsloot on 03/03/2018.
//  Copyright © 2018 Alex Zwetsloot. All rights reserved.
//

#include "Trace.hpp"


Point::Point(int x, int y) {
    X = x;
    Y = y;
}
Point Point::operator-(Point subpoint) {
    return Point(X - subpoint.X, Y - subpoint.Y);
}
Point Point::operator+(Point addPoint) {
    return Point(X+addPoint.X, Y+addPoint.Y);
}

Point Point::operator*=(int m) {
    X*=m;
    Y*=m;
    return *this;
}
