#include <bits/stdc++.h>

using namespace std;

int main() {
    // 31 dniowe miesiace
    // dni 0[1-9]|[1-2][0-9]|3[0-1] - 01-09 lub 10-29 lub 30 - 31
    // miesiace (01|03|05|07|08|10|12)
    // 30 dniowe miesiace
    // analogicznie
    // luty
    // analogicznie
    // ROK - \d cyfra dziesiętna [[:digit:]]
    regex dateRegex(R"(^(((0[1-9]|[1-2][0-9]|3[0-1])-(01|03|05|07|08|10|12))|((0[1-9]|[1-2][0-9]|30)-(04|06|09|11))|((0[1-9]|1[0-9]|2[0-8])-(02)))-\d{4}$)");
    string date;
    while(1) {
        cin >> date;
        if(regex_match(date, dateRegex)) {
            cout << "good date amigo\n";
        } else {
            cout << "bad date amigo\n";
        }
    }
}