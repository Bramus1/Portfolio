#include "ControllerGui.h"

ControllerGui::ControllerGui(Game & game, MainWindow& mainWindow):game(game),mainWindow(mainWindow){

    Board board = Board(20,10);
    this->game = Game(board);
    this->mainWindow.setControllerGui(this);
    mainWindow.makeInvisibleTab();
    game.addObservers(&mainWindow);
}

void ControllerGui::run(){
    mainWindow.show();
}

Game & ControllerGui::getGame(){
    return game;
}

void ControllerGui::moveBrickDown(){
    game.move(Direction::DOWN);
}


