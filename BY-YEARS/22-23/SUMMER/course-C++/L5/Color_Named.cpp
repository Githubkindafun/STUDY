#include "Color_Named.hpp"
#include "Color.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

    Color_Named::Color_Named() {
        this->Red = 0;
        this->Green = 0;
        this->Blue = 0;
        this->Color_name = "";
    }
	//??
    Color_Named::Color_Named(double given_Red, double given_Green, double given_Blue, string given_Color_name) {
        if (given_Red > 255 || given_Red < 0 || given_Green > 255 || given_Green < 0 || given_Blue > 255 || given_Blue < 0) {
            throw invalid_argument("Jedna z wartości R G B podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        }
        

        this->Red = given_Red;
        this->Green = given_Green;
        this->Blue = given_Blue;
        transform(given_Color_name.begin(), given_Color_name.end(), given_Color_name.begin(), ::tolower);
        this->Color_name = given_Color_name;
    }
