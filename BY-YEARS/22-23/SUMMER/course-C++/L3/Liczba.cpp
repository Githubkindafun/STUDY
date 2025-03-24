#include "Liczba.hpp"
#include <iostream>


    // funkcja pomocnicza do zawijania
    int Liczba::next_index() {
        return (index + 1) % history_size_max;
    }

    // Liczba::funkcja pomocnicza do zawijania
    int Liczba::previous_index() {
        return (index + history_size_max - 1) % history_size_max; // odejmowanie i modulo ma problemy ;_;
    }


    // konstruktor zwykly?
    Liczba::Liczba(double given_num) : index(0), current_size(1) {
        history_tab = new double[history_size_max];
        history_tab[index] = given_num;
    }

    // konstruktor domyślny / jednoczesnie delegatowy
   Liczba::Liczba() : Liczba(42) {}

    // konstruktor kopiujacy
    Liczba::Liczba(const Liczba &liczba1)
            : index(liczba1.index), current_size(liczba1.current_size) {
        // history_tab = liczba1.history_tab;
        history_tab = new double[history_size_max];
        for (int i = 0; i < history_size_max; i++) {
            history_tab[i] = liczba1.history_tab[i];
        }
    }

    // konstruktor przenoszący
    Liczba::Liczba(Liczba &&liczba1)
            : index(liczba1.index), current_size(liczba1.current_size),
        history_tab(liczba1.history_tab) {
        liczba1.history_tab =
        nullptr; // "czyscimy" history taba poniewaz tylko on jest dyamiczny
    }

    // przypisanie kopiujace
    Liczba &Liczba::operator=(const Liczba &liczba1) {
        if (this != &liczba1) {
            history_tab = new double[history_size_max];
        	for (int i = 0; i < history_size_max; i++) {
            	history_tab[i] = liczba1.history_tab[i];
        	}
            index = liczba1.index;
            current_size = liczba1.current_size;
        }
        return *this; // zwracam pointer tego obiektu
    }

    // przypisanie przenoszące
    Liczba &Liczba::operator=(Liczba &&liczba1) {
        if (this != &liczba1) {
            swap(history_tab, liczba1.history_tab); //zwykły swap powinieen byc
            swap(index, liczba1.index); //moze byc memory leak !!
            swap(current_size, liczba1.current_size);
        }
        return *this;
    }

    // poprostu zwracam wartosc liczby
    double Liczba::show() {
        return history_tab[index];
    }

    void Liczba::set_value(double given_num) { // done
        index = next_index();
        history_tab[index] = given_num; // korzystajac z funkcji pomocniczej "zawijam tablice"

        if (current_size < history_size_max) current_size++;
    }

    void Liczba::revert() { // cofanie wartości
        if (current_size > 1) {
            index = previous_index();
            current_size--;
        }
    }

    // [1] [2] [3] <- 4
    //          ^
    // size = 3
    
    // [4] [2] [3] <- 5
    //  ^
    // size = 3
    
    // [4] [5] [3] <- previous
    //      ^
    // size = 3
    
    // [4] [x] [3]
    //  ^
    // size = 2

    Liczba::~Liczba() { // destruktor sprzatacz
        if (history_tab != nullptr) delete[] history_tab;
    }


	