#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include "Color.hpp"
#include "Color_Transparent.hpp"

using namespace std;

    Color_Transparent::Color_Transparent() {
        this->Red = 0;
        this->Green = 0;
        this->Blue = 0;
        this->Alpha = 0;
    }

    Color_Transparent::Color_Transparent(double given_Red, double given_Green, double given_Blue, double given_Alpha) {
        if (given_Red > 255 || given_Red < 0 || given_Green > 255 || given_Green < 0 || given_Blue > 255 || given_Blue < 0 ) {
            throw invalid_argument("Jedna z wartości R G B podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        }
        if (given_Alpha > 100 || given_Alpha < 0) throw invalid_argument("Wartość alpha poza zakresem podaj ponowe wartość (0-100)!");

        this->Red = given_Red;
        this->Green = given_Green;
        this->Blue = given_Blue;
        this->Alpha = given_Alpha;
    }
