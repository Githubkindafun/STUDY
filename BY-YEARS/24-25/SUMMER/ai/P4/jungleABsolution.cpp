#include <bits/stdc++.h>

using namespace std;

const int WIDTH = 7;
const int HEIGHT = 9;
const int MAX_NO_KILL = 50; // zasadniczo ile moze byc ruchow bez bicia
const int LOWER = 0; // gracze
const int UPPER = 1;
const int RAT = 0;
const int CAT = 1;
const int DOG = 2;
const int WOLF = 3;
const int JAGUAR = 4; // niby pantera ale literka jest j
const int TIGER = 5;
const int LION = 6;
const int ELEPHANT = 7;
const int EMPTY = -1;

int agentSize;

// to z jakiejs pracy naukowej sb wziolem
const int MATERIAL[8] = {
    500, 200, 300, 400,
    500, 800, 900, 1000
};

const int TRAP_REWARD = 500;
const int DEN_PENALTY = 10;

const array<pair<int, int>, 6> TRAPS_LOCATIONS = {{
    {2, 8}, {3, 7}, {4, 8}, {2, 0}, {3, 1}, {4, 0}
}};

const array<pair<int, int>, 4> DIRECTIONS = {{
    {-1, 0},{0, -1}, {0, 1}, {1, 0}
}};

// helper funkcje

bool isPieceTrapped(int x, int y) {
    for(int i = 0; i < 6; i++) {
        int trapX = TRAPS_LOCATIONS[i].first;
        int trapY = TRAPS_LOCATIONS[i].second;
        if(x == trapX && y == trapY) {
            return true;
        }
    }
    return false;
}

