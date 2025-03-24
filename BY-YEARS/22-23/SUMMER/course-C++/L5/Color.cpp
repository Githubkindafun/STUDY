#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include "Color.hpp"

using namespace std;

    Color::Color() { // konstruktor bezargumentowy ustawiajacy kolor na 0 0 0 (czarny)
        this->Red = 0;
        this->Green = 0;
        this->Blue = 0;
    }

    Color::Color(double given_Red, double given_Green, double given_Blue) { //konstruktor gdzie inicjalizujemy z danymi danymi RGB
        
        if (given_Red > 255 || given_Red < 0 || given_Green > 255 || given_Green < 0 || given_Blue > 255 || given_Blue < 0) {
            throw invalid_argument("Jedna z wartości R G B podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        }

        this->Red = given_Red;
        this->Green = given_Green;
        this->Blue = given_Blue;
    }

    double Color::GetRed() { // Getter
        return Red;
    }

    double Color::GetGreen() { // Getter
        return Green;
    }

    double Color::GetBlue() { // Getter
        return Blue;
    }

    void Color::SetRed(double given_Red) { //seter
        if (given_Red > 255 || given_Red < 0) throw invalid_argument("Wartość dla Red podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        this->Red = given_Red;
    }

    void Color::SetGreen(double given_Green) { //seter
        if (given_Green > 255 || given_Green < 0) throw invalid_argument("Wartość dla Green podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        this->Green = given_Green;
    }

    void Color::SetBlue(double given_Blue) { //seter
        if (given_Blue > 255 || given_Blue < 0) throw invalid_argument("Wartość dla Blue podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        this->Blue = given_Blue;
    }

    void Color::Lightening(double given_value) { // rozjaśnianie

        if (Red + given_value > 255 || Red + given_value < 0 || Green + given_value > 255 || Green + given_value < 0 || Blue + given_value > 255 || Blue + given_value < 0) {
            throw invalid_argument("Nie da się bardziej rozjaśnić. Jedna z wartosci RGB wychodzi poza zakres (0-225). Sprawdz czy podajesz dobre dane!?");
        }

        this->Red += given_value;
        this->Green += given_value;
        this->Blue += given_value;
    }

    void Color::Darkening(double given_value) { // przyciemnianie

        if (Red - given_value > 255 || Red - given_value < 0 || Green - given_value > 255 || Green - given_value < 0 || Blue - given_value > 255 || Blue - given_value < 0) {
            throw invalid_argument("Nie da się bardziej przyciemnić. Jedna z wartosci RGB wychodzi poza zakres (0-225). Sprawdz czy podajesz dobre dane!?");
        }

        this->Red -= given_value;
        this->Green -= given_value;
        this->Blue -= given_value;
    }

    Color Color::Color_Merge(Color C1, Color C2) {
        Color C3 = Color((C1.GetRed() + C2.GetRed()) / 2, (C1.GetGreen() + C2.GetGreen()) / 2, (C1.GetBlue() + C2.GetBlue()) / 2);

        return C3;

    }


