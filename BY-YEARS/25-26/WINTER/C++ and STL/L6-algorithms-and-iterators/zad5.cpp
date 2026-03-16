/*
Napisz funkcję, która wygeneruje wszystkie możliwe permutacje liter w słowie umieszczonym w obiekcie typu string. 
Zauważ, że takie same litery występujące w słowie są nierozróżnialne. 
*/
#include <bits/stdc++.h>

using namespace std;

int main() {

    // swoja droga bardzo ladne slowo
    string textToDecompose = "equinox";
    int letters[26] = {0};

    // wstepna analiza jakie literki sie pojawily
    for(int i = 0; i < textToDecompose.size(); i++) {
        unsigned char losLetteros = (unsigned char)textToDecompose[i];
        if(isalpha(losLetteros)) {
            losLetteros = tolower(losLetteros);
            letters[losLetteros - 'a']++;
        }
    }

    // teraz zrobimy sobie wektor z literkami
    vector<char> lettersWeFound;
    for(int i = 0; i < 26; i++) {
        if(letters[i] > 0) {
            lettersWeFound.emplace_back(char('a' + i));
        }
    }

    for(int i = 0; i < lettersWeFound.size(); i++) {
        cout << lettersWeFound[i] << " "; 
    }

    // https://www.geeksforgeeks.org/cpp/how-to-shuffle-a-vector-in-cpp/
    random_device rd;
    mt19937 g(rd());

    // Shuffle the vector
    shuffle(lettersWeFound.begin(), lettersWeFound.end(), g);
    cout << "\nAfter shuffle\n";
    for(int i = 0; i < lettersWeFound.size(); i++) {
        cout << lettersWeFound[i] << " ";
    }
}