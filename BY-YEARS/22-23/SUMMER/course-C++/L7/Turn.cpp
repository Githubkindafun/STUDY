#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h> //for rand
#include "Turn.hpp"



using namespace std;


void  T::Tu::Turn(char grid[3][3]) { // co chcemy zrobic
        string input_cols;
        string input_rows;
        
        while (true) {
            cout << "Which box would you like to mark?\n Please first write an letter A or B or C that symbolise an column then provide an nuber from 1 to 3 that will symbolize an row. \n"; // to do
            getline(cin, input_cols);
            getline(cin, input_rows);
            
            int rows_tmp = stoi(input_rows);
            
            transform(input_cols.begin(), input_cols.end(), input_cols.begin(), ::toupper);

            if (input_rows == "" || input_cols == "") {
                cout << "You must choose something duh!\n"; // to do
                
            }else if (rows_tmp != 1 && rows_tmp != 2 && rows_tmp != 3){
                cout << "Numbers you provide must be 1, 2 or 3.";
            }
            else if (input_cols != "A" && input_cols != "B" && input_cols != "C" ) {
                cout << "As rows you must provide letterst A, B or C";
            }
            else { 
                int given_row = stoi(input_rows);
                int given_col;

                if (input_cols == "A") {
                    given_col = 1;
                }
                else if (input_cols == "B") {
                    given_col = 2;
                }
                else if (input_cols == "C") {
                    given_col = 3;
                }
                else {
                    throw ("Error");
                }
                //cout << given_col << " " << given_row << endl;

                char grid_pos = grid[given_row - 1][given_col - 1];
                
                if (grid_pos == 'X' || grid_pos == 'O') {
                    cout << "THere is something already!" << endl;
                }
                else {
                    grid[given_row - 1][given_col - 1] = 'X';
                    break;
                }
                
            }
        }
    }