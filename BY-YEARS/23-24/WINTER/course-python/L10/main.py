# lista 10
# zadanie "gra w zycie"
# zasady:
# [1] Any live cell with fewer than two live neighbours "dies", as if by underpopulation 
# [2] Any live cell with two or three live neighbours "lives" on to the next generation  
# [3] Any live cell with more than three live neighbours "dies", as if by overpopulation 
# [4] Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction 
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as anim

def check_neighbor(grid, i, j): # zwraca ilosc zywych sasiadow
    N = len(grid)
    return sum([ 
        grid[(i+1)%N, (j-1)%N], grid[(i+1)%N, j], grid[(i+1)%N, (j+1)%N], # tutaj % pozwala nam zawinac plasczyzne na ktorej dzialamy
        grid[i, (j-1)%N], grid[i, (j+1)%N],
        grid[(i-1)%N, (j-1)%N], grid[(i-1)%N, j], grid[(i-1)%N, (j+1)%N]
        ])

# N - rozmiar planszy
# A - procent zywych komorek 
# D - procent martwych komorek
def game_of_life(N, alive):
    dead = 1.0 - alive
    #
    og_gen = np.random.choice([1, 0], size=N*N, p=[alive, dead]).reshape(N, N)  
    # og_grid to nasza startowa plansza ktora tworzymy losowo ukladajac
    # w tablicy N na N (to nam daje reshape), 1 i 0 reprezentujace zywe i martwe komorki 

    def new_generation(data):
        nonlocal og_gen
        #
        new_gen = og_gen.copy()
        #
        for i in range(N): # w tym fragmencie sprawdzamy warunki game of life
            for j in range(N):
                state = check_neighbor(og_gen, i, j) # tutaj sprawdzamy ilosc zyjacych sasiadow komorki [i, j]
                if og_gen[i, j] == 1: # dla zyjacych komorek
                    if state < 2 or state > 3: new_gen[i, j] = 0
                    # gdy 2 , 3 to zyje dalej
                else: # dla martwych komorek
                    if state == 3: new_gen[i, j] = 1
        life.set_data(new_gen) # zycie updatetujemy o nowy stan (nowa generacje)
        og_gen = new_gen
        return life
    
    figure, axis = plt.subplots() 
    # figure ktora reprezentuje cale okienko (taki pojemnik na elementy naszego wykresu)
    # axis - oś                          
    life = axis.matshow(og_gen) # inicjalizuje wizualizacje naszego grida wyswietlajac jego poczatkowy stan
    ani = anim.FuncAnimation(figure, new_generation, interval=42) 
    # FuncAnimation ustawia animacje poprzez callowanie new_generation aby aktualizowac wykres 
    plt.show()

game_of_life(100, 0.42)
                

