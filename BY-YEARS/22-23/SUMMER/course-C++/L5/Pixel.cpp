
#include "Pixel.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

    Pixel::Pixel() {
        this->x = 0;
        this->y = 0;
    }

    Pixel::Pixel(int given_x, int given_y) {
        if (given_x < 0 || given_x >= 1920 || given_y < 0 || given_y >= 1080) throw invalid_argument("Pixel poza ekranem! Sprawdź czy podałes poprawne dane!?");

        this->x = given_x;
        this->y = given_y;
    }

    int  Pixel::To_Left_Border() const {
        return x;
    }

    int Pixel::To_Right_Border() const {
        return x_size - x;
    }

    int Pixel::To_Upper_Border() const {
        return y;
    }

    int Pixel::To_Lower_Border() const {
        return y_size - y;
    }

