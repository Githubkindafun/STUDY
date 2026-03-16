#include <bits/stdc++.h>

using namespace std;

int main() {

    int day;
    int month;
    int year;

    cout << "podaj datę która Cię interesuje: " << endl;

    cin >> day >> month >> year;

    switch (int M = month; M) {
    case 1:
        cout << day << ". stycznia " << year;
        break;
    case 2:
        cout << day << ". lutego " << year;
        break;
    case 3:
        cout << day << ". marca " << year;
        break;
    case 4:
        cout << day << ". kwietnia " << year;
        break;
    case 5:
        cout << day << ". maja " << year;
        break;
    case 6:
        cout << day << ". czerwca " << year;
        break;
    case 7:
        cout << day << ". lipca " << year;
        break;
    case 8:
        cout << day << ". śierpnia " << year;
        break;
    case 9:
        cout << day << ". września " << year;
        break;
    case 10:
        cout << day << ". października " << year;
        break;
    case 11:
        cout << day << ". listopada " << year;
        break;
    case 12:
        cout << day << ". grudnia " << year;
        break;
    default:
        cout << "coś chyba nie ten teges" << endl;
        break;
    }
}