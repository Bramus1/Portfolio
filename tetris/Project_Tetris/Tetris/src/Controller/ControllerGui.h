#ifndef CONTROLLERGUI_H
#define CONTROLLERGUI_H

#include "../Model/Game.h"
#include "../View/ViewGui/mainwindow.h"


class ControllerGui {


public:
    ControllerGui(Game &game,MainWindow &mainWindow);


    void run();
    Game &getGame();
    void moveBrickDown();
    void keyReleaseEvent(QKeyEvent *event);


private:
    Game &game;
    MainWindow  &mainWindow;


};

#endif // CONTROLLERGUI_H
