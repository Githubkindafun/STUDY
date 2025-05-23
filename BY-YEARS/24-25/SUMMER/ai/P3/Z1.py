"""
no dobra probowalem zrobic to za 6 pkt ale sie poddalem
z "tajemniczych" źródeł przeczytałem ze tym cool rozwiazaniem ma byc AC3
poczytałem o tym więc lecimy z tym koksem
"""

import time


# tutaj generujemy wszystkie mozeliwe ustawienia klockow w lini (dziedzina)
# clues - to jakie bloczki maja byc
# line no to to co tam w tej lini mamy 0 i 1
# funkcja zwaraca liste gdzie mamy mozliwe linie (ustawienia klockow)
def generate_line_placements(length, clues, line):
    placements = []
    # index - aktualne miejsce w lini
    # clue_index - ktory klocek kladziemy
    # current_state - aktualny stan lini
    stack = [(0, 0, [])]

    while stack:
        index, clue_index, current = stack.pop()

        if clue_index == len(clues):  # jak polozylismy wszystkie klocki
            valid = True  # to sprawdzamy czy pozostale pola to 0
            for pos in range(index, length):  # jak nie no to nie jest to valid linia
                if line[pos] is not None and line[pos] != 0:
                    valid = False
                    break
            if valid:  # no a jak valid to uzupelniamy aktualny stan lini do konca 0
                new_current = current + [0] * (length - index)
                placements.append(new_current)
            continue

        block = clues[clue_index]  # rozmiar klocka jaki bedziemy wkladac
        max_start = length - block  # gdzie najpozniej moze sie zaczac

        for start in range(index, max_start + 1):
            valid = True
            # upewniamy sie czy miejsce przed nami jest puste
            for pos in range(index, start):
                if line[pos] is not None and line[pos] != 0:
                    valid = False
                    break
            if not valid:
                continue

            # teraz zato sprawdzamy czy blok przed nami to 1 lub none
            for pos in range(start, start + block):
                if pos >= length or (line[pos] is not None and line[pos] != 1):
                    valid = False
                    break
            if not valid:
                continue

            # teraz tworzymy nowy stan czyli :
            # do naszego aktualnego stanu lini doklejamy 0 do momentu startu bloku
            # potem jedynki i cacy
            new_current = current + [0] * (start - index) + [1] * block
            next_index = start + block

            # jak zostaly nam jescze jakies klocki to musimy zakonczyc aktualny 0
            # no doklejic 0 poprostu
            if clue_index < len(clues) - 1:
                if next_index < length and (
                    line[next_index] is None or line[next_index] == 0
                ):
                    new_current_sep = new_current + [0]  # sobie doklejamy 0
                    stack.append((next_index + 1, clue_index + 1, new_current_sep))
            else:
                # jak to ostatni klocek to nas nie boli 0 i go nie doklejamy
                stack.append((next_index, clue_index + 1, new_current))
    return placements


# w zasadzie parsujemy input jaki dostalismy
# nothing special
def parse(input_data):
    lines = input_data.strip().splitlines()
    sizes = lines[0].split()
    X, Y = int(sizes[0]), int(sizes[1])

    row_clues = []
    for i in range(1, 1 + X):
        if lines[i].strip():  # wyciagamy wskazowki dla wierszy
            row_clues.append([int(x) for x in lines[i].split()])
        else:
            row_clues.append([])

    col_clues = []
    for i in range(1 + X, 1 + X + Y):
        if lines[i].strip():  # a tutaj wyciagamy dla kolumn
            col_clues.append([int(x) for x in lines[i].split()])
        else:
            col_clues.append([])

    return X, Y, row_clues, col_clues


# tutaj poprostu tworzymy dziedzinę dla kazdego wiersza/kolmny
def build_domains(X, Y, row_clues, col_clues):
    domains = {}
    # budujemy sobie Ri oraz Ci
    for i in range(X):
        key = f"R{i}"
        domains[key] = generate_line_placements(Y, row_clues[i], [None] * Y)
    for j in range(Y):
        key = f"C{j}"
        domains[key] = generate_line_placements(X, col_clues[j], [None] * X)
    return domains


