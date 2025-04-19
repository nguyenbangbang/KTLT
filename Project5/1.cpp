#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdio>
#include <conio.h>
#include <windows.h>
#define BOARD_SIZE 15

using namespace std;

struct Move {
    char player;
    int row, col;
};

class CaroGame {
private:
    char board[BOARD_SIZE][BOARD_SIZE];
    vector<Move> moves;
    char currentPlayer;
    int cursorRow, cursorCol;

public:
    CaroGame() {
        resetBoard();// set lai man hinh
        currentPlayer = 'X';// X choi truoc
        cursorRow = cursorCol = 0;// goc trai
    }

    void resetBoard() {//set lai ban co thanh .
        for (int i = 0; i < BOARD_SIZE; i++)
            for (int j = 0; j < BOARD_SIZE; j++)
                board[i][j] = '.';
    }

    void displayBoard() {
        // Windows: sử dụng API console để in màu.
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);// khai bao de thao tac trên console 
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        WORD defaultAttr = csbi.wAttributes;//mau
        COORD cursorPos = { 0,0 };//goc trai man hinh 
        SetConsoleCursorPosition(hConsole, cursorPos); // Xóa màn hình bằng cách di chuyển con trỏ về đầu

        // In tiêu đề cột
        cout << "     ";
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (j < 10)
                cout << " " << j << "  ";
            else
                cout << j << "  ";
        }
        cout << "\n";

        // In đường viền trên cùng
        cout << "    +";
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << "---+";
        }
        cout << "\n";

        // In từng dòng của bảng
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (i < 10) cout << " " << i << "  |";
            else        cout << i << "  |";

            for (int j = 0; j < BOARD_SIZE; j++) {
                bool selected = (i == cursorRow && j == cursorCol);
                char cell = board[i][j];

                if (selected) {
                    cout << "[";
                    if (cell == 'X') {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        cout << cell;
                        SetConsoleTextAttribute(hConsole, defaultAttr);
                    }
                    else if (cell == 'O') {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                        cout << cell;
                        SetConsoleTextAttribute(hConsole, defaultAttr);
                    }
                    else {
                        cout << cell;
                    }
                    cout << "]";
                }
                else {
                    cout << " ";
                    if (cell == 'X') {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        cout << cell;
                        SetConsoleTextAttribute(hConsole, defaultAttr);
                    }
                    else if (cell == 'O') {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                        cout << cell;
                        SetConsoleTextAttribute(hConsole, defaultAttr);
                    }
                    else {
                        cout << cell;
                    }
                    cout << " ";
                }
                cout << "|";
            }
            cout << "\n";

            // In đường phân cách giữa các dòng
            cout << "    +";
            for (int j = 0; j < BOARD_SIZE; j++) {
                cout << "---+";
            }
            cout << "\n";
        }

        cout << "\nPlayer " << getCurrentPlayer()
            << "'s turn. Use arrow keys to move and Enter to place.\n";
    }
   


    bool makeMove() {
        if (board[cursorRow][cursorCol] != '.')
            return false;

        board[cursorRow][cursorCol] = currentPlayer;
        moves.push_back({ currentPlayer, cursorRow, cursorCol });

        if (checkWin(cursorRow, cursorCol)) {
            displayBoard();
            cout << "Player " << currentPlayer << " wins!\n";
           saveGame("game.csv");
            return true;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        return false;
    }

    bool checkWin(int row, int col) {
        const int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
        for (auto& dir : directions) {
            int count = 1;
            for (int i = 1; i < 4; i++) {
                int r = row + i * dir[0], c = col + i * dir[1];
                if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != currentPlayer)
                    break;
                count++;
            }
            for (int i = 1; i < 4; i++) {
                int r = row - i * dir[0], c = col - i * dir[1];
                if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != currentPlayer)
                    break;
                count++;
            }
            if (count >= 4) return true;
        }
        return false;
    }

    void saveGame(const string& filename) {
        ofstream file("game.csv");
        file << BOARD_SIZE << "x" << BOARD_SIZE;
        for (const auto& move : moves) {
            file << ";" << move.player << "-" << move.row << "-" << move.col;
        }
        file.close();
    }

    char getCurrentPlayer() const {
        return currentPlayer;
    }

    // direction: 0-Up, 1-Down, 2-Left, 3-Right
    void moveCursor(int direction) {
        switch (direction) {
        case 0: if (cursorRow > 0) cursorRow--; break;
        case 1: if (cursorRow < BOARD_SIZE - 1) cursorRow++; break;
        case 2: if (cursorCol > 0) cursorCol--; break;
        case 3: if (cursorCol < BOARD_SIZE - 1) cursorCol++; break;
        }
    }
};



int main() {
    CaroGame game;
    char key;
    while (true) {
        game.displayBoard();
        key = _getch();
        if (key == -32) key = _getch(); // xử lý phím mở rộng
        if (key == 72) game.moveCursor(0);      // Up
        else if (key == 80) game.moveCursor(1);   // Down
        else if (key == 75) game.moveCursor(2);   // Left
        else if (key == 77) game.moveCursor(3);   // Right
        else if (key == 13) { // Enter
            if (game.makeMove())
                break;
        }
    }
    return 0;
}
