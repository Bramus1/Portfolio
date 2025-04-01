#include "GameBoard.h"
#include <cstdlib>
#include <ctime>
GameBoard::GameBoard(int rows, int cols, int numColors)
    : rows(rows), cols(cols),nbcolors(numColors), board(rows, std::vector<Cell>(cols, Cell(Color::White))) {
}

Color GameBoard::getRandomColor() {
    return static_cast<Color>(rand() % nbcolors);
}

void GameBoard::initializeBoard() {
    srand(static_cast<unsigned>(time(0)));
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            board[row][col].setColor(getRandomColor());
        }
    }
}

Color GameBoard::getCellColor(int row, int col) const {
    return board[row][col].getColor();
}

void GameBoard::setCellColor(int row, int col, Color color) {
    board[row][col].setColor(color);
}

bool GameBoard::isMonochrome() const {
    Color firstColor = board[0][0].getColor();
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (board[row][col].getColor() != firstColor) {
                return false;
            }
        }
    }
    return true;
}

int GameBoard::getRows() const {
    return rows;
}

int GameBoard::getCols() const {
    return cols;
}

int GameBoard::getNbColors() const {
    return nbcolors;
}


bool GameBoard::expandCapturedZone(Color newColor) {
    Color currentColor = board[0][0].getColor();
    if (newColor == currentColor) {
        return false;
    }
    floodFill(0, 0, currentColor, newColor);
    return true;
}

void GameBoard::floodFill(int row, int col, Color oldColor, Color newColor) {

    if (row < 0 || row >= rows || col < 0 || col >= cols) return;
    if (board[row][col].getColor() != oldColor) return;
    board[row][col].setColor(newColor);
    floodFill(row - 1, col, oldColor, newColor);
    floodFill(row + 1, col, oldColor, newColor);
    floodFill(row, col - 1, oldColor, newColor);
    floodFill(row, col + 1, oldColor, newColor);
}
