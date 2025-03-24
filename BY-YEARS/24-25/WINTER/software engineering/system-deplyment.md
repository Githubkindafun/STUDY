# IO zadanie na styczeń - Wprowadzenie systemu/aplikacji na rynek

# Adam Majchrowski & Szymon Fluder

# 1 - Niezbędna pojemność bazy danych
### Użytkownicy
- Tabela `users`: 
    - rekordy: 15 000
    - kolumny: `id`, `first_name`, `last_name`, `email`, `password_hash`, `date_registered`
    - ok. 500 B / rząd

### Produkty
- Tabela `products`:
    - rekordy: 1 000
    - kolumny: `id`, `name`, `photo_path`
    - ok. 200 B / rząd
    
### Przepisy
- Tabela `recipes`:
    - rekordy: 2 000
    - kolumny: `id`, `name`, `description`, `insctructions`, `prepare_time`, `photo_path`
    - ok. 6500 B / rząd

### Kuchnie
- Tabela `cuisine_types`:
    - rekordy: 100
    - kolumny: `id`, `name`
    - ok. 100 B / rząd
    
### Alergeny
- Tabela `allergens`:
    - rekordy: 1 000
    - kolumny: `id`, `name`
    - ok. 100 B / rząd
    
    
### Wartości odżywcze
- Tabela `nutrients`:
    - rekordy: 100
    - kolumny: `id`, `name`
    - ok. 100 B / rząd
    
    
### Tabele wiążące
- Tabela `user_allergies`:
    - rekordy: 5 000 000
    - kolumny: `allergen_id`, `user_id`
    - ok. 20 B / rząd
- Tabela `user_cuisine_preferences`:
    - rekordy: 5 000 000
    - kolumny: `user_id`, `cuisine_id`
    - ok. 20 B / rząd
- Tabela `recipe_cuisine_types`:
    - rekordy: 2 000
    - kolumny: `user_id`, `cuisine_id`
    - ok. 20 B / rząd
- Tabela `product_nutrients`:
    - rekordy: 5 000
    - kolumny: `product_id`, `nutrient_id`
    - ok. 20 B / rząd
- Tabela `user_products`:
    - rekordy: 10 000 000
    - kolumny: `id`, `user_id`, `product_id`, `quantity`, `expiration_date`
    - ok. 50 B / rząd
    
Zdjęcia nie są trzymane w bazie danych, ale też chcemy policzyć ile miejsca zajmą.

    
Sumarycznie ok. 720 MB + 3 GB na zdjęcia (założenie, że 1 zdjęcie = 1 MB).
 
# 2 - Plan wdrożenia

### Werjsa beta aplikacji
- Określenie na jakich platformach beta bedzie wypuszczona
- Przygotowanie środowisk produkcyjnych
- Certyfikat(y) SSL
- Wdrożenie aplikacji na środowisko produkcyjne

### Pierwsza wersja aplikacji
- Powiadomienie beta testerów o tym, że faza beta testowania się kończy
- Biorąc pod uwagę feedback z beta wersji aplikacji, wdrożyć zmiany do finalnej wersji
- Wdrożyć finalną wersję na produkcję


# 3 - Koncepcja organizacji szkoleń użytkowników

- Po założeniu konta użytkownikowi zostanie wyświetlony "step-by-step" tutorial, pokazujący wszystkie funkcjonalności aplikacji.

- Na platformie youtube zamieszczony będzie "walk-through", przdstawiający funkcjonalności aplikacji.

- Po aktualizacjach zmieniających funkcjonalności, będą wyświetlały się w aplikacji, podsumownia zmian wraz z "tutorialami" tłumaczącymi zmiany.


# 4 - Koncepcja wsparcia technicznego w tym usuwania błędów i zapewnienia ciągłości działania

- Obsługa klienta - Helpdesk
- Możliwość zgłaszania błędów przez użytkowników
- CI/CD pipeline: 
    1. commit zmian
    2. build nowej wersji
    3. automatyczne testy
    4. dodanie nowej wersji do staging area
    5. wdrożenie nowej wersji na produkcję
- Regularne monitorowanie stanu aplikacji i jej wyjajności
- Szybkie reagowanie na incydenty typu downtime / próby hackowania / DDOS
- Regularne tworzenie backupów bazy danych
- Regularne aktualizacje, aby standardy bezpieczeństwa były zachowane

# 5 - Główne punkty umów

- Dokładne zdefiniowanie stron zawierających umowę
- Prawa jakie rezerwują sobie właściciele aplikacji (n.p. co mogą zrobić przy wykryciu skanowania nielegalnych materiałów)
- Polityka prywatności
- Własność intelektualna i prawa autorskie
- Konsekwencje nieprzestrzegania umowy
- Określenie warunków i procedury zerwania umowy
- Określenie sposobów rozwiązywania konfliktów między stronami
- Zdefiniowanie akceptowanych środków płatności
- Czas obowiązywania umowy

# 6 - Sposób pomiaru satysfakcji klienta

- Śledzenie opinii użytkowników w mediach społecznościowych i w np.: 
    * App Store
    * Google Play Store
- Okresowe ankiety śledzące poziom zadowolenia klientów oraz późniejsza ich analiza.
- Przy wypowiedzeniu subskrypcji przez użytkownika opcja dodania powodu rezygnacji. Zbieranie tych danych i analizowanie ich.
- Okazjonalne powiadomienia o możliwości ocenienia aplikacji.
- Zbieranie feedbacku z profili na mediach społecznościowych.
