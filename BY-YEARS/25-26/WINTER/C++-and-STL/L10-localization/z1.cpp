#include <bits/stdc++.h>

using namespace std;

int main() {
    // z slajdow z wykladu tylko trzeba bylo dodac .UTF-8
    std::locale::global(std::locale("pl_PL.UTF-8"));
    
    // podrąbane z wykladu 
    // jest ą ę ć ś ń ó ż ź ł
    wstring diacreticSentence = L"zażółć gęślą jaźń";

    wcout << diacreticSentence << endl;
}