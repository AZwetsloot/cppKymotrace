//
//  Linker.hpp
//  cppKymotrace
//
//  Created by Alex Zwetsloot on 03/03/2018.
//  Copyright © 2018 Alex Zwetsloot. All rights reserved.
//

#ifndef Linker_hpp
#define Linker_hpp

#include <stdio.h>
#define cimg_display 0
#include "CImg.h"
#include <vector>
#include "Trace.hpp"
using namespace cimg_library;

class Linker {
public:
    Linker(CImg<uint> &refimg, CImg<uint> &refdetection) : img(refimg), detection(refdetection) {
        
    };
    std::vector<Trace> tracks;
    CImg<uint> &img;
    CImg<uint> &detection;
    void Link(int,int,int,int);
    void Draw();
    
};


#endif /* Linker_hpp */
