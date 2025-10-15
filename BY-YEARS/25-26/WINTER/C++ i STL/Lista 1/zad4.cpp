#include <bits/stdc++.h>

using namespace std;
// nie dokonca jestem pewien jak interpretujemy
// to ktora liczba Lucasa jest 1 ale przy zalozeniu, że
// 0-owa to 2 a pierwsza to 1 to jest git
auto nthLucas(uint16_t x) {
    switch (x) {
    case 0:
        return 2;
        break;
    case 1:
        return 1;
        break;
    default:
        return nthLucas(x - 1) + nthLucas(x - 2); 
    }
}

int main() {

    int limit = 10;
    for(int i = 0; i < limit; i++) {
        cout << i << ". Lucas number is: " << nthLucas(i) << endl;
    }

    return 0;
}