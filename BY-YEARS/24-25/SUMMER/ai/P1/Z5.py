import random


# z zadania 4
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


# pomocnicza funkcja sprawdzajaca czy dana linijka jest ok
def done_line(nono, D):
    total = sum(nono)
    if total != D:
        return False
    if D == 0:
        return True

    # pierwsze wystąpienie 1
    first = 0
    while first < len(nono) and nono[first] == 0:
        first += 1

    # ostatnie wystąpienie 1
    last = len(nono) - 1
    while last >= 0 and nono[last] == 0:
        last -= 1

    return (last - first + 1) == D


# sprawdzamy czy nonogram jest kompletny (chyba tak sie mowi)
def solved_nono(nono, nono_x, nono_y):
    size_x = len(nono)
    size_y = len(nono[0])
    for i in range(size_x):
        if not done_line(nono[i], nono_x[i]):
            return False
    for j in range(size_y):
        col = [nono[i][j] for i in range(size_x)]
        if not done_line(col, nono_y[j]):
            return False
    return True


def best_spot_in_row(nono, row_index, row_target, nono_y):
    best_improve = -float("inf")
    best_j = None
    size_y = len(nono[0])
    # w zasadzie translate robimy aby opt_dist działał
    row_str = "".join(str(x) for x in nono[row_index])
    # dostajemy jakiś wynik dla wiersza bez zmian
    old_row_val = opt_dist(row_str, row_target)
    for j in range(size_y):
        if nono[row_index][j] == 0:
            # niestety takie fikołki są potrzebne bo opt_dist ma lekko inny input
            new_row_list = list(row_str)
            new_row_list[j] = "1"
            new_row_val = opt_dist("".join(new_row_list), row_target)

            # sprawdzamy jaki to da efekt na j kolumnie
            col_list = [str(nono[i][j]) for i in range(len(nono))]
            old_col_val = opt_dist("".join(col_list), nono_y[j])
            new_col_list = col_list.copy()
            new_col_list[row_index] = "1"
            new_col_val = opt_dist("".join(new_col_list), nono_y[j])

            # tutaj korzystajac z wynikow z opt_dista sprawdzamy przy jakiej konfiguracji
            # row/col uzsykamy najelpsza redukcję
            improvement = (old_row_val + old_col_val) - (new_row_val + new_col_val)
            if improvement > best_improve:
                best_improve = improvement
                best_j = j
    return best_j


# tu mamy analogicznie tylko dla kolumn
def best_spot_in_col(nono, col_index, col_target, nono_x):
    best_improve = -float("inf")
    best_i = None
    size_x = len(nono)
    col_list = [str(nono[i][col_index]) for i in range(size_x)]
    old_col_val = opt_dist("".join(col_list), col_target)

    for i in range(size_x):
        if nono[i][col_index] == 0:
            new_col_list = col_list.copy()
            new_col_list[i] = "1"
            new_col_val = opt_dist("".join(new_col_list), col_target)

            row_str = "".join(str(x) for x in nono[i])
            old_row_val = opt_dist(row_str, nono_x[i])
            new_row_list = list(row_str)
            new_row_list[col_index] = "1"
            new_row_val = opt_dist("".join(new_row_list), nono_x[i])
            improvement = (old_col_val + old_row_val) - (new_col_val + new_row_val)
            if improvement > best_improve:
                best_improve = improvement
                best_i = i
    return best_i


def parse_input(input_data):
    values = input_data.split()
    numbers = [int(val) for val in values]
    size_x = numbers[0]
    size_y = numbers[1]
    nono_x = [numbers[i] for i in range(2, 2 + size_x)]
    nono_y = [numbers[i] for i in range(2 + size_x, 2 + size_x + size_y)]
    return size_x, size_y, nono_x, nono_y


def solve(input_data):
    # lekka translacja danych
    size_x, size_y, nono_x, nono_y = parse_input(input_data)
    # random.seed(seed)
    # nono = [[random.randint(0, 1) for _ in range(size_y)] for _ in range(size_x)]
    nono = [[0] * size_y for _ in range(size_x)]  # same 0

    while True:
        # nono = [[random.randint(0, 1) for _ in range(size_y)] for _ in range(size_x)]
        nono = [[0] * size_y for _ in range(size_x)]
        for swap in range(100):
            if solved_nono(nono, nono_x, nono_y):
                output = []
                for row in nono:
                    line = "".join("#" if cell == 1 else "." for cell in row)
                    output.append(line)
                return output

            # losowo wybieramy czy bedziemy zmieniac wiersz czy kolumne
            if random.randint(0, 1) == 0:  # wiersz
                # lista tych co nie sa ok
                unsolved_rows = [
                    i for i in range(size_x) if not done_line(nono[i], nono_x[i])
                ]
                if unsolved_rows:
                    i = random.choice(unsolved_rows)
                    j = best_spot_in_row(nono, i, nono_x[i], nono_y)
                    if j is not None:
                        nono[i][j] = 1
            else:  # kolumna
                # analogicznie
                unsolved_cols = []
                for j in range(size_y):
                    col = [nono[i][j] for i in range(size_x)]
                    if not done_line(col, nono_y[j]):
                        unsolved_cols.append(j)

                if unsolved_cols:
                    j = random.choice(unsolved_cols)
                    i = best_spot_in_col(nono, j, nono_y[j], nono_x)
                    if i is not None:
                        nono[i][j] = 1


def Z5(input):
    res = solve(input)
    return "\n".join(res)


print(Z5("7 7\n7\n7\n7\n7\n7\n7\n7\n7\n7\n7\n7\n7\n7\n7\n"))
print("\n")
print(Z5("7 7\n2\n2\n7\n7\n2\n2\n2\n2\n2\n7\n7\n2\n2\n2\n"))
print("\n")
print(Z5("7 7\n2\n2\n7\n7\n2\n2\n2\n4\n4\n2\n2\n2\n5\n5\n"))
print("\n")
print(Z5("7 7\n7\n6\n5\n4\n3\n2\n1\n1\n2\n3\n4\n5\n6\n7\n"))
print("\n")
print(Z5("7 7\n7\n5\n3\n1\n1\n1\n1\n1\n2\n3\n7\n3\n2\n1\n"))
