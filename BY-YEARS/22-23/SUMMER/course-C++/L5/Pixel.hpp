#pragma once

class Pixel { // Wybrałem 1920 na 1080
protected:
    int x;
    int y;
    static const int x_size = 1920;
    static const int y_size = 1080;

public:

    Pixel();

    Pixel(int given_x, int given_y);

    int  To_Left_Border() const;

    int To_Right_Border() const;

    int To_Upper_Border() const;

    int To_Lower_Border() const;
};
