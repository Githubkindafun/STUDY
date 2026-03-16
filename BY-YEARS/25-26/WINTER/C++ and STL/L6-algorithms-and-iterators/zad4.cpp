#include <bits/stdc++.h>

/*
Napisz program, który oblicza i wyświetla histogram zawierający częstość występowania liter alfabetu angielskiego 
(bez rozróżniania małych i dużych liter) w zadanym tekście. 
Tekst pobierz z pliku, którego nazwę przekażesz do programu poprzez argumenty wywołania. 
Częstość występowania danej litery jest zdefiniowana jako proporcja liczby wystąpień tej litery w 
stosunku do wszystkich liter w tekście (częstość musi być określona na podstawie liczby liter a nie wszystkich znaków w tekście). 
Wykorzystaj w swoim programie iterator strumieniowy.
*/

using namespace std;

int main(int argc, char* argv[]) {
    
    // tam te error checki z command line skip bo to dla mnie
    // wiec ja wiem jak to odpalic (chyba)

    ifstream input(argv[1]);
    int letters[26] = {0};
    long allOfThemLetters = 0;
    long long allOfThemChars = 0;
    // podkradzione z wykladu
    istream_iterator<char> it_cin {input}; 
    istream_iterator<char> end_cin;
    while (it_cin != end_cin) {
        unsigned char losLetteros = (unsigned char)(*it_cin);
        // z zadania z lekserem
        if(isalpha(losLetteros)) {
            losLetteros = tolower(losLetteros);
            int letterIdx = losLetteros - 'a';
            letters[letterIdx]++;
            allOfThemLetters++;
        }
        it_cin++;
        allOfThemChars++;
    }
    for(int i = 0; i < 26; i++) {
        char currLetter = char('a' + i);
        cout << currLetter << " wystapila: " << letters[i] << " co daje czestotliwosc na poziomie " << (double)(letters[i]) / allOfThemLetters << endl; 
    }
    
    cout << "wszystkich znakow: " << allOfThemChars << " wszystkich liter: " << allOfThemLetters << endl; 

}