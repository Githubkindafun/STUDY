#include <bits/stdc++.h>

using namespace std;

using sSet = set<string>;

int main() {

    sSet dogBreeds{"Border Collie", "German Shepard", "Great Dane", "Jack Russel Terrier"};
    int cnt = 1;
    for (const auto& breed : dogBreeds) {
        cout << cnt << "."<< breed << endl;
        cnt++;
    }

    return 0;
}