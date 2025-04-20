# IO Zadanie na grudzień
# Adam Majchrowski & Szymon Fluder

## 1. Opracowanie dokładnych przypadków testowych.

### Rejestracja użytkownika 

    Opis : Sprawdzenie działania procesu rejestracji użytkownika.

    Oczekiwany rezultat : Po wprowadzeniu poprawnych danych, konto zostaje założone 
    dla użytkownika, przy wprowadzeniu błędnych użytkownik zostaje poinformowany o
    błędzie.


#### Przypadek I :

##### Stan początkowy : 
    Użytkownik jest na stronie rejestracji.
    Użytkownik nie posiada konta.
    W bazie danych nie ma konta o e-mailu użytkownika. 

##### Proces rejestracji : 
    1. Użytkownik wprowadza swoje dane wymagane przy rejestracji nowego konta :
    -> Imię : Juliusz
    -> Nazwisko : Cezar
    -> Adres @ : juliusz.cezar@wp.com
    -> Hasło : masłoHasło476
    -> Kraj : Polska
    -> Województwo : Kujawsko-Pomorskie
    -> Miasto : Rzym
    -> Ulica : Długa
    -> Nr. Domu : 1
    -> Kod pocztowy : 88-420
    2. Użytkownik zatwierdza dane wciskając przycisk "SIGN UP"

##### Stan końcowy :
    Konto użytkownika zostało utworzone.
    Użytkownik zostaje przekierowany do strony głównej aplikacji.

#### Przypadek II :

##### Stan początkowy : 
    Użytkownik jest na stronie rejestracji.
    Użytkownik posiada konto.
    W bazie danych jest konto o e-mailu użytkownika. 

##### Proces rejestracji : 
    1. Użytkownik wprowadza swoje dane wymagane przy rejestracji konta :
    -> Imię : Juliusz
    -> Nazwisko : Cezar
    -> Adres @ : juliusz.cezar@wp.com
    -> Hasło : masłoHasło476
    -> Kraj : Polska
    -> Województwo : Kujawsko-Pomorskie
    -> Miasto : Rzym
    -> Ulica : Długa
    -> Nr. Domu : 1
    -> Kod pocztowy : 88-420
    2. Użytkownik zatwierdza dane wciskając przycisk "SIGN UP"

##### Stan końcowy :
    Nowe konto nie powstaje.
    Użytkownik zostaje pooinformowany o tym ,że konto o podanym przez niego
    adresie e-mail istnieje.
    Użytkownik zostaje przekierowany z powrotem do ekranu rejestracji, a pole
    z powatrzającym się adresem e-mail zostaje podświetlone.

#### Przypadek III (złe dane):

##### Stan początkowy :
    Użytkownik jest na stronie rejestracji.
    Użytkownik nie posiada konta.
    W bazie danych nie ma konta o e-mailu użytkownika. 

##### Proces rejestracji : 
    1. Użytkownik wprowadza swoje dane wymagane przy rejestracji konta :
    -> Imię : Juliusz
    -> Nazwisko : Cezar
    -> Adres @ : juliusz.cezar@wp.com
    -> Hasło : masłoHasło476
    -> Kraj : Polska
    -> Województwo : Kujawsko-Pomorskie
    -> Miasto : żym
    -> Ulica : Majestatyczna 
    -> Nr. Domu : 1
    -> Kod pocztowy : 88-420
    2. Użytkownik zatwierdza dane wciskając przycisk "SIGN UP"

##### Stan końcowy :
    Nowe konto nie powstaje.
    Użytkownik zostaje pooinformowany o tym ,że dane które wpisał są niepoprawne.
    Użytkownik zostaje przekierowany z powrotem do ekranu rejestracji, a pole
    z błędnym adresem zostaje podświetlone.

### Skanowanie produktów

    Opis : Sprawdzenie działania funkcji skanowania produktów.

    Oczekiwany rezultat : Po zrobieniu zdjęcia produktów system poprawnie rozpoznaje 
    produkty i wyświetla użytkownikowi ich listę do zaakceptowania.


#### Przypadek I :

##### Stan początkowy : 
    Użytkownik jest zalogowany.
    Użytkownik jest na widoku skanowania produktu.

##### Proces skanowania (udany) : 
    1. Użytkownik robi zdjęcie na którym są widoczne produkty :
    -> 2 x Gruszka
    -> 4 x Pomarańcz
    -> 6 x Śliwka
    2. Aplikacja wyświetla liste na której znadjdują się :
    -> 2 x Gruszka
    -> 4 x Pomarańcz
    -> 6 x Śliwka
    3. Użytkownik akceptuje listę produktów.
    4. Użytkownik zostaje przekierowany na stronę główną aplikacji.


