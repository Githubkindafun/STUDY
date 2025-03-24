from collections import Counter
import random

"""
oki doki obserwacja jest (chyba, nie wiem czy dobrze rozumuje) calkiem latwa
(pisze to na nowo już nie poraz pierwszy więc możę aż tak nie jest oczywista)
jak przyjzymy sie temu jakie sa uklady karciane to jedyne mozliwe sytuacje gdzie wygrywa blotkarz to te gdzie ma
straight / pokera bo w innych zestawieniach head to head tj. para vs para etc to zawsze wygra figurant
zatem musimy sprawdzic te uniqe cases najpierw i hierahicznie zejsc w dol az do po prostu porownywania ich head to head 
co powinno byc juz luzne
"""

# lista 52 kart najlepiej tuple (2-14, P/K/C/T)
# losowanie po 5 kart
# no i funkcja do sprawdzenia co tam Figurant / Blotkarz maja
# decyzja kto wygral i tyle ;)# List of cards from 2 to 10

cards_numbers = [
    (value, suit) for value in range(2, 11) for suit in ["H", "D", "C", "S"]
]


cards_faces = [
    (value, suit) for value in range(11, 15) for suit in ["H", "D", "C", "S"]
]


def isConsecutive(values):  # czy sa po koleji
    return (
        max(values) - min(values) == 4 and len(set(values)) == 5
    )  # czy roznica jest odpowiednia i czy jest 5 uniqe kart


def isFlush(hand):  # czy sa jednego koloru
    return set(card[1] for card in hand) == 1


def didBWon(hand_f, hand_b):  # tu basicly decyzja bedzie kto wygrywa
    values_f = [t[0] for t in hand_f]  # karty figuranta
    values_b = [t[0] for t in hand_b]  # karty blotkarza

    # primo poker krolewski odpada nie moze poprostu wystapic
    # sprawdzmy czy jest poker
    if isConsecutive(values_b) and isFlush(hand_b):
        return True

    counter_f = Counter(values_f)
    counter_b = Counter(values_b)

    # sprawdzamy top 2 najpopularniejsze karty
    most_common_two_f = counter_f.most_common(2)
    most_common_two_b = counter_b.most_common(2)
    # print(most_common_two_f)
    # print(most_common_two_b)

    if isConsecutive(values_b) or isFlush(hand_b):  # sytuacja koloru badz strita
        if (
            most_common_two_f[0][1] == 4
            or (most_common_two_f[0][1] == 3 and most_common_two_f[1][1] == 2)
            # or isFlush(hand_f) # niemożliwe
        ):
            return False

    # dobra zasadniczo zostaly te bardziej straight-forward
    if most_common_two_f[0][1] < most_common_two_b[0][1]:
        return True
    elif (
        most_common_two_f[0][1] == most_common_two_b[0][1]
        and most_common_two_f[1][1] < most_common_two_b[1][1]
    ):
        return True
    else:
        return False


# ok moze jest brzydko ale taki byl pomysl w srodku nocy (ok. 7% ~ 8%)
print("Test 1. normalny zestaw tak jak podane w zadaniu")
r = 100
R = 100
C = 0
for j in range(0, R):
    c = 0
    for i in range(0, r):
        f = random.sample(cards_faces, 5)
        b = random.sample(cards_numbers, 5)
        if didBWon(f, b):
            c += 1
    # print(c / r)
    C += c / r
print(f"{round((C / R) * 100)}% sredniza z {R} prob po {r}")


# ok teraz chcemy sobie potestowac rozne mozliwe decki dla Blotkarza
# jakie byly by najbardziej oplacalne przy zachowaniu jak najwiekszego rozmiaru
# Obserwacja : nie ma znaczenia jakie karty wezmiemy jezeli chodzi o wartosc bo i tak beda
#   mniejsze w bezposrednim zestawieniu ewentualnie jakbysmy chcieli cos robic ze stritem to wtedy musza byc pod rzad

# Test 2. : wezmy piec kolejnych kart reszte odrzucmy np.: 2, 3, 4, 5, 6


cards_numbers_2 = [
    (value, suit) for value in range(2, 7) for suit in ["H", "D", "C", "S"]
]

print("Test 2. zestaw gdzie Blotkarz ma karty 2 - 6")  # (ok. 22% ~ 23%)
r = 100
R = 100
C = 0
for j in range(0, R):
    c = 0
    for i in range(0, r):
        f = random.sample(cards_faces, 5)
        b = random.sample(cards_numbers_2, 5)
        if didBWon(f, b):
            c += 1
    # print(c / r)
    C += c / r
print(f"{round((C / R) * 100)}% sredniza z {R} prob po {r}")


# dobra jest duzo lepiej zobaczmy czy to sie zmieni jak zmniejszymy ilosc kart
# skoro statystycznie lepiej miec np czestsze 2 / 3 / 4 niz potencjalnego strita (chyba)

# Test 3.: karty 2, 3, 4, 5

cards_numbers_3 = [
    (value, suit) for value in range(2, 6) for suit in ["H", "D", "C", "S"]
]

print("Test 3. zestaw gdzie Blotkarz ma karty 2 - 5")  # (ok. 34% ~ 35%)
r = 100
R = 100
C = 0
for j in range(0, R):
    c = 0
    for i in range(0, r):
        f = random.sample(cards_faces, 5)
        b = random.sample(cards_numbers_3, 5)
        if didBWon(f, b):
            c += 1
    # print(c / r)
    C += c / r
print(f"{round((C / R) * 100)}% sredniza z {R} prob po {r}")


# okej intuicja nie zawiodla (zakladajac ze dalej dobrze mysle), wiec sprobojmy jeszcze mniej kart
# Test 4.: karty od 2, 3, 4

cards_numbers_4 = [
    (value, suit) for value in range(2, 5) for suit in ["H", "D", "C", "S"]
]

print("Test 4. zestaw gdzie Blotkarz ma karty 2 - 4")  # (ok. 56% ~ 58%)
r = 100
R = 100
C = 0
for j in range(0, R):
    c = 0
    for i in range(0, r):
        f = random.sample(cards_faces, 5)
        b = random.sample(cards_numbers_4, 5)
        if didBWon(f, b):
            c += 1
    # print(c / r)
    C += c / r
print(f"{round((C / R) * 100)}% sredniza z {R} prob po {r}")

# hmm ciekawie sprobojmy dalej
# Test 5.: 2, 3 (przy okazji dalej juz tak poprostu nie zejdziemy, aleee chyba wystarczy)

cards_numbers_5 = [
    (value, suit) for value in range(2, 4) for suit in ["H", "D", "C", "S"]
]

print("Test 5. zestaw gdzie Blotkarz ma karty 2 - 3")  # (ok. 93% ~ 94%)
r = 100
R = 100
C = 0
for j in range(0, R):
    c = 0
    for i in range(0, r):
        f = random.sample(cards_faces, 5)
        b = random.sample(cards_numbers_5, 5)
        if didBWon(f, b):
            c += 1
    # print(c / r)
    C += c / r
print(f"{round((C / R) * 100)}% sredniza z {R} prob po {r}")
