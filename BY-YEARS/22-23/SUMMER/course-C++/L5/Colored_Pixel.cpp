


#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include "Colored_Pixel.hpp"
#include "Pixel.hpp"

using namespace std;

    Pixel_Colored::Pixel_Colored(int given_x, int given_y, double given_Red, double given_Green, double given_Blue, double given_Alpha) :
        Pixel(given_x, given_y),
        ct(given_Red, given_Green, given_Blue, given_Alpha)
    {}

    double Pixel_Colored::GetRed() { // Getter
        return ct.GetRed();
    }

    double Pixel_Colored::GetGreen() { // Getter
        return ct.GetGreen();
    }

    double Pixel_Colored::GetBlue() { // Getter
        return ct.GetBlue();
    }

	double Pixel_Colored::GetX() { // Getter
        return x;
    }

	double Pixel_Colored::GetY() { // Getter
        return y;
    }

    void Pixel_Colored::SetRed(double given_Red) { //seter
        if (given_Red > 255 || given_Red < 0) throw invalid_argument("Wartość dla Red podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        ct.SetRed(given_Red);
    }

    void Pixel_Colored::SetGreen(double given_Green) { //seter
        if (given_Green > 255 || given_Green < 0) throw invalid_argument("Wartość dla Green podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        ct.SetGreen(given_Green);
    }

    void Pixel_Colored::SetBlue(double given_Blue) { //seter
        if (given_Blue > 255 || given_Blue < 0) throw invalid_argument("Wartość dla Blue podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        ct.SetBlue(given_Blue);
    }

    void Pixel_Colored::Shift(int given_x, int given_y) {
        if (x + given_x < 0 || x + given_x > x_size || y + given_y < 0 || y + given_y > y_size) {
            throw invalid_argument("Przy takim przesunięciu pixel będzie poza ekranem! Sprawdz czy podajesz poprawne dane!?");
        }

        x += given_x;
        y += given_y;
    }
