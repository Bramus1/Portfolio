#include "mainwindow.h"
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <qcolor.h>
#include<QPainter>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_MainWindow.h"
#include<QTimer>
#include"../../Controller/ControllerGui.h" // include this in order to have access to the controller

Board boardReset = Board(20,10); //global variable to reset the board

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), _scene(this), _scene_NextBrick(this){
    ui->setupUi(this);
    QRect viewContentsRect = ui->graphicsView->contentsRect();
    QRect viewContentsRectNextBrick = ui->graphicsView_NextBrick->contentsRect();
    _scene.setSceneRect(viewContentsRect);
    _scene_NextBrick.setSceneRect(viewContentsRectNextBrick);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(moveBrickDown()));
    timerGame = new QTimer(this);
    timeUp=false;
    connect(timerGame, SIGNAL(timeout()), this, SLOT(updateGameTimer()));
    timer->start(controller->getGame().getSpeed());
    reset=false;

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView_NextBrick->setRenderHint(QPainter::Antialiasing);

    ui->lineEdit_Level->setText(QString::number(0));
    ui->lineEdit_Score->setText(QString::number(0));
    ui->lineEdit_Lines->setText(QString::number(0));
    ui->lineEdit_question_prefill->setEnabled(false);
    ui->lineEdit_blocksToFillValue->setEnabled(false);


}

void MainWindow::setControllerGui(ControllerGui* controller){
    this->controller=controller;

}


MainWindow::~MainWindow(){
    delete ui;
    delete timer;
    delete timerGame;


}




void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::updateTimer(){
    timer->start(controller->getGame().getSpeed());
}


void MainWindow::updateGameTimer()


{
    if (
        !timeUp
        && controller->getGame().getGameState()==State::PLAY
        &&
        ui->pushButton_Play->text() == "Pause"){


        static int initialValue = ui->lineEdit_setter_time_value->text().toInt() * 60000;
        static int counterDown = initialValue;

        counterDown = counterDown-1000;

        if (counterDown >= 0)
        {
            timeUp = false;
            ui->lcdNumber_timer->display(counterDown / 1000);
        }
        else
        {
            timeUp = true;


            counterDown = initialValue; //Restore the initial value
            controller->getGame().notifyObservers(); // notify the observersin case the time is up
        }


        if(controller->getGame().isGameOver()){

            counterDown = initialValue;
        }
        if(controller->getGame().getVictorie()){

            counterDown = initialValue;
        }    if(reset){
            counterDown = initialValue;
            reset=false;
        }

    }

}

void MainWindow::update(const Observable *obs)

