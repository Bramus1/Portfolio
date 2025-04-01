#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include "Cell.h"
#include "Color.h"
/**
 * @brief The GameBoard class
 */
class GameBoard{
private:
    int rows, cols,nbcolors;
    std::vector<std::vector<Cell>> board;
    Color getRandomColor();

public:
    /**
     * @brief GameBoard constructor with parameters rows, cols, numColors
     * @param rows
     * @param cols
     * @param numColors
     */
    GameBoard(int rows, int cols, int numColors);
    /**
     * @brief initializeBoard void function to initialize the board with random colors
     */
    void initializeBoard();
    /**
     * @brief getCellColor  function to get the color of a cell
     * @param row
     * @param col
     * @return
     */
    Color getCellColor(int row, int col) const;
    /**
     * @brief setCellColor function to set the color of a cell
     * @param row
     * @param col
     * @param color
     */
    void setCellColor(int row, int col, Color color);
    /**
     * @brief expandCapturedZone function to expand the captured zone
     * @param newColor
     * @return
     */
    bool expandCapturedZone(Color newColor);
    /**
     * @brief isMonochrome function to check if the board is monochrome
     * @return  bool
     */
    bool isMonochrome() const;
    /**
     * @brief floodFill function to fill the board with a new
     * @param row
     * @param col
     * @param oldColor
     * @param newColor
     */
    void floodFill(int row, int col, Color oldColor, Color newColor);
    /**
     * @brief getRows function to get the number of rows
     * @return int
     */
    int getRows() const;
    /**
     * @brief getCols function to get the number of columns
     * @return int
     */
    int getCols() const;
    /**
     * @brief getNbColors function to get the number of colors
     * @return int
     */
    int getNbColors () const;
};

#endif // GAMEBOARD_H
