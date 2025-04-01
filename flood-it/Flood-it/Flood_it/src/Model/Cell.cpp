#include "Cell.h"

Cell::Cell(Color color) : color(color) {}

Color Cell::getColor() const {
    return color;
}

void Cell::setColor(Color newColor) {
    color = newColor;
}

bool Cell::isSameColor(const Cell& other) const {
    return this->color == other.color;
}
