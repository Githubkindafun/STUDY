/*
Zdefiniuj klasę opakowującą plik tekstowy line_writer.
Obiekt takiej klasy ma przetrzymywać wskaźnik/referencję do strumienia plikowego ofstream stworzonego i otwartego w konstruktorze.
Zadaniem obiektów tej klasy będzie pisanie do pliku wiersz po wierszu. Klasa ta powinna zamykać
strumień w destruktorze.
W programie głównym stwórz kilka wskaźników shared_ptr odnoszących się do tego samego
obiektu plikowego. 
Niech każdy fragment programu posiadający taki własny wskaźnik zapisze w tym
pliku swoje dane. Plik powinien zostać zamknięty dopiero, gdy wszystkie sprytne wskaźniki zostaną
zlikwidowane.
*/

#include <bits/stdc++.h>

using namespace std;
// Zdefiniuj klasę opakowującą plik tekstowy line_writer. 
class line_writer {
protected:
    unique_ptr<ofstream> o;
    // dzieki unique ptr tylko ta klasa bedzie wlasnicielem tego strumienia

public:
    // Obiekt takiej klasy ma przetrzymywać wskaźnik/referencję do strumienia plikowego ofstream stworzonego i otwartego w konstruktorze. 
    line_writer(string fileName) {
        o = unique_ptr<ofstream>(new ofstream(fileName));
        if(!o->is_open()) {
            throw "Ziomek, strumyk nie otwarty ;c";
        }
    }
    // Zadaniem obiektów tej klasy będzie pisanie do pliku wiersz po wierszu.
    void writeSingleLine(string line) {
        *o << line << endl;
    }
    // . Klasa ta powinna zamykać strumień w destruktorze. 
    ~line_writer() {
        cerr << "zamkniecie" << endl;
        o->close();
    }
};

int main() {
    // W programie głównym stwórz kilka wskaźników shared_ptr odnoszących się do tego samego obiektu plikowego. 
    shared_ptr<line_writer> givenFile = make_shared<line_writer>("losNumeros.txt");
    cout << "Stan licznika po Cero (zero) " << givenFile.use_count() << endl;

    shared_ptr<line_writer> pointerUno = givenFile;
    cout << "Stan licznika po Uno " << givenFile.use_count() << endl;
    //  Niech każdy fragment programu posiadający taki własny wskaźnik zapisze w tym pliku swoje dane.
    pointerUno->writeSingleLine("Uno");

    {shared_ptr<line_writer> pointerDos = givenFile;
    cout << "Stan licznika po Dos " << givenFile.use_count() << endl;
    //  Niech każdy fragment programu posiadający taki własny wskaźnik zapisze w tym pliku swoje dane.
    pointerDos->writeSingleLine("Dos");}

    //  Niech każdy fragment programu posiadający taki własny wskaźnik zapisze w tym pliku swoje dane.
    shared_ptr<line_writer> pointerTres = givenFile;
    cout << "Stan licznika po Tres " << givenFile.use_count() << endl;
    pointerTres->writeSingleLine("Tres");

    givenFile->writeSingleLine("Quatro and the end of main");
    return 0;
}

// co do  Plik powinien zostać zamknięty dopiero, gdy wszystkie sprytne wskaźniki zostaną zlikwidowane. 
// Zadaniem wskaźników typu shared_ptr<> jest automatyzacja usuwania zasobów skojarzonych z obiektem 
// wskazywanym w momencie, kiedy obiekt ten nie będzie już więcej używany.