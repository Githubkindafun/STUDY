// Zdefiniuj rekurencyjną lambdę do wyliczenia współczynnika dwumianowego (nk) tylko dla naturalnych wartości n i k.
// Następnie przetestuj tą lambdę na kilku niedużych wartościach umieszczonych w kolekcji par vector<pair<int,int>>
// (umieść tam na ostatniej pozycji parę z jakąś liczbą ujemną, aby zaobserwować zgłoszenie wyjątku).


#include <bits/stdc++.h>

using namespace std;
// z wykladu
// std::function<int(int)> factorial;
// int(int) - pierwszy int to jaki jest zwracany typ drugi to jaki przyjmuje typ

int main() {

    // to jest takie pudelko aby lambda mogla sie wywolac rekurencyjie
    // sama nie ma nazwy wiec musi miec do czego sie odwolac
    function<int(int, int)> newtonsSymbol;

    // dobra notka [] to sa przechwycone rzeczy
    // roznia sie tym od parametrow ze parametry mozemy podawac
    // za kazdym razem inne a te zostaja takie jakie sa w momencie 
    // utworzenia lambdy
    // sam snipet to poprostu przerobiony kod z wykladu
    newtonsSymbol = [&newtonsSymbol](int n, int k) -> int {
        if(n < 0 or k < 0) throw invalid_argument("n oraz k musza byc liczbami wiekszymi od 0 (naturalnymi)");
        if(n < k) return 0;
        if(k == 0) {
            return 1;
        } else if(k == n) {
            return 1;
        }
        return newtonsSymbol(n - 1, k - 1) + newtonsSymbol(n - 1, k);
    };

    vector<pair<int, int>> testNewton = {{2, 1}, {5, 4}, {1, 7}, {1, 1}, {7, 0}, {-3, -5}};

    for(auto& [n, k] : testNewton) {
        cout << " Wartosc wspolczynnika dwomianowego dla n = " << n << " i dla k = " << k << " wynosi = ";
        try {
            int newtonRes = newtonsSymbol(n, k);
            cout << newtonRes << endl;
        } catch(exception& e) {
            cout << "los errors: " << e.what() << endl;
        } catch(...) {
            cout << "nie dziala i tyle ;>" << endl;
        }
    }
}