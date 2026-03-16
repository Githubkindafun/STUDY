#include<bits/stdc++.h>

using namespace std;



int main() {

    cout << "max(long long): "
    << numeric_limits<long long>::max()
    << "\n min(long long): "
    <<  numeric_limits<long long>::min()
    << "\n is signed ? (long long): "
    <<  numeric_limits<long long>::is_signed
    << "\n how many bits ? (long long): "
    <<  numeric_limits<long long>::digits
    << "\n how many digits(10) ? (long long): "
    <<  numeric_limits<long long>::digits10;
}