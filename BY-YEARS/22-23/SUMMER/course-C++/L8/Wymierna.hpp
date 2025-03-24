#pragma once
#include <iostream>
#include <algorithm>
#include <math.h>
#include <numeric>


namespace obliczenia {
	class wyjatek_wymierny: public std::logic_error {
		public:
		explicit wyjatek_wymierny(const std::string& wh): std::logic_error(wh) {};
	};
	
	class przekroczenie_zakresu: wyjatek_wymierny {
		public:
		explicit przekroczenie_zakresu(const std::string& wh): wyjatek_wymierny(wh) {};
	};
	
	class dzielenie_przez_0: public wyjatek_wymierny {
		public:
		explicit dzielenie_przez_0(const std::string& wh): wyjatek_wymierny(wh) {};
	};
	
	class Wymierna {
	    int licznik;
	    int mianownik;
	
	
	public:
			 Wymierna(int li = 0, int mi = 1); 
			 void operator+=(Wymierna& w1);
			 void operator-=(Wymierna& w1);
			 void operator*=(Wymierna& w1); 
			 void operator/=(Wymierna& w1);
			 explicit operator int(); 
			 explicit operator double();
			 int get_licznik();	
			 int get_mianownik();
			 void set_licznik(int given); 
			 void set_mianownik(int given); 
	
			friend Wymierna operator+(Wymierna& w1, Wymierna& w2);
			friend Wymierna operator-(Wymierna& w1, Wymierna& w2);
			friend Wymierna operator*(Wymierna& w1, Wymierna& w2);
			friend Wymierna operator/(Wymierna& w1, Wymierna& w2);
			friend void operator!(Wymierna& w1);
			friend void operator-(Wymierna& w1);
	 		
	};
	
	
	Wymierna operator+(Wymierna& w1, Wymierna& w2); 
	
	Wymierna operator-(Wymierna& w1, Wymierna& w2); 
	
	Wymierna operator*(const Wymierna& w1, Wymierna& w2); 
	
	Wymierna operator/(Wymierna& w1, Wymierna& w2);
	
	void operator!(Wymierna& w1); 
	
	void operator-(Wymierna& w1);	 
}



