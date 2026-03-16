#!/usr/bin/env python
# podbieram kod z sprawdzaczki

import argparse
import copy
import math
import numpy as np
import os
#import Queue as queue
import queue
import random
import signal
import subprocess
import threading
import time
import chess
import sys


class WrongMove(Exception):
    pass

class Reversi:
    M = 8
    DIRS = [(0, 1), (1, 0), (-1, 0), (0, -1),
            (1, 1), (-1, -1), (1, -1), (-1, 1)]

    def __init__(self):
        self.board = self.initial_board()
        self.fields = set()
        self.move_list = []
        self.history = []
        for i in range(self.M):
            for j in range(self.M):
                if self.board[i][j] is None:
                    self.fields.add((j, i))

    def initial_board(self):
        B = [[None] * self.M for _ in range(self.M)]
        B[3][3] = 1
        B[4][4] = 1
        B[3][4] = 0
        B[4][3] = 0
        return B

    def draw(self):
        for i in range(self.M):
            res = []
            for j in range(self.M):
                b = self.board[i][j]
                if b is None:
                    res.append('.')
                elif b == 1:
                    res.append('#')
                else:
                    res.append('o')
            print(''.join(res))
        print('')

    def moves(self, player):
        res = []
        for (x, y) in self.fields:
            if any(self.can_beat(x, y, direction, player)
                   for direction in self.DIRS):
                res.append((x, y))
        return res

    def can_beat(self, x, y, d, player):
        dx, dy = d
        x += dx
        y += dy
        cnt = 0
        while self.get(x, y) == 1 - player:
            x += dx
            y += dy
            cnt += 1
        return cnt > 0 and self.get(x, y) == player

    def get(self, x, y): # dziala troche jak inBounds()
        if 0 <= x < self.M and 0 <= y < self.M:
            return self.board[y][x]
        return None

    def do_move(self, move, player):
        self.history.append([x[:] for x in self.board])
        self.move_list.append(move)

        if move is None:
            return
        x, y = move
        x0, y0 = move
        self.board[y][x] = player
        self.fields -= set([move])
        for dx, dy in self.DIRS:
            x, y = x0, y0
            to_beat = []
            x += dx
            y += dy
            while self.get(x, y) == 1 - player:
                to_beat.append((x, y))
                x += dx
                y += dy
            if self.get(x, y) == player:
                for (nx, ny) in to_beat:
                    self.board[ny][nx] = player

    def result(self):
        res = 0
        for y in range(self.M):
            for x in range(self.M):
                b = self.board[y][x]
                if b == 0:
                    res -= 1
                elif b == 1:
                    res += 1
        return res

    def terminal(self):
        if not self.fields:
            return True
        if len(self.move_list) < 2:
            return False
        return self.move_list[-1] == self.move_list[-2] == None

    def random_move(self, player):
        ms = self.moves(player)
        if ms:
            return random.choice(ms)
        return None

# tyle z podbierania kodu

# co teraz potrzebujemy?
# na potrzeby tego zadania zajmiemy się implementacją agenta do reversi
# opartego na MCTS czyli algorytmu opartego na 4 fazach
# Selection
'''
Idac od korzenia przechodzimy w dol wybierajac w kazdym wezle najbardziej
obiecujace dziecko zgodnie z "UCT". Bedziemy schodzic w dol az nie napotkamy
takiego wezla ktory nie jest w pelni rozwiniety badz nie byl do tad odwiedzany
'''
# Expansion
'''
Gdy poprzednia faza zatrzyma sie w jakims wezle i nie jest to stan definitywnie
konczacy gre dodajemy do niego nowy potomny wezel ktory odpowiada jednemu z mozliwych
ruchow (dotad niedokonanego). Pozwala to na rozrastanie drzewa i pokrywanie coraz to 
wiekszej czesci przestrzeni stanow gry
'''
# Simulation
'''
Od wezla wybranego w poprzedniej fazie przeprowadzamy symulacje losowego rozegrania 
gry az do jej zakonczenia. Wynik takiej symulacji pozwoli nam na przyblizona ocene jakosci 
stanu.
'''
# Backpropagation
'''
Uzyskany wynik "przepychamy" przez kolejen wezly na sciezce od symulowanego wezla
do kozenia, aktualizujac statystyki kazdego z nich. Zaktualizwane dane posluza 
pozniejszym iteracjom do wyboru najabardziej obicujacych sciezek.
'''
BLACK = 1
WHITE = 0

