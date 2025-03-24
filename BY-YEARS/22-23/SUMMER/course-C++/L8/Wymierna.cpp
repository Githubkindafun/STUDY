#pragma once
#include <iostream>
#include <algorithm>
#include <math.h>
#include <numeric>
#include "Wymierna.hpp"

using namespace std;



		obliczenia::Wymierna::Wymierna(int li, int mi) : licznik(li), mianownik(mi) { //domyslny i zwykły jednoczesnie
			
			if (mi == 0) throw dzielenie_przez_0("zero nie moze byc mianownikiem");
			int tmp = __gcd(li, mi);
			this->licznik = li / tmp;
			this->mianownik = mi / tmp;
			if ((li < 0 && mi < 0) || (li > 0 && mi < 0)) {
				this->licznik *= -1;
				this->mianownik *= -1;
			}    
			
		}
		void obliczenia::Wymierna::operator+=(Wymierna & w1) {
			 *this + w1;
		}
		void obliczenia::Wymierna::operator-=(Wymierna& w1) {
			*this = *this - w1;
		}
		void obliczenia::Wymierna::operator*=(Wymierna& w1) {
			*this = *this * w1;
		}   
		void obliczenia::Wymierna::operator/=(Wymierna& w1) {
			*this = *this / w1;
		}
		obliczenia::Wymierna::operator int() {
			return round(this->licznik / this->mianownik);			
		}
		obliczenia::Wymierna::operator double() {
			return (double)(this->licznik / this->mianownik);			
		}
		int obliczenia::Wymierna::get_licznik(){
			return licznik;
		}	
		int obliczenia::Wymierna::get_mianownik(){
			return mianownik;
		}
		void obliczenia::Wymierna::set_licznik(int given) {
			this->licznik = given; 
		}
		void obliczenia::Wymierna::set_mianownik(int given) {
			this->mianownik = given; 
		}
		
		obliczenia::Wymierna operator+(obliczenia::Wymierna& w1, obliczenia::Wymierna& w2);
		obliczenia::Wymierna operator-(obliczenia::Wymierna& w1, obliczenia::Wymierna& w2);
		obliczenia::Wymierna operator*(obliczenia::Wymierna& w1, obliczenia::Wymierna& w2);
		obliczenia::Wymierna operator/(obliczenia::Wymierna& w1, obliczenia::Wymierna& w2);
		void operator!(obliczenia::Wymierna& w1);
		void operator-(obliczenia::Wymierna& w1);
		
 		



obliczenia::Wymierna obliczenia::operator+(obliczenia::Wymierna& w1, obliczenia::Wymierna& w2) {
	int new_mian = w1.get_mianownik() * w2.get_mianownik();
	int w1_licznik = w1.get_licznik() * w2.get_mianownik();
	int w2_licznik = w2.get_licznik() * w1.get_mianownik();
	if(((w1_licznik / new_mian) + (w2_licznik / new_mian)) > INT64_MAX / new_mian || ((w1_licznik / new_mian) + (w2_licznik / new_mian)) < INT64_MIN / new_mian) {
		throw obliczenia::przekroczenie_zakresu("przekroczenie zakresu");
	} 
	return obliczenia::Wymierna(w1_licznik + w2_licznik, new_mian);
}

obliczenia::Wymierna obliczenia::operator-(obliczenia::Wymierna& w1, obliczenia::Wymierna& w2) {
	int new_mian = w1.get_mianownik() * w2.get_mianownik();
	int w1_licznik = w1.get_licznik() * w2.get_mianownik();
	int w2_licznik = w2.get_licznik() * w1.get_mianownik();
	if(((w1_licznik / new_mian) - (w2_licznik / new_mian)) > INT64_MAX / new_mian || ((w1_licznik / new_mian) + (w2_licznik / new_mian)) < INT64_MIN / new_mian) {
		throw obliczenia::przekroczenie_zakresu("przekroczenie zakresu");
	}
	return obliczenia::Wymierna(w1_licznik - w2_licznik, new_mian);  
}

obliczenia::Wymierna obliczenia::operator*( obliczenia::Wymierna& w1, obliczenia::Wymierna& w2) {
	int new_mian = w1.get_mianownik() * w2.get_mianownik();
	if(((w1.get_licznik() / new_mian) * (w2.get_licznik() / new_mian)) > INT64_MAX / new_mian || ((w1.get_licznik() / new_mian) + (w2.get_licznik() / new_mian)) < INT64_MIN / new_mian) {
		throw obliczenia::przekroczenie_zakresu("przekroczenie zakresu");
	}
	return obliczenia::Wymierna(w1.get_licznik() * w2.get_licznik(), w1.get_mianownik() * w2.get_mianownik());  
}

obliczenia::Wymierna obliczenia::operator/(obliczenia::Wymierna& w1, obliczenia::Wymierna& w2) {
	int new_mian = w1.get_mianownik() * w2.get_mianownik();
	if(((w1.get_licznik() / new_mian) / (w2.get_licznik() / new_mian)) > INT64_MAX / new_mian || ((w1.get_licznik() / new_mian) + (w2.get_licznik() / new_mian)) < INT64_MIN / new_mian) {
		throw obliczenia::przekroczenie_zakresu("przekroczenie zakresu");
	}
	if(w1.get_licznik() == 0 || w2.get_licznik() == 0) {
		throw obliczenia::dzielenie_przez_0("Nie dzielimy przez zero :C");
	}  
	obliczenia::Wymierna* w3 = new obliczenia::Wymierna(w2.get_mianownik(), w2.get_licznik());
	*w3 *= w1;
	return *w3;
}

void operator!(obliczenia::Wymierna& w1) {
	int tmp = w1.get_licznik();
	w1.set_licznik(w1.get_mianownik());
	w1.set_mianownik(tmp);
}
void operator-(obliczenia::Wymierna& w1) {
	int xd = -1;
	w1.set_licznik(w1.get_licznik() * xd);  
}





