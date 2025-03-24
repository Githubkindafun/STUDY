#include <iostream>
#include <string>
#include <cmath>
#include <vector>



using namespace std;

class Wyrazenie {
public:
    virtual double oblicz() = 0;
    // tu poporostu mowimy ze jest ale nic o tym nie mowimy
    virtual string zapis() = 0;
};

class Liczba : public Wyrazenie {
public:
    double val;

    Liczba(double given_num) {
        this->val = given_num;
    }
    
    double oblicz() {
        return this->val;
    }

    string zapis() {
        string res = "";
        res = to_string(val);
        return res;
    }
};

class Zmienna : public Wyrazenie {
    vector<pair<string, double>> zmienne;

public:
    string nazwa;

    Zmienna(string given_name) {
        this->nazwa = given_name;
    }

    void insert(string given_name, double given_val) {
        int len = zmienne.size();

        int i = 0;
        while (i < len) {
            if (given_name == zmienne[i].first) cout << "to już mamy (insert)" << endl;
            i++;
        }
        zmienne.push_back(make_pair(given_name, given_val));
    }

    void change(string given_name, double given_val) {
        int len = zmienne.size();

        int i = 0;
        while (i < len) {
            if (given_name == zmienne[i].first) {
                zmienne[i].second = given_val;
                cout << "Spoczko wszystko sie zgadza zamieniłem co tam miałem zamienić :>";
            }
        }
        throw("No niestery nie znalzałem tego co miałem zamienić weź sprawdź prosze czy podałeś poprawne dane :c");
    }

    void remove(string given_name) {
        int len = zmienne.size();

        //tutaj sie przyznaje ze niemiałem zupełnie pojęcia 
        //co za błąd mi wyrzucało więc szukałem w internecie 
        //i jedyne co znalazłem zeby zrobic to w ten sposob bo nic
        //nie rozwiązywało problemu z typami przy erase
        for (auto it = zmienne.begin(); it != zmienne.end(); ++it) {
            if (it->first == given_name) {
                zmienne.erase(it);
                cout << "Remove done!" << endl;
                break;
            }
        }
        throw("Nie znalazłem elementu ktory mialem usunac sprawdz podane dane");
    }

    string zapis() {
        return nazwa;
    }

    double oblicz() {
        int len = zmienne.size();

        int i = 0;
        while (i < len) {
            if (nazwa == zmienne[i].first) return zmienne[i].second;
            i++;
        }
        throw("Error!? (zapis w zmienne)");
    }
};
 
class Stala : public Wyrazenie {
protected: //szczerze nwm czy cos ma tu byc więcej ;/
    double val;
};

class Pi : public Stala {
public:
    Pi() {
        this->val = 3.1415;
    }

    string zapis() {
        return "pi";
    }

    double oblicz() {
        return val;
    }
};

class E : public Stala {
public:
    E() {
        this->val = 2.7182;
    }

    string zapis() {
        return "e";
    }

    double oblicz() {
        return val;
    }
};

class Fi : public Stala {
public:
    Fi() {
        this->val = 1.61803;
    }

    string zapis() {
        return "fi";
    }

    double oblicz() {
        return val;
    }
};
//koniec operandów
//pora na operatory

// operatory

//1 arg

 class Operator1arg : public Wyrazenie {
 public:
     Wyrazenie *wyr;

     Operator1arg(Wyrazenie *given_wyr) {
         this->wyr = given_wyr;
     }
};

 class Sinus : public Operator1arg {
 public:
     Sinus(Wyrazenie *given_wyr) {
        this->wyr = given_wyr;
     }

     string zapis() {
         return ""; // tu koncze
     }
 };











int main()
{
    std::cout << "Hello World!\n";
}

