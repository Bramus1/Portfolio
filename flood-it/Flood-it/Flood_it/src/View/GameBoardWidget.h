#ifndef GAMEBOARDWIDGET_H
#define GAMEBOARDWIDGET_H

#include <QWidget>
#include "../Model/Game.h"
#include "../Model/RecordManager.h"
/**
 * @brief The GameBoardWidget class
 */
class GameBoardWidget : public QWidget {
    Q_OBJECT

private:

    Game* game;
    RecordManager* recordManager;

public:
    /**
     * @brief GameBoardWidget constructor with parameter parent QWidget
     * @param parent
     */
    explicit GameBoardWidget(QWidget *parent = nullptr);
    /**
     * @brief setGame set the game with parameter game
     * @param game
     */
    void setGame(Game* game);
    /**
      * @brief getGame get the game
      * @return Game
      */
     Game *getGame() const;

signals:
     /**
      * @brief gameUpdated signal to notify the game is updated
      */
    void gameUpdated();
    /**
     * @brief cellClicked signal to notify the cell is clicked with parameters row, col
     * @param row
     * @param col
     */
    void cellClicked(int row, int col);

protected:
    /**
     * @brief paintEvent  draw the game
     * @param event
     */
    void paintEvent(QPaintEvent* event) override;
    /**
     * @brief mousePressEvent  handle the mouse press
     * @param event
     */
    void mousePressEvent(QMouseEvent* event) override;
    /**
     * @brief resizeEvent handle the resize event
     * @param event
     */
    void resizeEvent(QResizeEvent *event) override;

};

#endif  // GAMEBOARDWIDGET_H
