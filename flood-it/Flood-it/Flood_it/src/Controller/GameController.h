#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "../Model/Game.h"
#include "../View/mainwindow.h"
#include "../Model/RecordManager.h"

/**
 * @brief The GameController class
 */
class GameController {
private:
    Game& game;
    MainWindow& mainWindow;
    RecordManager recordManager;

public:
    /**
     * @brief GameController constructor with parameters game, mainWindow and recordManager
     * @param game
     * @param mainWindow
     */
    GameController(Game& game, MainWindow& mainWindow);
    /**
     * @brief run void function to run the game
     */
    void run();
    /**
     * @brief startNewGame function to start a new game
     * @param rows
     * @param cols
     * @param numColors
     */

    void startNewGame(int rows, int cols, int numColors);
    /**
     * @brief onStartGameClicked void function to start the game
     */
    void onStartGameClicked();
    /**
     * @brief onCellClicked function to handle the cell click
     * @param row
     * @param col
     */

    void handleCellClick(int row, int col);
};

#endif
