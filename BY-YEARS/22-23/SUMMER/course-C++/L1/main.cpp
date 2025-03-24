#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

const vector<pair<int, string>> rzym = {
    {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"}, {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
};

string bin2rzym(int num) {
    string result = "";
    int i = 0;

    while (num != 0) {
        if( rzym[i].first <= num) {
            num -= rzym[i].first;
            result += rzym[i].second;
        }
        else {
            i++;
        }
    }
    return result;
}

int main(int argc, char* argv[]) { // argc - liczba argumentów, argv - tablica stringów
    if(argc <= 1) {
        clog << "Brak danych ;c " << endl;
        return 0;
    }
    int curr_argument = 0;
    for(int i = 1; i < argc; i++) {
        try { curr_argument = stoi(argv[i]); }
        catch(...) {
            clog << "B£ÊDNY ARGUMENT";
            continue;
            }
        if(curr_argument >= 1 && curr_argument <= 3999) {
            string result = bin2rzym(curr_argument);
            cout << result << endl;
        }
        else {
            clog << "LICZBA POZA ZAKRESEM" << endl;
        }
    }
    return 0;
}
