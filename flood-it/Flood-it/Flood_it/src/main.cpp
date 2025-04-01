#include <QApplication>
#include"../src/Controller/GameController.h"
#include"../src/Model/Game.h"
#include"../src/View/mainwindow.h"


/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    int rows = 1;
    int cols = 1;
    int numColors = 1;
    GameBoard board(rows, cols, numColors);
    Game game(board);
    MainWindow mainWindow;
    GameController controller(game, mainWindow);
    controller.run();
    return a.exec();
}
