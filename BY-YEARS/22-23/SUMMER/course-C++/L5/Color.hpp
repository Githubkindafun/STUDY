#pragma once


class Color {
protected:
    double Red;
    double Green;
    double Blue;

public:

    Color();

    Color(double given_Red, double given_Green, double given_Blue);

    double GetRed();

    double GetGreen();

    double GetBlue();

    void SetRed(double given_Red);

    void SetGreen(double given_Green);

    void SetBlue(double given_Blue);

    void Lightening(double given_value);

    void Darkening(double given_value);

    static Color Color_Merge(Color C1, Color C2);
};
