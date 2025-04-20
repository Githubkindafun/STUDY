# IO zadanie na listopad
# Szymon Fluder & Adam Majchrowski
TODO:
- [X] rozpisać scenariusze przypadków użycia
- [X] projekt UI
- [X] struktura logiczna oprogramowania 
- [X] model konceptualny 
- [X] schemat bazy danych
- [X] warunki integralności
- [X] więcej zasad kodowania zapisać 
- [X] zredagować sekcję o zagrożeniach


# Precyzyjniejsze określenie wymagań
## Historyjki użytkownika
### rejestracja użytkownika
Jako *nowy użytkownik* chcę się zarejestrować, *żeby* korzystać z aplikacji. Wpisuję swoje dane i wciskam przycisk aby się zarejestrować.
### logowanie użytkownika
Jako *użytkownik aplikacji* chcę się zalogować na konto, *aby* korzystać z aplikacji.
### skanowanie produktów
Jako *użytkownik aplikacji* chcę zeskanować produkty (zrobić ich zdjęcie), *aby* aplikacja je zapamiętała.
### manualne dodawanie produktów do "spiżarni"
Jako *użytkownik* chcę manualnie dodać produkt do "spiżarni", *aby* aplikacja go zapamiętała.
### zobaczenie listy produktów
Jako *użytkownik* chcę zobaczyć listę zapamiętanych produktów.
### proponowanie posiłków
Jako *użytkownik* chcę zobaczyć propozycję posiłku.
### wybranie diety
Jako *użytkownik* chcę wybrać dietę, na podstawie której aplikacja będzie proponowała mi przyszłe posiłki.
### wybranie alergenów
Jako *użytkownik* chcę sprecyzować co mogę, a czego nie mogę jeść, *aby* propozycje posiłków nie zawierały tych alergenów.

## Wymagania funkcjonalne
1. *Edycja profilu* - użytkownik ma możliwość zmiany swoich preferencji w ustawieniach.
2. *Skanowanie produktów* - użytkownik może zrobić zdjęcie produktów, które potem zostaną zapisane. Po zrobieniu zdjęcia aplikacja przechodzi do widoku, gdzie użytkownik widzi listę produtków, które rozpoznała aplikacja. Może wtedy każdy z nich edytować oraz dodawać oraz usuwać produkty.
3. *Zarządzanie listą zapisanych produktów* - użytkownik może zobaczyć listę zapisanych produktów i zmieniać każdy z nich (ile ma sztuk danego produktu, dodanie nowego produktu, usunięcie produktu, etc.). Po kliknięciu w miniaturkę produktu widać jego szczegóły i użytkownik może kliknąć w przycisk "edytuj".
4. *Proponowanie posiłków* - na podstawie preferencji użytkownika i zawartości lodówki dostaje on propozycję posiłku. 

## Wymagania niefunkcjonalne
- Aplikacja mobilna na IOS i Android
- Aplikacja musi się szybko ładować, aby użytkownik mógł szybko dodać produkty.
- Standardy dostępności - inne czcionki i opcjonalne kontrasty dla użytkowników z problemami z widzeniem.
- System musi mieć wysoką dostępność (>= 99%).
- System musi być w stanie obsłużyć 10 000 użytkowników jednocześnie.
- Aplikacja poprawnie odzyskuje dane w przypadku awarii urządzenia użytkownika (dane użytkownika przechowywane w chmurze).
- Intuicyjne UI.
- Spełnienie wymogów prawnych związanych z przechowywaniem danych użytkowników.

# Scenariusze przypadków użycia
- rejestracja użytkownika 
- logowanie użytkownika 
- skanowanie produktów
- manualne dodawanie produktów  
- zobaczenie listy produktów
- edycja produktu z listy 
- proponowanie posiłków
- wybranie diety 
- wybranie alergenów 

