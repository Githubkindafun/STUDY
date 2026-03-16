// Zaimplementuj funktor do składania funkcji poprzez wykonywanie ich po kolei.

// Funkcja po_kolei(f1, f2)(x) powinna wykonać najpierw funkcję f1(x) a potem f2(x).
// Wartości zwracane przez obie funkcje mają być ignorowane. Funkcja po_kolei() powinna
// zwracać taki funktor, aby możliwe było dalsze składanie funkcji, na przykład:
// po_kolei(po_kolei(f1, f2), f3)(x). 

// Podaj przykład użycia takiego funktora.

#include <bits/stdc++.h>

using namespace std;

// tu mamy nasze funktory 
// dla kazdego mamy jednego inta 
// i operator () dzieki czemu mozemy 
// uzywac ich jak funkcji
struct add{
    int valueToBeAdded;
    // const tutaj jest kluczowy bo pozwala wywolac funkcje bez 
    // modyfikowania stanu funktora
    void operator () (int& input) const {
        input += valueToBeAdded;
    }
};

struct sub{
    int valueToBeSubstructed;
    void operator () (int& input) const {
        input -= valueToBeSubstructed;
    }
};

struct mul{
    int valueToBeMul;
    void operator () (int& input) const {
        input *= valueToBeMul;
    }
};

struct divide{
    int valueToBeDiv;
    void operator () (int& input) const {
        input /= valueToBeDiv;
    }
};

// i po co nam template<...> ?
// zasadniczo dzięki temu po_koleji staje się uniwersalne
// bez templatki wymagane by bylo od nas aby wskazac konkretne typy 
// dla kazdego przypadku

// [capture] (parameters) { code }; 
// https://www.w3schools.com/cpp/cpp_functions_lambda.asp
template<class Op1, class Op2>
auto po_kolei(Op1 op1, Op2 op2) {
    // czyli tak mamy w lambdzie obiekty op1 i op2 (skopiowane)
    // argumentem naszej lambdy bedzie auto& input
    // w ciele funkcji wykonuje sie op1 i op2
    return [op1, op2](auto& input) {
        op1(input);
        op2(input);
    };
}
// ... dziala na zasadzie ze dowolna liczba 
// dodatkowych typów i są zapakowane w Opx
// albo analogicznie dowolna liczba doatkowych arg w funkcji
// albo rozwniecie paczki argumentow jak w lini nizej
template<class Op1, class Op2, class ... Opx>
auto po_kolei(Op1 op1, Op2 op2, Opx ... opx) {
    return po_kolei(po_kolei(op1, op2), opx ...);
}


int main() {

    vector<int> testSubjects = {4, 2, 0};
    
    add add4{4};
    sub sub2{2};
    mul mul6{6};
    divide div9{9};

    auto rock = po_kolei(add4, mul6);
    auto nroll = po_kolei(sub2, div9);
    auto rockNroll = po_kolei(rock, nroll);

    for_each(testSubjects.begin(), testSubjects.end(), rockNroll);
    auto head = testSubjects.front();
    while(!testSubjects.empty()) {
        cout << head << endl;
        testSubjects.erase(testSubjects.begin());
        head = testSubjects.front();
    }

}