#include<bits/stdc++.h>

/*
Stwórz macierze o rozmiarach odpowiednio
50×50, [done]
100×100 [done]
i 200×200 [done]
i wypełnij je losowymi wartościami z zakresu od 0.5 do 2.0. [done]
Następnie podnieś do kwadratu każdą z tych macierzy mierząc czas wykonania tych operacji. 
Pomiar czasu wykonaj za pomocą funkcji now() zegara o możliwie największej dokładności high_resolution_clock. 
Dla małych macierzy powtórz operację wielokrotnie a potem zmierzony czas podziel przez tą wielokrotność. 
Wyniki pomiarów przedstaw w milisekundach (typu całkowitego int) i minutach z dokładnością do 3 miejsc po kropce dziesiętnej (typu zmiennopozycyjnego double). 
*/

// https://www.geeksforgeeks.org/cpp/generate-a-random-float-number-in-cpp/
// https://www.geeksforgeeks.org/cpp/cpp-matrix-multiplication/
using namespace std;

void fillMatrix(float* matrix, int size) {
    static default_random_engine randGen; // sie raz zrobi wiec rozne liczby beda a nie caly czas ztego samego seed'a
    uniform_real_distribution<float> distribution(0.5, 2.0);
    for(int row = 0; row < size; row++) {
        for(int col = 0; col < size; col++) {
            // to sa floaty po koleji wpamieci a nie ladne 2D dlatego takie indeksowanie
            matrix[row * size + col] = distribution(randGen);
        }
    }
}

void matrixPow2(float* matrix, int size) {
    float tmp;
    for(int row = 0; row < size; row++) {
        for(int col = 0; col < size; col++) {
            tmp = 0.0f;
            for(int k = 0; k < size; k++) {
                tmp += matrix[row * size + k] * matrix[size * k + col];
            }
            matrix[row * size + col] = tmp;
        }
    }
}


int main() {
    float neo50[50][50];
    // [0][0] wskazujemy na pierwszy ze wszystkich
    fillMatrix(&neo50[0][0], 50);

    float trinity100[100][100];
    fillMatrix(&trinity100[0][0], 100);

    float morpheus200[200][200];
    fillMatrix(&morpheus200[0][0], 200);

    cout << "Start pomiaru dla kwadratu Neo: " << endl;
    chrono::high_resolution_clock::duration avgSum{0};
    
    for(int cnt = 0; cnt < 1000; cnt++) {
        auto poczatek = chrono::high_resolution_clock::now();
        matrixPow2(&neo50[0][0], 50);
        auto koniec = chrono::high_resolution_clock::now();
        avgSum += (koniec - poczatek);
    }

    // avgSum /= 1000;

    auto timeMiliSec = chrono::duration_cast<chrono::milliseconds>(avgSum);
    int timeMiliSecInt = static_cast<int>(timeMiliSec.count());
    auto timeMin = chrono::duration_cast<chrono::minutes>(avgSum);
    double timeMinDouble = static_cast<double>(timeMin.count());
    cout << "Koniec pomiaru dla kwadratu Neo: \n" 
    << "zajelo to this much [ms]: " << timeMiliSecInt
    << "\n oraz this much [min]: " << setprecision(6) << timeMinDouble 
    << endl; 
    cout << "-----------------------------" << endl;



    cout << "Start pomiaru dla kwadratu Trinity: " << endl;
    
    chrono::high_resolution_clock::duration avgSum2{0};
    
    for(int cnt = 0; cnt < 100; cnt++) {
        auto poczatek = chrono::high_resolution_clock::now();
        matrixPow2(&trinity100[0][0], 100);
        auto koniec = chrono::high_resolution_clock::now();
        avgSum2 += (koniec - poczatek);
    }

    avgSum2 /= 100;

    timeMiliSec = chrono::duration_cast<chrono::milliseconds>(avgSum2);
    timeMiliSecInt = static_cast<int>(timeMiliSec.count());
    timeMin = chrono::duration_cast<chrono::minutes>(avgSum2);
    timeMinDouble = static_cast<double>(timeMin.count());
    cout << "Koniec pomiaru dla kwadratu Trinity: \n" 
    << "zajelo to this much [ms]: " << timeMiliSecInt
    << "\n oraz this much [min]: " << setprecision(3) << timeMinDouble 
    << endl; 
    cout << "-----------------------------" << endl;



    cout << "Start pomiaru dla kwadratu Morfeusza: " << endl;
    
    auto poczatek = chrono::high_resolution_clock::now();
    matrixPow2(&morpheus200[0][0], 200);
    auto koniec = chrono::high_resolution_clock::now();
    auto roznica = koniec - poczatek;

    timeMiliSec = chrono::duration_cast<chrono::milliseconds>(roznica);
    timeMiliSecInt = static_cast<int>(timeMiliSec.count());
    timeMin = chrono::duration_cast<chrono::minutes>(roznica);
    timeMinDouble = static_cast<double>(timeMin.count());
    cout << "Koniec pomiaru dla kwadratu Neo: \n" 
    << "zajelo to this much [ms]: " << timeMiliSecInt
    << "\n oraz this much [min]: " << setprecision(3) << timeMinDouble
    << endl; 
    cout << "-----------------------------" << endl;

}