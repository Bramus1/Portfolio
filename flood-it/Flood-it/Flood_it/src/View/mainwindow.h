#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include "GameBoardWidget.h"
#include "../Model/Observer.h"
#include <QPushButton>
/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow, public Observer{
    Q_OBJECT

public:
    /**
      * @brief startGameButton QPushButton
      */
     QPushButton* startGameButton;
    /**
     *  @brief Constructor
     *  @param parent
     */
    explicit MainWindow(QWidget *parent = nullptr);
     /**
      *  @brief Destructor
      */
    ~MainWindow();
    /**
     *  @brief getGameBoardWidget
     *  @return GameBoardWidget
     */
    GameBoardWidget* getGameBoardWidget();
    /**
     *  @brief getSelectedRows
     *  @return int
     */
    int getSelectedRows() const;
    /**
     *  @brief getSelectedCols
     *  @return int
     */
    int getSelectedCols() const;
    /**
     *  @brief getSelectedColors
     *  @return int
     */
    int getSelectedColors() const;
    /**
     *  @brief getMovesLabel
     *  @return QLabel
     */
    void setupUI();
    /**
     *  @brief getMovesLabel
     *  @return QLabel
     */
    void updateStatus(const QString& message);
    /**
     *  @brief getMovesLabel
     *  @return QLabel
     */
    void update() override;
    /**
     *  @brief getStartGameButton
     *  @return QPushButton
     */
    QPushButton* getStartGameButton() const;
    /**
     *  @brief initializeGameBoard
     *  @param game
     */
    void initializeGameBoard(Game & game);

private:
    GameBoardWidget* boardWidget;
    QComboBox* rowsComboBox;
    QComboBox* colsComboBox;
    QComboBox* colorsComboBox;
    QLabel* movesLabel;

};


#endif // MAINWINDOW_H
