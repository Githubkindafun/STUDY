#pragma once
#include "Color.hpp"
#include <string>

using namespace std;

class Color_Named : public virtual Color {
protected:
    string Color_name;

public:
    Color_Named();

    Color_Named(double given_Red, double given_Green, double given_Blue, string given_Color_name);
};