class MctsNode:
    def __init__(self, state, parent, parentMove, player):
        self.state = state # aktualny stan planszy
        self.parent = parent
        self.parentMove = parentMove
        self.children = []
        self.visits = 0 # n_i / N z wzorku
        self.wins = 0.0 # w_i z wzorku
        self.player = player # kto gra
        self.opponent = 1 - player
        # dla przyszlego mnie dlaczego tu mamy ruchy dla przeciwnika
        # zasadniczo jak spojrzymy no to jakis gracz zagral ruch
        # oznacza to, ze nastepny ruch bedzie przeciwnika
        # zatem bedziemy sprawdzac te ruchy a nie nasze no bo nie nasza
        # tura ;>
        moves = state.moves(self.opponent)
        if moves:
            self.untriedMoves = moves
        else:
            self.untriedMoves = [None]

    # dobra to jak mamy konstruktor za nami to pora na wziecie sie za
    # etapy MCTS'a
    def selection(self):
        # to na wszelki zeby nie bylo jakis problemow potem i tego nigdy
        # nie znajde
        if not self.children:
            return self
        # to tak na wszelki wypadek zeby wzorek nie wylecial w kosmos
        for child in self.children:
            if child.visits == 0:
                return child
        bestChild = None
        bestUct = float("-inf")
        # no dobra teraz dla kazdego dostepnego potomka aktualnego
        # wezla liczymy uct no i wybieramy tego z najwieksza wartoscia
        c = math.sqrt(2)
        for child in self.children:
            uct = (child.wins / child.visits) + c * math.sqrt(math.log(self.visits) / child.visits)
            if uct > bestUct:
                bestUct = uct
                bestChild = child
        return bestChild
    
    # dobra no to pora na faze druga expansion
    def expansion(self): # rollout
        move = self.untriedMoves.pop()
        childState = copy.deepcopy(self.state)
        childState.do_move(move, self.opponent)
        # skoro mamy juz stan potomka do podpinamy go do calego drzewa
        child = MctsNode(childState, self, move, self.opponent)
        self.children.append(child)
        return child
    # do mcts sie przyda 
    def isNodeFullyExpanded(self):
        return len(self.untriedMoves) == 0
    
    # pora na kolejna faze simulation (rollout)
    # tutaj bedziemy zwracac info jak koncowo potoczyla sie rozgrywka
    # z aktualnego stanu
    def simulation(self):
        simulationState = copy.deepcopy(self.state)
        currentTurn = self.opponent
        while not simulationState.terminal():
            move = simulationState.random_move(currentTurn)
            simulationState.do_move(move, currentTurn)
            currentTurn = 1 - currentTurn
        # no dobra skoro rozegralismy juz losowa gre z tego stanu
        # zwracamy info kto wygral z tego stanu
        result = simulationState.result()
        # no tak dziala kod sprawdzaczkowy ale juz go nie zmieniam
        if result > 0:
            winner = BLACK
        elif result < 0:
            winner = WHITE
        else: # remis
            return 0.5
        # tu logiczne jak wygrywa to super jak nie no to nie
        if winner == self.player:
            return 1.0
        else:
            return 0.0

    # no to pora na ostatnia faze backpropagation czyli chcemy poinformowac
    # wszystkich przodkow o naszym wyniku z gry
    def backpropagation(self, result):
        node = self
        while node is not None:
            # wytlumaczenie dla mnie o 8 rano
            # zasadniczo chcemy odpowiednio przyznawac wygrane etc odpowiednim graczom
            # zatem jezeli czarny wygral no to "w swojej turze" bedzie dostawal punk
            # ale jak jest "tura bialego" no to dostanie bialy 1 - 1 czyli zero
            node.visits += 1
            if node.player == self.player:
                node.wins += result
            else:
                node.wins += (1.0 - result)
            node = node.parent

