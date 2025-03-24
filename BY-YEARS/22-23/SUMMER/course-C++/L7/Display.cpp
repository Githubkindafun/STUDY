#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h> //for rand
#include "Display.hpp"

using namespace std;

void  D::Dis::Display(char grid[3][3]) { // dosłownie display
        
        int row = 1;

        printf("     A   B   C  ");
        printf("\n   ~~~~~~~~~~~~~\n");
        for (int x = 0; x < 3; x++) { // our x 

            printf(" %c |", (row + '0'));

            for (int y = 0; y < 3; y++) { // our y 
                printf(" %c |", grid[x][y]);
            }
            row++;
            printf("\n   ~~~~~~~~~~~~~\n");
        }
    }