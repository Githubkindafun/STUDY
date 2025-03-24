#pragma once
#include <iostream>
#include <algorithm>
#include <math.h>
#include <numeric>
#include "Wymierna.hpp"
using namespace std;
using namespace obliczenia;


int main() {
	Wymierna wym1 = Wymierna(3, 5);
	Wymierna wym2 = Wymierna(1, 4);
	cout << wym1.get_licznik() << " " << wym2.get_licznik() << endl;
	cout << wym1.get_mianownik() << " " << wym2.get_mianownik() << endl;

	
	wym1 *= wym2;
	cout << endl;
	cout << wym1.get_licznik() << endl << wym1.get_mianownik() << endl;
}



