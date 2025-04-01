#ifndef CELL_H
#define CELL_H

#include "Color.h"
/**
 * @brief The Cell class
 */
class Cell {
private:
    Color color;

public:
    /**
     * @brief Cell constructor with parameters color
     */
    Cell(Color color);
    /**
     * @brief getColor function to get the color of a cell
     * @return Color
     */
    Color getColor() const;
    /**
     * @brief setColor function to set the color of a cell
     * @param newColor
     */
    void setColor(Color newColor);
    /**
     * @brief is  the Same Color ?
     * @param other
     * @return bool
     */
    bool isSameColor(const Cell& other) const;
};

#endif // CELL_H