{

    int score=ui->lineEdit_vicotory_score_value->text().toInt();
    int level=ui->lineEdit_victory_level_value->text().toInt();
    int clearedLines=ui->lineEdit_victory_lines_values->text().toInt();
    controller->getGame().setVictorie(score,level,clearedLines);

    const Game * game {static_cast<const Game*>(obs)} ;
    paintEvent(&_scene,game->getBoard());

    paintCurrentBrick(&_scene, game->getCurrentBrick());
    paintEventNextBrick(&_scene_NextBrick, game->getNextBrick());

    updateTimer();


    if(game->isGameOver()){
        QMessageBox::information(this, "Game Over", "Game Over  \n Your Score : "+QString::number(game->getScore())+" points" );
        timerGame->stop();

        controller->getGame().restart();
        controller->getGame().setBoard(boardReset);// reset the board when is game over





        paintEvent(&_scene,game->getBoard());
        paintCurrentBrick(&_scene, game->getCurrentBrick());
        paintEventNextBrick(&_scene_NextBrick,game->getNextBrick());
        ui->lineEdit_Level->setText(QString::number(game->getLevel()));
        ui->lineEdit_Score->setText(QString::number(game->getScore()));
        ui->lineEdit_Lines->setText(QString::number(game->getClearedLines()));
        controller->getGame().pause();
        ui->pushButton_Play->setText("Play");
        timerGame->start(1000);

    }





    if(timeUp){
        QMessageBox::information(this, "Time Up", "Time Up  \n Your Score : "+QString::number(game->getScore())+" points" );
        timerGame->stop();


        timeUp=false;
        controller->getGame().restart();
        controller->getGame().setBoard(boardReset);//reset the board in case of time up




        paintEvent(&_scene,game->getBoard());
        paintCurrentBrick(&_scene, game->getCurrentBrick());
        paintEventNextBrick(&_scene_NextBrick,game->getNextBrick());
        ui->lineEdit_Level->setText(QString::number(game->getLevel()));
        ui->lineEdit_Score->setText(QString::number(game->getScore()));
        ui->lineEdit_Lines->setText(QString::number(game->getClearedLines()));
        controller->getGame().pause();
        ui->pushButton_Play->setText("Play");

    }




    if(game->getVictorie()){
        QMessageBox::information(this, "VICTORY", "VICTORY Congratulations you win \n Your score : "+QString::number(game->getScore())+" points");
        controller->getGame().restart();
        controller->getGame().setBoard(boardReset); //reset the board in case of victory
        updateTimer();




        paintEvent(&_scene,game->getBoard());
        paintCurrentBrick(&_scene, game->getCurrentBrick());
        paintEventNextBrick(&_scene_NextBrick,game->getNextBrick());
        ui->lineEdit_Level->setText(QString::number(game->getLevel()));
        ui->lineEdit_Score->setText(QString::number(game->getScore()));
        ui->lineEdit_Lines->setText(QString::number(game->getClearedLines()));

        controller->getGame().pause();
        ui->pushButton_Play->setText("Play");
        timerGame->start(1000);

    }

    ui->lineEdit_Level->setText(QString::number(game->getLevel()));
    ui->lineEdit_Score->setText(QString::number(game->getScore()));
    ui->lineEdit_Lines->setText(QString::number(game->getClearedLines()));
}
void MainWindow::moveBrickDown(){ // the thmer will call this function every 1000 ms to move the brick down
    if (controller->getGame().getGameState()==State::PLAY
        &&
        ui->pushButton_Play->text() == "Pause"
        &&
        !controller->getGame().isGameOver()
        &&!controller->getGame().getVictorie()
        &&!timeUp) {


        controller->moveBrickDown();

    }
}
void MainWindow::on_pushButton_Play_clicked()
{
    controller->getGame().play();


    if(ui->pushButton_Play->text() == "Pause")
    {



        controller->getGame().pause();
        ui->pushButton_Play->setText("Play");
        timerGame->stop();

    }
    else
    {
        controller->getGame().play();
        ui->pushButton_Play->setText("Pause");
        timerGame->start(1000); // here i start the game timer

    }



}
void MainWindow::on_pushButton_Restart_clicked()

{

    reset=true;
    timerGame->stop();
    controller->getGame().restart();

    controller->getGame().setBoard(boardReset);



    paintEvent(&_scene,controller->getGame().getBoard());
    paintEventNextBrick(&_scene_NextBrick,controller->getGame().getNextBrick());
    ui->lineEdit_Level->setText(QString::number(controller->getGame().getLevel()));
    ui->lineEdit_Score->setText(QString::number(controller->getGame().getScore()));
    ui->lineEdit_Lines->setText(QString::number(controller->getGame().getClearedLines()));

    controller->getGame().pause();
    ui->pushButton_Play->setText("Play");
    timerGame->start(1000);


}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        controller->getGame().move(Direction::LEFT);
        break;
    case Qt::Key_D:
        controller->getGame().move(Direction::RIGHT);
        break;
    case Qt::Key_S:
        controller->getGame().move(Direction::DOWN);
        break;
    case Qt::Key_E:
        controller->getGame().rotateCurrentBrick(RotationClock::CLOCKWISE);
        break;
    case Qt::Key_X:
        controller->getGame().drop();
        break;
    case Qt::Key_A:
        controller->getGame().rotateCurrentBrick(RotationClock::COUNTER_CLOCKWISE);
        break;
    case Qt::Key_U:
        controller->getGame().LevelUp(); //left this for testing purpose
        break;
    }
}


void MainWindow::makeInvisibleTab(){
    ui->tabWidget->setTabVisible(0,true);
    ui->tabWidget->setTabVisible(1,false);
    ui->tabWidget->setTabVisible(2,false);
    ui->tabWidget->setTabVisible(3,false);

}

