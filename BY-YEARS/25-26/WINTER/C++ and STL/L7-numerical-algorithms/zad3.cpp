#include <bits/stdc++.h>

using namespace std;

const double losLetterosNumeros[23] = {
    // jako osoba z lekutka dysleksja byl to horror
    0.0837, // a
    0.0193, // b
    0.0389, // c
    0.0335, // d
    0.0868, // e
    0.0026, // f
    0.0146, // g
    0.0125, // h
    0.0883, // i
    0.0228, // j
    0.0301, // k
    0.0224, // l
    0.0281, // m
    0.0569, // n
    0.0753, // o
    0.0287, // p 
    0.0415, // r
    0.0413, // s
    0.0385, // t
    0.0206, // u
    0.0411, // w
    0.0403, // y
    0.0533, // z
};
// to sie nie sumuje do 1 tylko do 0.9211
double TOTAL_P = 0.0;
// wiec jak cos poza tym sie wylosuje to rerola zrobimy i bedzie git
const char losLetterosCharos[23] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
    'm', 'n', 'o', 'p', 'r', 's', 't', 'u', 'w', 'y', 'z' 
};
// horror ponownie

// przypomnialem sobie ostatnio ze tak ladnie mozna robic
// ze sb prototypy piszemy, i like it
array<double, 23> cumulativeSumArray();
char rollForLetter(default_random_engine& dre, const array<double, 23>);


int timeSeed(); 


int main(int argc, char* argv[]) {
    // error check powinien byc ale czas goni

    
    string fileName = argv[1];
    ofstream outputFile(fileName);
    
    int textLength = stoi(argv[2]);
    
    int rndSeed = timeSeed(); 
    default_random_engine dre(rndSeed);
    binomial_distribution distribution(12, 0.5);
    array<double, 23> cArray = cumulativeSumArray();
    TOTAL_P = cArray.back();

    int endliner = 0;
    while(textLength > 0) {
        int wordLength = distribution(dre) + 1;
        if(wordLength > textLength) {
            // no bo jeszcze spacja
            wordLength = textLength - 1;
        }
        endliner += wordLength + 1;
        for(int i = 0; i < wordLength; i++) {
            char letteros = rollForLetter(dre, cArray);
            outputFile << letteros;
        }
        outputFile << " ";
        textLength -= wordLength + 1;
        
        if(endliner > 100) {
            endliner = 0;
            outputFile << endl;
        }
    }

    return 0;
}

int timeSeed() {
    // https://www.geeksforgeeks.org/cpp/localtime-function-in-c/
    time_t time_ptr;
    time_ptr = time(NULL);
    tm* tm_local = localtime(&time_ptr);

    int timeVal = tm_local->tm_hour + tm_local->tm_min + tm_local->tm_sec;
    return timeVal;
}

// aby jak kolwiek sensownie to losowac i nie zrobic sobie krzywdy ponownie
// wygondym rozwiazaniem wydaje sie suma kumulacyjna 
array<double, 23> cumulativeSumArray() {
    array<double, 23> res = {};
    double cumulation = 0.0;
    for(int i = 0; i < 23; i++) {
        cumulation += losLetterosNumeros[i];
        res[i] = cumulation;
    }
    return res;
}

char rollForLetter(default_random_engine& dre, const array<double, 23> cArray) {
    
    uniform_real_distribution distribution(0.0, TOTAL_P);

    double prob = distribution(dre);

    
    for(int i = 0; i < 23; i++) {
        if(prob < cArray[i]) {
            return losLetterosCharos[i];
        }
    }
    cout << "ziomek kodzik nie dziala jednak" << endl;
    return '?';
}


