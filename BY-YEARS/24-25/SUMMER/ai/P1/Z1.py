# dobra zacznijmy od tego co chcemy w tym zadaniu zrobić
# naszym celem jest odpowiedzenie na pytanie ile ruchów potrzeba
# aby zamatować czarnego króla (minimalnie).
# Na planszy znajduje się biały król, biała wierza oraz czarny król.
# dane są podane jako white/black WK WR BK.
# plan zasadniczo jest dość prosty wykorzystamy BFS po drzewie stanów.
# Ważne dla nas aby sprawdzać czy potencjalne ruchy są
# 1) legalne
# 2) nie prowadzą do stanu w którym szachownica już była
# tyle ;>


from collections import deque


# sprawdzamy czy row / col mieszcza sie w przedziale 0, ..., 7
def in_board(coords):
    return (coords[0] < 8 and coords[0] >= 0) and (coords[1] < 8 and coords[1] >= 0)


# generujemy możliwe ruchy dla króla white / black
def king_moves(coords):
    moves = []
    for row_direction in [-1, 0, 1]:  # dół / bez zmian / góra
        for col_direction in [-1, 0, 1]:
            if row_direction == 0 == col_direction:
                continue
            new_coords = (coords[0] + row_direction, coords[1] + col_direction)
            if in_board(new_coords):  # tu sie przyda taka funkcja
                moves.append(new_coords)
    return moves


def adjacent(coords1, coords2):
    return abs(coords1[0] - coords2[0]) <= 1 and abs(coords1[1] - coords2[1]) <= 1


def legal_white_king(new_coords, wr, bk):
    if not in_board(new_coords):
        return False
    if wr is not None and new_coords == wr:
        return False
    if adjacent(new_coords, bk):
        return False
    return True


def between(coords1, coords2, block_coords):
    if coords1[0] == coords2[0] == block_coords[0]:  # są w tym samym rzedzie
        if min(coords1[1], coords2[1]) < block_coords[1] < max(coords1[1], coords2[1]):
            return True
    if coords1[1] == coords2[1] == block_coords[1]:  # są w tej samej kolumnie
        if min(coords1[0], coords2[0]) < block_coords[0] < max(coords1[0], coords2[0]):
            return True
    return False


def rook_attack(coords, wr, wk):
    if wr is None:
        return False
    if coords[0] == wr[0]:
        if not between(wr, coords, wk):
            return True
    if coords[1] == wr[1]:
        if not between(wr, coords, wk):
            return True
    return False


def legal_black_king(new_cords, wk, wr):
    if not in_board(new_cords):
        return False
    if adjacent(new_cords, wk):
        return False
    rook_slaughter = False
    if wr is not None and new_cords == wr:
        rook_slaughter = True
    if not rook_slaughter and rook_attack(new_cords, wr, wk):
        return False
    return True


# dobra tutaj mamy wszystkie ruchy wiezy
def rook_moves(coords, wk, bk):
    moves = []
    for row_directions, col_directions in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
        row_current, col_current = coords
        while True:
            row_current += row_directions
            col_current += col_directions
            if not in_board((row_current, col_current)):
                break
            if (row_current, col_current) == wk or (row_current, col_current) == bk:
                break
            moves.append((row_current, col_current))
    return moves


# przydadzą nam się ruchy wszystkich figur
def generate_moves(state):
    turn, wk, wr, bk = state
    moves = []
    if turn == "white":
        for new_coords in king_moves(wk):
            if legal_white_king(new_coords, wr, bk):
                new_state = ("black", new_coords, wr, bk)
                description = f"White king at {wk} to {new_coords}"
                moves.append((description, new_state))
        if wr is not None:  # zasadniczo jeśli istnieje
            for new_coords in rook_moves(wr, wk, bk):
                new_state = ("black", wk, new_coords, bk)
                description = f"White rook at {wr} to {new_coords}"
                moves.append((description, new_state))
    else:
        for new_coords in king_moves(bk):
            if legal_black_king(new_coords, wk, wr):  # to trzeba dodać
                if wr is not None and new_coords == wr:  # mordowana wieża
                    new_state = ("white", wk, None, new_coords)
                    description = (
                        f"Black king at {bk} to {new_coords} (just killed the rook)"
                    )
                else:
                    new_state = ("white", wk, wr, new_coords)
                    description = f"Black king at {bk} to {new_coords}"
                moves.append((description, new_state))
    return moves


def check(state):
    turn, wk, wr, bk = state
    if wr is None:  # pat
        return False
    if bk[0] == wr[0]:
        if not between(wr, bk, wk):
            return True
    if bk[1] == wr[1]:
        if not between(wr, bk, wk):
            return True
    return False


def mate(state):
    turn, wk, wr, bk = state
    if turn != "black":
        return False
    if wr is None:  # pat
        return False
    if not check(state):
        return False
    for new_coords in king_moves(bk):
        if legal_black_king(new_coords, wk, wr):
            return False
    return True


# bfs
def bfs(initialization):
    queue = deque()
    queue.append((initialization, []))
    visited = set()
    visited.add(initialization)

    while queue:
        state, moves = queue.popleft()
        if mate(state):
            return moves, state
        for move, new_state in generate_moves(state):
            if new_state not in visited:
                visited.add(new_state)
                queue.append((new_state, moves + [move]))
    return None, None


def translate(position):
    letter = position[0]
    number = position[1]
    col = ord(letter) - ord("a")
    row = 8 - int(number)
    return (row, col)


def parse_input(input):
    data = input.strip().split()
    turn = data[0].lower()
    wk = translate(data[1])
    wr = translate(data[2])
    bk = translate(data[3])
    return turn, wk, wr, bk


# tutaj wszystko będziemy odpalać
def solve(input):
    initial_state = parse_input(input)
    moves, mate_state = bfs(initial_state)
    if moves is None:
        return "inf"
    return moves, mate_state


if __name__ == "__main__":

    # str_input = "black g8 h1 c4" # 10
    # str_input = "black b4 f3 e8" # 6
    # str_input = "white a1 e3 b7" # 9
    # str_input = "black h7 a2 f2"  # 6
    str_input = "black a2 e4 a4"  # 8
    moves_seq, mate_state = solve(str_input)

    if moves_seq == "inf":
        print("Mate is impossible from the given configuration (inf).")
    else:
        print("Sequence of moves to deliver mate:")
        for move in moves_seq:
            print(move)
        print("\nMate reached in state:", mate_state)
        print("\n" + str(len(moves_seq)))
