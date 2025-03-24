# zad. 7. sudoku
def rozwiązanie_sudoku(s):
    sudoku = s

    edge_case = []
    for row in sudoku:
        for x in row:
            edge_case.append(x)
    if all(edge_case.count(n) == 0 for n in range(0,1)): return sudoku

    # mam cyferki od 1 do 9
    def check_line_horizontal(y): # sprawdzamy wiersz
        line = sudoku[y]
        return all(line.count(n) <= 1 for n in range(1, 10))

    def check_line_vertical(x):
        line = [sudoku[y][x] for y in range(9)]
        return all(line.count(n) <= 1 for n in range(1, 10))

    def check_square(x, y): #cords for left upper
        square = [sudoku[y + j][x + i] for i in range(3) for j in range(3)] 
        #square = [sudoku[y][x] for x in range(x, x + 3) for y in range(y, y + 3)] 
        return all(square.count(n) <= 1 for n in range(1, 10))

    def is_valid():
        for i in range(9):
            if not (check_line_horizontal(i)): return False
            if not (check_line_vertical(i)): return False
        for x in range(3):
            for y in range(3):
                if not (check_square(3 * x, 3 * y)): return False
        return True


    def any_zeros():
        return any(0 in line for line in sudoku) # ide po kazdej lini i sprawdzam czy gdzies jest zero , jak nigdzie nie ma to false   

    #print(sudoku)
    #print(any_zeros())

    def solve_s():
        #if is_valid: return sudoku
        for x in range(9):
            for y in range(9):
                if sudoku[y][x] == 0:
                    for i in range(1, 10):
                        sudoku[y][x] = i
                        # ten print ladnie pokazuje jak nam dziala solve 
                        #print(f"{x} {y} {i} {is_valid()} {not any_zeros()}")
                        if is_valid():
                            if not any_zeros(): return sudoku
                            else: return solve_s()
        return None 
    return solve_s()
    

s1 = [ # sudkou z zadania
    [5, 3, 4, 6, 7, 8, 9, 1, 2],
    [6, 7, 2, 1 ,9, 5, 3, 4, 8],
    [1, 9, 8, 3, 4, 2, 5, 6, 7],
    [8, 5, 9, 7, 6, 1, 4, 2, 3],
    [4, 2, 6, 8, 5, 3, 7, 9, 1],
    [7, 1, 3, 9, 2, 4, 8, 5, 6],
    [9, 6, 1, 5, 3, 7, 2, 8, 4],
    [2, 8, 7, 4, 1, 9, 6, 3, 5],
    [3, 4, 5, 2, 8, 6, 1, 7, 9],
]

s2 = [ # sudkou z zadania z pustymi miejscami (0 - puste miejsce)
    [0, 3, 4, 6, 7, 8, 9, 1, 2],
    [6, 7, 2, 1 ,9, 0, 3, 4, 8],
    [1, 9, 8, 3, 4, 2, 5, 6, 0],
    [8, 5, 9, 7, 6, 1, 4, 2, 3],
    [4, 2, 6, 8, 5, 3, 7, 9, 1],
    [7, 1, 0, 9, 2, 4, 8, 5, 6],
    [9, 6, 1, 5, 3, 7, 2, 8, 4],
    [2, 8, 7, 4, 1, 9, 6, 3, 5],
    [0, 4, 5, 2, 8, 6, 1, 7, 0],
]
# nice print
def sudoku_print(sudoku):
    if sudoku == None: return None
    for row in sudoku:
        print(row[0], end="")
        for i in range(1, len(row) - 2):
            print(f"|{row[i]}", end="")
        print(f"|{row[len(row) - 1]}")
    print("\n")


su1 = rozwiązanie_sudoku(s1)
sudoku_print(su1)

su2 = rozwiązanie_sudoku(s2)
sudoku_print(su2)


