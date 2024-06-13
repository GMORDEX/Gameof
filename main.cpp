#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <windows.h>

using namespace std;

int x = 0;
int y = 0;
int counter = 0;

void displayBoard(vector<vector<int>>& board) {
    system("cls");
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            cout << (board[i][j] ? 'O' : '.') << ' ';
        }
        cout << endl;
    }
}

int countNeighbors(vector<vector<int>>& board, int x, int y) {
    int count = 0;
    int height = board.size();
    int width = board[0].size();
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int col = (x + i + height) % height;
            int row = (y + j + width) % width;
            count += board[col][row];
        }
    }
    return count;
}

void updateBoard(vector<vector<int>>& board) {
    int height = board.size();
    int width = board[0].size();
    vector<vector<int>> newBoard(height, vector<int>(width));

    int unchanged = 1;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int liveNeighbors = countNeighbors(board, i, j);

            if (board[i][j] == 1 && (liveNeighbors < 2 || liveNeighbors > 3)) {
                newBoard[i][j] = 0; // Die of underpopulation or overpopulation
                unchanged = 0;
            }
            else if (board[i][j] == 0 && liveNeighbors == 3) {
                newBoard[i][j] = 1; // Become alive through reproduction
                unchanged = 0;
            }
            else {
                newBoard[i][j] = board[i][j]; // stay in the same state
            }
        }
    }

    if (unchanged) {
        counter++;
    }
    else {
        counter = 0;
    }

    board.swap(newBoard);
}

int readConfig(const string& configPath, int* width, int* height) {
    ifstream file(configPath);
    if (!file) {
        cout << "Error: config file not found." << endl;
        return 1;
    }

    char ch;
    file >> ch >> ch >> *width >> ch >> ch >> *height;
    file.close();
    return 0;
}

bool readBMPGrid(const string& filePath, vector<vector<int>>& board, int width, int height) {
    ifstream file(filePath, ios::binary);
    if (!file) {
        cout << "Error: Game grid file not found." << endl;
        return false;
    }


    file.seekg(54, ios::beg);

    // Read pixel data
    unsigned char pixel[3];
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            file.read(reinterpret_cast<char*>(pixel), 3);
            board[i][j] = (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0) ? 0 : 1;
        }
    }

    file.close();
    return true;
}

void waitForEnter() {
    cout << "Press Enter to continue...";
    cin.ignore(); // Clear any extra inputs
    cin.get();    // Wait for Enter 
}

int main() {
    if (readConfig("config.txt", &x, &y)) {
        return 1;
    }

    vector<vector<int>> board(y, vector<int>(x));

    if (!readBMPGrid("game_grid.bmp", board, x, y)) {
        return 1;
    }

    displayBoard(board);
    waitForEnter(); // Waiting for enter to start the game

    while (counter < 2) {
        displayBoard(board);
        updateBoard(board);
        Sleep(300); 
    }

    cout << "Life has ended. Press Enter to exit...";
    waitForEnter(); 

    return 0;
}