1. Rejestracja użytkownika
    - *Aktorzy:* użytkownik
    - *Cel:* utworzenie konta dla nowego użytkownika
    - *Priorytet:* wysoki
    - *Opis:* Użytkownik wpisuje swoje dane do formularza i odsyła go. Następnie serwer sprawdza czy dane się zgadzają (nie ma niedozwolonych znaków, wszystkie obowiązkowe dane są wypełnione, hasło ma wystarczająco dużo znaków), tworzy konto użytkownika
    - *Wyzwalacze:* Użytkownik chce założyć konto
    - *Zdarzenia alternatywne:* Użytkownik ma już konto => jest on proszony o zalogowanie się. Dane są niepoprawne => prośba do użytkownika o przesłanie poprawionych danych.
    
2. Logowanie użytkownika
    - *Aktorzy:* użytkownik
    - *Cel:* zalogowanie się na konto
    - *Priorytet:* wysoki
    - *Opis:* Użytkownik wpisuje swoje dane do formularza i odsyła go. Następnie serwer sprawdza czy dane się zgadzają (użytkownich o danych danych istnieje w bazie danych, hasz hasła pasuje do tego z bazy dadnych), a następnie loguje użytkownika.
    - *Wyzwalacze:* Użytkownik chce się zalogować na konto
    - *Zdarzenia alternatywne:* Użytkownik wprowadzi złe hasło => do użytkownika jest wysyłana informacja zwrotna o błędnym haśle. Użytkownik wprowadzi dane, które nie istnieją w bazie danych => serwer odsyła informację zwrotną, że nie ma użytkownika o takich danych.

3. Skanowanie produktów
    - *Aktorzy:* zalogowany użytkownik
    - *Cel:* Zeskanowanie produktów aby dodać je do listy
    - *Priorytet:* wysoki
    - *Opis:* Użytkownik robi zdjęcie produktów, które chce dodać do listy posiadanych produktów. Zdjęcie jest wysyłane na serwer, gdzie model sztucznej inteligencji przetwarza zdjęcie i zwraca listę produktów na zdjęciu. Użytkownikowi następnie jest pokazywana lista rozpoznanych produktów. Użytkownik może ją zaakceptować/odrzucić/zmodyfikować (dodać manualnie nowe produkty, usunąć któryś z rozpoznanych, zmienić ilość jakiegoś produktu). Po zaakceptowaniu listy przez użytkownika jest ona wysyłana na serwer, który zapisuje ją w bazie danych.
    - *Wyzwalacze:* Użytkownik chce zeskanować produkty
    - *Zdarzenia alternatywne:* Użytkownik zrobi zdjęcie, na którym nie ma żadnego produktu spożywczego albo modelowi na serwerze nie udało się niczego rozpoznać => zwracana jest wiadomość do użytkownika o błędzie.

4. Manualne dodawanie produktów
    - *Aktorzy:* zalogowany użytkownik
    - *Cel:* Dodanie nowego produktu do listy produktów
    - *Priorytet:* średni
    - *Opis:* Użytkownik wpisuje dane produktu do formularza, po czym dane te są wysyłane na serwer. Serwer sprawdza ich poprawność (czy wszystkie obowiązkowe pola zostały wypełnione), po czym dodaje produkt do listy i odsyła użytkownikowi informację zwrotną o pomyślnym dodaniu produktu.
    - *Wyzwalacze:* Użytkownik chce manualnie dodać nowy produkt
    - *Zdarzenia alternatywne:* Taki sam produkt istnieje już na liście => Użytkownikowi odsyłana jest wiadomość, że dany produkt już istnieje. Wtedy użytkownik może potwierdzić operację zwiększenia liczby sztuk danego produktu. Jeśli potwierdzi, to następuje aktualizacja sztuk danego produktu na liście.

5. Oglądanie listy produktów
    - *Aktorzy:* zalogowany użytkownik
    - *Cel:* Zobaczenie przez użytkownika listy jego produktów
    - *Priorytet:* wysoki
    - *Opis:* Użytkownik wchodzi w odpowiednią zakładkę w UI, serwer zwraca listę produktów, która jest pokazywana użytkownikowi.
    - *Wyzwalacze:* Użytkownik chce zobaczyć listę produktów
    - *Zdarzenia alternatywne:* brak
    

