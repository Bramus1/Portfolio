# Flood-It Game 

Flood-It is a puzzle game where the goal is to fill the entire board with one color using the fewest number of moves. The game board is composed of cells of different colors, and starting from the top-left corner, the player selects a color to flood neighboring cells. The objective is to fill the entire board with the same color within a set number of moves.

## Features
- **Customizable Board**: The player can select the number of rows, columns, and colors for the game board.
- **Dynamic Board Resizing**: The game board dynamically resizes and remains centered on the screen.
- **Move Counter**: Track the number of moves the player has made.
- **Best Score Tracking**: The game tracks the fewest number of moves needed to win for each board configuration.

## Game Rules
- **Objective** :The goal of the game is to fill the entire board with a single color in as few moves as possible.
- **How to Play**: The game starts with a random arrangement of colors on the board.
Starting from the top-left corner, select a color to flood neighboring cells of the same color.
Continue flooding cells until the entire board is filled with a single color.
You win if you can fill the board in fewer moves than the best score for that configuration.

## Project Structure
- **Model**: The model is responsible for handling the game logic (moves, game state, best score tracking).
Game.cpp, GameBoard.cpp
- **View**: The view manages the graphical interface of the game.
MainWindow.cpp, GameBoardWidget.cpp
- **Controller**: The controller handles interactions between the view and the model.
GameController.cpp
- **Observer/Observable**: Implements the Observer-Observable pattern to manage communication between the game logic and the view.

## Programmer 
Giuliano Taurone 