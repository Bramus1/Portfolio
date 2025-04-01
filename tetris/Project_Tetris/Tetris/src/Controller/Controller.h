#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Model/Game.h"
#include "../View/View.h"


class Controller{
public:
    Controller(Game &game,View &view);

    void run();

private:
    Game &game;
    View &view;


};

#endif // CONTROLLER_H
