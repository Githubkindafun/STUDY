#include <iostream>
#include <string>
#include "Liczba.hpp"

using namespace std;


int main() {
    cout << "Test set value oraz revert" << endl;
    Liczba l = Liczba(1);
    cout << l.show() << endl;
    l.set_value(2);
    cout << l.show() << endl;
    l.set_value(3);
    cout << l.show() << endl;
    l.set_value(4);
    cout << l.show() << endl;
    l.revert();
    cout << l.show() << endl;
    l.revert();
    cout << l.show() << endl;
    l.revert();
    cout << l.show() << endl;
    l.revert();
    cout << l.show() << endl;
    l.revert();
    cout << l.show() << endl;
	
	cout << "\n";
	
    Liczba l2 = Liczba();
    cout << "LICZBA 2: ";
    cout << l2.show() << endl;
  	
	cout << "\n";
    
	Liczba l3 = l2;
    cout << "LICZBA 3: ";
    cout << l3.show() << endl;
	
	cout << "\n";
	
	l2.set_value(21);
    cout << l2.show() << endl;
	l2.revert();
	cout << l2.show() << endl;

	cout << "\n";

	cout << "LICZBA 4: ";
	Liczba l4 = move(l2);
	cout << l4.show() << endl;
	//cout << l2.show() << endl;

	cout << "\n";

	l4 = l;
	cout << "LICZBA 5: ";
	cout << l4.show() << endl;

	cout << "\n";

	cout << "LICZBA 6: ";
	l4 = move(l3);
	cout << l4.show() << endl;
}