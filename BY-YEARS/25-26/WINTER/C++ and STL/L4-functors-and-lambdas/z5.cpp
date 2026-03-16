// Zdefiniuj szablon lambdy []<typename T>, której argumentem jest jakaś kolekcja przechowująca 
// obiekty typu T a wynikiem wartość typu T. Podaj przykład jej zastosowania. 

#include <bits/stdc++.h>

using namespace std;

int main() {
    // <typename T> daje to ze lambda ma "wlasny typ" T
    // kompilator sb wydedukuje jakim typem jest T no i klasa
    auto whateverLambda = []<typename T>(vector<T>& whatever) -> T {
        T lastElement = whatever.back();
        whatever.pop_back();
        return lastElement;
    };

    vector<int> vectorOfInts = {4, 2, 0, 42};
    vector<float> vectorOfFloats = {4.2f, 2.4f, 0.5f, 42.42};
    vector<string> vectorOfStrings = {"4.2f", "2.4f", "0.5f", "42.42f"};

    cout << "ostatni element vectorOfInts = " << vectorOfInts.back() << " = "; 
    int i = whateverLambda(vectorOfInts);
    cout << i << " a teraz po usunieciu ostatni element rowna sie = " << vectorOfInts.back() << endl;
    
    cout << "ostatni element vectorOfFloats = " << vectorOfFloats.back() << " = "; 
    float f = whateverLambda(vectorOfFloats);
    cout << f << " a teraz po usunieciu ostatni element rowna sie = " << vectorOfFloats.back() << endl;
    
    cout << "ostatni element vectorOfStrings = " << vectorOfStrings.back() << " = "; 
    string s = whateverLambda(vectorOfStrings);
    cout << s << " a teraz po usunieciu ostatni element rowna sie = " << vectorOfStrings.back() << endl;
    
}