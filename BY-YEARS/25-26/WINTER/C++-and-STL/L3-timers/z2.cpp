#include<bits/stdc++.h>

using namespace std;



int main() {

    cout << "najblizsza 0 liczba dodatnia "
    // https://en.cppreference.com/w/cpp/types/numeric_limits.html
    << numeric_limits<long double>::denorm_min()
    << "\n max(long double): (long double): "
    <<  numeric_limits<long double>::max
    << "\n has inf (long double): "
    <<  numeric_limits<long double>::has_infinity
    << "\n inf val ? (long double): "
    <<  numeric_limits<long double>::infinity()
    << "\n 1 - min ? (long double): "
    <<  numeric_limits<long double>::epsilon();
}