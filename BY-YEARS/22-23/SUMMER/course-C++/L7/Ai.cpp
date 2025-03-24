#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h> //for rand
#include "Ai.hpp"
#include "Game.hpp"
#include "time.h"

using namespace std;

void  Ai::Ai::AI_turn(char grid[3][3]) {
        srand(time(0));
        while (true) {
            int AI_move = (rand() % 9);

            int row = AI_move / 3;
            int col = AI_move % 3;

            char grid_char_check = grid[row][col];

            if (grid_char_check != 'X' && grid_char_check != 'O') {
                cout << "Skynet made its move" << endl;
                grid[row][col] = 'O';
                break;
            }
            else {
				continue;
			}


        }
    }