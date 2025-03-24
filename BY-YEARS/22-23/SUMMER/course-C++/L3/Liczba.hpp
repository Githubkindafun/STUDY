#pragma once
using namespace std;

class Liczba {
    static const int history_size_max = 3; // rozmiar historii
    double *history_tab;                   // tablica dynamiczna
    int index;        // zmienna wskazujaca w którym miejscu w history_tab jestem aktualnie
    int current_size; // aktualna wielkość tablicy

    // funkcja pomocnicza do zawijania
    int next_index();

    // funkcja pomocnicza do zawijania
    int previous_index(); // odejmowanie i modulo ma problemy ;_;
    

public:
    // konstruktor zwykly?
    Liczba(double given_num);

    // konstruktor domyślny / jednoczesnie delegatowy
    Liczba();

    // konstruktor kopiujacy
    Liczba(const Liczba &liczba1);

    // konstruktor przenoszący
    Liczba(Liczba &&liczba1);

    // przypisanie kopiujace
    Liczba &operator=(const Liczba &liczba1);

    // przypisanie przenoszące
    Liczba &operator=(Liczba &&liczba1);

    // poprostu zwracam wartosc liczby
    double show();

    void set_value(double given_num);

    void revert();

    ~Liczba();
};
