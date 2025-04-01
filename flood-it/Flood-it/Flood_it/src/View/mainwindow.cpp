#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), boardWidget(new GameBoardWidget(this)) {
    setupUI();
    resize(800, 600);
}

GameBoardWidget* MainWindow::getGameBoardWidget() {
    return boardWidget;
}

int MainWindow::getSelectedRows() const {
    int rows = rowsComboBox->currentText().toInt();
    return rows;
}

int MainWindow::getSelectedCols() const {
    int cols = colsComboBox->currentText().toInt();
    return cols;
}

int MainWindow::getSelectedColors() const {
    int colors = colorsComboBox->currentText().toInt();
    return colors;
}

QPushButton* MainWindow::getStartGameButton() const {
    return startGameButton;

}

MainWindow::~MainWindow() {
    delete boardWidget;
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);


    QHBoxLayout *inputLayout = new QHBoxLayout();

    rowsComboBox = new QComboBox(this);
    colsComboBox = new QComboBox(this);
    colorsComboBox = new QComboBox(this);
    for (int i = 3; i <= 26; i++) {
        rowsComboBox->addItem(QString::number(i));
        colsComboBox->addItem(QString::number(i));
    }

    for (int i = 3; i <= 8; i++) {
        colorsComboBox->addItem(QString::number(i));
    }
    inputLayout->addWidget(new QLabel("Rows:"));
    inputLayout->addWidget(rowsComboBox);
    inputLayout->addWidget(new QLabel("Columns:"));
    inputLayout->addWidget(colsComboBox);
    inputLayout->addWidget(new QLabel("Colors:"));
    inputLayout->addWidget(colorsComboBox);
    mainLayout->addLayout(inputLayout);
    startGameButton = new QPushButton("Start New Game", this);
    mainLayout->addWidget(startGameButton);
    movesLabel = new QLabel("Moves Made: ", this);
    movesLabel->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(movesLabel);


  boardWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  mainLayout->addWidget(boardWidget);
  boardWidget->hide();



    setCentralWidget(centralWidget);
    statusBar()->showMessage("Welcome to Flood-It! by Giuliano ");
}

void MainWindow::updateStatus(const QString &message) {
    statusBar()->showMessage(message);

}

void MainWindow::update() {

    if (!boardWidget || !boardWidget->getGame()) {
        return;
    }
    // Ensure the Game object is valid
    Game* currentGame = boardWidget->getGame();
    if (currentGame) {
        movesLabel->setText("Moves Made: " + QString::number(currentGame->getMoves()));
    }
    boardWidget->update();
}
