#include <bits/stdc++.h>

using namespace std;


bool areYouACouple(char uno, char dos) {
    return ((uno == dos) && (uno == '=' || uno == '&' || uno == '|')) 
            || ( uno == '!' && dos == '=')
            || ( uno == '<' && dos == '=')
            || ( uno == '>' && dos == '=');
             
}

int whoAreYou(unsigned char uno, char dos) {
    if(isspace(uno)) {
        // jak sama nazwa wskazuje
        return 1;
    }
    if(isdigit(uno)) {
        // jak sama nazwa wskazuje
        return 2;
    }
    if(isalpha(uno)) {
        // literki
        return 3;
    }
    if(areYouACouple((char)uno, dos)) {
        return 4;
    }
    return 5;
}

forward_list<span<char>> processText(string& input) {
    forward_list<span<char>> leksems;

    int inputSize = input.size();
    int inputIdx = 0;


    // basicly helper lambda do dodawania nowego leksema do listy
    // .data() + start - wskaznik na pierwszy znak
    // lambda bascly korzysta z leksems i inputu by reference nie kopiujac tych rzeczy
    // na pdoastawie przekazanych elementow start i end dodaje do leksems odpowiedniego spana
    auto addProcessedLeksem = [&](int start, int end) {
        leksems.push_front(span<char>(input.data() + start, end - start));
    };

    while(inputIdx < inputSize) {
        // https://en.cppreference.com/w/cpp/string/byte/isspace.html
        // metody do sprawdzania tak sie prezentuja
        unsigned char charahter = (unsigned char)input[inputIdx];
        char charahterNext = ' ';
        if(inputIdx + 1 < inputSize) {
            charahterNext = input[inputIdx + 1];
        }
        // no i teraz analiza 
        int losTyposDelCharachteros = whoAreYou(charahter, charahterNext);
        int start = 0;
        switch (losTyposDelCharachteros) {
            case 1:
                inputIdx++;
                break;
            case 2:
                start = inputIdx;
                while(inputIdx < inputSize && isdigit(char(input[inputIdx]))) {
                    inputIdx++;
                }
                addProcessedLeksem(start, inputIdx);
                break;
            case 3:
                start = inputIdx;
                inputIdx++;
                while(inputIdx < inputSize) {
                    unsigned char nextInCheck = (unsigned char)input[inputIdx];
                    if(isalnum(nextInCheck)) {
                        inputIdx++;
                    } else {
                        break;
                    }
                }
                addProcessedLeksem(start, inputIdx);
                break;
            case 4:
                start = inputIdx;
                inputIdx += 2;
                addProcessedLeksem(start, inputIdx);
                break;
            case 5:
            default:
                start = inputIdx;
                inputIdx++;
                addProcessedLeksem(start, inputIdx);
                break;
        }
    }
    leksems.reverse();
    return leksems;
}

int main() {
    string inputText = "Los Pollos Hermanos, where something delicious is always cooking, a == 42 || b >= 21.";
    // string inputText = " I've seen things you people wouldn't believe. Attack ships <> on fire off || the shoulder of Orion. I watched >= C-beams == glitter in the dark 21 near the Tannhäuser && Gate. All those moments will be 42 lost in time, like tears in rain. Time to die,";
    forward_list<span<char>> leksems = processText(inputText);

    for(span<char> leksem : leksems) {
        cout << "(";
        for(char charahter : leksem) {
            cout << charahter;
        }
        cout << ")\n";
    }
}
