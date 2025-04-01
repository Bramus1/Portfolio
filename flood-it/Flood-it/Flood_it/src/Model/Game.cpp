#include "Game.h"
#include <cmath>


Game::Game(GameBoard & board) : board(board), moves{0} {


}

void Game::startNewGame(int rows, int cols, int numColors) {

   GameBoard newboard (rows,cols,numColors);  // Create a new game board
    board = newboard;  //  Assign the new game board to the game
    moves = 0;
    board.initializeBoard();
    notifyObservers();
}

void Game::makeMove(Color color) {
        if (color !=board.getCellColor(0,0)) {
        board.expandCapturedZone(color);
        moves++;
        notifyObservers();
    }
}
int Game::getMoves() const {
    return moves;
}
int Game::getCol() const {
    return board.getCols();
}
int Game::getRow() const {
    return board.getRows();
}

GameBoard& Game::getBoard()  {
    return board;
}
bool Game::isGameWon() const {
    return board.isMonochrome();
}
int Game::getNumColors() const {
    return board.getNbColors();
}