//page 1
void MainWindow::on_pushButton_submit_personalise_clicked()
{

    if(ui->lineEdit_vicotory_score_value->text().toInt()<=100){
        QMessageBox::information(this, "Warning", "The victory score must be greater than 100");
        return;
    }
    if(ui->lineEdit_victory_level_value->text().toInt()<2 ^ ui->lineEdit_victory_level_value->text().toInt()>20){
        QMessageBox::information(this, "Warning", "The level must be between 2 and 20");
        return;
    }
    if(ui->lineEdit_victory_lines_values->text().toInt()<=9){
        QMessageBox::information(this, "Warning", "The lines must be greater than 9");
        return;
    }
    if(ui->checkBox_perso_size_yes->checkState()){
        ui->tabWidget->setTabVisible(0,false);
        ui->tabWidget->setTabVisible(2,true);
        ui->tabWidget->removeTab(0);
    }if(ui->checkBox_perso_size_no->checkState()){
        ui->tabWidget->setTabVisible(0,false);
        ui->tabWidget->setTabVisible(1,true);
        ui->tabWidget->removeTab(0);
    }



}

//page2
void MainWindow::on__submit_new_size_clicked()
{


    QString heightStr = ui->lineEdit_height_value->text().trimmed(); // Remove leading and trailing whitespace
    QString widthStr = ui->lineEdit_width_value->text().trimmed();

    if (!heightStr.isEmpty() && !widthStr.isEmpty()){
        int height = ui->lineEdit_height_value->text().toInt();
        int width = ui->lineEdit_width_value->text().toInt();

        if (height >= 20 && height <= 30 &&
            width  >= 10 && width  <= 25 ){
            Board newBoard(height, width);
            controller->getGame().setBoard(newBoard);
            ui->tabWidget->setTabVisible(1, true);
            ui->tabWidget->removeTab(0);
        }

    }
}


//page 3
void MainWindow::on_pushButton_submit_prefil_board_clicked()
{
    ui->graphicsView->setScene(&_scene);
    ui->graphicsView_NextBrick->setScene(&_scene_NextBrick);
    if(ui->checkBox_yesPrefill->checkState()){

        if(ui->lineEdit_blocksToFillValue->text().toInt() >= (controller->getGame().getBoard().getHeight()*controller->getGame().getBoard().getWidth())/2){ // here i check if the number of blocks to fill is less than half of the board size
            QMessageBox::information(this, "Warning", "The number of blocks to fill is too high. Please enter a number less than "+QString::number((controller->getGame().getBoard().getHeight()*controller->getGame().getBoard().getWidth())/2));
            return;
        }

        Board board = controller->getGame().getBoard();
        board.fillRandomlyBoard(ui->lineEdit_blocksToFillValue->text().toInt()); // i couldt use the game.getBoard as it is marked as const
        boardReset=board;// here i saved the board settings in case of restart
        controller->getGame().setBoard(board);
        ui->tabWidget->setTabVisible(2,false);
        ui->tabWidget->setTabVisible(1,true);
        paintEvent(&_scene,controller->getGame().getBoard());
        paintEventNextBrick(&_scene_NextBrick,controller->getGame().getNextBrick());
        ui->tabWidget->removeTab(0);

    }
    if(ui->checkBox_noPrefill->checkState()){

        ui->tabWidget->setTabVisible(2,false);
        ui->tabWidget->setTabVisible(1,true);
        paintEvent(&_scene,controller->getGame().getBoard());
        paintEventNextBrick(&_scene_NextBrick,controller->getGame().getNextBrick());
        ui->tabWidget->removeTab(0);
    }

}



void MainWindow::on_checkBox_yesPrefill_clicked()
{
    ui->lineEdit_question_prefill->setEnabled(true);
    ui->lineEdit_blocksToFillValue->setEnabled(true);
}


void MainWindow::on_checkBox_noPrefill_clicked()
{
    ui->lineEdit_question_prefill->setEnabled(false);
    ui->lineEdit_blocksToFillValue->setEnabled(false);
}


void MainWindow::paintEvent(QGraphicsScene * scene , const Board &board) const {



    int boardHeight = board.getHeight();
    int boardWidth = board.getWidth();

    int cellWidth = ui->graphicsView->size().width()/boardWidth;
    int cellHeight= ui->graphicsView->size().height()/boardHeight;

    for (int row = 0; row < board.getHeight(); ++row) {
        for (int col = 0; col < board.getWidth(); ++col) {
            QGraphicsRectItem * rect = scene->addRect(col * cellWidth, row * cellHeight, cellWidth, cellHeight);
            // Determine the color based on the cell type
            QColor color;
            switch (board.getBoard()[row][col]) {
            case Shapes::NO_SHAPE:
                color = Qt::white;
                break;
            case Shapes::J_SHAPE:
                color = Qt::black;
                break;
            case Shapes::I_SHAPE:
                color = Qt::blue;
                break ;
            case Shapes::L_SHAPE:
                color = Qt::red;
                break;
            case Shapes::S_SHAPE:
                color = Qt::yellow;
                break;
            case Shapes::O_SHAPE:
                color = Qt::green;
                break;
            case Shapes::T_SHAPE:
                color = Qt::cyan;
                break ;
            case Shapes::Z_SHAPE:
                color = Qt::gray;
                break;
            }
            rect->setBrush(color);

        }
    }
    scene->backgroundBrush();

}



