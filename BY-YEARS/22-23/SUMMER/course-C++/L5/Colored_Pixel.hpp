#pragma once
#include "Pixel.hpp"
#include "Color_Transparent.hpp"

class Pixel_Colored : public Pixel {
protected:
    Color_Transparent ct;

public:

    Pixel_Colored(int given_x, int given_y, double given_Red, double given_Green, double given_Blue, double given_Alpha);

    double GetRed();

    double GetGreen();

    double GetBlue();

	double GetX();

	double GetY();

    void SetRed(double given_Red);

    void SetGreen(double given_Green);

    void SetBlue(double given_Blue);

    void Shift(int given_x, int given_y);
};