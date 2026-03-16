/*
Napisz funkcję, która zwróci najczęściej pojawiający się element w zbiorze danych oraz liczbę jego wystąpień. 
Dane to liczby całkowite umieszczone w wektorze vector<int>, które lubią się powtarzać. 
Jeśli więcej niż jeden element pojawia się pojawia się taką samą maksymalną liczbę razy, to funkcja powinna zwrócić wszystkie je wszystkie 
– na przykład dla danych {1, 1, 3, 5, 8, 9, 5, 8, 8, 5} powinny zostać zwrócone dwie pary {5, 3} oraz {8, 3} 
(pierwsza pozycja w parze to wartość a druga to liczba wystąpień). 
*/

#include <bits/stdc++.h>

// dobra niby mozemy pobwaic sie w Aisd i zrobic jakas auto sortujaca strukture bla bla bla ale odpuszcze sobie :)

using namespace std;

vector<pair<int, int>> funFun(vector<int>& v) {
    vector<pair<int, int>> thePairVector;

    // skip error size 0 sam ustalam wartosc

    // populujemy thePairVector i zliczamy ~unikalne wystapienia
    for(int i = 0; i < v.size(); i++) {
        bool found = false;
        for(auto& thePair : thePairVector) {
            if(thePair.first == v[i]) {
                thePair.second++;
                found = true;
                break;
            }
        }
        if(!found) {
            thePairVector.emplace_back(v[i], 1);
        }
    }

    int mostPopularOfThemAll = 0;
    for(auto& thePair : thePairVector) {
        if(thePair.second > mostPopularOfThemAll) {
            mostPopularOfThemAll = thePair.second;
        }
    }
    
    vector<pair<int, int>> losResultos;
    for(auto& thePair : thePairVector) {
        if(thePair.second == mostPopularOfThemAll) {
            losResultos.emplace_back(thePair);
        }
    }

    return losResultos;
}

int main() {
    vector<int> losTestos = {1, 1, 3, 5, 8, 9, 5, 8, 8, 5};
    auto losTestosResultos = funFun(losTestos);
    int i = 1;
    for(auto& resultPair : losTestosResultos) {
        cout << "para: " << i << " wartosc: " << resultPair.first << " #wystapien: " << resultPair.second << endl;
        i++;
    }
}