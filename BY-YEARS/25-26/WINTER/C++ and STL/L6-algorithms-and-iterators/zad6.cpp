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
    string getInicials() {
        string inicials = "";
        inicials.push_back(name[0]);
        inicials.push_back(surname[0]);
        return inicials;
    }
    double getBmi() {
        return double(weight) / pow(height, 2);
    }
};

tuple<string, int, double> getT1(person& ppl) {
    return make_tuple(ppl.getInicials(), ppl.getAge(), ppl.getBmi());
}

tuple<int, int, double> getT2(person& ppl) {
    return make_tuple(ppl.getAge(), ppl.getWeight(), ppl.getHeight());
}

tuple<string, string, int> getT3(person& ppl) {
    return make_tuple(ppl.getName(), ppl.getSurname(), ppl.getAge());
}

int main() {
    deque<person> pple;
    person ppl1("Frodo", "Baggins", 50, 80, 1.10);
    
    // 1) inicjaly wiek bmi
    // https://www.geeksforgeeks.org/cpp/structured-binding-c/
    cout <<"1)\n";
    auto [initials, age, bmi] = getT1(ppl1);
    cout << "inicjaly: " << initials 
         << "\nwiek: " << age
         << "\nbmi: " << bmi << endl; 

    // 2) wiek waga wzrost
    auto t2 = getT2(ppl1);
    cout << "wiek: " << get<0>(t2) << endl;
    cout << "waga: " << get<1>(t2) << endl;
    cout << "wzors: " << get<2>(t2) << endl;
    

    // 3) imie nazwisko wiek
    auto t3 = getT3(ppl1);
    cout << "imie: " << get<0>(t3) << endl;
    cout << "nazwisko: " << get<1>(t3) << endl;
    cout << "wiek: " << get<2>(t3) << endl;

}