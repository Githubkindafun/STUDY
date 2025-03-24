#include "Color.hpp"
#include "Color_Transparent.hpp"
#include "Color_Named.hpp"
#include "Color_NT.hpp"
#include "Pixel.hpp"
#include "Colored_Pixel.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;
/*
class Color {
protected:
    double Red;
    double Green;
    double Blue;

public:

    Color() { // konstruktor bezargumentowy ustawiajacy kolor na 0 0 0 (czarny)
        this->Red = 0;
        this->Green = 0;
        this->Blue = 0;
    }

    Color(double given_Red, double given_Green, double given_Blue) { //konstruktor gdzie inicjalizujemy z danymi danymi RGB
        
        if (given_Red > 255 || given_Red < 0 || given_Green > 255 || given_Green < 0 || given_Blue > 255 || given_Blue < 0) {
            throw invalid_argument("Jedna z wartości R G B podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        }

        this->Red = given_Red;
        this->Green = given_Green;
        this->Blue = given_Blue;
    }

    double GetRed() { // Getter
        return Red;
    }

    double GetGreen() { // Getter
        return Green;
    }

    double GetBlue() { // Getter
        return Blue;
    }

    void SetRed(double given_Red) { //seter
        if (given_Red > 255 || given_Red < 0) throw invalid_argument("Wartość dla Red podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        this->Red = given_Red;
    }

    void SetGreen(double given_Green) { //seter
        if (given_Green > 255 || given_Green < 0) throw invalid_argument("Wartość dla Green podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        this->Green = given_Green;
    }

    void SetBlue(double given_Blue) { //seter
        if (given_Blue > 255 || given_Blue < 0) throw invalid_argument("Wartość dla Blue podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        this->Blue = given_Blue;
    }

    void Lightening(double given_value) { // rozjaśnianie

        if (Red + given_value > 255 || Red + given_value < 0 || Green + given_value > 255 || Green + given_value < 0 || Blue + given_value > 255 || Blue + given_value < 0) {
            throw invalid_argument("Nie da się bardziej rozjaśnić. Jedna z wartosci RGB wychodzi poza zakres (0-225). Sprawdz czy podajesz dobre dane!?");
        }

        this->Red += given_value;
        this->Green += given_value;
        this->Blue += given_value;
    }

    void Darkening(double given_value) { // przyciemnianie

        if (Red - given_value > 255 || Red - given_value < 0 || Green - given_value > 255 || Green - given_value < 0 || Blue - given_value > 255 || Blue - given_value < 0) {
            throw invalid_argument("Nie da się bardziej przyciemnić. Jedna z wartosci RGB wychodzi poza zakres (0-225). Sprawdz czy podajesz dobre dane!?");
        }

        this->Red -= given_value;
        this->Green -= given_value;
        this->Blue -= given_value;
    }

    static Color Color_Merge(Color C1, Color C2) {
        Color C3 = Color((C1.GetRed() + C2.GetRed()) / 2, (C1.GetGreen() + C2.GetGreen()) / 2, (C1.GetBlue() + C2.GetBlue()) / 2);

        return C3;

    }

};

class Color_Transparent : public virtual Color {
protected:
    double Alpha;
public:
    Color_Transparent() {
        this->Red = 0;
        this->Green = 0;
        this->Blue = 0;
        this->Alpha = 0;
    }

    Color_Transparent(double given_Red, double given_Green, double given_Blue, double given_Alpha) {
        if (given_Red > 255 || given_Red < 0 || given_Green > 255 || given_Green < 0 || given_Blue > 255 || given_Blue < 0 ) {
            throw invalid_argument("Jedna z wartości R G B podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        }
        if (given_Alpha > 100 || given_Alpha < 0) throw invalid_argument("Wartość alpha poza zakresem podaj ponowe wartość (0-100)!");

        this->Red = given_Red;
        this->Green = given_Green;
        this->Blue = given_Blue;
        this->Alpha = given_Alpha;
    }
};


class Color_Named : public virtual Color {
protected:
    string Color_name;

public:
    Color_Named() {
        this->Red = 0;
        this->Green = 0;
        this->Blue = 0;
        this->Color_name = "";
    }

    Color_Named(double given_Red, double given_Green, double given_Blue, string given_Color_name) {
        if (given_Red > 255 || given_Red < 0 || given_Green > 255 || given_Green < 0 || given_Blue > 255 || given_Blue < 0) {
            throw invalid_argument("Jedna z wartości R G B podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        }
        

        this->Red = given_Red;
        this->Green = given_Green;
        this->Blue = given_Blue;
        transform(given_Color_name.begin(), given_Color_name.end(), given_Color_name.begin(), ::tolower);
        this->Color_name = given_Color_name;
    }
};

class Color_NT : public Color_Named, public Color_Transparent {
public:
    
    Color_NT() {
        this->Red = 0;
        this->Green = 0;
        this->Blue = 0;
        this->Color_name = "";
        this->Alpha = 0;
    }

    Color_NT(double given_Red, double given_Green, double given_Blue, string given_Color_name, double given_Alpha) {
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

};

class Pixel { // Wybrałem 1920 na 1080
protected:
    int x;
    int y;
    static const int x_size = 1920;
    static const int y_size = 1080;

public:

    Pixel() {
        this->x = 0;
        this->y = 0;
    }

    Pixel(int given_x, int given_y) {
        if (given_x < 0 || given_x >= 1920 || given_y < 0 || given_y >= 1080) throw invalid_argument("Pixel poza ekranem! Sprawdź czy podałes poprawne dane!?");

        this->x = given_x;
        this->y = given_y;
    }

    int  To_Left_Border() const {
        return x;
    }

    int To_Right_Border() const {
        return x_size - x;
    }

    int To_Upper_Border() const {
        return y;
    }

    int To_Lower_Border() const {
        return y_size - y;
    }
};

class Pixel_Colored : public Pixel {
protected:
    Color_Transparent ct;

public:

    Pixel_Colored(int given_x, int given_y, double given_Red, double given_Green, double given_Blue, double given_Alpha) :
        Pixel(given_x, given_y),
        ct(given_Red, given_Green, given_Blue, given_Alpha)
    {}

    double GetRed() { // Getter
        return ct.GetRed();
    }

    double GetGreen() { // Getter
        return ct.GetGreen();
    }

    double GetBlue() { // Getter
        return ct.GetBlue();
    }

	double GetX() { // Getter
        return x;
    }

	double GetY() { // Getter
        return y;
    }

    void SetRed(double given_Red) { //seter
        if (given_Red > 255 || given_Red < 0) throw invalid_argument("Wartość dla Red podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        ct.SetRed(given_Red);
    }

    void SetGreen(double given_Green) { //seter
        if (given_Green > 255 || given_Green < 0) throw invalid_argument("Wartość dla Green podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        ct.SetGreen(given_Green);
    }

    void SetBlue(double given_Blue) { //seter
        if (given_Blue > 255 || given_Blue < 0) throw invalid_argument("Wartość dla Blue podana przez ciebie nie znajduje się w przedziale (0 - 225) sprawdz czy wszystko sie zgadza!");
        ct.SetBlue(given_Blue);
    }

    void Shift(int given_x, int given_y) {
        if (x + given_x < 0 || x + given_x > x_size || y + given_y < 0 || y + given_y > y_size) {
            throw invalid_argument("Przy takim przesunięciu pixel będzie poza ekranem! Sprawdz czy podajesz poprawne dane!?");
        }

        x += given_x;
        y += given_y;
    }
};
*/




