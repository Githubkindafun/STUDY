#pragma once
#include <iostream>
#include "Klasy.hpp"

/*
<done> (i) wstawienie elementu na zadaną pozycję;
<done> (ii) wstawienie elementu na początek listy (czyli na pozycję 0);
<done> (iii) wstawienie elementu na koniec listy;
<done> (iv) usunięcie elementu z określonej pozycji;
<done> (v) usunięcie elementu z początku listy (czyli z pozycji 0);
<done> (vi) usunięcie elementu z końca listy;
<done> (vii) usunięcie elementu o zadanej wartości (pierwszego od początku);
<done> (viii) usunięcie wszystkich elementów o zadanej wartości;
<done> (ix) określenie pozycji elementu o zadanej wartości (pierwszego od początku);
<done> (x) policzenie wszystkich elelemtów o zadanej wartości;
<done> (xi) zliczenie wszystkich elementów;
<done> (xii) sprawdzenie czy lista jest pusta.
*/

using namespace std;
using namespace calc;

int main() {
	LinkedList<int> list;
	
	cout << "Is list empty? " << list.Is_empty() << endl;
	cout << "List length: " << list.length() << endl;
	
	list.add_end(3);
	cout << "List length: " << list.length() << endl;
	list.add_begi(4);
	cout << "List length: " << list.length() << endl;
	list.add_index(2, 7);
	cout << "List length: " << list.length() << endl;
	list.add_end(42);
	cout << "List length: " << list.length() << endl;
	list.add_end(77);
	cout << "List length: " << list.length() << endl;
	list.add_end(12);
	cout << "List length: " << list.length() << endl;
	list.add_end(12);
	cout << "List length: " << list.length() << endl;
	list.add_end(12);
	cout << "List length: " << list.length() << endl;
	///////////////////////////////////////////////////////////
	LinkedList<int> list2 =  LinkedList<int>(list);
	
	cout << "Is list empty? " << list.Is_empty() << endl;

	cout << endl;
	list.show();

	cout << "kopiujacy hehe" << endl;
	list2.show();
	
	list.count_them(12);
	
	list.remove_end();
	cout << "List length: " << list.length() << endl;
	list.show();
	list.remove_beg();
	cout << "List length: " << list.length() << endl;
	list.remove_index(1);
	cout << "List length: " << list.length() << endl;
	list.remove_val(42);
	cout << "List length: " << list.length() << endl;
	list.remove_val_all(12);
	cout << "List length: " << list.length() << endl;
	list.search(7);
	cout << "List length: " << list.length() << endl;

	cout << "end" << endl;
	list.show();
	
	LinkedList<int> list1 =  LinkedList<int>(std::move(list));
	list1.show();

	cout << endl;
		
	cout <<  "initialize list" << endl;
	LinkedList<int> list3 =  LinkedList<int>{4, 2, 0, 42, 7};
	list3.show();

	//cout << list3 << endl;
	///////////////////////////////

	Queue<int> q1; //Queue<int>{4, 2, 0, 6, 9};
	cout << q1.Is_empty() << endl;
	cout << q1.length() << endl;
	q1.add_end(4);
	q1.add_end(2);
	cout << q1.get_head() << endl;
	q1.remove_beg();
	cout << q1.get_head() << endl;
	cout << q1.Is_empty() << endl;
	cout << q1.length() << endl;
	//
	cout << endl;

	Stack<int> s1; //Stack<int>{4, 2, 0, 6, 9};
	cout << s1.Is_empty() << endl;
	cout << s1.length() << endl;
	s1.add_begi(4);
	s1.add_begi(2);
	cout << s1.get_head() << endl;
	s1.remove_beg();
	cout << s1.get_head() << endl;
	cout << s1.Is_empty() << endl;
	cout << s1.length() << endl;
	
	
}
