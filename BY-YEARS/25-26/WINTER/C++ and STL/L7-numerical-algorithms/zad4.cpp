#include<bits/stdc++.h>

using namespace std;


void rollTheDiceUniform(const string& filename, int rollsCount, default_random_engine& dre ) {
    ofstream outputFile(filename);
    outputFile << "UniformDis\n";
    // https://www.geeksforgeeks.org/cpp/stduniform_real_-distribution-class-in-c-with-examples/
    uniform_real_distribution<double> distribution(0.0, 1.0);
    
    double rolledValue;
    for(int i = 0; i < rollsCount; i++) {
        rolledValue = distribution(dre);
        outputFile << rolledValue << endl;
    }
}

void rollTheDiceBinomial(const string& filename, int rollsCount, default_random_engine& dre ) {
    ofstream outputFile(filename);
    outputFile << "BinomialDis\n";
    // https://en.cppreference.com/w/cpp/numeric/random/binomial_distribution.html
    binomial_distribution<int> distribution(4, 0.5);
    int rolledValue;
    for(int i = 0; i < rollsCount; i++) {
        rolledValue = distribution(dre);
        outputFile << rolledValue << endl;
    }
}

void rollTheDiceNormal(const string& filename, int rollsCount, default_random_engine& dre ) {
    ofstream outputFile(filename);
    outputFile << "NormalDis\n";
    // https://cplusplus.com/reference/random/normal_distribution/
    normal_distribution<double> distribution(5.0, 2.0);
    double rolledValue;
    for(int i = 0; i < rollsCount; i++) {
        rolledValue = distribution(dre);
        outputFile << rolledValue << endl;
    }
}

int timeSeed() {
    // https://www.geeksforgeeks.org/cpp/localtime-function-in-c/
    time_t time_ptr;
    time_ptr = time(NULL);
    tm* tm_local = localtime(&time_ptr);

    int timeVal = tm_local->tm_hour + tm_local->tm_min + tm_local->tm_sec;
    return timeVal;
}

int main() {

    int amountOfRolls = 1000;
    int rndSeed = timeSeed(); 
    default_random_engine dre(rndSeed);

    cout << "taki seed nam sie wylosowal: " << rndSeed << endl;
    // i teraz odpalimy tutaj losowanie liczb ;)
    rollTheDiceUniform("uniform.csv", amountOfRolls, dre);
    rollTheDiceBinomial("binomial.csv", amountOfRolls, dre);
    rollTheDiceNormal("normal.csv", amountOfRolls, dre);

    return 0;
}