6. Edycja produktu z listy
    - *Aktorzy:* zalogowany użytkownik
    - *Cel:* Edycja przez użytkownika produktu spożywczego zapisanego na liście
    - *Priorytet:* średni
    - *Opis:* Użytkownik wypełnia/zmienia formularz z informacjami o danym produkcje. Następnie wysyła go na serwer, który weryfikuje dane, po czym aktualizuje produkt w bazie danych.
    - *Wyzwalacze:* Użytkownik chce edytować produkt z listy
    - *Zdarzenia alternatywne:* Użytkownik wpisał złe dane => wysyłany jest z serwera odpowiedni komunikat, który jest użytkownikowi wyświetlany.

7. Wybranie diety
    - *aktorzy:* zalogowany użytkownik
    - *cel:* wybranie diety przez użytkownika
    - *priorytet:* średni
    - *opis:* użytkownikowi wyświetlana jest lista diet, które może wybrać. użytkownik wybiera te, które mu odpowiadają, po czym odsyła formularz na serwer, który zapamiętuje preferencje użytkownika w bazie danych.
    - *wyzwalacze:* użytkownik chce wybrać dietę
    - *zdarzenia alternatywne:* brak

8. Wybranie alergenów
    - *aktorzy:* zalogowany użytkownik
    - *cel:* wybranie alergenów
    - *priorytet:* średni
    - *opis:* użytkownikowi wyświetlana jest lista alergenów, które może wybrać. Użytkownik zaznacza te, na które ma alergię, po czym odsyła formularz. Serwer zapisuje to w bazie danych
    - *wyzwalacze:* użytkownik chce wybrać alergeny
    - *zdarzenia alternatywne:* brak
    
8. Propozycja posiłku
    - *aktorzy:* zalogowany użytkownik
    - *cel:* Pokazanie użytkownikowi propozycji posiłku
    - *priorytet:* średni
    - *opis:* Po wysłaniu stosownego żądania na serwer, odpowiada on z propozycją posiłku dla danego użytkownika. Podczas wybierania propozycji są brane pod uwagę preferencje dietetyczne użytkownika oraz jego możliwe alergeny. Po wysłaniu propozycji jest ona wyświetlana użytkownikowi.
    - *wyzwalacze:* użytkownik chce zobaczyć propozycję posiłku
    - *zdarzenia alternatywne:* brak

