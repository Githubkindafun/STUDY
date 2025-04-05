READ ME - Zadanie 7

Opis:
    Możemy oglądać różne modele oraz dodawać szum ,który możemy parametryzować wg amplitudy, czestotliwości
    oraz możemy kolorki zmieniać


Odpalanie z konsoli :
    1) najpierw robimy "make"
    2) teraz już możemy normalnie : "./project -catalog <przykladkowyKatalog>" 

Dane :
    zamieszczam w katalogu objs przykładowe modele :
    -> -garden-hut
    -> -piesek
    -> -small-oven
    -> -space-craft (potrzebuje troche czasu aby się załadować)
    -> -starship (ogromy model potrzeuje (przynajmniej u mnie) czasu aby się załadować)
    -> -table
    -> -AWP

W programie mamy 6 opcjonalnych argumentów :
    -> "-amp <float>" : zmiana amplitudy szumu
    -> "-freq <float>" : zmiana częstotliwości szumu
    -> "-r <float>" : stała red
    -> "-g <float>" : stała green
    -> "-b <float>" : stała blue
    -> "-a <float>" : jak bardzo chcemy mieć "widoczny" szum

Obsługa :
    -> SPACEBAR : ON / OFF widoczność bounding boxa
    -> ORBITALNE :
        -> WSAD : przód / tył 
        -> +/- : zmiana prędkości
        -> strzałki : obracanie


Proponowane modele + szum :
    -> ./project -catalog space-craft -amp 0.2 -freq 0.8 -r 0.1 -g 0.0 -b 0.1 -a 1.0
    -> ./project -catalog garden-hut -amp 0.08 -freq 0.08 -r 0.09 -g 0.05 -b 0.0 -a 0.8
    -> ./project -catalog table -amp 0.05 -freq 0.05 -r 0.09 -g 0.1 -b 0.05 -a 0.5
    -> ./project -catalog small-oven -amp 0.15 -freq 0.1 -r 0.1 -g 0.0 -b 0.1 -a 0.5
    -> ./project -catalog piesek -amp 0.25 -freq 0.25 -r 0.25 -g 0.08 -b 0.02 -a 0.6
    -> ./project -catalog AWP -amp 0.2 -freq 0.2 -a 1.0 (efekt cienia rzucanego przez liscie)


Komentarz:
    -> usunołem mysz oraz kamerę "free"
    -> zaimplementowałem orbitalną kamerę
    -> naprawilem resize 
    -> dodane parametry szumu (częstotliwości)
    -> dodana prędkość
    -> zmieniony bounding box
    -> dodanie wizualnego bounding boxa
    -> backpack jest z tutoriala 

Źródła :
korzystałem z tutoriala z learnopengl.com
