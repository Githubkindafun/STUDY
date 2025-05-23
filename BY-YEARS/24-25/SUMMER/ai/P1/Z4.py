# okay pierwszy i chyba najprostszy pomysł to rozpoczynanie od poczatku kazdego wiersza i
# po koleji zmieniajac "start" (oznacza gdzie zaczyna sie "blok") sprawdzac ile bitow musze zamienic aby
# osiagnac oczekiwany rozmiar bloku D
# jedyny wyjatek to D = 0 wtedy ofc poprostu przechodzimy i zmieniamy bity na 0 inaczej sie chyba nie da


def opt_dist(nono, D):
    if D == 0:
        m = 0
        for bit in nono:
            if bit == "1":
                m += 1
        return m

    # teraz sprawdzamy zasadnicze wartosci D
    # a i oczywiscie zakladam ze D nalezy do naturalnych
    res = []
    n = len(nono)
    for start in range(0, n - D + 1):
        # c = 0  # ilosc 1
        m = 0  # ilosc zmian wartosci bitow

        for bit in range(start, start + D):  # tutaj ustawiamy nasz blok zaczynajac od D
            if nono[bit] == "0":
                m += 1

        # tutaj wszystkie 1 poza blokiem ustawiamy na 0
        for bit in range(0, start):
            if nono[bit] == "1":
                m += 1

        for bit in range(start + D, n):
            if nono[bit] == "1":
                m += 1

        res.append(m)
    return min(res)


# print(opt_dist("0010001000", 5))
# print(opt_dist("0010001000", 4))
# print(opt_dist("0010001000", 3))
# print(opt_dist("0010001000", 2))
# print(opt_dist("0010001000", 1))
# print(opt_dist("0010001000", 0))

# teraz tak moje szeroko otwarte uszy wylapaly ze da sie to zrobic lepiej (kluczem sa sumy prefiskowe)
# TO DO