# Projekty dialogów i dokumentów
![Zrzut ekranu 2024-12-08 211056](https://hackmd.io/_uploads/r1b36d7NJx.jpg)
![Zrzut ekranu 2024-12-08 211252](https://hackmd.io/_uploads/HJACTdXEye.jpg)
![Zrzut ekranu 2024-12-08 211346](https://hackmd.io/_uploads/HyVGCuXNkl.jpg)









# Architektura systemu
## Przewidywane rozwiązania sprzętowe
- Serwery aplikacyjne - użycie usług chmurowych (typu AWS, Google Cloud Platform) na hostowanie kodu back-end.
- Zapewnienie skalowalności przez użycie Dockera i Kubernetes.
## Oprogramowanie systemowe, bazy danych, narzędzia programistyczne
- Baza danych - wysoce skalowalna relacyjna baza danych (n.p. MariaDB, CassandraDB)
- Django do napisania backendu
- React Native do napisania aplikacji mobilnej
- Load balancing za pomocą odpowiednio skonfigurowanego nginx - zapewni wydajność i dostępność.
- Git jako system kontroli wersji
- Biblioteki do zarządania wersjami pakietów
- Edytory - VSCode, Odpowiednie edytory JetBrains

## Oprogramowanie do automatycznego testowania
- *Jest* do testowania aplikacji mobilnej.
- *Jenkins* do automatyzacji CI/CD. 

## Struktura logiczna oprogramowania
Z racji, że chcemy używać React Native, to najlepszy sposób na napisanie backendu to API do bazy danych. Wtedy frontend napisany w React Native będzie mógł tam gdzie to potrzebne robić zapytania do bazy danych.

React Native zapewni dużą elastyczność i wygodę w pisaniu interfejsu użytkownika, gdyż będzie można napisać parę bazowych komponentów (typu textBox, przycisk, powiadomienie ...), które potem będą używane do tworzenia coraz to bardziej skomplikowanych widoków. 

Widoki to będą:
- landing page
- skanowanie produktów
- oglądanie/zarządzanie listą produktów
- indywidualny produkt
- polecane przepisy
- detale poleconego przepisu
- ustawienia konta użytkownika

# Czynności i zadania uzupełniające
## Model konceptualny rzeczywistości
- Model użytkownika
- Model produktu spożywczego
- Model przepisu

1. **Użytkownik**
    - Imię
    - Nazwisko
    - Email
    - Hasło
    - Data utworzenia konta
    - Alergie
    - Preferencje
    - Lista produktów
2. **Produkt spożywczy**
    - Nazwa
    - Wartości odżywcze
    - Alergeny
    - Data przydatności
    - Przykładowe zdjęcie do miniaturki
3. **Przepis**
    - Nazwa
    - Składniki
    - Alergeny
    - Czas przygotowania
    - Przykładowe zdjęcie posiłku do miniaturki
    - Typ kuchni do jakiej należy przepis
    - Instrukcja przygotowania
    - opis przepisu


## Schemat bazy danych
![IO_listopad_db_diagram](https://hackmd.io/_uploads/S181eHmQye.png)


## Warunki integralności
**WIDAĆ WSZYSTKO NA OBRAZKU**

# Główne zasady kodowania
- Trzymanie się standardów kodowania w danym języku (n.p. PEP8 w Pythonie)
- Unikanie nadmiernej liczby pustych linijek
- Pisanie w CamelCase lub snake_case, w zależności od standardów języka
- Używanie opisowych i zrozumiałych nazw klas, funkcji i zmiennych
- Stosowanie się do zasady DRY (don't repeat yourself)
- Stosowanie się do zasady KISS (keep it simple stupid)
- Unikanie zbyt dużych klas i funkcji (rozbijanie ich na mniejsze kawałki, które jest prościej zrozumieć, zarządzać i testować)
- Używanie operacji asynchronicznych w odpowiednich do tego sytuacjach (czekanie na odpowiedź z bazy danych, odczytywanie plików z dysku ...)
- Unikanie przedwczesnej optymalizacji, lecz bycie świadomym gdzie mogą zajść wąskie gardła w programie
- Używanie menadżerów pakietów (typu npm, pip)
- Utrzymywanie na bieżąco dokumentacji

# Zagrożenia i zasady zarządzania ryzykiem
## Zagrożenia
### Narazie wypisze jakie potencjalnie pojawiają się problemy
1. Jednoznaczność w rozpoznawaniu prodktów (błędy w modelu AI)
2. Błędne użytkowanie aplikacji przez użytkowników 
3. Bezpieczeństwo danych
4. Błędy/Bugi w aplikacji
5. Problemy z wydajnością
6. Niezadowolnie użytkowników -> zbieramy feedback -> adekwatna reakcja

## Zasady Zarządzania Ryzykiem


* **Zapewnienie Bezpieczeństwa:**
Regularne aktualizacje zabezpieczeń.

* **Testy i Monitorowanie:**
Systematyczne testowanie modułów, integracji i wydajności, połączone z bieżącym nadzorem nad działaniem aplikacji.

* **Sytuacje kryzysowe:**
Stworzenie planu reagowania na sytuacje kryzysowe.

* **Regularne Aktualizacje:**
Cykliczne aktualizacje aplikacji.

* **Wsparcie Użytkownika:**
Zapewnienie skutecznego wsparcia technicznego i analizowanie feedbacku od użytkowników.

* **Przestrzeganie Prawa:**
Dostosowywania aplikacji do obowiązujących norm prawnych.


# Zgodność prac z wizją i założeniami w tablicy koncepcyjnej
- musieliśmy zmienić - część kosztów pójdzie na edytory JetBrains.