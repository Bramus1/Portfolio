#ifndef GAME_H
#define GAME_H

#include "GameBoard.h"
#include "Observable.h"
/**
 * @brief The Game class
 */
class Game: public Observable {
private:
    GameBoard& board;
    int moves;
public:
    /**
     * @brief Game constructor with parameter board
     * @param board
     */
    Game(GameBoard & board);
    /**
     * @brief startNewGame start a new game with parameters row, col, nbColors
     * @param row
     * @param col
     * @param nbColors
     * @return void
     */
    void startNewGame(int row, int col, int nbColors);
    /**
     * @brief makeMove make a move with parameter color
     * @param color
     */
    void makeMove(Color color);
    /**
     * @brief getMoves get the number of moves
     * @return int
     */

    int getMoves() const;
    /**
     * @brief isGameWon check if the game is won
     * @return bool
     */
    bool isGameWon() const;
    /**
     * @brief expandCapturedZone expand the captured zone with parameter newColor
     * @param newColor
     */
    void expandCapturedZone(Color  newColor);
    /**
     * @brief getBoard get the board
     * @return &  GameBoard
     */
    GameBoard& getBoard();
    /**
     * @brief getNumColors get the number
     * @return int
     */
    int getNumColors() const;
    /**
     * @brief getCol get the column
     * @return int
     */
    int getCol()const;
    /**
     * @brief getRow get the row
     * @return int
     */
    int getRow()const;


};

#endif // GAME_H
