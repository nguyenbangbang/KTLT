#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>

using namespace std;

const int SIZE = 15;
char board[SIZE][SIZE];

void initBoard() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = ' ';
}

// Hàm thay ??i màu ch?
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// In bàn c? ra console
void drawBoard() {
    system("cls"); // Xóa màn hình
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << " ";
            if (board[i][j] == 'X') {
                setColor(12); // Màu ??
                cout << "X";
            }
            else if (board[i][j] == 'O') {
                setColor(9); // Màu xanh
                cout << "O";
            }
            else {
                setColor(6); // Màu cam cho ???ng k?
                cout << "|";
            }
            setColor(7); // Reset v? tr?ng
        }
        cout << endl;

        // In ???ng k? ngang
        for (int j = 0; j < SIZE; j++) {
            setColor(6);
            cout << "---";
            setColor(7);
        }
        cout << endl;
    }
}

// Ki?m tra th?ng/thua (4 ô liên ti?p)
bool checkWin(int row, int col, char player) {
    int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
    for (auto d : directions) {
        int count = 1;
        for (int step = 1; step < 4; step++) {
            int r = row + step * d[0];
            int c = col + step * d[1];
            if (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board[r][c] == player)
                count++;
            else break;
        }
        for (int step = 1; step < 4; step++) {
            int r = row - step * d[0];
            int c = col - step * d[1];
            if (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board[r][c] == player)
                count++;
            else break;
        }
        if (count >= 4) return true;
    }
    return false;
}

// L?y v? trí chu?t và x? lý n??c ?i
bool getMouseClick(int& row, int& col) {
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD numEvents = 0;
    GetNumberOfConsoleInputEvents(hIn, &numEvents);

    if (numEvents > 0) {
        INPUT_RECORD event;
        DWORD numRead;
        ReadConsoleInput(hIn, &event, 1, &numRead);

        if (event.EventType == MOUSE_EVENT) {
            int x = event.Event.MouseEvent.dwMousePosition.X;
            int y = event.Event.MouseEvent.dwMousePosition.Y;

            row = y / 2;  // ?i?u ch?nh t? l? v? trí
            col = x / 2;

            return true;
        }
    }
    return false;
}

int main() {
    initBoard();
    bool isXTurn = true;
    int row, col;

    while (true) {
        drawBoard();
        cout << "Nguoi choi " << (isXTurn ? "X" : "O") << " danh: ";

        // Nh?n input t? chu?t
        while (!getMouseClick(row, col));

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != ' ') {
            cout << "Vi tri khong hop le! Chon lai.\n";
            Sleep(1000);
            continue;
        }

        board[row][col] = isXTurn ? 'X' : 'O';

        // Ki?m tra th?ng/thua
        if (checkWin(row, col, board[row][col])) {
            drawBoard();
            setColor(isXTurn ? 12 : 9);
            cout << "Nguoi choi " << board[row][col] << " thang!\n";
            setColor(7);
            break;
        }

        isXTurn = !isXTurn;
    }

    return 0;
}
