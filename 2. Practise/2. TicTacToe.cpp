#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// =========================================================
// PieceType (enum)
// =========================================================
enum class PieceType {
    X,
    O
};

string pieceTypeToString(PieceType pieceType) {
    if (pieceType == PieceType::X) return "X";
    return "O";
}

// =========================================================
// PlayingPiece
// =========================================================
class PlayingPiece {
public:
    PieceType pieceType;

    PlayingPiece(PieceType pieceType) {
        this->pieceType = pieceType;
    }
};

// =========================================================
// PlayingPieceX
// =========================================================
class PlayingPieceX : public PlayingPiece {
public:
    PlayingPieceX() : PlayingPiece(PieceType::X) {}
};

// =========================================================
// PlayingPieceO
// =========================================================
class PlayingPieceO : public PlayingPiece {
public:
    PlayingPieceO() : PlayingPiece(PieceType::O) {}
};

// =========================================================
// Player
// =========================================================
class Player {
public:
    string name;
    PlayingPiece* playingPiece;

    Player(string name, PlayingPiece* playingPiece) {
        this->name = name;
        this->playingPiece = playingPiece;
    }

    string getName() {
        return this->name;
    }

    void setName(string name) {
        this->name = name;
    }

    PlayingPiece* getPlayingPiece() {
        return this->playingPiece;
    }

    void setPlayingPiece(PlayingPiece* playingPiece) {
        this->playingPiece = playingPiece;
    }
};

// =========================================================
// Board
// =========================================================
class Board {
public:
    int size;
    vector<vector<PlayingPiece*>> board;

    Board(int size) {
        this->size = size;
        board.resize(size, vector<PlayingPiece*>(size, nullptr));
    }

    bool addPiece(int row, int column, PlayingPiece* playingPiece) {
        if (board[row][column] != nullptr) {
            return false;
        } else {
            board[row][column] = playingPiece;
            return true;
        }
    }

    vector<pair<int, int>> getFreeCells() {
        vector<pair<int, int>> freeCells;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == nullptr) {
                    freeCells.push_back({i, j});
                }
            }
        }

        return freeCells;
    }

    void printBoard() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] != nullptr) {
                    cout << pieceTypeToString(board[i][j]->pieceType) << "   ";
                } else {
                    cout << "    ";
                }
                cout << " | ";
            }
            cout << "\n";
        }
    }
};

// =========================================================
// TicTacToeGame
// =========================================================
class TicTacToeGame {
public:
    deque<Player*> players;
    Board* gameBoard;

    void initializeGame() {
        players.clear();

        PlayingPiece* crossPiece = new PlayingPieceX();
        Player* player1 = new Player("Player1", crossPiece);

        PlayingPiece* noughtsPiece = new PlayingPieceO();
        Player* player2 = new Player("Player2", noughtsPiece);

        players.push_back(player1);
        players.push_back(player2);

        gameBoard = new Board(3);
    }

    string startGame() {
        bool noWinner = true;

        while (noWinner) {
            Player* playerTurn = players.front();
            players.pop_front();

            gameBoard->printBoard();

            vector<pair<int, int>> freeSpaces = gameBoard->getFreeCells();
            if (freeSpaces.empty()) {
                noWinner = false;
            } else {
                cout << "Player:" << playerTurn->name << " Enter row,column: ";
                string s;
                getline(cin, s);

                // same logic: split by comma
                vector<string> values;
                string temp;
                for (char ch : s) {
                    if (ch == ',') {
                        values.push_back(temp);
                        temp.clear();
                    } else {
                        temp.push_back(ch);
                    }
                }
                values.push_back(temp);

                int inputRow = stoi(values[0]);
                int inputColumn = stoi(values[1]);

                bool pieceAddedSuccessfully =
                    gameBoard->addPiece(inputRow, inputColumn, playerTurn->playingPiece);

                if (!pieceAddedSuccessfully) {
                    cout << "Incorredt possition chosen, try again\n";
                    players.push_front(playerTurn);
                } else {
                    players.push_back(playerTurn);

                    bool winner =
                        isThereWinner(inputRow, inputColumn, playerTurn->playingPiece->pieceType);

                    if (winner) {
                        return playerTurn->name;
                    }
                }
            }
        }

        return "tie";
    }

    bool isThereWinner(int row, int column, PieceType pieceType) {
        bool rowMatch = true;
        bool columnMatch = true;
        bool diagonalMatch = true;
        bool antiDiagonalMatch = true;

        // row check
        for (int i = 0; i < gameBoard->size; i++) {
            if (gameBoard->board[row][i] == nullptr ||
                gameBoard->board[row][i]->pieceType != pieceType) {
                rowMatch = false;
            }
        }

        // column check
        for (int i = 0; i < gameBoard->size; i++) {
            if (gameBoard->board[i][column] == nullptr ||
                gameBoard->board[i][column]->pieceType != pieceType) {
                columnMatch = false;
            }
        }

        // diagonal check
        int i = 0;
        for (int j = 0; i < gameBoard->size; j++) {
            if (gameBoard->board[i][j] == nullptr ||
                gameBoard->board[i][j]->pieceType != pieceType) {
                diagonalMatch = false;
            }
            i++;
        }

        // anti-diagonal check
        i = 0;
        for (int j = gameBoard->size - 1; i < gameBoard->size; j--) {
            if (gameBoard->board[i][j] == nullptr ||
                gameBoard->board[i][j]->pieceType != pieceType) {
                antiDiagonalMatch = false;
            }
            i++;
        }

        return rowMatch || columnMatch || diagonalMatch || antiDiagonalMatch;
    }
};

// =========================================================
// Main
// =========================================================
int main() {
    TicTacToeGame game;
    game.initializeGame();
    game.startGame();
    cout << "game winner is: " << game.startGame() << "\n";
    return 0;
}
