#include <bits/stdc++.h>

using namespace std;

int main() {

    // ([0-1][0-9]|2[0-3]) - godziny od 00 do 19 lub 20/21/22/23
    // [0-5][0-9] - minuty
    // ()? - opcjonalna zawartosc
    regex timeRegex(R"(^([0-1][0-9]|2[0-3]):[0-5][0-9](:[0-5][0-9])?$)");
    string time;
    while(1) {
        cin >> time;
        if(regex_match(time, timeRegex)) {
            cout << "good time amigo\n";
        } else {
            cout << "bad time amigo\n";
        }
    }
}