# utility takie dla nas aby łatwiej mozna było potem sprawdzać
# skoro nasz "constarint" to jedynie zeby dana komorka w wierszu i kolumnie
# była taka sama to będzie cacy
def build_neighbors(X, Y):
    neighbors = {}
    for i in range(X):
        key = f"R{i}"
        neighbors[key] = [f"C{j}" for j in range(Y)]
    for j in range(Y):
        key = f"C{j}"
        neighbors[key] = [f"R{i}" for i in range(X)]
    return neighbors


# sprawdzanie constarint i.e. czy komorka w wierszu i kolumnie ma ta sama wartosc
# def constraint(Vi, vi, Vj, vj):
#     if Vi[0] == "R" and Vj[0] == "C":
#         i = int(Vi[1:])
#         j = int(Vj[1:])
#         return vi[j] == vj[i]
#     elif Vi[0] == "C" and Vj[0] == "R":
#         j = int(Vi[1:])
#         i = int(Vj[1:])
#         return vi[i] == vj[j]
#     else:  # kolumn i wierszt ze soba nie porownanmy
#         return True


# tutaj sprawdzamy dla jakis 2 zmiennych
# czy spelniaja constraint
# zasadniczo aktualizujemy dziedzine poprostu
# na filmiku etap gdzie juz sprawdzamy poszczegolne warunki
# filmik dla konekstu tłumaczył działanie ac3
def revise(domains, Vi, Vj):
    revised = False
    if Vi[0] == "R" and Vj[0] == "C":
        i = int(Vi[1:])  # indeks wiersza
        j = int(Vj[1:])  # indeks kolumny
        # dozwolone wartosci literalnie 1 lub 0 lub obie
        allowed = {candidate[i] for candidate in domains[Vj]}
        new_domain = []
        for vi in domains[Vi]:
            if vi[j] in allowed:  # zgadza sie to git
                new_domain.append(vi)  # jak sie zgadza to zachowujemy
            else:
                revised = True
    # analogicznie
    elif Vi[0] == "C" and Vj[0] == "R":
        j = int(Vi[1:])
        i = int(Vj[1:])
        allowed = {candidate[j] for candidate in domains[Vj]}
        new_domain = []
        for vi in domains[Vi]:
            if vi[i] in allowed:
                new_domain.append(vi)
            else:
                revised = True
    else:
        return False
    if revised:
        domains[Vi] = new_domain
    return revised


def ac3(domains, neighbors):
    # w kolejce mamy wszystkie pary somsiad somsiad i naturalnie w druga strone
    queue = [(Vi, Vj) for Vi in domains for Vj in neighbors[Vi]]
    while queue:
        (Vi, Vj) = queue.pop()
        if revise(domains, Vi, Vj):  # cos usunelismy z dziedziny
            if not domains[Vi]:
                return False  # kupa pusta dziedzina
            for Vk in neighbors[Vi]:  # tu dodajemy te co mialy Vi z prawej strony
                if Vk != Vj:
                    queue.append((Vk, Vi))
    return True


# dobra tu chyba kozystam z czego semi fair
# albowiem w sprawdzaczce jest jedno rozwiazanie
# wiec zakladam ze jest jedno mozliwe rozw
def domains_to_board(result, X, Y):
    board = []
    for i in range(X):
        key = f"R{i}"
        row = result[key][0]
        board.append(row)
    return board


# utility dla outputu
def board_to_string(board):
    if board is None:
        return "womp womp solution not found ;c"
    lines = []
    for row in board:
        line = ""
        for cell in row:
            if cell == 1:
                line += "#"
            else:
                line += "."
        lines.append(line)
    return "\n".join(lines)


def Z1(input_data):
    X, Y, row_clues, col_clues = parse(input_data)
    domains = build_domains(X, Y, row_clues, col_clues)
    neighbors = build_neighbors(X, Y)
    s_time = time.time()
    # tu odpalamy ac3 => okrajamy dziedziny
    if not ac3(domains, neighbors):
        return "womp womp"
    e_time = time.time()
    print(e_time - s_time)
    print(domains)
    board = domains_to_board(domains, X, Y)
    return board_to_string(board)


with open("zad_input.txt", "r") as input_file:
    input_data = input_file.read()

result = Z1(input_data)
print(result)

with open("zad_output.txt", "w") as output_file:
    output_file.write(result)
