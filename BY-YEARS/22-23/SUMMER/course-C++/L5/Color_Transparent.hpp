#pragma once
#include "Color.hpp"

class Color_Transparent : public virtual Color {
protected:
    double Alpha;
public:
    Color_Transparent();

    Color_Transparent(double given_Red, double given_Green, double given_Blue, double given_Alpha);

};