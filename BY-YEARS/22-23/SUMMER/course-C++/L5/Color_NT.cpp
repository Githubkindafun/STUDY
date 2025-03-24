#include "Color_NT.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

using namespace std;
    
    Color_NT::Color_NT() {
        this->Red = 0;
        this->Green = 0;
        this->Blue = 0;
        this->Color_name = "";
        this->Alpha = 0;
    }

    Color_NT::Color_NT(double given_Red, double given_Green, double given_Blue, string given_Color_name, double given_Alpha) {
        if (given_Red > 255 || given_Red < 0 || given_Green > 255 || given_Green < 0 || given_Blue > 255 || given_Blue < 0) {
            throw invalid_argument("Jedna z wartości R G B podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        }
        if (given_Alpha > 100 || given_Alpha < 0) throw invalid_argument("Wartość alpha poza zakresem podaj ponowe wartość (0-100)!");

        this->Red = given_Red;
        this->Green = given_Green;
        this->Blue = given_Blue;
        transform(given_Color_name.begin(), given_Color_name.end(), given_Color_name.begin(), ::tolower);
        this->Color_name = given_Color_name;
        this->Alpha = given_Alpha;
    }