##### Stan końcowy :
    Spiżarnia użytkownika zostaje zaaktualizowana o nowe produkty.
    

#### Przypadek II :

##### Stan początkowy : 
    Użytkownik jest zalogowany.
    Użytkownik jest na widoku skanowania produktu.

##### Proces skanowania (nie udany) : 
    1. Użytkownik robi zdjęcie na którym są widoczne produkty :
    -> 2 x Gruszka
    -> 4 x Pomarańcz
    -> 6 x Śliwka
    2. Aplikacja wyświetla liste na której znadjdują się :
    -> 2 x Gruszka
    -> 4 x Grejpfrut 
    -> 6 x Śliwka
    3. Użytkownik poprawia na liście produktów :
    -> Grejpfrut => Pomarańcz
    4. Użytkownik akceptuje listę produktów.
    5. Użytkownik zostaje przekierowany na stronę główną aplikacji.


##### Stan końcowy :
    Spiżarnia użytkownika zostaje zaaktualizowana o nowe produkty.

#### Przypadek III :

##### Stan początkowy : 
    Użytkownik jest zalogowany.
    Użytkownik jest na widoku skanowania produktu.

##### Proces skanowania (porzucony) : 
    1. Użytkownik robi zdjęcie na którym są widoczne produkty:
    -> 2 x Gruszka
    -> 4 x Pomarańcz
    -> 6 x Śliwka
    2. Aplikacja wyświetla liste na której znadjdują się :
    -> 2 x Gruszka
    -> 4 x Grejpfrut
    -> 6 x Śliwka
    3. Użytkownik odrzuca listę produktów.
    4. Użytkownik zostaje przekierowany na stronę główną aplikacji.


##### Stan końcowy :
    Spiżarnia użytkownika zostaje bez zmian.
    


### Propozycja posiłku

    Opis : Sprawdzenie działania funkcji proponowania posiłków.

    Oczekiwany rezultat : Po wybraniu funckji "zaproponuj posiłek" aplikacja 
    zwraca listę proponowanych posiłków na podstawie spiżarki użytkownika


#### Przypadek I :

##### Stan początkowy :
    Użytkownik jest na widoku "propozycje posiłku".
    Użytkownik jest zalogowany. 
    W spiżarce znajdują się jakieś produkty.

##### Proces proponowania : 
    1. Użytkownik klika opcje "zaproponuj posiłek".
    2. Użytkonikowi przdstawione są proponowane posiłki.
    3. Użytkownik wybiera jedą z propozycji, i jest mu wyświetlany przepis.
    4. Użytkownik aktualizuje stan spiżarki poprzez zaznaczenie zużycia produktów.
    5. Użytkownik zostaje przekierowany na stronę główną aplikacji.


##### Stan końcowy :
    Stan spiżarni jest zaktualizowany o zużyte produkty.

#### Przypadek II :

##### Stan początkowy : 
    Użytkownik jest na widoku "propozycje posiłku".
    Użytkownik jest zalogowany. 
    Spiżarka nie jest pusta.

##### Proces proponowania : 
    1. Użytkownik klika opcje "zaproponuj posiłek".
    2. Użytkonikowi przdstawione są proponowane posiłki.
    3. Użytkownik wybiera jedą z propozycji.
    4. Użytkownikowi wyświetlany jest komunikat o braku pewnych produktów.
    5. Użytkownik klika "zrezygnuj".
    6. Użytkownik zostaje przekierowany na stronę główną aplikacji.

##### Stan końcowy :
    Brak zmian.
    

#### Przypadek III :

##### Stan początkowy : 
    Użytkownik jest na widoku "propozycje posiłku".
    Użytkownik jest zalogowany. 
    Spiżarka jest pusta.

##### Proces proponowania : 
    1. Użytkownik klika opcje "zaproponuj posiłek".
    2. Użytkonikowi zostaje wyświetlony komunikat : 
    "Spiżarka jest pusta"
    3. Użytkownik zostaje przekierowany na strone główną aplikacji.


##### Stan końcowy :
    Brak zmian.
    
## 2. Opracowanie pomiarów sprawdzających czy system spełnia wymagania niefunkcjonalne. ISO/IEC 9126 i 25 000

### Wydajność
*Cel:* Zapewnienie że aplikacja  jest responsywna i nie zwalnia znacząco gdy używa ją wiele klientów na raz.