int Distance(const Pixel& p, const Pixel& q) {
    int x_distance = q.To_Left_Border() - p.To_Left_Border();
    int y_distance = q.To_Upper_Border() - p.To_Upper_Border();

    return sqrt(x_distance * x_distance + y_distance * y_distance);
}

int main()
{
    Color C1 = Color(4, 2, 0);
    Color C2 = Color(42, 42, 42);
    Color C3 = Color(21, 42, 63);
    Color C4 = Color(1, 1, 1);
    Color C5 = Color();
    
    C2.Lightening(42);
    C3.Darkening(21);

    cout << C2.GetRed() << " " << C2.GetGreen() << " " << C2.GetBlue() << endl;
	cout << C3.GetRed() << " " << C3.GetGreen() << " " << C3.GetBlue() << endl;

	Color C6 = Color::Color_Merge( C1, C2);
	cout << C6.GetRed() << endl;
	
//	cout << Color::Color_Merge(C1, C4).GetRed() << " " << Color::Color_Merge(C1, C4).GetGreen() << " " << Color::Color_Merge(C1, C4).GetBlue() << endl; 

    Pixel P1 = Pixel(1, 2);
    Pixel P2 = Pixel(42, 42);
    Pixel P3 = Pixel(4, 2);
    Pixel P4 = Pixel(2, 1);
    Pixel P5 = Pixel();

    cout << P2.To_Left_Border() << endl;
    cout << P1.To_Right_Border() << endl;
    cout << P4.To_Upper_Border() << endl;
    cout << P3.To_Lower_Border() << endl;

	Pixel_Colored PC1 = Pixel_Colored(4, 2,  10,  20,  30,  81);
	PC1.Shift(2, 4);
	Pixel_Colored PC2 = Pixel_Colored(2, 1,  20,  30,  40,  18);
	
	
	cout << PC1.GetX() << " " << PC1.GetY() << endl;
	cout << Distance(PC1, PC2) << endl;

}

