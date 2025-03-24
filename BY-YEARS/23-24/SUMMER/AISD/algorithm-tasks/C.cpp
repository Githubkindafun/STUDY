#include <bits/stdc++.h>

using namespace std;




// dla potencjalnie osoby sprawdzajacej
// sorki za balagan ;<


int main() {
    ios_base::sync_with_stdio(false);
    cout.tie(0);
    cin.tie(0);
    
// wczytywanie danych do tablicy
    int box_weight; cin >> box_weight; // waga pudełka
    int coins_amount; cin >> coins_amount; // ilośc monetek

    int coins_val[coins_amount]; // tablica na wartości monetek
    int coins_grams[coins_amount]; // tablica na wagi monetek

    int value;
    int grams;
    for(int i = 0; i < coins_amount; i++) { // wypelnianie tablic wartosci / wag
        cin >> value >> grams;
        coins_val[i] = value;
        coins_grams[i] = grams;
        // cout << "v "<< value << " " << grams << endl;
    }

// main logic
// chcemy teraz przeprocesować dane tak aby uzyskać ostatni
// rząd dp-kowej tablicy, zarowno przy zalozniu max i min
// (nie generujemy calej tabelki aby uzyskac oszczednosc pamieciowa)

    long long max_dp[box_weight + 1];
    long long min_dp[box_weight + 1];
    // max_dp[0] = 0;
    // min_dp[0] = 0;
    for(int i = 0; i <= box_weight; i++) { // tutaj zapelnililismy sobie tabelki
        // max_dp[i] = LONG_LONG_MIN;
        // min_dp[i] = LONG_LONG_MAX; 
        max_dp[i] = -1;
        min_dp[i] = 1000000000000000LL; 
    }
    min_dp[0] = 0;
    max_dp[0] = 0;
    // cout << endl; // !
    for(int i = 0; i < coins_amount; i++) { // tutaj mamy cala logike realnego wypelniana tabelki
        for(int j = 0; j <= box_weight; j++) {
            if(coins_grams[i] <= j) {

                // long long using_current_coin_max = coins_val[i] + max_dp[j - coins_grams[i]]; // wartosc jezeli uzyjemy aktualnej monetki
                // long long using_current_coin_min = coins_val[i] + min_dp[j - coins_grams[i]];

                // cout << j << " " << coins_val[i] << " " << j - coins_grams[i] << " " << max_dp[j - coins_grams[i]] << endl;
                long long using_current_coin_min = min_dp[j - coins_grams[i]];
                if(using_current_coin_min != 1000000000000000LL) {
                    // long long using_current_coin_min = coins_val[i] + tmp1;
                    using_current_coin_min += coins_val[i];
                    min_dp[j] = min(using_current_coin_min, min_dp[j]);
                }
                // else {
                    // continue;
                // }
                // min_dp[j] = min(using_current_coin_min, min_dp[j]);
                long long using_current_coin_max = max_dp[j - coins_grams[i]];
                if(using_current_coin_max != -1) {
                    // long long using_current_coin_max = coins_val[i] + tmp2;
                    using_current_coin_max += coins_val[i];
                    max_dp[j] = max(using_current_coin_max, max_dp[j]); // wybieranie jaka wartosc ma sie znalezc w tablicy
                }
                // max_dp[j] = max(using_current_coin_max, max_dp[j]); // wybieranie jaka wartosc ma sie znalezc w tablicy

            }  
        }
    }
    
    // cout << max_dp[box_weight] << endl;
    // cout << min_dp[box_weight] << endl;
    if(max_dp[box_weight] == -1) { // tutaj sprawdzamy czy z naszych monetek wypelnimy pudelko
        cout << "NIE";
        return 0;
    }

    // odtwarzanie jakie monetki
    int count_coins_min[coins_amount];
    int count_coins_max[coins_amount];
    for(int i = 0; i < coins_amount; i++) { // counter uzytych monetek
        count_coins_min[i] = 0;
        count_coins_max[i] = 0;
    }

    int i = coins_amount - 1;
    int j = box_weight;
    // cout << endl;
    while(j > 0) {
        // cout << j << endl;
        if(j >= coins_grams[i] and min_dp[j - coins_grams[i]] + coins_val[i] == min_dp[j]) {
            count_coins_min[i]++;
            j = j - coins_grams[i];
        }
        else {
            i--;
        }
    }


    i = coins_amount - 1;
    j = box_weight;
    // cout << endl;
    while(j > 0) {
        // cout << j << endl;
        if(j >= coins_grams[i] and max_dp[j - coins_grams[i]] + coins_val[i] == max_dp[j]) {
            count_coins_max[i]++;
            j = j - coins_grams[i];
        }
        else {
            i--;
        }
    }



    // cout << endl;
    // for(int i = 0; i <= box_weight; i++) {
    //     cout << max_dp[i] << " " ;
    // }


    cout << "TAK" << "\n";
    cout << min_dp[box_weight] << "\n";
    // for(int i = 0; i <= box_weight; i++) {
    //     cout << max_dp[i] << " ";
    // }
    // cout << endl;
    // cout << endl;
    // cout << endl;
    for(int i = 0; i < coins_amount; i++) {
        cout << count_coins_min[i] << " ";
    }
    cout << "\n";
    // cout << endl;
    // cout << endl;
    // cout << endl;
    cout << max_dp[box_weight] << "\n";

    for(int i = 0; i < coins_amount; i++) {
        cout << count_coins_max[i] << " ";
    }
    // for(int i = 0; i <= box_weight; i++) {
    //     cout << min_dp[i] << " ";
    // }


}
