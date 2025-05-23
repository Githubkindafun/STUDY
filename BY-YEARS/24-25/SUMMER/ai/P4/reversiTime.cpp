/*
HALL OF FAILS:
for(int i = moves.size(); i < moves.size(); i--)

moja niepomierna glupota i upartosc 
*/
#include <bits/stdc++.h>

using namespace std;

const int BOARD_SIZE = 8;
const int WHITE = -1;
const int EMPTY = 0;
const int BLACK = 1;

const array<pair<int, int>, 8> DIRECTIONS = {{
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1}, {0, 1},
    {1, -1}, {1, 0}, {1, 1}
}};

const array<pair<int, int>, 4> AXES = {{
    {1, 0}, {0, 1}, {1, 1}, {1, -1}
}};

int agentColor;

//
//
struct Board {
    int grid[BOARD_SIZE][BOARD_SIZE];
    int currentPlayer;

    Board(int startingPlayer = BLACK) {
        reset(startingPlayer);
    }

    void reset(int startingPlayer) {
        for(int i = 0; i < BOARD_SIZE; i++) {
            for(int j = 0; j < BOARD_SIZE; j++) {
                grid[i][j] = EMPTY;
            }
        }
        grid[3][3] = BLACK;
        grid[3][4] = WHITE;
        grid[4][3] = WHITE;
        grid[4][4] = BLACK;
        currentPlayer = startingPlayer;
    }

    bool inBounds(int row, int col) {
        return (row >= 0 && row < BOARD_SIZE && 
                col >= 0 && col < BOARD_SIZE);
    }

    bool isLegal(int row, int col) {
        // zasadniczo jezeli ruch mial by byc poza plansza lub cos lezy na danym polu
        // no to tam nic nie postawimy
        if(!inBounds(row, col) || grid[row][col] != EMPTY) {
            return false;
        }

        for(int i = 0; i < 8; i++) {
            int rowDirection = DIRECTIONS[i].first;
            int colDirection = DIRECTIONS[i].second;
            int newRow = row + rowDirection;
            int newCol = col + colDirection;
            bool opponentMet = false;

            // zasadniczo jezeli jezeli znajdziemy jakis dysk przeciwnika, to super
            // nastepnie idziemy w tym kierunku tak daleko jak nam sie uda
            while(inBounds(newRow, newCol) && grid[newRow][newCol] == -currentPlayer) {
                opponentMet = true;
                newRow += rowDirection;
                newCol += colDirection;
            }
            // teraz jezeli okaze sie ze szlismy tak daleko ze konczy sie to na naszym dysku
            // to alleluja bo oznacza to ruch ktory pozwoli "przekrecic" dysk/dyski przeciwnika
            // czyli bedzie to legalny ruch
            if(opponentMet && inBounds(newRow, newCol) && grid[newRow][newCol] == currentPlayer) {
                return true;
            }
        }
        return false; // ;c
    }

    vector<pair<int, int>> legalMoves() {
        vector<pair<int, int>> moves;
        for(int row = 0; row < BOARD_SIZE; row++) {
            for(int col = 0; col < BOARD_SIZE; col++) {
                if(isLegal(row, col)) {
                    moves.push_back(make_pair(row, col));
                }
            }
        }
        return moves;
    }

    Board applyMove(int row, int col) {
        // drobny komentarz zebym sam nie zapomnial dla czego w tym miejscu robie kopie planszy zamiast
        // robic to na aktualnej planszy.
        // zasadniczo jak min-max bedzie korzystal z tego potem to zeby mial swoja niezalezna kopie bo inaczej
        // musial bym jakies cofanie ruchow robic czy cos
        Board newBoard = *this;
        newBoard.grid[row][col] = currentPlayer;

        for(int i = 0; i < 8; i++) {
            int rowDirection = DIRECTIONS[i].first;
            int colDirection = DIRECTIONS[i].second;
            int newRow = row + rowDirection;
            int newCol = col + colDirection;
            int discs = 0;

            while(newBoard.inBounds(newRow, newCol) && newBoard.grid[newRow][newCol] == -currentPlayer) {
                discs++;
                newRow += rowDirection;
                newCol += colDirection;
            }

            if(discs > 0 && newBoard.inBounds(newRow, newCol) && newBoard.grid[newRow][newCol] == currentPlayer) {
                for(int k = 1; k <= discs; k++) {
                    newBoard.grid[row + k*rowDirection][col + k*colDirection] = currentPlayer;
                }
            }

        }
        newBoard.currentPlayer = -currentPlayer;
        return newBoard;
    }

    int score() {
        int discsScore = 0;
        for(int row = 0; row < BOARD_SIZE; row++) {
            for(int col = 0; col < BOARD_SIZE; col++) {
                discsScore += grid[row][col];
            } // za kazdy czarny +1 a za kazdy bialy -1 c:
        }
        return discsScore;
    }
    
