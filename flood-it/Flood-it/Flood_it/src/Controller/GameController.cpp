#include "GameController.h"


GameController::GameController(Game& game, MainWindow& mainWindow)
    : game(game), mainWindow(mainWindow) {
        mainWindow.getGameBoardWidget()->setGame(&game);

    game.addObserver(&mainWindow);
    QObject::connect(mainWindow.getStartGameButton(), &QPushButton::clicked, [this]() {

        this->onStartGameClicked();
    });

    QObject::connect(mainWindow.getGameBoardWidget(), &GameBoardWidget::cellClicked, [this](int row, int col) {

        this->handleCellClick(row, col);
    });
}

void GameController::run() {


    mainWindow.show();

}

void GameController::onStartGameClicked() {
    mainWindow.getGameBoardWidget()->show();
    int rows = mainWindow.getSelectedRows();
    int cols = mainWindow.getSelectedCols();
    int numColors = mainWindow.getSelectedColors();
    game.startNewGame(rows, cols, numColors);
    mainWindow.getGameBoardWidget()->setGame(&game);
    int bestScore = recordManager.getRecord(rows, cols, numColors);

    if (bestScore > 0) {
        mainWindow.updateStatus("Best score for this configuration: " + QString::number(bestScore) + " moves.");
    } else {
        mainWindow.updateStatus("No record yet for this configuration. Set a new record!");
    }
}

void GameController::handleCellClick(int row, int col) {
    Color clickedColor = game.getBoard().getCellColor(row, col);
    game.makeMove(clickedColor);
    if (game.isGameWon()) {
        int rows = game.getRow();
        int cols = game.getCol();
        int numColors = game.getNumColors();
        int currentMoves = game.getMoves();
        int bestScore = recordManager.getRecord(rows, cols, numColors);

        if (bestScore == 0 || currentMoves <= bestScore) {
            recordManager.updateRecord(rows, cols, numColors, currentMoves);
            mainWindow.updateStatus(" You won in " + QString::number(currentMoves) + " moves.");


        } else {
            mainWindow.updateStatus("You lost , your moves are " + QString::number(currentMoves) + " Best score is " + QString::number(bestScore) + ".");
        }
    }
}
