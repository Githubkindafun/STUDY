#include <bits/stdc++.h>

using namespace std;

int gcd(int uno, int dos) {
    while(dos != 0) {
        int tres = uno % dos;
        uno = dos;
        dos = tres;
    }
    return uno;
}

int phiPhi(int x) {
    int losCounteros = 0;
    if(x == 0) {
        throw("What are you dooing amigo?");
    }
    // <= bo nie wieksze
    for(int i = 1; i <= x; i++) {
        if(gcd(i, x) == 1) {
            losCounteros++;
        }
    }
    return losCounteros;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        throw("Wrong number of arguments my friend !!\n");
    }
    int k = stoi(argv[1]);

    vector<int> phiCagedInVector(k);

    for(int i = 0; i < k; i++) {
        phiCagedInVector[i] = phiPhi(i + 1);
    }

    ofstream output("phi.txt");
    // ostream_iterator<int> intWriter(output, "\n");
    if(k == 1) {
        output << phiCagedInVector[0];
    } else {
        copy(phiCagedInVector.cbegin(), phiCagedInVector.cend(), ostream_iterator<int>(output, "; "));
    }
}