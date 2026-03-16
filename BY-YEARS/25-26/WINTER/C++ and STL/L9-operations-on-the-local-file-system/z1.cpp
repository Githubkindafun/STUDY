#include <bits/stdc++.h>

using namespace std;

int main() {
    ifstream input("liczby.txt");
    istream_iterator<int> inputIterator(input);
    istream_iterator<int> inputIteratorEOF;
    vector<int> numeros;
    while(inputIterator != inputIteratorEOF) {
        int readNumeros = *inputIterator;
        numeros.push_back(readNumeros);
        inputIterator++;
    }

    // bang liczymy srednia
    float avg = 0.0;
    for(int i = 0; i < numeros.size(); i++) {
        avg += numeros[i];
    }
    avg /= numeros.size();

    // bang bang liczymy odchylenie standardowe
    // https://docs.vultr.com/cpp/examples/calculate-standard-deviation
    float sum = 0.0;
    float mean = 0.0;
    float sieOdcylilemDoscStandardowo = 0.0;
    for(int val : numeros) {
        sum += val;
    }
    mean = sum / numeros.size();
    for(int val : numeros) {
        sieOdcylilemDoscStandardowo += pow(val - mean, 2);
    }
    float standardDeviation = sqrt(sieOdcylilemDoscStandardowo / numeros.size());

    cout << "Avg: " << avg << "\nSD: " << standardDeviation << endl;
}