int manhattanDist(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int charToPiece(char c) {
    c = tolower(c);
    switch(c) {
        case 'r':
            return RAT;
        case 'c':
            return CAT;
        case 'd':
            return DOG;
        case 'w':
            return WOLF;
        case 'j':
            return JAGUAR;
        case 't':
            return TIGER;
        case 'l':
            return LION;
        case 'e':
            return ELEPHANT;
        default: 
            return EMPTY;    
    }
}


// przyda sie bo dla 4 klockow to juz by bylo sporo babrania
struct Move {
    int sx;
    int sy;
    int dx;
    int dy;
    Move(int Sx = -1, int Sy = -1, int Dx = -1, int Dy = -1) {
        sx = Sx;
        sy = Sy;
        dx = Dx;
        dy = Dy;
    }
};

const char* STARTING_BOARD[HEIGHT] = {
    "L.....T",
    ".D...C.",
    "R.J.W.E",
    ".......",
    ".......",
    ".......",
    "e.w.j.r",
    ".c...d.",
    "t.....l"
};

struct Board {
    char state[HEIGHT][WIDTH];
    int currentPlayer;
    int noKillCount;

    Board(int startingPlayer) {
        reset(startingPlayer);
    }

    void reset(int startingPlayer) {
        for(int row = 0; row < HEIGHT; row++) {
            for(int col = 0; col < WIDTH; col++) {
                state[row][col] = STARTING_BOARD[row][col];
            }
        }
        currentPlayer = startingPlayer;
        noKillCount = 0;
    }

    bool inBounds(int x, int y) {
        if((x >= 0 && x < WIDTH) && (y >= 0 && y < HEIGHT)) {
            return true;
        }
        return false;
    }

    bool isWater(int x, int y) {
        if(((x >= 1 && x <= 2) || (x >= 4 && x <= 5 )) && y >= 3 && y <= 5) {
            return true;
        }
        return false;
    }

    bool isInOwnDen(int x, int y, int player) {
        if(player == LOWER) {
            return x == 3 && y == 8;
        } else {
            return x == 3 && y == 0;
        }
    }

    bool isLowerPiece(char pieceC) {
        return pieceC >= 'a' && pieceC <= 'z';
    }
    bool isUpperPiece(char pieceC) {
        return pieceC >= 'A' && pieceC <= 'Z';
    }

    bool canPieceJump(int sx, int sy, int dx, int dy) {
        char pieceC = state[sy][sx];
        int type = charToPiece(pieceC);
        if(type != TIGER && type != LION) {
            return false;
        }
        // dla mnie w przyszlosci to sprawdzamy tu czy skacze wzdloz "jednej osi"
        if(sx != dx && sy != dy) {
            return false;
        }
        int stepX; // bedzie dzialalo jak wektor kierunku
        if(dx > sx) {
            stepX = 1;
        } else if(dx < sx) {
            stepX = -1;
        } else {
            stepX = 0;
        }
        int stepY; // bedzie dzialalo jak wektor kierunku
        if(dy > sy) {
            stepY = 1;
        } else if(dy < sy) {
            stepY = -1;
        } else {
            stepY = 0;
        }
        int currentX = sx + stepX;
        int currentY = sy + stepY;
        // if(!inBounds(currentX, currentY) || !isWater(currentX, currentY)) {
        //     return false;
        // }
        bool seenWater = false;
        while(inBounds(currentX, currentY) && isWater(currentX, currentY)) {
            seenWater = true;
            if(state[currentY][currentX] == 'r' || state[currentY][currentX] == 'R') {
                return false;
            }
            currentX += stepX;
            currentY += stepY;
        }
        if(seenWater && currentX == dx && currentY == dy) {
            return true;
        }
        return false;
    }

    bool isLegal(int sx, int sy, int dx, int dy) {
        if(!inBounds(sx, sy) || !inBounds(dx, dy)) {
            return false;
        }
        char pieceC = state[sy][sx];
        if(pieceC == '.') {
            return false;
        }
        bool isMine;
        if(currentPlayer == LOWER) {
            isMine = isLowerPiece(pieceC);
        } else {
            isMine = isUpperPiece(pieceC);
        }
        if(!isMine) {
            return false;
        }
        if(isInOwnDen(dx, dy, currentPlayer)) {
            return false;
        }
        int type = charToPiece(pieceC);
        int power = type + 1;
        if(isPieceTrapped(sx, sy)) {
            power = 0;
        }
        bool fromTrap = isPieceTrapped(sx, sy);
        int moveDistX = abs(dx - sx);
        int moveDistY = abs(dy - sy);
        if(moveDistX + moveDistY == 1) {
            if(isWater(dx, dy) && power != 1) {
                // tylko sczury moga
                return false;
            } 
        } else {
                if(!canPieceJump(sx, sy, dx, dy)) {
                    return false;
                }
            }
        char destinationC = state[dy][dx];
        if(destinationC != '.') {
            bool isDestinationMine;
            if(currentPlayer == LOWER) {
                isDestinationMine = isLowerPiece(destinationC);
            } else {
                isDestinationMine = isUpperPiece(destinationC);
            }
            if(isDestinationMine) {
                return false;
            }
            int opponentType = charToPiece(destinationC);
            if(fromTrap) {
                power = 0;
            }
            int opponentPower = opponentType + 1;
            if(isPieceTrapped(dx, dy)) {
                opponentPower = 0;
            }
            if(power == 8 && opponentPower == 1) {
                return false;
            }
            if(power == 1 && opponentPower == 8) {
                if(isWater(sx, sy) || isWater(dx, dy)) {
                    return false;
                }
            }
            if(power == 1 && opponentPower == 1) {
                if((isWater(sx, sy) != isWater(dx, dy))) {
                    return false;
                }
            }
            if(power < opponentPower) {
                return false;
            }
        }
        return true;
    }

    vector<Move> legalMoves() {
        vector<Move> moves;
        for(int row = 0; row < HEIGHT; row++) {
            for(int col = 0; col < WIDTH; col++) {
                char pieceC = state[row][col];
                if(pieceC == '.') {
                    continue;
                }
                bool isMine;
                if(currentPlayer == LOWER) {
                    isMine = isLowerPiece(pieceC);
                } else {
                    isMine = isUpperPiece(pieceC);
                }
                if(!isMine) {
                    continue;
                }
                int type = charToPiece(pieceC);
                int power = type + 1;
                if(isPieceTrapped(col, row)) {
                    power = 0;
                }
                for(int i = 0; i < 4; i++) {
                    int nextCol = col + DIRECTIONS[i].first;
                    int nextRow = row + DIRECTIONS[i].second;
                    if(isLegal(col, row, nextCol, nextRow)) {
                        moves.emplace_back(col, row, nextCol, nextRow);
                    }
                }
                if(type == TIGER || type == LION) {
                    for(int i = 0; i < 4; i++) {
                        int currentCol = col + DIRECTIONS[i].first;
                        int currentRow = row + DIRECTIONS[i].second;
                        if(!inBounds(currentCol, currentRow) || !isWater(currentCol, currentRow)) {
                            continue;
                        }
                        while(inBounds(currentCol + DIRECTIONS[i].first, currentRow + DIRECTIONS[i].second) && isWater(currentCol + DIRECTIONS[i].first, currentRow + DIRECTIONS[i].second)) {
                            currentCol += DIRECTIONS[i].first;
                            currentRow += DIRECTIONS[i].second;
                        }
                        int landingCol = currentCol + DIRECTIONS[i].first;
                        int landingRow = currentRow + DIRECTIONS[i].second;
                        if(inBounds(landingCol, landingRow) && isLegal(col, row, landingCol, landingRow)) {
                            moves.emplace_back(col, row, landingCol, landingRow);
                        }
                    }
                }
            }
        }
        return moves;
    }

    Board applyMove(int sx, int sy, int dx, int dy) {
        Board newBoard = *this;

        if(sx >= 0) {
            char pieceC = newBoard.state[sy][sx];
            char destinationC = newBoard.state[dy][dx];
            bool isKilling = false;
            if(destinationC != '.') {
                isKilling = true;
            }
            newBoard.state[sy][sx] = '.';
            newBoard.state[dy][dx] = pieceC;
            if(isKilling) {
                newBoard.noKillCount = 0;
            } else {
                newBoard.noKillCount++;
            }
        } else {
            newBoard.noKillCount++;
        }
        newBoard.currentPlayer = 1 - currentPlayer;
        return newBoard;
    }

    bool gameEnd() {
        if(state[0][3] >= 'a' && state[0][3] <= 'z') {
            return true;
        }
        if(state[8][3] >= 'A' && state[8][3] <= 'Z') {
            return true;
        }
        bool hasLowerPieces = false;
        bool hasUpperPieces = false;
        for(int row = 0; row < HEIGHT; row++) {
            for(int col = 0; col < WIDTH; col++) {
                char cell = state[row][col];
            
                if(cell >= 'a' && cell <= 'z') {
                    hasLowerPieces = true;
                } else if(cell >= 'A' && cell <= 'Z') {
                    hasUpperPieces = true;
                }
            }
        }
        if(!hasLowerPieces || !hasUpperPieces) {
            return true;
        }
        if(noKillCount >= MAX_NO_KILL) {
            return true;
        }
        return false;
    }
};


// ewaluacja w tym zadaniu bedzie skladala sie z 3 rzeczy
// znalazlem jakis dziwny paper skad zasugerowalem sie "material"
// kolejne to odlg tygryskow i lwow od nory przeciwnika
// i ostatecznie nagroda za to ze nikt nam nie atakuje dziury

int boardEvaluation(Board& board) {
    int evaluation = 0;
    
    // material wspomniany wczeniej
    for(int row = 0; row < HEIGHT; row++) {
        for(int col = 0; col < WIDTH; col++) {
            char cell = board.state[row][col];
            if(cell == '.') {
                continue;
            }
            int piece = charToPiece(cell);
            if(piece == EMPTY) {
                continue;
            }
            int size;
            if(cell >= 'a' && cell <= 'z') {
                size = LOWER;
            } else {
                size = UPPER;
            }
            int pieceValue = MATERIAL[piece];
            if(size == agentSize) {
                evaluation += pieceValue;
            } else {
                evaluation -= pieceValue;
            }
        }
    }

    // teraz tygryski i lwy
    int denX = 3;
    int myDenY;
    int opponentDenY;
    if(agentSize == LOWER) {
        myDenY = 8;
        opponentDenY = 0;
    } else {
        myDenY = 0;
        opponentDenY = 8;
    }
    for(int row = 0; row < HEIGHT; row++) {
        for(int col = 0; col < WIDTH; col++) {
            char cell = board.state[row][col];
            int piece = charToPiece(cell);
            if(piece == TIGER || piece == LION) {
                int size;
                if(cell >= 'a' && cell <= 'z') {
                    size = LOWER;
                } else {
                    size = UPPER;
                }
                int distToMe = manhattanDist(col, row, denX, myDenY);
                int distToOpponent = manhattanDist(col, row, denX, opponentDenY);
                int distDiff;
                if(size == agentSize) {
                    distDiff = distToMe - distToOpponent;
                } else {
                    distDiff = distToOpponent - distToMe;
                }
                evaluation += DEN_PENALTY * distDiff;
            }

        }
    }
    
    // ostatni klocek ukladanki niezagrozone plapki
    for(int row = 0; row < HEIGHT; row++) {
        for(int col = 0; col < WIDTH; col++) {
            char cell = board.state[row][col];
            if(cell == '.') {
                continue;
            }
            int size;
            if(cell >= 'a' && cell <= 'z') {
                size = LOWER;
            } else {
                size = UPPER;
            }
            if(isPieceTrapped(col, row)) {
                bool threatened = false;
                for(int i = 0; i < 4; i++) {
                    int nextCol = col + DIRECTIONS[i].first;
                    int nextRow = row + DIRECTIONS[i].second;
                    if(!board.inBounds(nextCol, nextRow)) {
                        continue;
                    }
                    char neighbourCell = board.state[nextRow][nextCol];
                    if(neighbourCell == '.') {
                        continue;
                    }
                    int neighbourSize;
                    if(neighbourCell >= 'a' && neighbourCell <= 'z') {
                        neighbourSize = LOWER;
                    } else {
                        neighbourSize = UPPER;
                    }
                    if(neighbourSize != size) {
                        threatened = true;
                        break;
                    }
                }
                if(!threatened) {
                    if(size == agentSize) {
                        evaluation += TRAP_REWARD;
                    } else {
                        evaluation -= TRAP_REWARD;
                    }
                }
            }
        }
    }
    return evaluation;
}

bool comparison(pair<Move, int>& p1, pair<Move, int>& p2) {
    return p1.second > p2.second;
}

vector<pair<Move, int>> evaluatedMovesInOrder(Board& board) {
    vector<Move> moves = board.legalMoves();
    vector<pair<Move, int>> evaluatedMoves;
    // //
    // // zylowanie wydajnosci na potrzebe zyskania 0.5 sekundy
    evaluatedMoves.reserve(moves.size());
    // //
    // //
    for(int i = 0; i < moves.size(); i++) {
        Move move = moves[i];
        Board afterMove = board.applyMove(move.sx, move.sy, move.dx, move.dy);
        int evaluation = boardEvaluation(afterMove);
        // //
        // // zylowanie wydajnosci
        evaluatedMoves.emplace_back(make_pair(move, evaluation));
        // //
        // //
        // evaluatedMoves.push_back(make_pair(move, evaluation));
    }
    sort(evaluatedMoves.begin(), evaluatedMoves.end(), comparison);
    return evaluatedMoves;
}

int minMax(Board& board, int depth, int alpha, int beta) {
    if(depth == 0 || board.gameEnd()) {
        return boardEvaluation(board);
    }
    
    
    vector<pair<Move, int>> moves = evaluatedMovesInOrder(board);


    if(moves.empty()) {
        Board turnPassed = board;
        turnPassed = turnPassed.applyMove(-1, -1, -1, -1);
        // turnPassed.currentPlayer = 1 - board.currentPlayer;
        // turnPassed.noKillCount = board.noKillCount + 1;
        return minMax(turnPassed, depth - 1, alpha, beta);
    }
    // 
    if(board.currentPlayer == agentSize) {
        int maxEval = INT_MIN;
        for(int i = 0; i < moves.size(); i++) {
            Move move = moves[i].first;
            Board child = board.applyMove(move.sx, move.sy, move.dx, move.dy);
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
            Move move = moves[i].first;
            Board child = board.applyMove(move.sx, move.sy, move.dx, move.dy);
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

Move selectBestMove(Board& board, int maxDepth) {
    
    vector<pair<Move, int>> moves = evaluatedMovesInOrder(board);
    if(moves.empty()) {
        return Move(); // -1 -1 -1 -1 tak o passujemy
    }

    bool maximizing = (board.currentPlayer == agentSize);
    int alpha = INT_MIN;
    int beta = INT_MAX;
    int bestEval;
    if(maximizing) {
        bestEval = INT_MIN;    
    } else {
        bestEval = INT_MAX;
    }

    Move bestMove = moves[0].first;

    for(int i = 0; i < moves.size(); i++) {
        Move move = moves[i].first;
        Board child = board.applyMove(move.sx, move.sy, move.dx, move.dy);
        int eval = minMax(child, maxDepth - 1, alpha, beta);
        

        if((maximizing && eval > bestEval) || (!maximizing && eval < bestEval)) {
            bestEval = eval;
            bestMove = move;
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

int main() {
    // szybkie linijki
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Board board(LOWER);
    bool firstMove = true;

    cout << "RDY\n" << flush;
    string msg;
    while(cin >> msg) {
        if(msg == "UGO") {
            double moveTime;
            double gameTime;
            cin >> moveTime >> gameTime;

            if(firstMove) {
                agentSize = LOWER;
                board.reset(agentSize);
                firstMove = false;
            }
            // liczymy
            Move bestMove = selectBestMove(board, 5);
            cout << "IDO " << bestMove.sx << " " << bestMove.sy << " " << bestMove.dx << " " << bestMove.dy << "\n" << flush;

            // board.applyMove(bestMove.sx, bestMove.sy, bestMove.dx, bestMove.dy);
            board = board.applyMove(bestMove.sx, bestMove.sy, bestMove.dx, bestMove.dy);


        } else if(msg == "HEDID") {
            double moveTime;
            double gameTime;
            // cin >> moveTime >> gameTime;
            int sx;
            int sy;
            int dx;
            int dy;
            cin >> moveTime >> gameTime >> sx >> sy >> dx >> dy;

            // he did czyli nie jestesmy pierwsi
            if(firstMove) {
                agentSize = UPPER;
                board.reset(agentSize);
                board.currentPlayer = 1 - agentSize;
                firstMove = false;
            }
            // ruch oponenta
            board = board.applyMove(sx, sy, dx, dy);
            Move bestMove = selectBestMove(board, 5);
            cout << "IDO " << bestMove.sx << " " << bestMove.sy << " " << bestMove.dx << " " << bestMove.dy << "\n" << flush;
            // board.applyMove(bestMove.sx, bestMove.sy, bestMove.dx, bestMove.dy);
            board = board.applyMove(bestMove.sx, bestMove.sy, bestMove.dx, bestMove.dy);

            
        } else if(msg == "ONEMORE") {
            // resecick
            board.reset(agentSize);
            firstMove = true;
            cout << "RDY\n" << flush;
        } else if(msg == "BYE") {
            break;
        }
    }
    return 0;
}