#include <bits/stdc++.h>

using namespace std;

void losDataDeLocalesLosPrintes(string losLocales) {
    // cyk z wykladu skopiowane
    std::cout.imbue(std::locale(losLocales));
    cout << "kilka liczb calkowitych: " 
        << 0 << " " 
        << 42 <<  " "
        << -42 
        << endl;

    cout << "kilka liczb zmiennopzycyjnych: "
        << 42.42 << " " 
        << -42.42 << " " 
        << 123.321 
        << endl;

    // https://en.cppreference.com/w/cpp/locale/money_put.html
    // auto& f = std::use_facet<std::money_put<char>>(std::cout.getloc());
    // f.put({std::cout}, false, std::cout, std::cout.fill(), 12345678.9);
    // tak wg strony jest uzywajac facet directly ale tez opisuja inna metode put_money
    // i dziala
    // https://en.cppreference.com/w/cpp/io/manip/showbase.html
    // "The showbase flag affects the behavior ... monetary output"
    cout << showbase;
    cout << "los mamomnos: "
        << put_money(4200) << " " 
        << put_money(5) << " "
        << put_money(2137) 
        << endl;
    
    // https://en.cppreference.com/w/cpp/locale/time_put.html
    // https://en.cppreference.com/w/cpp/io/manip/put_time.html
    auto now = time(nullptr);
    cout << put_time(localtime(&now), "%A %x") << endl;
}


int main() {
    cout << "Deuchland\n";
    losDataDeLocalesLosPrintes("de_DE.UTF-8");
    cout << endl;

    cout << "England\n";
    losDataDeLocalesLosPrintes("en_GB.UTF-8");
    cout << endl;

    cout << "日本\n";
    losDataDeLocalesLosPrintes("ja_JP.UTF-8");
    cout << endl;
}
