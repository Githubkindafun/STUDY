#include <bits/stdc++.h>

using namespace std;

class point {
private:
    double x;
    double y;
    double R;
    double G;
    double B;
    string name;
public:
    point(double newX, double newY, double red, double green, double blue, string n) {
        x = newX;
        y = newY;
        R = red;
        G = green;
        B = blue;
        name = n;
    }

    double getX() {
        return x;
    }
    double getY() {
        return y;
    }
    double getRed() {
        return R;
    }
    double getGreen() {
        return G;
    }
    double getBlue() {
        return B;
    }
    string getName() {
        return name;
    }
    void setX(double newX) {
        x = newX;
    }
    void setY(double newY) {
        y = newY;
    }
    void setRed(double newR) {
        R = newR;
    }
    void setGreen(double newG) {
        G = newG;
    }
    void setBlue(double newB) {
        B = newB;
    }
    void setName(double newName) {
        name = newName;
    }
};

int whichQuadrantYouAreFrom(int x, int y) {
    if(x > 0 && y > 0) {
        return 1;
    } else if(x < 0 && y > 0) {
        return 2;
    } else if(x < 0 && y < 0) {
        return 3;
    } else {
        return 4;
    }
}

int main() {
    list<point> losPointos = {
        {0.0, 0.0, 1.0, 0.0, 0.0, "red"},
        {0.0, 0.0, 0.0, 1.0, 0.0, "green"},
        {0.0, 0.0, 0.0, 0.0, 1.0, "blue"},
        {0.0, 0.0, 1.0, 1.0, 0.0, "yellow"},
        {0.0, 0.0, 0.0, 1.0, 1.0, "Cyan"},
        {0.0, 0.0, 1.0, 0.0, 1.0, "magenta"},
        {0.0, 0.0, 1.0, 0.65, 0.0, "orange"},
        {0.0, 0.0, 0.5, 0.0, 0.5, "purple"},
        {0.0, 0.0, 1.0, 0.75, 0.8, "pink"},
        {0.0, 0.0, 0.65, 0.16, 0.16, "brown"},
        {0.0, 0.0, 0.5, 0.5, 0.5, "gray"},
        {0.0, 0.0, 0.0, 0.0, 0.0, "black"},
        {0.0, 0.0, 1.0, 1.0, 1.0, "white"},
        {0.0, 0.0, 0.2, 0.8, 0.2, "lime"},
        {0.0, 0.0, 0.0, 0.0, 0.5, "navy"},
        {0.0, 0.0, 0.0, 0.5, 0.5, "teal"},
        {0.0, 0.0, 0.5, 0.5, 0.0, "olive"},
        {0.0, 0.0, 0.5, 0.0, 0.0, "maroon"},
        {0.0, 0.0, 1.0, 0.85, 0.0, "gold"},
        {0.0, 0.0, 0.75, 0.75, 0.75, "silver"},
        {0.0, 0.0, 0.3, 0.0, 0.5, "indigo"},
        {0.0, 0.0, 0.9, 0.5, 0.9, "violet"},
        {0.0, 0.0, 0.9, 0.9, 0.85, "beige"},
    };

    srand(time(NULL));
    auto giveRandomCoords =
        []() {
            double coord = (double)rand() / RAND_MAX;
            // -42 42
            return (coord * 84) - 42;  
        };
    for(auto& pt: losPointos) {
        pt.setX(giveRandomCoords());
        pt.setY(giveRandomCoords());
    }
    // a)
    cout << "a)\n";
    auto letter5Cmp = 
        [](point& pt) {
            return pt.getName().length() > 5;
        };
    // https://www.geeksforgeeks.org/cpp/listremove-listremove_if-c-stl/
    losPointos.remove_if(letter5Cmp);
    for(auto& pt: losPointos) {
        cout << pt.getName() << " " << pt.getX() << " " << pt.getY() << endl;
    }

    // b)
    cout << "b)\n";
    int q1 = count_if(losPointos.begin(), losPointos.end(), 
        [](point& pt) {
            return whichQuadrantYouAreFrom(pt.getX(), pt.getY()) == 1;
    });
    int q2 = count_if(losPointos.begin(), losPointos.end(), 
        [](point& pt) {
            return whichQuadrantYouAreFrom(pt.getX(), pt.getY()) == 2;
    });
    int q3 = count_if(losPointos.begin(), losPointos.end(), 
        [](point& pt) {
            return whichQuadrantYouAreFrom(pt.getX(), pt.getY()) == 3;
    });
    int q4 = count_if(losPointos.begin(), losPointos.end(), 
        [](point& pt) {
            return whichQuadrantYouAreFrom(pt.getX(), pt.getY()) == 4;
    });
    cout << "points in q1: " << q1
         << "\npoints in q2: " << q2
         << "\npoints in q3: " << q3
         << "\npoints in q4: " << q4 << endl;

    // c)
    cout << "c)\n";
    auto luminanceCmp = [](point& pt1, point& pt2) {
        double pt1Lumi = pt1.getRed() * 0.3 + pt1.getGreen() * 0.59 + pt1.getBlue() * 0.11;
        double pt2Lumi = pt2.getRed() * 0.3 + pt2.getGreen() * 0.59 + pt2.getBlue() * 0.11;
        return pt1Lumi < pt2Lumi;
    };
    losPointos.sort(luminanceCmp);
    for(auto& pt: losPointos) {
        cout << pt.getName() << endl;
    }

    // d)
    cout << "d)\n";
    // chyba nie chodzilo o 64 unless rgb mowimy jako 0 - 255
    // no ja zrobilem 0-1 wiec skalujemy 64 -> 64 / 255 = 0,2509803921568627 czyli 0.251
    auto areYouDark = [](point& pt) {
        return (pt.getRed() * 0.3 + pt.getGreen() * 0.59 + pt.getBlue() * 0.11) < 0.251;
    };
    list<point> darkOnes;
    int darkOnesCount = 0;
    for(point& pt : losPointos) {
        if(areYouDark(pt)) {
            darkOnesCount++;
            darkOnes.emplace_back(pt);
        }
    }
    losPointos.remove_if(areYouDark);
    cout << "losPointos(" << losPointos.size() << ")\n";
    for(auto& pt: losPointos) {
        cout << pt.getName() << endl;
    }
    cout << "darkOnes(" << darkOnesCount << ")\n";
    for(auto& pt: darkOnes) {
        cout << pt.getName() << endl;
    }
}