    bool gameEnd() {
        if(!legalMoves().empty()) {
            return false;
        }

        Board opponent = *this;
        opponent.currentPlayer = -opponent.currentPlayer;
        return opponent.legalMoves().empty();
    }

};

// narazie jakas totalnie bazowa funckja ale spokojnie bedzie upgrade 
// int boardEvaluation(Board& board) {
//     return board.score();
// }

// pora na kolejne podrasowanie aby poprawic winratio
int countStableDiscs(Board& board, int color) {
    int stableDiscCount = 0;

    for(int row = 0; row < BOARD_SIZE; row++) {
        for(int col = 0; col < BOARD_SIZE; col++) {
            if(board.grid[row][col] != color) {
                continue;
            }

            bool isStable = true;

            for(int axis = 0; axis < 4; axis++) {
                int rowDirection = AXES[axis].first;
                int colDirection = AXES[axis].second;

                bool plusOK = true; // sprawdzamy plusowe
                int rowNext = row + rowDirection;
                int colNext = col + colDirection;
                while(board.inBounds(rowNext, colNext)) {
                    if(board.grid[rowNext][colNext] != color) {
                        plusOK = false;
                        break;
                    }
                    rowNext += rowDirection;
                    colNext += colDirection;
                }

                bool minusOK = true; // tu minusowe (czyli idziemy w prawo albo lewo)
                rowNext = row - rowDirection;
                colNext = col - colDirection;
                while(board.inBounds(rowNext, colNext)) {
                    if(board.grid[rowNext][colNext] != color) {
                        minusOK = false;
                        break;
                    }
                    rowNext -= rowDirection;
                    colNext -= colDirection;
                }

                if(!plusOK || !minusOK) {
                    isStable = false;
                    break;
                }
            }
            if(isStable) {
                stableDiscCount++;
            }
        }
    }
    return stableDiscCount;
}


// pora na upgrade, wezmy cos w stylu tego co mialem na cwiczeniach
int boardEvaluation(Board& board) {
    int discDiffrence = board.score(); 

    const pair<int, int> corners[4] = { {0, 0}, {0, 7}, {7, 0}, {7, 7}};
    int cornersTaken = 0;
    for(int i = 0; i < 4; i++) {
        int row = corners[i].first;
        int col = corners[i].second;
        if(board.grid[row][col] == BLACK) {
            cornersTaken++;
        } else if(board.grid[row][col] == WHITE) {
            cornersTaken--;
        }
    }

    int agentStableDiscs = countStableDiscs(board, agentColor);
    int opponentStableDiscs = countStableDiscs(board,-1 * agentColor);
    int diffStableDiscs = agentStableDiscs - opponentStableDiscs;
    int finalEvaluation = 20 * discDiffrence + 420 * cornersTaken + 100 * diffStableDiscs;
    return agentColor * finalEvaluation;
}

// tutaj mamy utility funkcje potrzebne do posortowania ruchow na podstawie
// zewaluowanej "listy" ruchow
bool comparison(pair<pair<int, int>, int>& p1, pair<pair<int, int>, int>& p2) {
    return p1.second > p2.second;
}


// troche crazy ale najpierw napisalem jedna wersje kodu
// i potem sie okazalo ze ten dodatkowy punkt wymaga tego sortowania 
// na podstawie tego jak sa zewaluowane ruchy
vector<pair<pair<int, int>, int>> evaluatedMovesInOrder(Board& board) {
    vector<pair<int, int>> moves = board.legalMoves();
    vector<pair<pair<int, int>, int>> evaluatedMoves;
    // //
    // // zylowanie wydajnosci na potrzebe zyskania 0.5 sekundy
    // evaluatedMoves.reserve(moves.size());
    // //
    // //
    for(int i = 0; i < moves.size(); i++) {
        pair<int, int> move = moves[i];
        Board afterMove = board.applyMove(move.first, move.second);
        int evaluation = boardEvaluation(afterMove);
        // //
        // // zylowanie wydajnosci
        // evaluatedMoves.emplace_back(make_pair(move, evaluation));
        // //
        // //
        evaluatedMoves.push_back(make_pair(move, evaluation));
    }
    sort(evaluatedMoves.begin(), evaluatedMoves.end(), comparison);
    return evaluatedMoves;
}

