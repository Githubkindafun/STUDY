/*
Pojedyncza osoba jest opisana za pomocą:
imienia (string), 
nazwiska (string), 
wieku (int), 
wagi (int) mierzonej w kilogramach 
i wzrostu (double) mierzonego w metrach
zdefiniuj klasę person reprezentującą osobę. Dane o 12 osobach umieść w deku deque<person>. 
*/

#include <bits/stdc++.h>

using namespace std;

class person {
private:
    string name;
    string surname;
    int age;
    int weight;
    double height;

public:
    person(string n, string sn, int a, int w, double h) {
        name = n;
        surname = sn;
        age = a;
        weight = w;
        height = h;
    }

    string getName() {
        return name;
    }
    string getSurname() {
        return surname;
    }
    int getAge() {
        return age;
    }
    int getWeight() {
        return weight;
    }
    double getHeight() {
        return height;
    }
    void setName(string newName) {
        name = newName;
    }
    void setSurname(string newSurname) {
        surname = newSurname;
    }
    void setAge(int newAge) {
        age = newAge;
    }
    void setWeight(int newWeight) {
        weight = newWeight;
    }
    void setHeight(double newHeight) {
        height = newHeight;
    }
};



int main() {
    deque<person> pple;
    pple.emplace_back("Frodo", "Baggins", 50, 80, 1.10);
    pple.emplace_back("Samwise", "Gamgee", 45, 110, 1.15);
    pple.emplace_back("Meriadoc", "Brandybuck", 37, 80, 1.08);
    pple.emplace_back("Peregrin", "Took", 35, 75, 1.15);
    pple.emplace_back("Gandalf", "The Grey", 4200, 85, 2.00);
    pple.emplace_back("Aragorn", "Elessar", 55, 75, 1.85);
    pple.emplace_back("Legolas", "Greenleaf", 420, 65, 1.90);
    pple.emplace_back("Gimli", "Gloisson", 85, 150, 1.40);
    pple.emplace_back("Boromir", "Of Gondor", 41, 90, 1.85);
    pple.emplace_back("Faramir", "Of Gondor", 38, 80, 1.85);
    pple.emplace_back("Eowym", "Of Rohan", 30, 60, 1.70);
    pple.emplace_back("Galadriel", "Of Lorien", 2500, 60, 1.78);

    /*
    a)
    posortuj osoby według współczynnika BMI (funkcją porównującą niech będzie lambda); 
    posortowaną grupę osób wypisz na standardowym wyjściu; 
    */
    // wg przegladarki BMI = masa ciała (kg) / (wzrost (m))^2
    cout << "a)\n";
    auto bmiCmp = [](person& pplA, person& pplB) {
        double bmiOfA = (double)(pplA.getWeight() / pow(pplA.getHeight(), 2));
        double bmiOfB = (double)(pplB.getWeight() / pow(pplB.getHeight(), 2));
        return bmiOfA < bmiOfB;
    };
    sort(pple.begin(), pple.end(), bmiCmp);
    for(auto& ppl : pple) {
        double pplBmi = ppl.getWeight() / pow(ppl.getHeight(), 2);
        cout << ppl.getName() << " " << pplBmi << endl;
    }
    cout << "\n";
    /*
    b)
    osoby odchudziły się na obozie sportowym i straciły 10% swojej pierwotnej wagi (funkcją modyfikującą niech będzie lambda);
    zmodyfikowaną grupę osób wypisz na standardowym wyjściu; 
    */
    cout << "b)\n";
    auto sportsCamp = [](person& ppl) {
        int changedW = ppl.getWeight() - (ppl.getWeight() * 0.1);
        ppl.setWeight(changedW);
    };
    for(auto& ppl : pple) {
        cout << ppl.getName() << " " << ppl.getWeight() << endl;
    }
    for_each(pple.begin(), pple.end(), sportsCamp);
    for(auto& ppl : pple) {
        cout << ppl.getName() << " " << ppl.getWeight() << endl;
    }
    cout << "\n";
    /*
    c)
    podziel osoby na dwie grupy: ciężkie z wagą powyżej 100[kg] i lekkie z wagą nie większą niż 100[kg] (użyj lambdy jako predykatu); 
    wypisz grupę osób po podziale na standardowym wyjściu; 
    */
    cout << "c)\n";
    auto isFat = [](person& ppl) {
        return ppl.getWeight() > 100;
    };
    cout << "Heavy ppl:\n";
    for(auto& ppl : pple) {
        if(isFat(ppl)) {
            cout << ppl.getName() << " " << ppl.getWeight() << endl;
        }
    }
    cout << "~Heavy ppl:\n";
    for(auto& ppl : pple) {
        if(!isFat(ppl)) {
            cout << ppl.getName() << " " << ppl.getWeight() << endl;
        }
    }

    
    /*
    d)
    wypisz osobę najstarszą i najmłodszą na standardowym wyjściu (nie korzystaj z sortowania danych). 
    */

    cout << "d)\n";
    auto weigthCmp = [](person& pplA, person& pplB) {
        return pplA.getWeight() < pplB.getWeight();
    };
    auto minItW = min_element(pple.begin(), pple.end(), weigthCmp);
    auto maxItW = max_element(pple.begin(), pple.end(), weigthCmp);

    cout << "Min waga " << minItW->getName() << " " << minItW->getWeight() << endl;
    cout << "Max waga " << maxItW->getName() << " " << maxItW->getWeight() << endl;

    
}