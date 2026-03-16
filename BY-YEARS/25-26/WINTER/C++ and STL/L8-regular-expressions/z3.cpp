#include <bits/stdc++.h>

using namespace std;

int main() {
    // [A-Z][a-z]* - pierwsza litera duza potem dowolna ilosc malych
    // delikatna poprawka wynikajaca z parsowani, jak starczy czasu poprawie
    // przy napisaniu W- W to juz drugie W bedzie jako nowy znak
    regex placeRegex(R"(^[A-Z][a-z]*(-[A-Z][a-z]*|\s*[A-Z][a-z]*)*$)");
    string place;
    while(1) {
        // cin >> place;
        // https://www.geeksforgeeks.org/cpp/read-line-of-input-text-in-cpp/
        getline(cin, place);
        if(regex_match(place, placeRegex)) {
            cout << "good place amigo\n";
        } else {
            cout << "bad place amigo\n";
        }
    }
}