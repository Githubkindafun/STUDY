#include <bits/stdc++.h>

using namespace std;

// delta = b^2 - 4*a*c

int main() {
    int a = 4;
    // int b = 6;
    int b = 12;
    // int b = 14;
    int c = 9;

    if (int delta = pow(b, 2) - 4 * a * c; delta > 0) {
        cout << "miejsca zerowe istnieja (2)" << endl;
    } else if (delta == 0) {
        cout <<"jedno miejsce zerowe istnieje (1)" << endl;
    } else {
        cout << "nie ma ani jednego miejsca zerowego :c (0)" << endl;
    }

    return 0;
}