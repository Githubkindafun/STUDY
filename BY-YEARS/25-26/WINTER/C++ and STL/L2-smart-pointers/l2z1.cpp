/*
Zdefiniuj prostą klasę zawierającą licznik typu uint64_t, początkowo ustawiony na wartość 1.
W kasie tej, oprócz innych funkcjonalności, umieść: wirtualny destruktor, który wypisze komunikat ze
stanem licznika na standardowym wyjściu dla błędów cerr.

Następnie stwórz n-elementową tablicę takich liczników i opakuj ją wskaźnikiem unique_ptr
(opakowanie ma dotyczyć tablicy a nie pojedynczych liczników).

Napisz też funkcję, która wywoła się rekurencyjnie m razy z argumentem będącym wskaźnikiem na
taką tablicę (wskaźnik unique_ptr ustaw za pomocą przeniesienia). W i-tym wywołaniu funkcja ta
ma zwiększyć o wartość i 10% losowo wybranych elementów.

W programie głównym wywołaj tą funkcję, a po jej wywołaniu wypisz wartości wszystkich n liczników
umieszczonych w tablicy. Parametry n i m należy ustalić w programie arbitralnie (na przykład n = 40 i
m = 20).
*/
#include <bits/stdc++.h>
#include <random>

using namespace std;


class SimpleCounter {
protected:
    uint64_t cnt;
public:
    SimpleCounter() {
        cnt = 1;
    }

    virtual ~SimpleCounter() {
        cerr << "cnt = " << cnt << " przy wywolaniu dekonstruktora" << endl;
    }

    void incCounter(int val) {
        cnt += (uint64_t)val;
    }

    uint64_t getCounterValue() {
        return cnt;
    }

};

unique_ptr<SimpleCounter[]> funRec(unique_ptr<SimpleCounter[]> recArray, int size, int step, int maxSteps, mt19937& rd) {
    if(step > maxSteps) {
        return move(recArray);
    }
    int countersToModifyAmount = (size + 9) / 10;
    
    // https://cplusplus.com/forum/general/170805/
    vector<int> idxs(size);
    iota(idxs.begin(), idxs.end(), 0);
    // random_device rd;
    https://stackoverflow.com/questions/39288595/why-not-just-use-stdrandom-device
    
    shuffle(idxs.begin(), idxs.end(), rd);

    for(int i = 0; i < countersToModifyAmount; i++) {
        recArray[idxs[i]].incCounter(step);
    }
    return funRec(move(recArray), size, step + 1, maxSteps, rd);
}

// Wskaźnik unique_ptr<> o wartości innej niż nullptr zawsze posiada to, co wskazuje. 

int main() {

    int n = 10;
    int m = 16;
    mt19937 rd(42);
    unique_ptr<SimpleCounter[]> ArrayOfCounters { new SimpleCounter[n]};
    ArrayOfCounters = funRec(move(ArrayOfCounters), n, 1, m, rd);

    for(int i = 0; i < n; i++) {
        cout << "Counter nr. " << i + 1 << " = " << ArrayOfCounters[i].getCounterValue() << endl;
    }

    return 0;
}