*Kryteria:*
- Obciążenie - sprawdzenie jak system radzi sobie z dużą liczbą użytkowników korzystających z systemu jednocześnie (czy czas odpowiedzi sie zbytnio nie wydłuża; czy baza danych nie zostaje przeciążona itp.)
- Czas odpowiedzi - mierzenie ile czasu upływa od momentu wysłania żądania do odtrzymania odpowiedzi od systemu. Należy to przeprowadzić dla różnych operacji i scenariuszy (rejestracja, logowanie, skanowanie produktu, propozycja posiłku)

### Niezawodność
*Cel:* Sprawdzenie jak bardzo system jest odporny na downtime oraz jak szybko można przywrócić jego sprawność.

*Kryteria:*:
- Odporność na downtime - Czy da się tak przeciążyć system, aby przestał odpowiadać i się zawiesił.
- Symulacja downtime - Jak szybko można przywrócić działanie systemu po awarii jego części (n.p. przeciążenie bazy danych).

### Kompatybilność
*Cel:* Sprawdzenie czy aplikacja jest dostępna dla jak najszerszej gamy użytkowników.

*Kryteria:*:
- Czy aplikacja działa na Android i iOS
- Czy aplikacja jest dostępna na komputer, a jeśli tak, to na jakie systemy operacyjne
- Jeśli jest to webowa aplikacja - na jakich przeglądarkach i na jakich ich wersjach działa poprawnie

### Bezpieczeństwo
*Cel:* Zapewnienie bezpieczeństwa przechowywania danych oraz przestrzegania prawa RODO. Sprawdzenie czy aplikacja jest odporna na typowe cyberataki.

*Kryteria:*
- Czy hasła użytkowników są przechowywanie jako hasze
- Sprawdzenie podatności na typowe ataki - Cross Site Scripting, SQL injection itp.

### Utrzymanie
*Cel:* Sprawdzenie jak kosztowne (pieniężnie i roboczogodzinowo) jest długoterminowe utrzymanie aplikacji i jej rozwój.

*Kryteria:*
- Jak dobrze jest udokumentowany kod aplikacji
- Czy kod systemu ma strukturę zgodnie z którą był i będzie rozwijany
- Jak kosztowne jest utrzymanie serwerów oraz baz danych

### Dostępność
*Cel:* Sprawdzenie jak bardzo aplikacja jest przystosowana do używania przez osoby z niepełnosprawościami.

*Kryteria:*
- Czy w aplikacji jest dostępna opcja zwiększenia kontrastu
- Czy jest opcja zwiększenia czcionki

## 3. Opracowanie planu beta testowania.
Celem beta testowania jest rzetelne sprawdzenie tego jak aplikacja radzi sobie w świecie rzeczywistym. Pozwala na wychwycenie błędów, które trudno jest wychwycić podczas testów jednostkowych oraz na opinie zwrotne od osób, które używają aplikacji z perspektywy klienta końcowego, a nie osoby zaangażowanej w jej rozwijanie.

### Wybór użytkowników
- Wybranie wystarczająco dużej grupy użytkowników, która jest jak najbardziej różnorodna, gdyż nasza aplikacja jest skierowana do wszystkich.
- Zaproszenie takich użytkowników przez social media oraz newslettery.

### Przygotowanie wersji aplikacji, która będzie testowana
- Przygotowanie beta-wersji aplikacji, którą testerzy będą używać. Zapewnienie w niej wiele kanałów na odsyłanie feedbacku.
- Przygotowanie małego tutorialu/dokumentacji o tym jak z tej aplikacji się korzysta.

### Kryteria oceniane podczas beta-testowania
- Wydajność - Ile czasu zajmuje aplikacji przetworzenie różnych żądań, jak długo ładuje się aplikacja, jak bardzo jakość połączenia internetowego wpływa na działanie aplikacji.
- Stabilność - Czy aplikacja czasami zwalnia / zawiesza się / jest nieresponsywna.
- Użyteczność - Jak bardzo ta aplikacja się przydaje użytkownikom
- Niezawodność - Czy aplikacja jest ciągle dostępna i nie ma losowych przerw w jej działaniu
- Prostota i intuicyjność UI/UX - Czy przeciętny użytkownik aplikacji nie ma problemów z jej używaniem i jest w stanie samemu korzystać z wszystkich funkcji aplikacji.

### Analiza i poprawki
- Analiza feedbacku od testerów
- Wprowadzanie poprawek w oparciu o feedback od beta-testerów oraz informowanie ich o nowo wprowadzonych poprawkach.

### Koniec beta-testowania
Po zakończeniu fazy beta testowania należy sporządzić raport o opiniach użytkowników, poprawkach wprowadzonych do beta-wersji aplikacji oraz wydajności aplikacji. W raporcie zawrzeć plan działania na wdrożenie zmian w końcowej wersji aplikacji.

