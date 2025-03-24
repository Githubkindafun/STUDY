#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h> //for rand
#include "Game.hpp"
#include "Ai.hpp"
#include "Display.hpp"
#include "Turn.hpp"


using namespace std;


const string convert[9] = { "A1", "A2", "A3", "B1", "B2", "B3", "C1", "C2", "C3" };


 
     void g::Game::Generate() { // generator naszego pola zabawy
        
        char filling = '1';
        for (int x = 0; x < 3; x++) { // our x 

            for (int y = 0; y < 3; y++) { // our y 
                grid[x][y] = filling;
                filling++;
            }
        }
    }

   

    
    void g::Game::Win() {
        // possible setups for a win
        const char* winners[8] = { "123", "456", "789", "147", "258", "369", "159", "357" };
	
		
        for (int i = 0; i < 8; i++) {
            bool did_win = true;
            char prev_grid = '0';
            const char* winner = winners[i];

            for (int j = 0; j < 3; j++) {
                char tmp = winner[j];
                int spot = (tmp - '0') - 1;

                int col = spot % 3; 
                int row = spot / 3;

                char grid_char_check = grid[row][col];

                if (prev_grid == '0') {
                    prev_grid = grid_char_check;
                }
                else if (prev_grid == grid_char_check) continue;
                else {
                    did_win = !did_win;
                    break; //konczymy petle
                }
            }
		//	
		int x = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if(grid[i][j] != 'X' || grid[i][j] != 'O') {
					x++;
				}
				
					//cout << "#### Wohohoh we have a draw ####";
            		//exit(0); //tu wychodzimy z pętli
					//break;
			}
		}
		if(x == 0) {
			cout << "#### Wohohoh we have a draw ####";
        	exit(0); //tu wychodzimy z pętli
		}
		//	
        if (did_win) {
            printf("#### Wohohoh the winner is %c ####", prev_grid);
            exit(0); //tu mi z pętli nie chicało wyjsc to podpatrzyłem na necie
			break;
            }
        }
    }



    g::Game::Game() { //konstruktor
        Generate();
        cout << "Here you ll play some tic tac to, one thing i'd like to ask you is to play \n correctly just simply type collumn you like (choose from A B C) \n and then press enter \n after that just chooose from 1 to 3 and press enter again thanks." << endl;
        
        D::Dis::Display(grid);
        Win();
        while (true) {
            T::Tu::Turn(grid);
            D::Dis::Display(grid);
            Win();
            //Skynet here 
            Ai::Ai::AI_turn(grid);
            D::Dis::Display(grid);
            Win();
        }
    }

    



