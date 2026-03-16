// Stwórz w swoim programie trzy zbiory danych różnych typów: vector<double>, list<int>
// i set<string>. Wypełnij te kontenery przypadkowymi wartościami (możesz losować albo
// arbitralnie coś wpisać). [done]


// Następnie na zbiorach tych wykonaj pewne obliczenia z użyciem
// zdefiniowanych wcześniej funktorów (nie używaj lambd):

// Wypisz wszystkie wartości z zadanego zakresu (większe od a i mniejsze od b).
// Wypisz co k-tą wartość zaczynając od pozycji p-tej.
// Wyznacz element minimalny i maksymalny (zwróć parę wartości).
// Wyznacz sumę (albo konkatenację dla łańcuchów znakowych) wszystkich elementów.

#include <bits/stdc++.h>

using namespace std;

// Wypisz wszystkie wartości z zadanego zakresu (większe od a i mniejsze od b).
template<typename T>
struct rangeAToB {
    T a;
    T b;
    // const bo set sie splakal
    void operator () (const T& input) {
        if(a < input && input < b) {
            cout << input << endl;
        }
    }
};

// Wypisz co k-tą wartość zaczynając od pozycji p-tej.

// zakladam ze p i k to int bo nie ma co sie bawic w inne
template<typename T>
class fromPForEveryK {
private:
    int k;
    int p;
    int iteration = 0;
public:
    fromPForEveryK(int kInput, int pInput) {
        if(pInput < 0 or kInput < 0) throw invalid_argument("ziomek p i k musza byc wieksze od 0");

        k = kInput;
        p = pInput;
    }
    void operator () (const T& input) {
        if(iteration >= p and (iteration % k) == (p % k)) {
            cout << input << endl;
        }
        iteration++;
    }
};
// Wyznacz element minimalny i maksymalny (zwróć parę wartości).
template<typename T>
// https://en.cppreference.com/w/cpp/utility/optional.html
class returnLimits {
private:
    T max = numeric_limits<T>::lowest();
    T min = numeric_limits<T>::max();
    int alreadyAnalyzed = 0;
    int size;
public:
    returnLimits(int inputSize) {
        size = inputSize;
    }
    optional<pair<T,T>> operator () (const T& input) {
        if(input < min) {
            min = input;
        }
        if(input > max) {
            max = input;
        }
        alreadyAnalyzed++;
        if(alreadyAnalyzed == size) {
            return pair<T,T>(min, max);
        }
        return nullopt;
    }
};
// Wyznacz sumę (albo konkatenację dla łańcuchów znakowych) wszystkich elementów.
template<typename T> 
struct glueTogether {
    T losResultos{};
    void operator () (const T& input) {
        losResultos = losResultos + input;
    }
};

int main() {

    vector<double> doubleTrouble = {1.23, 2.34, 3.45, 4.56, 1.32, 2.43, 3.54, 4.65};
    list<int> mostWanted = {1, 2, 3, 4, 3, 2, 1, 4};
    set<string> setAside = {"abc", "a", "acb", "b", "bca", "c", "bac"};

    // cout << "doubleTrouble rangeAToB" << endl;
    // for_each(doubleTrouble.begin(), doubleTrouble.end(), rangeAToB<double>{2.0, 4.0});

    // cout << "mostWanted rangeAToB" << endl;
    // for_each(mostWanted.begin(), mostWanted.end(), rangeAToB<int>{2, 4});

    // cout << "setAside rangeAToB" << endl;
    // for_each(setAside.begin(), setAside.end(), rangeAToB<string>{"a", "bac"});

    // -----

    // cout << "doubleTrouble fromPForEveryK" << endl;
    // fromPForEveryK<double> dbPK{2, 2};
    // for_each(doubleTrouble.begin(), doubleTrouble.end(), dbPK);

    // cout << "mostWanted fromPForEveryK" << endl;
    // fromPForEveryK<int> mwPK{2, 2};
    // for_each(mostWanted.begin(), mostWanted.end(), mwPK);

    // cout << "setAside fromPForEveryK" << endl;
    // fromPForEveryK<string> saPK{2, 2};
    // for_each(setAside.begin(), setAside.end(), saPK);

    // -----
    cout << "doubleTrouble returnLimits" << endl;
    returnLimits<double> dbLimits(doubleTrouble.size());
    optional<pair<double, double>> dbRes;
    for(auto& input : doubleTrouble) {
        if(auto potentialRes = dbLimits(input)) {
            dbRes = potentialRes;
        }
    }
    cout << dbRes->first << " " << dbRes->second << endl;

    cout << "mostWanted returnLimits" << endl;
    returnLimits<int> mwLimits(mostWanted.size());
    optional<pair<int, int>> mwRes;
    for(auto& input : mostWanted) {
        if(auto potentialRes = mwLimits(input)) {
            mwRes = potentialRes;
        }
    }
    cout << mwRes->first << " " << mwRes->second << endl;

    cout << "setAside returnLimits" << endl;
    returnLimits<string> saLimits(setAside.size());
    optional<pair<string, string>> saRes;
    for(auto& input : setAside) {
        if(auto potentialRes = saLimits(input)) {
            saRes = potentialRes;
        }
    }
    cout << saRes->first << " " << saRes->second << endl;



    // -----
    // glueTogether<double> dbGlue;
    // dbGlue = for_each(doubleTrouble.begin(), doubleTrouble.end(), dbGlue);
    // cout << "doubleTrouble glueTogether" << endl;
    // cout << dbGlue.losResultos << endl;

    // glueTogether<int> mwGlue;
    // mwGlue = for_each(mostWanted.begin(), mostWanted.end(), mwGlue);
    // cout << "mostWanted glueTogether" << endl;
    // cout << mwGlue.losResultos << endl;

    // glueTogether<string> saGlue;
    // saGlue = for_each(setAside.begin(), setAside.end(), saGlue);
    // cout << "setAside glueTogether" << endl;
    // cout << saGlue.losResultos << endl;
}