# ok no to zalozmy ze drzewo juz jest
# teraz bedziemy chcieli wybrac odpowiedniego potomka roota
# jak ?
# zatem interesuje nas taki potomek ktory byl najczesciej odwiedzany?
# dlaczego robimy tak a nie sugerujemy sie uct ?
# no jest to potomek ktory byl najczesciej wybierany wiec jego wartosci
# jestesmy najbardziej pewni => najbardziej bedzie eliminowal niepewnosc
# i z wykonanych "selekcji" zostal wybrany najczesciej zatem jest tym pewniejszym
# kandydatem

def bestChildNode(root):
    bestChild = None
    bestChildVisits = -1
    for child in root.children:
        if child.visits > bestChildVisits:
            bestChild = child
            bestChildVisits = child.visits
    return bestChild

# no i przyszla pora na samego mcts czyli polaczenie
# wszystkich 4 faz

# def mcts(rootState, rootPlayer, iterations):
def mcts(rootState, rootPlayer, timeTurnLimit):
    # znowu przypomnienie player = 1 - rootPlayer bo oznacza to ze teraz 
    # ruch bedzie mial przeciwnik
    rootNode = MctsNode(rootState, None, None, 1 - rootPlayer)


    start = time.time()
    # while iterations > 0
    while time.time() - start < timeTurnLimit:
        # iterations -= 1
        currentNode = rootNode
        while currentNode.isNodeFullyExpanded() and not currentNode.state.terminal():
            currentNode = currentNode.selection() # faza 1 selection
        if not currentNode.state.terminal():
            currentNode = currentNode.expansion() # faza 2 expansion
        # teraz tak albo wybralismy czaderski wezel albo wlasnie stworzylismy nowy
        result = currentNode.simulation() # faza 3 simlation (rollout)
        currentNode.backpropagation(result) # faza 4 backpropagation
    # mcts sie pokrecil pora wybrac najlepszy ruch
    bestChild = bestChildNode(rootNode)
    return bestChild.parentMove


def main():
    board = Reversi()
    firstMove = True
    agentColor = None
    print("RDY", flush=True)

    while True:
        line = sys.stdin.readline()
        if not line:
            break
        lineContent = line.strip().split()
        if not lineContent:
            continue
        msg = lineContent[0]

        if msg == "UGO":
            msg, moveTime, gameTime = lineContent
            moveTime = float(moveTime)
            gameTime = float(gameTime)
            if(firstMove):
                agentColor = WHITE
                firstMove = False
            # bestMove = mcts(board, agentColor, 250)
            bestMove = mcts(board, agentColor, 0.5)
            if bestMove is None:
                bestRow = -1
                bestCol = -1
            else:
                bestRow, bestCol = bestMove
            print(f"IDO {bestRow} {bestCol}", flush=True) # potencjalnie sie zesra
            board.do_move((bestRow, bestCol), agentColor)
        elif msg == "HEDID":
            msg, moveTimeString, gameTimeString, opponentRowString, opponentColString = lineContent
            moveTime = float(moveTimeString)
            gameTime = float(gameTimeString)
            opponentRow = int(opponentRowString)
            opponentCol = int(opponentColString)
            if(firstMove):
                agentColor = BLACK
                firstMove = False
            if opponentRow >= 0 and opponentCol >= 0:
                board.do_move((opponentRow, opponentCol), 1 - agentColor)
            else:
                board.do_move(None, 1 - agentColor)
            
            # bestMove = mcts(board, agentColor, 250)
            bestMove = mcts(board, agentColor, 0.5)
            if bestMove is None:
                bestRow = -1
                bestCol = -1
            else:
                bestRow, bestCol = bestMove
            print(f"IDO {bestRow} {bestCol}", flush=True) # potencjalnie sie wysra
            board.do_move((bestRow, bestCol), agentColor)
        elif msg == "ONEMORE":
            board = Reversi()
            firstMove = True
            print("RDY", flush=True)
        elif msg == "BYE":
            break

main()