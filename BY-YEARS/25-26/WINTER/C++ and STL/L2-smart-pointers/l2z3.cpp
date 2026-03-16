#include <bits/stdc++.h>

using namespace std;


enum class Gender {
    Female,
    Male,
    Other
};

string genderToString(Gender gender) {
    switch(gender){
        case(Gender::Female):   
            return "Female";
        case(Gender::Male):
            return "Male";
        default:
            return "Other";
    }
}

class llama {
private:

    llama(string name, Gender gender, shared_ptr<llama> mom=nullptr, shared_ptr<llama> pops=nullptr) {
        nameLama = name;
        genderLama = gender;
    }

    string nameLama;
    Gender genderLama;
    shared_ptr<llama> momLama;
    shared_ptr<llama> popsLama;
    vector<weak_ptr<llama>> ofspirngLama;

public:

    llama(string name, Gender gender, shared_ptr<llama> mom=nullptr, shared_ptr<llama> pops=nullptr, bool isBorn, bool isTransfered) {
        nameLama = name;
        genderLama = gender;
        if(isBorn) {
            cout << "urodzila sie nowa lama :> " << nameLama << " jest to " << genderToString(gender) <<  endl;
        } else if(isTransfered) {
            cout << "Nowa lama z transferu ma na imie " << nameLama << endl;
        }

        if(mom) {
            momLama = mom;
        } 
        if(pops) {
            popsLama = pops;
        }
        // tutaj jeszcze kwestia tego noname
    }

    void addOfspring(shared_ptr<llama> ofspring) {
        ofspirngLama.push_back(ofspring);
    }


};



class herd {
//
};


int main() {

    return 0;
}