#pragma once
#include "Color_Transparent.hpp"
#include "Color_Named.hpp"

class Color_NT : public Color_Named, public Color_Transparent {
public:
    
    Color_NT();

    Color_NT(double given_Red, double given_Green, double given_Blue, string given_Color_name, double given_Alpha);

};