// tu mamy nasze min-max-alpha-beta pruning 
// jest taki swietny material na yt od goscia Sebastian Lague
// duzo cool rzeczy robi 
int minMax(Board& board, int depth, int alpha, int beta) {
    if(depth == 0 || board.gameEnd()) {
        return boardEvaluation(board);
    }
    // vector<pair<int, int>> moves = board.legalMoves();

    // w zadaniu zasugerowane jest aby posortowac ruchy na podstawie tego
    // jak wypadaja w ewaluacji
    
    vector<pair<pair<int, int>, int>> moves = evaluatedMovesInOrder(board);


    if(moves.empty()) {
        Board turnPassed = board;
        turnPassed.currentPlayer = -turnPassed.currentPlayer;
        return minMax(turnPassed, depth - 1, alpha, beta);
    }
    // 
    if(board.currentPlayer == agentColor) {
        int maxEval = INT_MIN;
        for(int i = 0; i < moves.size(); i++) {
            int row = moves[i].first.first;
            int col = moves[i].first.second;
            Board child = board.applyMove(row, col);
            int eval = minMax(child, depth - 1, alpha, beta); // zeby nie zapomniec zmieniamy gracza w applyMove
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if(beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for(int i = moves.size() - 1; i >= 0; i--) {
            int row = moves[i].first.first;
            int col = moves[i].first.second;
            Board child = board.applyMove(row, col);
            int eval = minMax(child, depth - 1, alpha, beta);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if(beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

pair<int, int> selectBestMove(Board& board, int maxDepth) {
    
    vector<pair<pair<int, int>, int>> moves = evaluatedMovesInOrder(board);
    pair<int, int> bestMove = {-1, -1};
    bool maximizing = (board.currentPlayer == agentColor);
    int alpha = INT_MIN;
    int beta = INT_MAX;
    int bestEval;
    if(maximizing) {
        bestEval = INT_MIN;    
    } else {
        bestEval = INT_MAX;
    }

    for(int i = 0; i < moves.size(); i++) {
        int row = moves[i].first.first;
        int col = moves[i].first.second;
        Board child = board.applyMove(row, col);
        int eval = minMax(child, maxDepth - 1, alpha, beta);
        
        // if(eval > bestEval) {
        //     bestEval = eval;
        //     bestMove = moves[i].first;
        // }
        // alpha = max(alpha, bestEval);

        if((maximizing && eval > bestEval) || (!maximizing && eval < bestEval)) {
            bestEval = eval;
            bestMove = moves[i].first;
        }
        if(maximizing) {
            alpha = max(alpha, bestEval);
        } else {
            beta = min(beta, bestEval);
        }
        if(beta <= alpha) {
            break;
        }

    }
    return bestMove;
}

// przy sprawdzaniu za dodatkowy punk
// to co chcemy zrobic to zamiast wybierac ruch na podstawie min maxa
// to chcemy podac min maxowi losowy ruch
// co oznacza ze musze je posortowac jakos po ewaluowaniu

int main() {
    // szybkie linijki
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Board board;
    bool firstMove = true;

    cout << "RDY\n" << flush;
    string msg;
    while(cin >> msg) {
        if(msg == "UGO") {
            double moveTime;
            double gameTime;
            cin >> moveTime >> gameTime;

            if(firstMove) {
                agentColor = WHITE;
                board.reset(agentColor);
                firstMove = false;
            }
            // liczymy
            pair<int, int> bestMove = selectBestMove(board, 5);
            int bestRow = bestMove.first;
            int bestCol = bestMove.second;
            cout << "IDO " << bestCol << " " << bestRow << "\n" << flush;

            if(bestRow >= 0) { // wpp to nie znalezlismy ruchu = oddajemy ture
                board = board.applyMove(bestRow, bestCol);
            } else {
                board.currentPlayer = -board.currentPlayer;
            }

        } else if(msg == "HEDID") {
            double moveTime;
            double gameTime;
            // cin >> moveTime >> gameTime;
            int opponentRow;
            int opponentCol;
            cin >> moveTime >> gameTime >> opponentCol >> opponentRow;

            // he did czyli nie jestesmy pierwsi
            if(firstMove) {
                agentColor = BLACK;
                board.reset(agentColor);
                firstMove = false;
            }

            if(opponentRow >= 0) { // oddaje nam ture ziomal
                board.currentPlayer = -agentColor;
                board = board.applyMove(opponentRow, opponentCol);
            } else {
                board.currentPlayer = -board.currentPlayer;
            }

            pair<int, int> bestMove = selectBestMove(board, 5);
            int bestRow = bestMove.first;
            int bestCol = bestMove.second;
            cout << "IDO " << bestCol << " " << bestRow << "\n" << flush;

            if(bestRow >= 0) { // wpp to nie znalezlismy ruchu = oddajemy ture
                board = board.applyMove(bestRow, bestCol);
            } else {
                board.currentPlayer = -board.currentPlayer;
            }
        } else if(msg == "ONEMORE") {
            // resecick
            board.reset(agentColor);
            firstMove = true;
            cout << "RDY\n" << flush;
        } else if(msg == "BYE") {
            break;
        }
    }
    return 0;
}