void MainWindow::paintEventNextBrick(QGraphicsScene *scene, const Brick &brick) const {
    // Clear the scene
    scene->clear();

    int boardHeight = controller->getGame().getBoard().getHeight()/2;
    int boardWidth = controller->getGame().getBoard().getWidth()/2;

    int sceneWidth = scene->width();
    int sceneHeight = scene->height();

    int cellWidth = sceneWidth / boardWidth;
    int cellHeight = sceneHeight / boardHeight;

    // Calculate the center of the scene
    int centerX = sceneWidth / 2;
    int centerY = sceneHeight / 2;

    // Calculate the size of the next brick
    int nextBrickWidth = 5 * cellWidth;
    int nextBrickHeight = 5 * cellHeight;

    // Calculate the top-left position of the next brick to center it
    int startX = centerX - nextBrickWidth /4;
    int startY = centerY - nextBrickHeight /4;

    // Draw the next brick
    for (const auto& block : brick.getShapeBricks()) {
        int row = block.getY();
        int col = block.getX();
        int x = startX + col * cellWidth;
        int y = startY + row * cellHeight;
        QGraphicsRectItem *rect = scene->addRect(x, y, cellWidth, cellHeight);
        QColor color;
        switch (brick.getShapeType()) {
        case Shapes::J_SHAPE:
            color = Qt::black;
            break;
        case Shapes::I_SHAPE:
            color = Qt::blue;
            break ;
        case Shapes::L_SHAPE:
            color = Qt::red;
            break;
        case Shapes::S_SHAPE:
            color = Qt::yellow;
            break;
        case Shapes::O_SHAPE:
            color = Qt::green;
            break;
        case Shapes::T_SHAPE:
            color = Qt::cyan;
            break ;
        case Shapes::Z_SHAPE:
            color = Qt::gray;
            break;
        default:
            color = Qt::white; // Default color
            break;
        }
        rect->setBrush(color);
    }
}


void MainWindow::paintCurrentBrick(QGraphicsScene *scene, const Brick &currentBrick ) const {
    // Calculate the cell size on the scene based on the size of the window and the board
    int boardHeight = controller->getGame().getBoard().getHeight();
    int boardWidth = controller->getGame().getBoard().getWidth();

    int cellWidth = ui->graphicsView->size().width()/boardWidth;
    int cellHeight= ui->graphicsView->size().height()/boardHeight;

    // Iterate through each block of the current brick and draw it onto the scene
    for (const auto& block : currentBrick.getShapeBricks()) {

        int row = block.getY() + currentBrick.getPositionCentral().getY();
        int col = block.getX() + currentBrick.getPositionCentral().getX();

        // Make sure the current brick is within the bounds of the board
        if (row >= 0 && row < controller->getGame().getBoard().getHeight() && col >= 0 && col < controller->getGame().getBoard().getWidth()) {
            // Calcola le coordinate sulla scena per il blocco corrente
            int x = col * cellWidth;
            int y = row * cellHeight;

            // Create a rectangle on the scene to represent the current block
            QGraphicsRectItem *rect = scene->addRect(x, y, cellWidth, cellHeight);

            // Set the color of the rectangle based on the brick type
            QColor color;
            switch (currentBrick.getShapeType() ){
            case Shapes::J_SHAPE:
                color = Qt::black;
                break;
            case Shapes::I_SHAPE:
                color = Qt::blue;
                break ;
            case Shapes::L_SHAPE:
                color = Qt::red;
                break;
            case Shapes::S_SHAPE:
                color = Qt::yellow;
                break;
            case Shapes::O_SHAPE:
                color = Qt::green;
                break;
            case Shapes::T_SHAPE:
                color = Qt::cyan;
                break ;
            case Shapes::Z_SHAPE:
                color = Qt::gray;
                break;
            default:
                color = Qt::white; // Default color
                break;
            }
            rect->setBrush(color);
        }
    }
}