### Wdrożenie korekt do finalnego produktu
Bazując na raporcie z poprzedniego punktu, stopniowo wdrożyć zmiany do finalnej wersji aplikacji.


## 4. Napisanie planu zarządzania jakością oprogramowania.

- Ustalenia co do metryk - Określenie standardów i norm których się należy trzymać (n.p. ISO/IEC 9126. Zdefiniowanie  kryteriów akceptacji - n.p. Definition of Done.
- Testy - Użycie testów jednostkowych. Regularne testy wydajnościowe aby zapewnić stałe optymalne funkcjonowanie aplikacji. Testy obciążeniowe aby upewnić się, że serwery działają dobrze. Testy użyteczności i testy intuicyjności UI/UX aby sprawdzić jak typowy użytkownik odnajduje się w aplikacji. Testy bezpieczeństwa. Testy integracyjne aby zapewnić że wdrażanie oprogramowania na "produkcję" jest szybkie i bezbolesne.
- Kontrola jakości - Regularne code reviews. Używanie Jira do trackingu postępu projektu i bugów.
- Zbieranie feedbacku od beta-testerów (podczas fazy beta-testowania) i użytkowników, aby na bieżąco wprowadzać zmiany zgodnie z feedbackiem.


## 5. Sporządzenie dokładniejszego planu wykonania produktu, dokładniejszej oceny pracochłonności, dokładniejszego harmonogramu.

### Analiza i planowanie (1 - 10 tydzień)

#### 1 - 4 tydzień (ustalenie szegółów)
* Dokładne rozmowy z interseraiuszami aby jednoznacznie zrozumieć i udokumentować wymagania.
* Priorytetyzacja celów projektu.
* Wstępne określenie rozmiaru budżetu oraz wymagań co do składu zespołu.
* Ustalenie wstępnego planu realizacji projektu.

#### 5 - 8 tydzień (analiza rynku)
* Dogłębne przebadanie rynku w celu określenia końcowej wersji aplikacji. (t.j. czy takiej aplikacji już nie ma? a jeżeli jest to co będzie miała nasza czego nie mają inne?). 
* Inspiracja czyimiś istniejącymi rozwiązaniami (legalnie).
* Określenie zapotrzebowania na planowaną przez nas aplikację.
* Odpowiedzenie na pytania : 
    * Kto będzie naszą grupą odbiorczą?
    * Czy jest ona wystarczająco duża aby całość się opłacała?

#### 9 tydzień (poprawki)
* Zaktualizowanie i potwierdzenie planów projektu o informacje zdobyte w czasie analizy rynku. (w tym harmonogram, planowany budżet, skład zespołu, planowane technologie).

#### 10 - 11 tydzień (design UI)
* Zaprojektowanie intuicyjnego UI, zgodnego z najnowszymi trendami.
### 12 - 26 tydzień (zdobycie danych i wytrenowanie modelu)
* Zgromadzenie adekwatnie dużej ilości danych zawierających :
    * przepisy,
    * produkty,
    * modne diety,
    * itp.

### 27 - 42 tydzień (implementacja)
- Implementowanie funkcji aplikacji zgodnie z wcześniej ustalonym planem.
- Trzymanie się zasad kodowania.
- Pisanie dokumentacji.
- Testy jednostkowe.
- Testy integracyjne.

### 43 - 46 tydzień (wypuszczenie MVP)
- Przygotowanie planu na wdrożenie aplikacji.
- Przygotowanie infrastruktury na pierwszy deployment.
- Wdrożenie aplikacji.
- Baczne monitorowanie stanu aplikacji i na bieżąco naprawianie potencjalnych problemów.
- Zbieranie informacji zwrotnych od wczesnych klientów.

### 47 - 51 tydzień (końcowe poprawki i monitorowanie aplikacji)
- Opierając się na feedbacku od wczesnych użytkowników wprowadzenie poprawek.
- Skrupulatne monitorowanie stanu aplikacji, a w szczególności jej wydajności (patrząc na to, że w tym momencie liczba użytkowników będzie szybko rosła).

### 52 - 57 tydzień (wdrożenie aplikacji + advertisement)
- Opracowanie planu wdrożenia finalnej wersji aplikacji.
- Przygotowanie infrastruktury na zmiany.
- Wdrożenie aplikacji na produkcję.
- Rozpoczęciu marketingu.

### Od 58 tygodnia
- Monitorowanie stanu aplikacji.
- Zbieranie feedbacku od użytkowników.


## 6. Ocena zgodności wykonanych prac z wizją systemu i specyfikacją wymagań.

- Dodaliśmy 2 tygodnie na design UI.
- Przenieśliśmy reklamowanie do sekcji wdrożenia.
