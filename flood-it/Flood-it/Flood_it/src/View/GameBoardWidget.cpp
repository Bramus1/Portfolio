#include "GameBoardWidget.h"
#include <QPainter>
#include <QMouseEvent>


GameBoardWidget::GameBoardWidget(QWidget *parent) : QWidget(parent),
    game(nullptr) {
}


void GameBoardWidget::setGame(Game* game) {
    this->game = game;
}

Game *GameBoardWidget::getGame() const {
    return game;
}

void GameBoardWidget::paintEvent(QPaintEvent* event) {

    QPainter painter(this);
    int rows = game->getBoard().getRows();
    int cols = game->getBoard().getCols();
    // Calculate the size of the largest square that fits
    int cellSize = qMin(width() / cols, height() / rows);

    // Center the board within the widget
    int xOffset = (width() - (cellSize * cols)) / 2;
    int yOffset = (height() - (cellSize * rows)) / 2;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QColor color;

            switch (game->getBoard().getCellColor(row, col)) {
            case Color::Green: color = Qt::green; break;
            case Color::Red: color = Qt::red; break;
            case Color::Blue: color = Qt::blue; break;
            case Color::Yellow: color = Qt::yellow; break;
            case Color::Magenta: color = Qt::magenta; break;
            case Color::Cyan: color = Qt::cyan; break;
            case Color::Purple: color = Qt::GlobalColor::darkMagenta; break;
            case Color::Orange: color = Qt::GlobalColor::darkYellow; break;

            default: color = Qt::BlankCursor; break;
            }

            // Draw the square cell
            painter.fillRect(xOffset + col * cellSize, yOffset + row * cellSize, cellSize, cellSize, color);
            painter.drawRect(xOffset + col * cellSize, yOffset + row * cellSize, cellSize, cellSize);
        }
    }

}

void GameBoardWidget::mousePressEvent(QMouseEvent* event) {
    if (!game ) {
        return;
    }

    int rows = game->getBoard().getRows();
    int cols = game->getBoard().getCols();

    // Calculate the size of the largest square that fits
    int cellSize = qMin(width() / cols, height() / rows);

    // Calculate the offsets to center the board within the widget
    int xOffset = (width() - (cellSize * cols)) / 2;
    int yOffset = (height() - (cellSize * rows)) / 2;

    // Adjust the mouse click position based on the offset
    int col = (event->x() - xOffset) / cellSize;
    int row = (event->y() - yOffset) / cellSize;

    // Ensure that the click is within the bounds of the board
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        Color clickedColor = game->getBoard().getCellColor(row, col);

        emit cellClicked(row, col);  // Notify if the game was updated
        update();  // Repaint the board
    }
}

void GameBoardWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    int rows = game->getBoard().getRows();
    int cols = game->getBoard().getCols();
    // Ensure we do not divide by zero
    if (rows == 0 || cols == 0) {
        return;
    }
    // Calculate the largest possible square cell size
    int cellSize = qMin(width() / cols, height() / rows);
    // Redraw the board when the widget is resized
    update();

}
