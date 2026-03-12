#include <iostream>
#include <bits/stdc++.h>
#include <random>
#include <unordered_set>
using namespace std;

//JUMP Entities
class Jump {
public:
    int start;
    int end;

    Jump(int start, int end) {
        this->start = start;
        this->end = end;
    }
};


//DICE Entities
class Dice {
public:
    int diceCount;
    int min = 1;
    int max = 6;

    Dice(int diceCount){
        this->diceCount = diceCount;
    }

    int rollDice() {
        int totsum = 0;
        int rollcount = diceCount;
        while(rollcount>0) {
            totsum += (rand()%6) + 1;
            rollcount--;
        }
        return totsum;
    }
};

class DiceStrategy {
    virtual int rollDice() = 0;
};


//CELL Entities
class Cell {
private:
    Jump* jump;

public:
    Cell() {
        this->jump = nullptr;
    }

    void addJump(Jump* jump) {
        this->jump = jump;
    }
    Jump* getJump() {
        return jump;
    }
};


//PLAYER Entities
class Player {
public:
    string id;
    int currentPos;

    Player(string id, int currentPos) {
        this->id = id;
        this->currentPos = currentPos;
    }
};


//Board Entity
class Board {
private:
    vector<vector<Cell*>>* cells;
    unordered_set<string> checkDuplicate;

public:
    Board(int boardSize, int noOfSnakes, int noOfLadders) {
        initializeCells(boardSize);
        addSnakesAndLadders(boardSize, noOfSnakes, noOfLadders);
    }

    int getCellSize() {
        return this->cells->size();
    }

private:
    void initializeCells(int boardSize) {
        this->cells = new vector<vector<Cell*>>(boardSize, vector<Cell*>(boardSize, nullptr));
        for(int i=0; i<boardSize; i++) {
            for(int j=0; j<boardSize; j++) {
                Cell* cellobj = new Cell();
                (*this->cells)[i][j] = cellobj;
            }
        }
    }

    int getRandomNumber(int st, int en) {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<int> dist(st, en);
        return dist(gen);
    }

    void addSnakesAndLadders(int boardSize, int noOfSnakes, int noOfLadders) {
        int st = 1;
        int en = boardSize*boardSize-2;
        
        while(noOfSnakes>0 || noOfLadders>0) {
            int rand1 = getRandomNumber(st, en);
            int rand2 = getRandomNumber(st, en);
            int mini = min(rand1, rand2);
            int maxi = max(rand1, rand2);
            if(checkDuplicate.count({to_string(mini) + "-" + to_string(maxi)})>0 || rand1==rand2) continue;

            if(noOfSnakes==0 && rand1>rand2) continue;
            if(noOfLadders==0 && rand1<rand2) continue;
            Jump* jump = new Jump(rand1, rand2);
            Cell* cell = getCellFromPosition(rand1, boardSize);
            if(!cell->getJump()) cell->addJump(jump);
            if(rand1 < rand2) {
                noOfLadders--;
            } else {
                noOfSnakes--;
            }

            checkDuplicate.insert({to_string(mini) + "-" + to_string(maxi)});
        }
    }

public:
    Cell* getCellFromPosition(int boardPos, int boardSize) {
        int row = boardPos/boardSize;
        int col = boardPos%boardSize;
        if(row%2!=0) {
            col = boardSize-col-1;
        }
        return (*this->cells)[row][col];
    }
};





//SnakeAndLadderGame -> Driver class
class SnakeAndLadderGame {
public:
    Board* board;
    Dice* dice;
    deque<Player*> playersList;   // Deque<Player>
    Player* winner;

    SnakeAndLadderGame() {
        initializeGame();
    }

private:
    void initializeGame() {
        this->board = new Board(10, 10, 20);
        this->dice = new Dice(2);
        this->winner = nullptr;
        addPlayers();
    }

    void addPlayers() {
        Player* player1 = new Player("p1", 0);
        Player* player2 = new Player("p2", 0);

        this->playersList.push_back(player1);
        this->playersList.push_back(player2);
    }

public:
    void startGame() {
        while(this->winner == nullptr) {
            Player* playerTurn = findPlayerTurn();

            cout << "player turn is:" << playerTurn->id
                 << " current position is: " << playerTurn->currentPos << "\n";

            int diceNumbers = this->dice->rollDice();
            int playerNewPosition = playerTurn->currentPos + diceNumbers;

            playerNewPosition = jumpCheck(playerNewPosition);

            playerTurn->currentPos = playerNewPosition;

            cout << "player turn is:" << playerTurn->id
                 << " new Position is: " << playerNewPosition << "\n";

            int n = this->board->getCellSize();
            int lastCell = (n * n) - 1;

            if(playerNewPosition >= lastCell) {
                this->winner = playerTurn;
            }
        }

        cout << "WINNER IS:" << this->winner->id << "\n";
    }

private:
    Player* findPlayerTurn() {
        Player* playerTurns = this->playersList.front();
        this->playersList.pop_front();
        this->playersList.push_back(playerTurns);
        return playerTurns;
    }

    int jumpCheck(int playerNewPosition) {
        int n = this->board->getCellSize();
        int lastCell = n * n - 1;

        if(playerNewPosition > lastCell) {
            return playerNewPosition;
        }

        Cell* cell = this->board->getCellFromPosition(playerNewPosition, n);

        if(cell->getJump() != nullptr && cell->getJump()->start == playerNewPosition) {
            string jumpBy = (cell->getJump()->start < cell->getJump()->end) ? "ladder" : "snake";
            cout << "jump done by: " << jumpBy << "\n";
            return cell->getJump()->end;
        }

        return playerNewPosition;
    }
};



int main() {
    SnakeAndLadderGame* game = new SnakeAndLadderGame();
    game->startGame();
    return 0;
}

